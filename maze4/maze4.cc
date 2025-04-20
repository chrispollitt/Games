// INCLUDES //////////////////////////////////////////////////////////////////
#define NCURSES_WIDECHAR 1
#define _GNU_SOURCE 1

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <cerrno>

// DEFINES ///////////////////////////////////////////////////////////////////

// macros
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// UTF-8 characters for maze elements
#define WALL_CHAR "▒"
#define PATH_CHAR " "
#define VISITED_CHAR1 "◇" // RED
#define VISITED_CHAR2 "◇" // CYAN
#define VISITED_CHAR3 "◇" // YELLOW
#define VISITED_CHAR4 "◇" // GREEN
#define SOLUTION_CHAR1 "◆"
#define SOLUTION_CHAR2 "◆"
#define SOLUTION_CHAR3 "◆"
#define SOLUTION_CHAR4 "◆"
#define START_CHAR1 "A"
#define START_CHAR2 "B"
#define START_CHAR3 "C"
#define START_CHAR4 "D"
#define END_CHAR1 "1"
#define END_CHAR2 "2"
#define END_CHAR3 "3"
#define END_CHAR4 "4"
#define CURRENT_CHAR1 "①"
#define CURRENT_CHAR2 "②"
#define CURRENT_CHAR3 "③"
#define CURRENT_CHAR4 "④"
#define TELEPORTER_CHAR "◎"
#define MONSTER_CHAR "☠"
#define DEFEATED_MONSTER_CHAR "†"

// Internal representations
#define WALL '#'
#define PATH ' '
#define VISITED1 '1'
#define VISITED2 '2'
#define VISITED3 '3'
#define VISITED4 '4'
#define SOLUTION1 'a'
#define SOLUTION2 'b'
#define SOLUTION3 'c'
#define SOLUTION4 'd'
#define END1 'W'
#define END2 'X'
#define END3 'Y'
#define END4 'Z'
#define TELEPORTER 'T'
#define MONSTER 'M'
#define DEFEATED_MONSTER 'N'

// mins, maxes, etc.
#define MIN_ROWS 25
#define MIN_COLS 40
#define MAX_ROWS 1024
#define MAX_COLS 1024
#define DEF_GAME_SPEED 50
#define MAX_GAME_DELAY 200
#define DEF_TELEPORTER_DENSITY 1000
#define MAX_TELEPORTERS 10
#define DEF_MONSTER_DENSITY 500
#define MAX_MONSTERS 26
#define DEF_MONSTER_STRENGTH 10
#define MAX_MONSTER_STRENGTH 15
#define MAX_ATTEMPTS 100 // Maximum attempts for placing teleporters/monsters
#define POLL_INTERVAL_MS 333
#define NUM_PLAYERS 4
#define MAX_HIGH_SCORES 10
#define HIGH_SCORE_FILENAME "maze4_high_scores.dat"
#define LOG_FILENAME "maze4_logfile.txt"
#define STATUS_LINE_HISTORY 20
#define STATUS_LINE_MAX 80

// messages
#define DELAY_MSG "Delaying for you to read..."
#define LOST_MSG  "LOST the battle"
#define PAUSE_MSG "↑ Paused, press any key to continue..."

// TYPEDEFS //////////////////////////////////////////////////////////////////

typedef struct {
  int x, y;
} Coord;

typedef struct {
  char name[32];   // Player name
  int player_id;   // Player ID (1-4)
  int score;       // calculate_score() value
  int battles_won; // Number of battles won
  int strength;    // Final strength
  int this_run;    // Was this score achieved during this run
  time_t date;     // Date of the score
} HighScore;

typedef struct {
  int rows;
  int cols;
  char **grid;
  // Separate visited grids for each player
  char **visited[NUM_PLAYERS];
} Maze;

typedef struct {
  int x;
  int y;
  int parentX;
  int parentY;
} Position;

typedef struct {
  int x1, y1; // First teleporter location
  int x2, y2; // Second teleporter location
} Teleporter;

typedef struct {
  int x, y;           // Current position
  int dx, dy;         // Direction of movement
  int patrol_length;  // How far it patrols
  int steps;          // Current step count
  int strength;       // Monster strength (1-10)
  int defeated;       // Whether the monster has been defeated
  int recovery_turns; // lick wounds time to prevent battle loops
} Monster;

typedef struct {
  int id;             // Player ID (1-4)
  Position start;     // Starting position
  Position end;       // Target position
  Position current;   // Current position
  int strength;       // Player's strength (increases with battles won)
  int battles_won;    // Number of battles won
  int battles_lost;   // Number of battles lost
  int recovery_turns; // lick wounds time to prevent battle loops
  int justTeleported; // prevent loops
  int moves;          // Number of moves made
  int reached_goal;   // Whether player has reached goal
  int finished_rank;  // Finishing rank (1st, 2nd, etc)
  int abandoned_race; // Whether player has abandoned the race
  char visited_char;  // Character to represent visited cells
  char solution_char; // Character to represent solution path
  int color_pair;     // Color pair for this player
  int dx[4];          // Player-specific direction arrays
  int dy[4];          // Direction preferences for exploration
} Player;

typedef struct Node {
  Position pos;
  struct Node *next;
} Node;

typedef struct Status {
  int move;
  char msg[STATUS_LINE_MAX];
} Status;

// GLOBALS VARS //////////////////////////////////////////////////////////////

// Bot names
const char *BOT_NAMES[] = {
    "(n/a)", 
    "RapRas", // "Rapid Raspberry", 
    "BusBlu", // "Bussin' Blueberry",
    "LigLem", // "Lightening Lemon", 
    "KwiKiw" // "Kwick Kiwi"
};

const char *MONSTER_NAMES[] = {
    "Abyssal Artichoke",   "Brutal Broccoli",    "Creeping Carrot",
    "Dreadful Daikon",     "Eerie Eggplant",     "Fiendish Fennel",
    "Ghastly Garlic",      "Horrid Horseradish", "Infernal Iceberg",
    "Jagged Jicama",       "Killer Kale",        "Lurking Leek",
    "Mean Mushroom",       "Nightmare Nori",     "Ominous Onion",
    "Petrifying Potato",   "Quagmire Quinoa",    "Ravaging Radish",
    "Sinister Spinach",    "Terror Tomato",      "Unholy Ube",
    "Vile Vine Spinach",   "Wicked Wasabi",      "Xenophobic Xigua",
    "Yawning Yam",         "Zealous Zucchini"};

// Misc globals
WINDOW *battle_win;
int Base_dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
int Base_dy[] = {-1, 0, 1, 0};

// Global arrays for teleporters and monsters
Teleporter teleporters[MAX_TELEPORTERS];
int Num_teleporters = -1;
Monster monsters[MAX_MONSTERS];
int Num_monsters = -1;
int Liv_monsters = 0;
int Max_monster_strength = -1;

// Array of players
Player players[NUM_PLAYERS];
Coord  parent_map[NUM_PLAYERS][MAX_ROWS][MAX_COLS];

// Game state
int Players_finished = 0;
int Game_finished = 0;
int Game_moves = 0;
int Game_speed = -1;
int Game_delay = 0;
int Game_rounds = -1;
int Game_roundsB = -1;
int ShowWindows = -1;
int WaitForKey  = -1;
int LastSLupdate = 0;
int current_status_index = 0;
int Screen_reduced = 0;
Status status_lines[STATUS_LINE_HISTORY];
int old_rows = 0; 
int old_cols = 0;
int in_read_keyboard  = 0;
int in_mysleep = 0;

// maze state
Maze *maze;

// FUNCTION PROTOTYPES ///////////////////////////////////////////////////////
void animate_bullseye(int y, int x, int max_radius, int delay_ms, int direction);
int  battle_bot_monster(int monster_index, int player_id);
int  battle_bots(int player1_id, int player2_id);
int  battle_monsters(int monster1_idx, int monster2_idx);
int  battle_unified(int combatant1_idx, int combatant2_idx, int type);
void calc_game_speed();
int  calculate_score(int moves, int width, int height);
int  check_monster(int x, int y);
int  check_teleporter(int x, int y, int *newX, int *newY);
void clear_stack(Node **stack);
void create_maze(int rows, int cols);
void delay_with_polling(long total_delay_ms);
void display_high_scores_window(int count, HighScore best_scores[], HighScore worst_scores[]);
void display_player_alert(int p_idx, int rank);
void display_player_stats();
void ensure_path_between_corners();
void exit_game(const char *format, ...);
void free_maze();
void generate_maze();
char get_player_solution_char(int player_id);
char get_player_visited_char(int player_id);
void highlight_player_solution_path(int p);
void initialize_players(int stage);
void insert_high_score(HighScore scores[], int *count, HighScore new_score, int is_best);
int  is_dead_end(int x, int y);
int  is_empty(Node *stack);
int  is_player_position(int x, int y, Position *current_pos);
void logMessage(const char *format, ...);
void mysleep(long total_delay_ms);
void pauseForUser();
void pauseGame();
void place_monsters();
void place_teleporters();
Position pop_stack(Node **stack);
void print_maze();
void push_stack(Node **stack, Position pos);
int  read_high_scores(HighScore best_scores[], HighScore worst_scores[]);
void read_keyboard();
void run_round();
void save_high_scores(HighScore best_scores[], HighScore worst_scores[], int count);
void show_battle_art(int aart, int leftside, int type, int player, int left_idx, int right_idx);
void shuffle_directions_for_player(int idx);
void sleep_millis(long ms);
void solve_maze_multi();
void update_high_scores(int rows, int cols);
void update_monsters();
void update_status_line(const char *format, ...);

// FUNCTION DEFS /////////////////////////////////////////////////////////////

// MAIN function
int main(int argc, char *argv[]) {
  int opt;
  struct winsize size;

  // Process command line arguments
  while ((opt = getopt(argc, argv, "t:m:s:g:r:kwh")) != -1) {
    switch (opt) {
    case 't':
      Num_teleporters = atoi(optarg);
      break;
    case 'm':
      Num_monsters = atoi(optarg);
      break;
    case 's':
      Max_monster_strength = atoi(optarg);
      break;
    case 'g':
      Game_speed = atoi(optarg);
      break;
    case 'r':
      Game_rounds = atoi(optarg);
      break;
    case 'k':
      WaitForKey = 1;
      break;
    case 'w':
      ShowWindows = 1;
      break;
    case 'h':
      printf("Usage: %s [options]\n", argv[0]);
      printf("Options:\n");
      printf("  -t N    Set number of teleporter pairs (default: "
             "auto-calculated)\n");
      printf("  -m N    Set number of monsters (default: auto-calculated)\n");
      printf("  -s N    Set maximum monster strength (1-%d, default: %d)\n",
             MAX_MONSTER_STRENGTH, DEF_MONSTER_STRENGTH);
      printf("  -g N    Set game speed (1-100, default: %d)\n", DEF_GAME_SPEED);
      printf("  -k      Wait for key-press to continue (default: No)\n");
      printf("  -w      Show battle windows (default: No)\n");
      printf("  -h      Display this help message\n");
      return 0;
    default:
      fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
      return 1;
    }
  }

  // Set the random seed
  srand(static_cast<unsigned int>(time(NULL)));

  // Set locale for UTF-8 support
  setlocale(LC_ALL, "");

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  use_default_colors();
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  printf("\033[?1003h\n");  // Enable mouse movement events

  // Set up the resize handler
  signal(SIGWINCH, SIG_IGN);

  // Define color pairs
  init_pair(1, COLOR_BLACK, -1);            // Path
  init_pair(2, COLOR_RED, -1);              // Player 1
  init_pair(3, COLOR_CYAN, -1);             // Player 2
  init_pair(4, COLOR_YELLOW, -1);           // Player 3
  init_pair(5, COLOR_GREEN, -1);            // Player 4
  init_pair(6, COLOR_MAGENTA, -1);          // Current position
  init_pair(7, COLOR_CYAN, COLOR_BLUE);     // Teleporter
  init_pair(8, COLOR_WHITE, COLOR_MAGENTA); // Monster
  init_pair(9, COLOR_MAGENTA, COLOR_BLACK); // Defeated monster
  init_pair(10, COLOR_BLACK, COLOR_WHITE);  // Game stats
  init_pair(11, COLOR_BLACK, COLOR_YELLOW); // Alert
  //
  init_pair(12, COLOR_RED, COLOR_BLACK);
  init_pair(13, COLOR_WHITE, COLOR_BLACK);
  init_pair(14, COLOR_YELLOW, COLOR_BLACK);
  init_pair(15, COLOR_BLUE, COLOR_BLACK);
  init_pair(16, COLOR_CYAN, COLOR_BLACK);

  calc_game_speed();

  if (Game_rounds < 0) {
    Game_rounds = 1;
  }
  Game_roundsB = Game_rounds;
  if (WaitForKey < 0) {
    WaitForKey = 0;
  }
  if (ShowWindows < 0) {
    ShowWindows = 0;
  }

  // make sceen match reported size
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); 
  resizeterm(size.ws_row, size.ws_col);
  old_rows = size.ws_row ; old_cols = size.ws_col;
  logMessage("Init term size is %d, %d", size.ws_row, size.ws_col);

  // loop for the game rounds  
  for (; Game_rounds > 0; Game_rounds--) {
    logMessage("Starting round %d", Game_rounds);
    run_round();
    if (Screen_reduced) {
      Screen_reduced = 0;
      Game_rounds++;
    }
    // make sceen match reported size
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); 
    resizeterm(size.ws_row, size.ws_col);
    old_rows = size.ws_row ; old_cols = size.ws_col;      
  }

  // leave game
  exit_game("Game over\n");
}

// run a round of the game
void run_round() {
  int rows, cols, maze_area;

  // Initialize players
  initialize_players(0);

  // Get terminal dimensions
  getmaxyx(stdscr, rows, cols);
  maze_area = rows * cols;

  // Calculate default max values based on screen size if not provided
  if (Num_teleporters < 0) {
    // 1 teleporter per 100 cells, with a minimum of 1 and maximum of
    // MAX_TELEPORTERS
    Num_teleporters = maze_area / DEF_TELEPORTER_DENSITY;
  }
  Num_teleporters = Num_teleporters < 0
                        ? 0
                        : (Num_teleporters > MAX_TELEPORTERS ? MAX_TELEPORTERS
                                                             : Num_teleporters);
  if (Num_monsters < 0) {
    // 1 monster per 20 cells, with a minimum of 1 and maximum of MAX_MONSTERS
    Num_monsters = maze_area / DEF_MONSTER_DENSITY;
  }
  Num_monsters =
      Num_monsters < 0
          ? 0
          : (Num_monsters > MAX_MONSTERS ? MAX_MONSTERS : Num_monsters);
  if (Max_monster_strength < 0) {
    Max_monster_strength = DEF_MONSTER_STRENGTH; // Default maximum strength
  }
  Max_monster_strength = Max_monster_strength < 0
                             ? 0
                             : (Max_monster_strength > MAX_MONSTER_STRENGTH
                                    ? MAX_MONSTER_STRENGTH
                                    : Max_monster_strength);
  // Ensure minimum maze size
  if (rows < MIN_ROWS || cols < MIN_COLS) {
    exit_game("Screen too small, min %d rows, %d cols\n", MIN_ROWS, MIN_COLS);
  }
  // Ensure maximum maze size
  if (rows > MAX_ROWS || cols > MAX_COLS) {
    exit_game("Screen too big, max %d rows, %d cols\n", MAX_ROWS, MAX_COLS);
  }
  // Adjust for maze walls and borders
  rows = (rows - 6) / 2 * 2 - 1; // Reserve space for messages and player stats
  cols = (cols - 2) / 2 * 2 - 1;
  // Ensure rows and cols are odd, even if initial calculation is even.
  if (rows % 2 == 0)
    rows--;
  if (cols % 2 == 0)
    cols--;

  // Create and initialize maze
  create_maze(rows, cols);
  if (!maze) {
    exit_game("Failed to allocate memory for maze\n");
  }

  // Generate maze using enhanced DFS for paths between corners
  generate_maze();

  // Implement and call ensure_path_between_corners
  ensure_path_between_corners();

  // Place teleporters and monsters
  place_teleporters();
  place_monsters();
  Liv_monsters = Num_monsters;

  // Finsih initializing players
  initialize_players(1);

  // Print initial maze
  clear();
  print_maze();
  display_player_stats();
  pauseForUser();
  update_status_line("The race is ON!");
  LastSLupdate = 1;

  // Solve maze concurrently for all players
  solve_maze_multi();

  // highscore
  if(!Screen_reduced) update_high_scores(rows, cols);

  // Clean up
  free_maze();
}

void initialize_players(int stage) {
  // Stage 0
  if (stage == 0) {
    ///////////////////////////////
    // Global vars
    memset(teleporters, 0, sizeof(teleporters));    
    memset(monsters, 0, sizeof(monsters));
    Liv_monsters = 0;
    memset(players, 0, sizeof(players));
    memset(parent_map, 0, sizeof(parent_map));
    Players_finished = 0;
    Game_finished = 0;
    Game_moves = 0;
    maze = NULL;
    // Clear all status line entries
    for (int i = 0; i < STATUS_LINE_HISTORY; i++) {
        status_lines[i].msg[0] = '\0';
        status_lines[i].move = 0;
    }

    ///////////////////////////////
    // Copy the base direction arrays to start
    int dx_copy[4];
    int dy_copy[4];
    memcpy(dx_copy, Base_dx, sizeof(Base_dx));
    memcpy(dy_copy, Base_dy, sizeof(Base_dy));
  
    // Initialize player 1 with default directions (Up, Right, Down, Left)
    players[0].id = 1;
    players[0].strength = 5;
    players[0].battles_won = 0;
    players[0].battles_lost = 0;
    players[0].recovery_turns = 0;
    players[0].justTeleported = 0;
    players[0].moves = 0;
    players[0].reached_goal = 0;
    players[0].finished_rank = 0;
    players[0].abandoned_race = 0;
    players[0].visited_char = VISITED1;
    players[0].solution_char = SOLUTION1;
    players[0].color_pair = 2; // Red
    // Set direction preferences - Right, Down, Left, Up (good for top-left to
    // bottom-right)
    players[0].dx[0] = 1; // Right
    players[0].dy[0] = 0;
    players[0].dx[1] = 0; // Down
    players[0].dy[1] = 1;
    players[0].dx[2] = -1; // Left
    players[0].dy[2] = 0;
    players[0].dx[3] = 0; // Up
    players[0].dy[3] = -1;
  
    // Initialize player 2 with different direction preference
    players[1].id = 2;
    players[1].strength = 5;
    players[1].battles_won = 0;
    players[1].battles_lost = 0;
    players[1].recovery_turns = 0;
    players[1].justTeleported = 0;
    players[1].moves = 0;
    players[1].reached_goal = 0;
    players[1].finished_rank = 0;
    players[1].abandoned_race = 0;
    players[1].visited_char = VISITED2;
    players[1].solution_char = SOLUTION2;
    players[1].color_pair = 3; // Cyan
    // Set direction preferences - Left, Down, Right, Up (good for top-right to
    // bottom-left)
    players[1].dx[0] = -1; // Left
    players[1].dy[0] = 0;
    players[1].dx[1] = 0; // Down
    players[1].dy[1] = 1;
    players[1].dx[2] = 1; // Right
    players[1].dy[2] = 0;
    players[1].dx[3] = 0; // Up
    players[1].dy[3] = -1;
  
    // Initialize player 3 with different direction preference
    players[2].id = 3;
    players[2].strength = 5;
    players[2].battles_won = 0;
    players[2].battles_lost = 0;
    players[2].recovery_turns = 0;
    players[2].justTeleported = 0;
    players[2].moves = 0;
    players[2].reached_goal = 0;
    players[2].finished_rank = 0;
    players[2].abandoned_race = 0;
    players[2].visited_char = VISITED3;
    players[2].solution_char = SOLUTION3;
    players[2].color_pair = 4; // Yellow
    // Set direction preferences - Right, Up, Left, Down (good for bottom-left to
    // top-right)
    players[2].dx[0] = 1; // Right
    players[2].dy[0] = 0;
    players[2].dx[1] = 0; // Up
    players[2].dy[1] = -1;
    players[2].dx[2] = -1; // Left
    players[2].dy[2] = 0;
    players[2].dx[3] = 0; // Down
    players[2].dy[3] = 1;
  
    // Initialize player 4 with different direction preference
    players[3].id = 4;
    players[3].strength = 5;
    players[3].battles_won = 0;
    players[3].battles_lost = 0;
    players[3].recovery_turns = 0;
    players[3].justTeleported = 0;
    players[3].moves = 0;
    players[3].reached_goal = 0;
    players[3].finished_rank = 0;
    players[3].abandoned_race = 0;
    players[3].visited_char = VISITED4;
    players[3].solution_char = SOLUTION4;
    players[3].color_pair = 5; // Green
    // Set direction preferences - Left, Up, Right, Down (good for bottom-right to
    // top-left)
    players[3].dx[0] = -1; // Left
    players[3].dy[0] = 0;
    players[3].dx[1] = 0; // Up
    players[3].dy[1] = -1;
    players[3].dx[2] = 1; // Right
    players[3].dy[2] = 0;
    players[3].dx[3] = 0; // Down
    players[3].dy[3] = 1;
  
    // Now randomize each player's directions slightly for variety
    for (int i = 0; i < NUM_PLAYERS; i++) {
      shuffle_directions_for_player(i);
    }
  // Stage 1
  } else {
    // Set player start and end positions to corners
    players[0].start.x = 1;
    players[0].start.y = 1;
    players[0].end.x = maze->cols - 2;
    players[0].end.y = maze->rows - 2;
    maze->grid[players[0].end.y][players[0].end.x]     = END1;
  
    players[1].start.x = maze->cols - 2;
    players[1].start.y = 1;
    players[1].end.x = 1;
    players[1].end.y = maze->rows - 2;
    maze->grid[players[1].end.y][players[1].end.x]     = END2;
  
    players[2].start.x = 1;
    players[2].start.y = maze->rows - 2;
    players[2].end.x = maze->cols - 2;
    players[2].end.y = 1;
    maze->grid[players[2].end.y][players[2].end.x]     = END3;
  
    players[3].start.x = maze->cols - 2;
    players[3].start.y = maze->rows - 2;
    players[3].end.x = 1;
    players[3].end.y = 1;
    maze->grid[players[3].end.y][players[3].end.x]     = END4;
  
    // Initialize player current positions
    for (int i = 0; i < NUM_PLAYERS; i++) {
      players[i].current = players[i].start;
    }
  }
}

// Shuffle the directions for a specific player
void shuffle_directions_for_player(int idx) {
  // Apply a Fisher-Yates shuffle but only to the last 2 directions
  // This preserves the main direction preference but adds some randomness
  for (int i = 3; i >= 2; i--) {
    int j = rand() % (i + 1);

    // Swap dx[i] with dx[j]
    int temp = players[idx].dx[i];
    players[idx].dx[i] = players[idx].dx[j];
    players[idx].dx[j] = temp;

    // Swap dy[i] with dy[j]
    temp = players[idx].dy[i];
    players[idx].dy[i] = players[idx].dy[j];
    players[idx].dy[j] = temp;
  }
}

// Helper function to get the visited character for a player
char get_player_visited_char(int player_id) {
  switch (player_id) {
  case 1:
    return VISITED1;
  case 2:
    return VISITED2;
  case 3:
    return VISITED3;
  case 4:
    return VISITED4;
  default:
    return '!';
  }
}

// Helper function to get the solution character for a player
char get_player_solution_char(int player_id) {
  switch (player_id) {
  case 1:
    return SOLUTION1;
  case 2:
    return SOLUTION2;
  case 3:
    return SOLUTION3;
  case 4:
    return SOLUTION4;
  default:
    return '!';
  }
}

// Check if a position is a player's position
__attribute__((no_instrument_function))
int is_player_position(int x, int y, Position *current_pos) {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (current_pos[i].x == x && current_pos[i].y == y) {
      return i + 1; // Return player id (1-based)
    }
  }
  return 0; // Not a player position
}

// Check if a position is a dead end
__attribute__((no_instrument_function))
int is_dead_end(int x, int y) {
  // Don't consider special positions as dead ends
  if (
      maze->grid[y][x] == END1 || 
      maze->grid[y][x] == END2 ||
      maze->grid[y][x] == END3 || 
      maze->grid[y][x] == END4) {
    return 0;
  }

  // Only check PATH cells
  if (maze->grid[y][x] != PATH) {
    return 0;
  }

  // Count adjacent PATH/START*/END* cells
  int path_neighbors = 0;
  for (int dir = 0; dir < 4; dir++) {
    int newX = x + Base_dx[dir];
    int newY = y + Base_dy[dir];

    if (newX >= 0 && newX < maze->cols && newY >= 0 && newY < maze->rows) {
      char cell = maze->grid[newY][newX];
      if (cell == PATH || 
          cell == END1 || 
          cell == END2 || 
          cell == END3 ||
          cell == END4) {
        path_neighbors++;
      }
    }
  }

  // If only one PATH neighbor, it's a dead end
  return (path_neighbors == 1);
}

// Display player stats with combined battles column and status column
void display_player_stats() {
  int base_row = maze->rows + 1;

  mvwprintw(stdscr, base_row - 1, 0, "  NAME | ST | BATS  | MOVES | STATUS");
  wclrtoeol(stdscr);
  for (int i = 0; i < NUM_PLAYERS; i++) {
    attron(COLOR_PAIR(players[i].color_pair) | A_BOLD);
    mvwprintw(stdscr, base_row + i, 0, "%6s | %2d | %2d/%-2d |  %4d | ",
             BOT_NAMES[players[i].id], players[i].strength,
             players[i].battles_won, players[i].battles_lost, players[i].moves);

    // Add status column showing player's solve status
    if (players[i].reached_goal) {
      wprintw(stdscr, "Finished #%d!", players[i].finished_rank);
    } else if (players[i].abandoned_race == 1) {
      wprintw(stdscr, "DNF: Trapped");
    } else if (players[i].abandoned_race == 2) {
      wprintw(stdscr, "DNF: Loser");
    } else if (Game_moves == 0) {
      wprintw(stdscr, "Ready To Start");
    } else {
      wprintw(stdscr, "Solving");
    }
    wclrtoeol(stdscr);

    attroff(COLOR_PAIR(players[i].color_pair) | A_BOLD);
  }
  wrefresh(stdscr);
}

// Display player alert with variable parameters
//                     player_index  -2=abandoned, -1=out of moves, >0 is rank
void display_player_alert(int p_idx, int rank) {
  if (ShowWindows == 0) {
    pauseForUser();
    return;
  }
  // Save current window and create a larger battle screen
  // Calculate window dimensions
  int height = MIN_ROWS -2;
  int width  = MIN_COLS;

  // Create a new window centered on screen
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  battle_win = newwin(height, width, (max_y - height) / 2, (max_x - width) / 2);
  box(battle_win, 0, 0);

  // Set up colors for battle screen
  wbkgd(battle_win, COLOR_PAIR(11));

  // Display battle title based on battle type
  wattron(battle_win, COLOR_PAIR(players[p_idx - 1].color_pair) | A_BOLD);
  if (rank > 0) {
    mvwprintw(battle_win, 2, 2, "PLAYER REACHED GOAL!");
    // figlet goal
    mvwprintw(battle_win, 16, 1,   "        RANK = %d!                    ", rank);
    mvwprintw(battle_win, 17, 1, R"( _____ _       _     _              _ )");
    mvwprintw(battle_win, 18, 1, R"(|  ___(_)_ __ (_)___| |__   ___  __| |)");
    mvwprintw(battle_win, 19, 1, R"(| |_  | | '_ \| / __| '_ \ / _ \/ _` |)");
    mvwprintw(battle_win, 20, 1, R"(|  _| | | | | | \__ \ | | |  __/ (_| |)");
    mvwprintw(battle_win, 21, 1, R"(|_|   |_|_| |_|_|___/_| |_|\___|\__,_|)");
  } else if (rank == -1) {
    mvwprintw(battle_win, 2, 2, "PLAYER ABANDONED RACE");
    // figlet abandoned
    mvwprintw(battle_win, 16, 1,   " _____  OUT OF MOVES!               _ ");
    mvwprintw(battle_win, 17, 1, R"(|_   _| __ __ _ _ __ _ __   ___  __| |)");
    mvwprintw(battle_win, 18, 1, R"(  | || '__/ _` | '_ | '_ \ / _ \/ _` |)");
    mvwprintw(battle_win, 19, 1, R"(  | || | | (_| | |_)| |_) |  __/ (_| |)");
    mvwprintw(battle_win, 20, 1, R"(  |_||_|  \__,_| .__| .__/ \___|\__,_|)");
    mvwprintw(battle_win, 21, 1, R"(               |_|  |_|               )");
  } else if (rank == -2) {
    mvwprintw(battle_win, 2, 2, "PLAYER ABANDONED RACE");
    // figlet abandoned
    mvwprintw(battle_win, 16, 1,   "        TOO MANY LOSSES!        ");
    mvwprintw(battle_win, 17, 1, R"( _     ___  ____  _____ ____  _ )");
    mvwprintw(battle_win, 18, 1, R"(| |   / _ \/ ___|| ____|  _ \| |)");
    mvwprintw(battle_win, 19, 1, R"(| |  | | | \___ \|  _| | |_) | |)");
    mvwprintw(battle_win, 20, 1, R"(| |__| |_| |___) | |___|  _ <|_|)");
    mvwprintw(battle_win, 21, 1, R"(|_____\___/|____/|_____|_| \_(_))");
  }
  show_battle_art(1, 1, 1, 1, p_idx - 1, -1);
  wattroff(battle_win, COLOR_PAIR(players[p_idx - 1].color_pair) | A_BOLD);
  wrefresh(battle_win);
  pauseForUser(); 

  // Clean up battle window
  delwin(battle_win);
  battle_win = NULL; // Good practice to avoid dangling pointers

  // Redraw the main screen
  // clear();
  print_maze();
  display_player_stats();
}

// Place teleporters at dead ends
void place_teleporters() {
  // Arrays to store dead end positions
  int *dead_ends_x = (int *)malloc(maze->rows * maze->cols * sizeof(int));
  int *dead_ends_y = (int *)malloc(maze->rows * maze->cols * sizeof(int));
  int dead_end_count = 0;

  if (!dead_ends_x || !dead_ends_y) {
    free(dead_ends_x);
    free(dead_ends_y);
    return;
  }

  // Find all dead ends
  for (int y = 1; y < maze->rows - 1; y++) {
    for (int x = 1; x < maze->cols - 1; x++) {
      if (is_dead_end(x, y)) {
        dead_ends_x[dead_end_count] = x;
        dead_ends_y[dead_end_count] = y;
        dead_end_count++;
      }
    }
  }

  // If we don't have enough dead ends for at least one teleporter, exit
  if (dead_end_count < 2) {
    free(dead_ends_x);
    free(dead_ends_y);
    return;
  }

  // Adjust number of teleporters based on available dead ends
  int max_possible_teleporters = dead_end_count / 2;
  Num_teleporters = (max_possible_teleporters < Num_teleporters)
                        ? max_possible_teleporters
                        : Num_teleporters;

  // Place teleporters at randomly selected dead ends
  for (int i = 0; i < Num_teleporters; i++) {
    int idx1 = rand() % dead_end_count;
    int x1 = dead_ends_x[idx1];
    int y1 = dead_ends_y[idx1];

    // Remove this dead end from the pool
    dead_ends_x[idx1] = dead_ends_x[dead_end_count - 1];
    dead_ends_y[idx1] = dead_ends_y[dead_end_count - 1];
    dead_end_count--;

    int idx2 = rand() % dead_end_count;
    int x2 = dead_ends_x[idx2];
    int y2 = dead_ends_y[idx2];

    // Remove this dead end too
    dead_ends_x[idx2] = dead_ends_x[dead_end_count - 1];
    dead_ends_y[idx2] = dead_ends_y[dead_end_count - 1];
    dead_end_count--;

    // Skip corners (player start/end positions)
    if ((x1 <= 2 && y1 <= 2) ||
        (x1 >= maze->cols - 3 && y1 <= 2) ||
        (x1 <= 2 && y1 >= maze->rows - 3) ||
        (x1 >= maze->cols - 3 && y1 >= maze->rows - 3)
    ) {
      i--;
      continue;
    }
    if ((x2 <= 2 && y2 <= 2) ||
        (x2 >= maze->cols - 3 && y2 <= 2) ||
        (x2 <= 2 && y2 >= maze->rows - 3) ||
        (x2 >= maze->cols - 3 && y2 >= maze->rows - 3)
    ) {
      i--;
      continue;
    }

    // Place teleporters
    teleporters[i].x1 = x1;
    teleporters[i].y1 = y1;
    teleporters[i].x2 = x2;
    teleporters[i].y2 = y2;

    maze->grid[y1][x1] = TELEPORTER;
    maze->grid[y2][x2] = TELEPORTER;
  }

  free(dead_ends_x);
  free(dead_ends_y);
}

// Place monsters at random locations, avoiding teleporters and corners
void place_monsters() {
  for (int i = 0; i < Num_monsters; i++) {
    // Find a random empty space
    int x, y;
    int attempts = 0;

    do {
      x = rand() % (maze->cols - 4) + 2; // Avoid edges and corners
      y = rand() % (maze->rows - 4) + 2;
      attempts++;

      // Skip if we can't find a spot after many attempts
      if (attempts > MAX_ATTEMPTS) {
        Num_monsters = i;
        return;
      }

      // Skip corners (player start/end positions)
      if ((x <= 2 && y <= 2) || (x >= maze->cols - 3 && y <= 2) ||
          (x <= 2 && y >= maze->rows - 3) ||
          (x >= maze->cols - 3 && y >= maze->rows - 3)) {
        continue;
      }
    } while (maze->grid[y][x] != PATH || is_dead_end(x, y));

    // Place monster
    monsters[i].x = x;
    monsters[i].y = y;

    // Random direction
    int dir = rand() % 4;
    monsters[i].dx = Base_dx[dir];
    monsters[i].dy = Base_dy[dir];

    // Random patrol length
    monsters[i].patrol_length = rand() % 10 + 5;
    monsters[i].steps = 0;

    // Random strength (1-10)
    monsters[i].strength = rand() % Max_monster_strength + 1;

    // Recovery after fight
    monsters[i].recovery_turns = 0;

    // Not defeated initially
    monsters[i].defeated = 0;

    maze->grid[y][x] = MONSTER;
  }
}

// Update monster positions
void update_monsters() {
  for (int i = 0; i < Num_monsters; i++) {
    // Skip defeated monsters
    if (monsters[i].defeated) {
      continue;
    }

    // decrement recovery_turns
    if (monsters[i].recovery_turns)
      monsters[i].recovery_turns--;

    // Clear current position
    maze->grid[monsters[i].y][monsters[i].x] = PATH;

    // Update position
    monsters[i].steps++;
    if (monsters[i].steps >= monsters[i].patrol_length) {
      // Change direction
      int dir = rand() % 4;
      monsters[i].dx = Base_dx[dir];
      monsters[i].dy = Base_dy[dir];
      monsters[i].steps = 0;
    }

    // Try to move
    int newX = monsters[i].x + monsters[i].dx;
    int newY = monsters[i].y + monsters[i].dy;
    char cell = maze->grid[newY][newX];

    // Check if the new position is valid and not a special cell
    if (newX > 0 && newX < maze->cols - 1 && newY > 0 &&
        newY < maze->rows - 1 && (cell == PATH || cell == MONSTER)) {
      monsters[i].x = newX;
      monsters[i].y = newY;
    } else {
      // Change direction if blocked
      int dir = rand() % 4;
      monsters[i].dx = Base_dx[dir];
      monsters[i].dy = Base_dy[dir];
      monsters[i].steps = 0;
    }

    // Mark new position
    maze->grid[monsters[i].y][monsters[i].x] = MONSTER;
  }

  // Check for monster vs monster collisions
  for (int i = 0; i < Num_monsters; i++) {
    // Skip defeated monsters
    if (monsters[i].defeated) {
      continue;
    }

    for (int j = i + 1; j < Num_monsters; j++) {
      // Skip defeated monsters
      if (monsters[j].defeated) {
        continue;
      }

      // Check if monsters are in the same cell
      if (monsters[i].x == monsters[j].x && monsters[i].y == monsters[j].y) {
        // Battle the monsters
        battle_monsters(i, j);

        // After battle, the winner stays at the position, loser is already
        // marked as defeated So no additional position updates needed
      }
    }
  }
}

// Check if position is a teleporter and get destination
int check_teleporter(int x, int y, int *newX, int *newY) {
  for (int i = 0; i < Num_teleporters; i++) {
    if (teleporters[i].x1 == x && teleporters[i].y1 == y) {
      *newX = teleporters[i].x2;
      *newY = teleporters[i].y2;
      return 1;
    } else if (teleporters[i].x2 == x && teleporters[i].y2 == y) {
      *newX = teleporters[i].x1;
      *newY = teleporters[i].y1;
      return 1;
    }
  }
  return 0;
}

// Check if position has a monster and return its index
int check_monster(int x, int y) {
  for (int i = 0; i < Num_monsters; i++) {
    if (monsters[i].x == x && monsters[i].y == y && !monsters[i].defeated) {
      return i + 1; // Return monster index + 1 (so 0 means no monster)
    }
  }
  return 0; // No monster
}

//                              winner         battle
//                 art/figlet, left/right,      type, player/monster  left-index
//                 right-index
void show_battle_art(int aart, int leftside, int type, int player, int left_idx, int right_idx) {
  // Set the recovery time (in turns) for both winner and loser
  const int WINNER_RECOVERY_TURNS = 3;
  const int LOSER_RECOVERY_TURNS  = 6;

  // Show ascii-art of battler /////////////////////////////////////////////
  if (aart) {
    if (ShowWindows == 0) return;
    int col = leftside ? 2 : 23;
    int idx = leftside ? left_idx : right_idx;
    if (player) { // Player Bot
      wattron(battle_win, COLOR_PAIR(players[idx].color_pair) | A_BOLD);
      mvwprintw(battle_win, 4, col, "%s", BOT_NAMES[idx + 1]);
      mvwprintw(battle_win, 5, col, "STR: %d  WINS: %d", players[idx].strength,
                players[idx].battles_won);

      // Bot ASCII Art
      mvwprintw(battle_win,  6, col, R"(               )");
      mvwprintw(battle_win,  7, col, R"(     ____      )");
      mvwprintw(battle_win,  8, col, R"(    /    \     )");
      mvwprintw(battle_win,  9, col, R"(   | o  o |    )");
      mvwprintw(battle_win, 10, col, R"(   | ____ |    )");
      mvwprintw(battle_win, 11, col, R"(   ||____||    )");
      mvwprintw(battle_win, 12, col, R"(    \____/     )");
      mvwprintw(battle_win, 13, col, R"(               )");
      wattroff(battle_win, COLOR_PAIR(players[idx].color_pair) | A_BOLD);
    } else { // Monster
      wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
      mvwprintw(battle_win, 4, col, "%s", MONSTER_NAMES[idx]);
      mvwprintw(battle_win, 5, col, "Strength: %d    ", monsters[idx].strength);

      // Monster ASCII Art
      mvwprintw(battle_win,  6, col, R"(    .----,     )");
      mvwprintw(battle_win,  7, col, R"(   /      \    )");
      mvwprintw(battle_win,  8, col, R"(  |  O  O  |   )");
      mvwprintw(battle_win,  9, col, R"(  | .vvvv. |   )");
      mvwprintw(battle_win, 10, col, R"(  / |    | \   )");
      mvwprintw(battle_win, 11, col, R"( /  `^^^^'  \  )");
      mvwprintw(battle_win, 12, col, R"(/  /|     |\ \.)");
      mvwprintw(battle_win, 13, col, R"(\_/ .~~~~~. \_/)");
      wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
    }
    // update stats for winner and loser /////////////////////////////////////
  } else {
    int widx, lidx;
    int winner_is_player = ((type == 0 && leftside)  || (type == 1));
    int loser_is_player  = ((type == 0 && !leftside) || (type == 1));
    if (leftside) {
      widx = left_idx;
      lidx = right_idx;
    } else {
      lidx = left_idx;
      widx = right_idx;
    }
    // handle winner /////////////////
    if (winner_is_player) {
      // winner is player
      players[widx].strength += 1;
      players[widx].battles_won += 1;
      players[widx].recovery_turns = WINNER_RECOVERY_TURNS;
    } else {
      // winner is monster
      monsters[widx].strength += 1;
      monsters[widx].recovery_turns = WINNER_RECOVERY_TURNS;
    }
    // handle loser //////////////////
    if (loser_is_player) {
      // loser is player
      players[lidx].battles_lost += 1;
      players[lidx].recovery_turns = LOSER_RECOVERY_TURNS;
      update_status_line("%s %s!", BOT_NAMES[lidx + 1], LOST_MSG);
    } else {
      // loser is monster
      Liv_monsters--;
      monsters[lidx].defeated = 1;
      maze->grid[monsters[lidx].y][monsters[lidx].x] = DEFEATED_MONSTER;
      update_status_line("%s %s!", MONSTER_NAMES[lidx], LOST_MSG);
    }

    if (ShowWindows != 0) {
      // X out eyes of loser
      int los_col = leftside ? 23 : 2;
      if (loser_is_player) {
        // bot
        mvwprintw(battle_win, 9, los_col + 2, R"( | x  x | )");
      } else {
        // monster
        mvwprintw(battle_win, 8, los_col + 1, R"( |  X  X  | )");
      }
      
      // Show "Wins!" text with correct positioning ////////////////////////
      int win_col = leftside ? 1 : 23;
      mvwprintw(battle_win, 17, win_col, R"( _    _ _       )");
      mvwprintw(battle_win, 18, win_col, R"(| |  | (_)_ __  )");
      mvwprintw(battle_win, 19, win_col, R"(| |/\| | | '_ \ )");
      mvwprintw(battle_win, 20, win_col, R"(|  __  | | | | |)");
      mvwprintw(battle_win, 21, win_col, R"(|_/  \_|_|_| |_|)");
    }
  }
}

// Unified battle function that handles all battle scenarios
// Type: 0 = player vs monster, 1 = player vs player, 2 = monster vs monster
int battle_unified(int combatant1_idx, int combatant2_idx, int type) {
  // Get player indices if needed (for player vs monster or player vs player)
  int p1_idx = (type == 0 || type == 1) ? combatant1_idx - 1
                                        : -1; // Adjust for 0-based indexing
  int p2_idx =
      (type == 1) ? combatant2_idx - 1 : -1; // Adjust for 0-based indexing

  // Get monster indices if needed (for player vs monster or monster vs monster)
  int m1_idx = (type == 0) ? combatant2_idx : (type == 2 ? combatant1_idx : -1);
  int m2_idx = (type == 2) ? combatant2_idx : -1;

  // Check if any combatant is in recovery mode - if so, skip battle entirely
  if ((type == 0 || type == 1) && players[p1_idx].recovery_turns > 0) {
    // Player 1 is recovering, no battle
    return -1; // Special code indicating battle was skipped
  }

  if (type == 1 && players[p2_idx].recovery_turns > 0) {
    // Player 2 is recovering, no battle
    return -1; // Special code indicating battle was skipped
  }

  if (type == 0 && !monsters[m1_idx].defeated &&
      monsters[m1_idx].recovery_turns > 0) {
    // Monster is recovering, no battle
    return -1;
  }

  if (type == 2) {
    if (monsters[m1_idx].recovery_turns > 0 ||
        monsters[m2_idx].recovery_turns > 0) {
      // One of the monsters is recovering, no battle
      return -1;
    }
  }

  // show battle spot
  int be_x = (type != 2) ? players[p1_idx].current.x : monsters[m1_idx].x;
  int be_y = (type != 2) ? players[p1_idx].current.y : monsters[m1_idx].y;
  animate_bullseye(be_y, be_x, 5, 100, 0);

  if (ShowWindows != 0) {
    // Save current window and create a larger battle screen
    // Calculate window dimensions
    int height = MIN_ROWS -2;
    int width  = MIN_COLS;
    
    // Create a new window centered on screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    battle_win = newwin(height, width, (max_y - height) / 2, (max_x - width) / 2);
    box(battle_win, 0, 0);
    
    // Set up colors for battle screen
    wbkgd(battle_win, COLOR_PAIR(10));
    
    // Display battle title based on battle type
    wattron(battle_win, A_BOLD);
    switch (type) {
    case 0:
      mvwprintw(battle_win, 2, 2, "PLAYER VS MONSTER BATTLE");
      break;
    case 1:
      mvwprintw(battle_win, 2, 2, "PLAYER VS PLAYER BATTLE");
      break;
    case 2:
      mvwprintw(battle_win, 2, 2, "MONSTER VS MONSTER BATTLE");
      break;
    }
    wattroff(battle_win, A_BOLD | COLOR_PAIR(8) |
                             (type == 0 ? players[p1_idx].color_pair : 0));
    
    // ASCII art for left combatant
    if (type == 0 || type == 1) { // Player Bot
      show_battle_art(1, 1, type, 1, p1_idx, type == 0 ? m1_idx : p2_idx);
    } else { // Monster
      show_battle_art(1, 1, type, 0, m1_idx, m2_idx);
    }
    
    //       .... for  right combatant
    if (type == 1) { // Player Bot
      show_battle_art(1, 0, type, 1, p1_idx, p2_idx);
    } else { // Monster
      show_battle_art(1, 0, type, 0, type == 0 ? p1_idx : m1_idx,
                      type == 0 ? m1_idx : m2_idx);
    }
    
    // Display vs text in the middle
    wattron(battle_win, A_BOLD);
    mvwprintw(battle_win, 9, 19, "VS");
    wattroff(battle_win, A_BOLD);
    
    // Display roll boxes
    mvwprintw(battle_win, 14, 5, "╔═════════╗        ╔═════════╗");
    mvwprintw(battle_win, 15, 5, "║         ║        ║         ║");
    mvwprintw(battle_win, 16, 5, "╚═════════╝        ╚═════════╝");
    
    wrefresh(battle_win);
  }
  
  // Calculate battle rolls based on combatant types
  int roll1, roll2;

  // Left combatant roll
  if (type == 0 || type == 1) { // Player
    roll1 = players[p1_idx].strength + (rand() % 6);
  } else { // Monster
    roll1 = monsters[m1_idx].strength + (rand() % 6);
  }

  // Right combatant roll
  if (type == 1) { // Player
    roll2 = players[p2_idx].strength + (rand() % 6);
  } else { // Monster
    roll2 =
        (type == 0 ? monsters[m1_idx].strength : monsters[m2_idx].strength) +
        (rand() % 6);
  }

  if (ShowWindows != 0) {
    // Display die rolls
    if (type == 0 || type == 1) { // Left is player
      wattron(battle_win, COLOR_PAIR(players[p1_idx].color_pair) | A_BOLD);
      mvwprintw(battle_win, 15, 10, "%d", roll1);
      wattroff(battle_win, COLOR_PAIR(players[p1_idx].color_pair) | A_BOLD);
    } else { // Left is monster
      wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
      mvwprintw(battle_win, 15, 10, "%d", roll1);
      wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
    }
    
    if (type == 1) { // Right is player
      wattron(battle_win, COLOR_PAIR(players[p2_idx].color_pair) | A_BOLD);
      mvwprintw(battle_win, 15, 29, "%d", roll2);
      wattroff(battle_win, COLOR_PAIR(players[p2_idx].color_pair) | A_BOLD);
    } else { // Right is monster
      wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
      mvwprintw(battle_win, 15, 29, "%d", roll2);
      wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
    }
    
    wrefresh(battle_win);
  }
  
  // Determine winner and loser
  int left_wins = 0;

  // For player vs monster, player wins on tie
  if (type == 0) {
    left_wins = (roll1 >= roll2);
  }
  // For player vs player or monster vs monster, higher roll wins
  // In case of tie, first combatant wins (arbitrary rule)
  else {
    left_wins =
      (roll1 > roll2 ||
      (roll1 == roll2 && 
      (type == 1 ? combatant1_idx > combatant2_idx
      : monsters[m1_idx].strength >=
      monsters[m2_idx].strength)));
  }

  // Display result and update stats based on battle type
  if (left_wins) {
    // Left combatant wins
    if (type == 0) {
      // Player vs monster - player wins
      show_battle_art(0, 1, type, 1, p1_idx, m1_idx);
    } else if (type == 1) {
      // Player vs player - left player wins
      show_battle_art(0, 1, type, 1, p1_idx, p2_idx);
    } else {
      // Monster vs monster - left monster wins
      show_battle_art(0, 1, type, 0, m1_idx, m2_idx);
    }
  } else {
    // Right combatant wins
    if (type == 0) {
      // Player vs monster - monster wins
      show_battle_art(0, 0, type, 0, p1_idx, m1_idx);
    } else if (type == 1) {
      // Player vs player - right player wins
      show_battle_art(0, 0, type, 1, p1_idx, p2_idx);
    } else {
      // Monster vs monster - right monster wins
      show_battle_art(0, 0, type, 0, m1_idx, m2_idx);
    }
  }

  if (ShowWindows != 0) {
    wrefresh(battle_win);
    pauseForUser(); 
    
    // Clean up battle window
    delwin(battle_win);
    battle_win = NULL; // Good practice to avoid dangling pointers
  }

  // Redraw the main screen
  print_maze();
  display_player_stats();

  // Return battle result
  if (type == 0) {
    return left_wins; // For player vs monster: 1 if player won, 0 if monster
                      // won
  } else if (type == 1) {
    return left_wins ? combatant1_idx : combatant2_idx; // Return winner's ID
  } else {
    return left_wins ? combatant1_idx
                     : combatant2_idx; // Return winner monster index
  }
}

// Convenience wrapper for player vs monster battles
int battle_bot_monster(int monster_index, int player_id) {
  // For player vs monster, combatant1 is player, combatant2 is monster
  return battle_unified(player_id, monster_index, 0);
}

// Convenience wrapper for player vs player battles
int battle_bots(int player1_id, int player2_id) {
  return battle_unified(player1_id, player2_id, 1);
}

// Convenience wrapper for monster vs monster battles
int battle_monsters(int monster1_idx, int monster2_idx) {
  battle_unified(monster1_idx, monster2_idx, 2);
  return 0; // Return value not used for monster vs monster
}

void create_maze(int rows, int cols) {
  maze = (Maze *)malloc(sizeof(Maze));
  if (!maze)
    return;

  maze->rows = rows;
  maze->cols = cols;

  // Allocate memory for the grid
  maze->grid = (char **)malloc(rows * sizeof(char *));
  if (!maze->grid) {
    free(maze);
    return;
  }

  for (int i = 0; i < rows; i++) {
    maze->grid[i] = (char *)malloc(cols * sizeof(char));
    if (!maze->grid[i]) {
      // Clean up already allocated memory
      for (int j = 0; j < i; j++) {
        free(maze->grid[j]);
      }
      free(maze->grid);
      free(maze);
      return;
    }

    for (int j = 0; j < cols; j++) {
      maze->grid[i][j] = WALL;
    }
  }

  // Create separate visited grids for each player
  for (int p = 0; p < NUM_PLAYERS; p++) {
    maze->visited[p] = (char **)malloc(rows * sizeof(char *));
    if (!maze->visited[p]) {
      // Clean up already allocated memory
      for (int i = 0; i < rows; i++) {
        free(maze->grid[i]);
      }
      free(maze->grid);
      for (int j = 0; j < p; j++) {
        for (int i = 0; i < rows; i++) {
          free(maze->visited[j][i]);
        }
        free(maze->visited[j]);
      }
      free(maze);
      return;
    }

    for (int i = 0; i < rows; i++) {
      maze->visited[p][i] = (char *)malloc(cols * sizeof(char));
      if (!maze->visited[p][i]) {
        // Clean up already allocated memory
        for (int j = 0; j < i; j++) {
          free(maze->visited[p][j]);
        }
        free(maze->visited[p]);
        for (int j = 0; j < p; j++) {
          for (int k = 0; k < rows; k++) {
            free(maze->visited[j][k]);
          }
          free(maze->visited[j]);
        }
        for (int j = 0; j < rows; j++) {
          free(maze->grid[j]);
        }
        free(maze->grid);
        free(maze);
        return;
      }

      // Initialize visited grid to unvisited
      for (int j = 0; j < cols; j++) {
        maze->visited[p][i][j] = 0;
      }
    }
  }

  return;
}

void free_maze() {
  if (!maze)
    return;

  if (maze->grid) {
    for (int i = 0; i < maze->rows; i++) {
      if (maze->grid[i]) {
        free(maze->grid[i]);
      }
    }
    free(maze->grid);
  }

  // Free the separate visited grids
  for (int p = 0; p < NUM_PLAYERS; p++) {
    if (maze->visited[p]) {
      for (int i = 0; i < maze->rows; i++) {
        if (maze->visited[p][i]) {
          free(maze->visited[p][i]);
        }
      }
      free(maze->visited[p]);
    }
  }

  free(maze);
}

// Implement ensure_path_between_corners to guarantee connectivity
void ensure_path_between_corners() {
  // Define the four corners
  Position corners[4] = {
      {1, 1, -1, -1},                          // Top-left
      {maze->cols - 2, 1, -1, -1},             // Top-right
      {1, maze->rows - 2, -1, -1},             // Bottom-left
      {maze->cols - 2, maze->rows - 2, -1, -1} // Bottom-right
  };

  // For each pair of corners, ensure a path exists
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      // Create a temporary grid to track visited cells for this pathfinding
      // attempt
      char **visited = (char **)malloc(maze->rows * sizeof(char *));
      if (!visited)
        continue;

      for (int r = 0; r < maze->rows; r++) {
        visited[r] = (char *)malloc(maze->cols * sizeof(char));
        if (!visited[r]) {
          for (int q = 0; q < r; q++) {
            free(visited[q]);
          }
          free(visited);
          continue;
        }

        for (int c = 0; c < maze->cols; c++) {
          visited[r][c] = 0;
        }
      }

      // Use BFS to find a path from corner i to corner j
      Node *queue = NULL;
      push_stack(&queue, corners[i]);
      visited[corners[i].y][corners[i].x] = 1;

      int path_found = 0;

      while (!is_empty(queue) && !path_found) {
        Position current = pop_stack(&queue);

        // Check if we've reached the destination corner
        if (current.x == corners[j].x && current.y == corners[j].y) {
          path_found = 1;
          break;
        }

        // Try all four directions
        for (int dir = 0; dir < 4; dir++) {
          int newX = current.x + Base_dx[dir];
          int newY = current.y + Base_dy[dir];

          // Check if valid cell and not visited and not a wall
          if (newX > 0 && newX < maze->cols - 1 && newY > 0 &&
              newY < maze->rows - 1 && !visited[newY][newX] &&
              maze->grid[newY][newX] != WALL) {

            visited[newY][newX] = 1;
            Position newPos = {newX, newY, current.x, current.y};
            push_stack(&queue, newPos);
          }
        }
      }

      // If no path found, create one
      if (!path_found) {
        // Start at corner i, carve a path toward corner j
        int x = corners[i].x;
        int y = corners[i].y;
        int destX = corners[j].x;
        int destY = corners[j].y;

        while (x != destX || y != destY) {
          // Move in the direction of the destination
          if (x < destX)
            x++;
          else if (x > destX)
            x--;

          if (y < destY)
            y++;
          else if (y > destY)
            y--;

          // Carve path
          maze->grid[y][x] = PATH;
        }
      }

      // Clean up
      for (int r = 0; r < maze->rows; r++) {
        free(visited[r]);
      }
      free(visited);
      clear_stack(&queue);
    }
  }
}

// generate_maze to ensure paths between all corners
void generate_maze() {
  // First, create a basic maze using DFS
  // Initialize stack for DFS
  Node *stack = NULL;
  Position start = {1, 1, -1, -1};
  push_stack(&stack, start); 

  maze->grid[start.y][start.x] = PATH;

  while (!is_empty(stack)) {
    Position current = pop_stack(&stack);

    // Get unvisited neighbors
    int unvisited[4] = {0};
    int count = 0;

    for (int dir = 0; dir < 4; dir++) {
      int newX = current.x + Base_dx[dir] * 2;
      int newY = current.y + Base_dy[dir] * 2;

      if (newX > 0 && newX < maze->cols - 1 && newY > 0 &&
          newY < maze->rows - 1 && maze->grid[newY][newX] == WALL) {
        unvisited[count++] = dir;
      }
    }

    if (count > 0) {
      // Push current cell back onto stack
      push_stack(&stack, current);

      // Choose random unvisited neighbor
      int randDir = unvisited[rand() % count];
      int newX = current.x + Base_dx[randDir] * 2;
      int newY = current.y + Base_dy[randDir] * 2;

      // Remove wall between current cell and chosen cell
      maze->grid[current.y + Base_dy[randDir]][current.x + Base_dx[randDir]] =
          PATH;

      // Mark the chosen cell as part of the path
      maze->grid[newY][newX] = PATH;

      // Push chosen cell onto stack
      Position newPos = {newX, newY, -1, -1};
      push_stack(&stack, newPos);
    }
  }

  // Free any remaining stack memory
  clear_stack(&stack);
}

//////////////////////////////////////////////////////
// function to use player-specific direction arrays
void solve_maze_multi() {
  // Create separate stacks for each player to ensure fairness
  Node *stacks[NUM_PLAYERS] = {NULL, NULL, NULL, NULL};

  // This is for tracking player collisions
  Position previous_positions[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; i++) {
    // Initialize with invalid positions
    previous_positions[i].x = -1;
    previous_positions[i].y = -1;
  }

  // Push each player's starting position onto their stack
  for (int p = 0; p < NUM_PLAYERS; p++) {
    push_stack(&stacks[p], players[p].start);
    // Mark start position as visited
    maze->visited[p][players[p].start.y][players[p].start.x] = 1;
  }

  //////////////////////////////////////////////////
  // Start Main solve loop - continue until all players finish or all stacks are empty
  while (Players_finished < NUM_PLAYERS) {
    // Check if all stacks are empty (no more moves for any player)
    int all_empty = 1;
    for (int p = 0; p < NUM_PLAYERS; p++) {
      if (!is_empty(stacks[p]) && !players[p].reached_goal &&
          !players[p].abandoned_race) {
        all_empty = 0;
        break;
      }
    }

    // After checking if all stacks are empty:
    if (all_empty) {
      // Before breaking, mark any players who didn't finish as having abandoned
      for (int p = 0; p < NUM_PLAYERS; p++) {
        if (!players[p].reached_goal && !players[p].abandoned_race) {
          players[p].abandoned_race = 1;
          highlight_player_solution_path(p);
          display_player_alert(p + 1, -1);
        }
      }
      break;
    }

    /////////////////////////////////////////
    // Start Rotate through players, giving each a turn to move one step
    for (int p = 0; p < NUM_PLAYERS; p++) {
      // Skip players who have already reached their goal or abandoned
      if (players[p].reached_goal || players[p].abandoned_race)
        continue;

      // Check if this player's stack is empty
      if (is_empty(stacks[p])) {
        // Mark player as abandoned if their stack is empty and they haven't
        // reached their goal
        if (!players[p].abandoned_race && !players[p].reached_goal) {
          players[p].abandoned_race = 1;
          highlight_player_solution_path(p);
          display_player_alert(p + 1, -1);

          // Update player stats display to show the new status
          display_player_stats();
        }
        continue;
      }

      int player_id = p + 1;
      Position current = pop_stack(&stacks[p]);

      // Update current position
      players[p].current = current;

      // Check for player vs player collision
      for (int other_p = 0; other_p < NUM_PLAYERS; other_p++) {
        // Skip self and players who've reached their goal or abandoned
        if (other_p == p || players[other_p].reached_goal ||
            players[other_p].abandoned_race) {
          continue;
        }

        // Check if players collide
        if (players[p].current.x == players[other_p].current.x &&
            players[p].current.y == players[other_p].current.y) {

          // Initiate player vs player battle
          int winner_id = battle_bots(player_id, other_p + 1);

          // Loser must retreat to previous position
          if (winner_id == player_id) {
            // Other player retreats to previous position
            if (previous_positions[other_p].x != -1) {
              players[other_p].current = previous_positions[other_p];
            }
            // Check if player has lost too many battles
            if (players[other_p].battles_lost >= 3) {
              players[other_p].abandoned_race = 2;
              highlight_player_solution_path(other_p);
              display_player_alert(other_p + 1, -2);

              // Clear the stack to stop the player's exploration
              clear_stack(&stacks[other_p]);
            }
            continue;

          } else if (winner_id < 0) {
            ; // no battle took place
          } else {
            // Current player retreats to parent position
            players[p].current.x = current.parentX;
            players[p].current.y = current.parentY;

            // If no parent position, stay at current but mark as visited to
            // avoid revisiting
            if (players[p].current.x == -1 || players[p].current.y == -1) {
              players[p].current = current;
              maze->visited[p][current.y][current.x] = 1;
            }
            // Check if player has lost too many battles
            if (players[p].battles_lost >= 3) {
              players[p].abandoned_race = 2;
              highlight_player_solution_path(p);
              display_player_alert(player_id, -2);

              // Clear the stack to stop the player's exploration
              clear_stack(&stacks[p]);
            }
            continue;
          }

          // Update display after battle
          print_maze();
          display_player_stats();
        }
      }

      // Store current position for potential retreat
      previous_positions[p].x = players[p].current.x;
      previous_positions[p].y = players[p].current.y;

      players[p].moves++;

      // Check if reached the end
      if (current.x == players[p].end.x && current.y == players[p].end.y) {
        players[p].reached_goal = 1;
        Players_finished++;
        players[p].finished_rank = Players_finished;

        // Mark as finished in UI
        highlight_player_solution_path(p);
        display_player_alert(player_id, players[p].finished_rank);

        continue;
      }

      // Check for teleporter
      int newX, newY;
      int teleporter_idx = -1;

      // Find teleporter index if on one
      for (int i = 0; i < Num_teleporters; i++) {
        if ((teleporters[i].x1 == current.x &&
             teleporters[i].y1 == current.y) ||
            (teleporters[i].x2 == current.x &&
             teleporters[i].y2 == current.y)) {
          teleporter_idx = i;
          break;
        }
      }

      // Handle teleportation
      if (!players[p].justTeleported && teleporter_idx >= 0 &&
          check_teleporter(current.x, current.y, &newX, &newY)) {

        // Record teleportation
        players[p].justTeleported = Game_moves + 1;

        Position teleported = {newX, newY, current.x, current.y};
        push_stack(&stacks[p], teleported);
        parent_map[p][newY][newX].x = current.x;
        parent_map[p][newY][newX].y = current.y;

        // Mark destination as visited
        maze->visited[p][newY][newX] = 1;

        // Visualize teleportation TODO
        //   FLICKER current.x, current.y, 
        //   FLICKER newX, newY
        continue;
      }

      // Check for monster
      int monster_idx = check_monster(current.x, current.y);
      if (monster_idx > 0) {
        monster_idx--; // Adjust index (monster_idx was returned +1)
        int battle_result = battle_bot_monster(monster_idx, player_id);

        if (battle_result == 0) {
          // Player lost battle, mark position as visited in player's array
          maze->visited[p][current.y][current.x] = 1;

          // Also update the visualization
          char visited_char = get_player_visited_char(player_id);
          if (
              maze->grid[current.y][current.x] != END1 &&
              maze->grid[current.y][current.x] != END2 &&
              maze->grid[current.y][current.x] != END3 &&
              maze->grid[current.y][current.x] != END4 &&
              maze->grid[current.y][current.x] != TELEPORTER) {
            maze->grid[current.y][current.x] = visited_char;
          }

          // Check if player has lost too many battles
          if (players[p].battles_lost >= 3) {
            players[p].abandoned_race = 2;
            highlight_player_solution_path(p);
            display_player_alert(player_id, -2);

            // Clear the stack to stop the player's exploration
            clear_stack(&stacks[p]);
          }

          continue;
        } else if (battle_result < 0) {
          ; // no battle took place
        }
      }

      // Mark as visited in player's separate visited array
      maze->visited[p][current.y][current.x] = 1;

      // Also update the visualization in the shared grid
      char visited_char = get_player_visited_char(player_id);
      if (
          maze->grid[current.y][current.x] != END1 &&
          maze->grid[current.y][current.x] != END2 &&
          maze->grid[current.y][current.x] != END3 &&
          maze->grid[current.y][current.x] != END4 &&
          maze->grid[current.y][current.x] != TELEPORTER &&
          maze->grid[current.y][current.x] != MONSTER &&
          maze->grid[current.y][current.x] != DEFEATED_MONSTER) {
        maze->grid[current.y][current.x] = visited_char;
      }

      // start Try all possible directions using player-specific direction arrays
      for (int i = 0; i < 4; i++) {
        int dir_idx = i;
        int nextX = current.x + players[p].dx[dir_idx];
        int nextY = current.y + players[p].dy[dir_idx];

        // Don't go back to parent
        if (nextX == current.parentX && nextY == current.parentY) {
          continue;
        }

        if (nextX >= 0 && nextX < maze->cols && nextY >= 0 &&
            nextY < maze->rows) {
          char cell = maze->grid[nextY][nextX];

          // Allow exploring if not a wall and not visited by THIS player
          if (cell != WALL && maze->visited[p][nextY][nextX] == 0) {
            maze->visited[p][nextY][nextX] =
                1; // Mark as visited for this player

            Position nextPos = {nextX, nextY, current.x, current.y};
            push_stack(&stacks[p], nextPos);
            parent_map[p][nextY][nextX].x = current.x;
            parent_map[p][nextY][nextX].y = current.y;
          }
        }
      }
      // end Try all possible directions
    }
    // end Rotate through players
    //////////////////////////////////////////

    // Update monsters occasionally
    if (Game_moves % 5 == 0) {
      update_monsters();
    }

    // Visualize exploration after each player's move
    print_maze();
    display_player_stats();
    read_keyboard();
    if (LastSLupdate && LastSLupdate+25 < Game_moves) {
      move(maze->rows + 5, 0);
      wclrtoeol(stdscr);
    }
    mysleep(Game_delay);    

    // Update game moves
    Game_moves++;
    for (int i = 0; i < NUM_PLAYERS; i++) {
      // prevent teleport loops
      if (players[i].justTeleported < Game_moves)
        players[i].justTeleported = 0;
      // decrement recovery_turns
      if (players[i].recovery_turns)
        players[i].recovery_turns--;
    }

    // Check for window resize
    if (1) {
      struct winsize size;

      ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); // Get new size
      if (old_rows != size.ws_row || old_cols != size.ws_col) {
        update_status_line("Caught term resize to %d, %d", size.ws_row, size.ws_col);
        logMessage("Caught term resize to %d, %d", size.ws_row, size.ws_col);
        if (old_rows > size.ws_row || old_cols > size.ws_col) {
          // abort current solve as screen has reduced in size
          Screen_reduced = 1;
          old_rows = size.ws_row ; old_cols = size.ws_col; 
          // spin wheels in lieu of debounce
          mysleep(2000);
          // break
          break;
        }
        old_rows = size.ws_row ; old_cols = size.ws_col;      
      }
    }
  }
  // end Main solve loop
  //////////////////////////////////////////////////

  // Free stacks
  for (int p = 0; p < NUM_PLAYERS; p++) {
    clear_stack(&stacks[p]);
  }

  // Show final results
  if(!Screen_reduced) display_player_stats();
}
// end solve_maze_multi
//////////////////////////////////////////////////////

// print_maze to display all players, teleporters, and monsters
void print_maze() {
  // Get current terminal dimensions
  int term_rows, term_cols;
  getmaxyx(stdscr, term_rows, term_cols);

  // Ensure we don't write outside the terminal
  int visible_rows = (maze->rows < term_rows - 6) ? maze->rows : term_rows - 6;
  int visible_cols = (maze->cols < term_cols) ? maze->cols : term_cols - 1;

  Position current_positions[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; i++) {
    current_positions[i] = players[i].current;
  }

  for (int i = 0; i < visible_rows; i++) {
    for (int j = 0; j < visible_cols; j++) {
      move(i, j);

      // Check if this is a player's current position
      int player = is_player_position(j, i, current_positions);

      if (player > 0) {
        // Show player marker
        attron(COLOR_PAIR(players[player - 1].color_pair) | A_BOLD);
        switch (player) {
        case 1:
          addstr(CURRENT_CHAR1);
          break;
        case 2:
          addstr(CURRENT_CHAR2);
          break;
        case 3:
          addstr(CURRENT_CHAR3);
          break;
        case 4:
          addstr(CURRENT_CHAR4);
          break;
        }
        attroff(COLOR_PAIR(players[player - 1].color_pair) | A_BOLD);
        continue;
      }

      // Otherwise, display the appropriate cell character
      switch (maze->grid[i][j]) {
      case WALL:
        addstr(WALL_CHAR);
        break;
      case PATH:
        attron(COLOR_PAIR(1));
        addstr(PATH_CHAR);
        attroff(COLOR_PAIR(1));
        break;

      // Player 1 cells
      case VISITED1:
        attron(COLOR_PAIR(2));
        addstr(VISITED_CHAR1);
        attroff(COLOR_PAIR(2));
        break;
      case SOLUTION1:
        attron(COLOR_PAIR(2) | A_BOLD);
        addstr(SOLUTION_CHAR1);
        attroff(COLOR_PAIR(2) | A_BOLD);
        break;
      case END1:
        attron(COLOR_PAIR(2) | A_BOLD);
        addstr(END_CHAR1);
        attroff(COLOR_PAIR(2) | A_BOLD);
        break;

      // Player 2 cells
      case VISITED2:
        attron(COLOR_PAIR(3));
        addstr(VISITED_CHAR2);
        attroff(COLOR_PAIR(3));
        break;
      case SOLUTION2:
        attron(COLOR_PAIR(3) | A_BOLD);
        addstr(SOLUTION_CHAR2);
        attroff(COLOR_PAIR(3) | A_BOLD);
        break;
      case END2:
        attron(COLOR_PAIR(3) | A_BOLD);
        addstr(END_CHAR2);
        attroff(COLOR_PAIR(3) | A_BOLD);
        break;

      // Player 3 cells
      case VISITED3:
        attron(COLOR_PAIR(4));
        addstr(VISITED_CHAR3);
        attroff(COLOR_PAIR(4));
        break;
      case SOLUTION3:
        attron(COLOR_PAIR(4) | A_BOLD);
        addstr(SOLUTION_CHAR3);
        attroff(COLOR_PAIR(4) | A_BOLD);
        break;
      case END3:
        attron(COLOR_PAIR(4) | A_BOLD);
        addstr(END_CHAR3);
        attroff(COLOR_PAIR(4) | A_BOLD);
        break;

      // Player 4 cells
      case VISITED4:
        attron(COLOR_PAIR(5));
        addstr(VISITED_CHAR4);
        attroff(COLOR_PAIR(5));
        break;
      case SOLUTION4:
        attron(COLOR_PAIR(5) | A_BOLD);
        addstr(SOLUTION_CHAR4);
        attroff(COLOR_PAIR(5) | A_BOLD);
        break;
      case END4:
        attron(COLOR_PAIR(5) | A_BOLD);
        addstr(END_CHAR4);
        attroff(COLOR_PAIR(5) | A_BOLD);
        break;

      // Special cells
      case TELEPORTER:
        attron(COLOR_PAIR(7) | A_BOLD);
        addstr(TELEPORTER_CHAR);
        attroff(COLOR_PAIR(7) | A_BOLD);
        break;
      case MONSTER:
        attron(COLOR_PAIR(8) | A_BOLD);
        addstr(MONSTER_CHAR);
        attroff(COLOR_PAIR(8) | A_BOLD);
        break;
      case DEFEATED_MONSTER:
        attron(COLOR_PAIR(9) | A_BOLD);
        addstr(DEFEATED_MONSTER_CHAR);
        attroff(COLOR_PAIR(9) | A_BOLD);
        break;
      default:
        attron(COLOR_PAIR(8));
        addstr("!");
        attroff(COLOR_PAIR(8));
        break;
      }
    }
  }
  // print game info
  attron(COLOR_PAIR(11) | A_BOLD);
  mvwprintw(stdscr, 0, 1,
           "[Maze Game] %d s, %d t, %d/%d [%d] m, %d/%d r",
           Game_speed, Num_teleporters, Liv_monsters, Num_monsters,
           Max_monster_strength, Game_rounds, Game_roundsB);
  attroff(COLOR_PAIR(11) | A_BOLD);
}

// Stack operations
__attribute__((no_instrument_function))
void push_stack(Node **stack, Position pos) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (!node)
    return; // Handle memory allocation failure

  node->pos = pos;
  node->next = *stack;
  *stack = node;
}

__attribute__((no_instrument_function))
Position pop_stack(Node **stack) {
  if (*stack == NULL) {
    Position empty = {-1, -1, -1, -1};
    return empty;
  }

  Node *temp = *stack;
  *stack = (*stack)->next;
  Position pos = temp->pos;
  free(temp);
  return pos;
}

__attribute__((no_instrument_function))
int is_empty(Node *stack) { 
  return stack == NULL; 
}

// Clear the entire stack and free memory
void clear_stack(Node **stack) {
  while (!is_empty(*stack)) {
    pop_stack(stack);
  }
}

/**
 * Animates a collapsing/expanding bullseye at the specified coordinates
 *
 * @param y The y-coordinate (row) for the center of the bullseye
 * @param x The x-coordinate (column) for the center of the bullseye
 * @param max_radius The maximum radius of the bullseye
 * @param delay_ms The delay in milliseconds between animation frames
 * @param direction The direction of the animation (0 = collapse, 1 = expand)
 */
void animate_bullseye(int y, int x, int max_radius, int delay_ms, int direction) {
  // UTF-8 characters for different rings of the bullseye
  const wchar_t *ring_chars[] = {
      L"●", // Filled circle
      L"◉", // Bullseye
      L"◎", // Circle with dot
      L"○", // Empty circle
      L"·"  // Small dot
  };
  const int num_rings = sizeof(ring_chars) / sizeof(ring_chars[0]);

  // Save current colors
  int old_pair = PAIR_NUMBER(getattrs(stdscr));

  // Initialize color pairs if they haven't been already
  if (!has_colors()) {
    wprintw(stdscr, "Your terminal does not support colors\n");
    return;
  }

  // Clear any previous content
  // clear();
  wrefresh(stdscr);

  int start, end, inc;
  if (direction) {
    start = max_radius;
    end = 0;
    inc = -1;
  } else {
    start = 0;
    end = max_radius;
    inc = 1;
  }

  // Animate the bullseye from max_radius down to 0
  for (int current_radius = start;
       (inc > 0) ? (current_radius <= end) : (current_radius >= end);
       current_radius += inc) {
    // clear();

    // Draw each ring of the bullseye
    for (int r = current_radius; r >= 0; r--) {
      // Select color based on ring position
      int color_pair = (r % 5) + 1;
      attron(COLOR_PAIR(color_pair));

      // Select character based on position in the bullseye
      const wchar_t *ring_char = ring_chars[r % num_rings];

      // For the innermost ring (when r=0), always use the filled circle
      if (r == 0) {
        ring_char = ring_chars[0];
        attron(COLOR_PAIR(1)); // Red for the center
      }

      // Draw a complete ring at radius r
      for (int i = 0; i <= 360; i += 10) {
        double angle = i * M_PI / 180.0;
        int ring_y = y + (int)(r * sin(angle));
        int ring_x =
            x + (int)(r * 2 * cos(angle)); // Multiply by 2 to account for
                                           // character aspect ratio

        // Make sure we don't draw outside the screen
        if (ring_y >= 0 && ring_y < LINES && ring_x >= 0 && ring_x < COLS-4) {
          // Use wide character functions for UTF-8
          mvaddwstr(ring_y, ring_x, ring_char);
        }
      }

      // Reset attributes
      attroff(COLOR_PAIR(color_pair));
    }

    // Refresh the screen
    wrefresh(stdscr);

    // Delay between frames
    mysleep(delay_ms);
  }

  // Restore original color pair
  attron(COLOR_PAIR(old_pair));
}

// Read high scores from file
int read_high_scores(HighScore best_scores[], HighScore worst_scores[]) {
  FILE *file = fopen(HIGH_SCORE_FILENAME, "rb");
  if (!file) {
    // File doesn't exist yet, which is fine for first run
    return 0;
  }

  // Read the number of high scores stored
  int count = 0;
  fread(&count, sizeof(int), 1, file);

  if (count > MAX_HIGH_SCORES) {
    count = MAX_HIGH_SCORES; // Safety check
  }

  // Read the best scores
  fread(best_scores, sizeof(HighScore), count, file);

  // Read the worst scores
  fread(worst_scores, sizeof(HighScore), count, file);

  fclose(file);
  return count;
}

// Save high scores to file
void save_high_scores(HighScore best_scores[], HighScore worst_scores[], int count) {
  FILE *file = fopen(HIGH_SCORE_FILENAME, "wb");
  if (!file) {
    // Can't print to stderr in ncurses mode, so just return
    return;
  }

  // Write the number of high scores
  fwrite(&count, sizeof(int), 1, file);

  // Clear this_run flags before saving
  for (int i = 0; i < count; i++) {
    best_scores[i].this_run = 0;
    worst_scores[i].this_run = 0;
  }

  // Write the best scores
  fwrite(best_scores, sizeof(HighScore), count, file);

  // Write the worst scores
  fwrite(worst_scores, sizeof(HighScore), count, file);

  fclose(file);
}

// Insert a score into the high score lists
void insert_high_score(HighScore scores[], int *count, HighScore new_score, int is_best) {
  int i, pos = -1;

  // Determine the position to insert based on whether it's best or worst list
  for (i = 0; i < *count; i++) {
    if (is_best) {
      // For best scores, lower moves is better
      if (new_score.score < scores[i].score) {
        pos = i;
        break;
      }
    } else {
      // For worst scores, higher moves is "better" for the worst list
      if (new_score.score > scores[i].score) {
        pos = i;
        break;
      }
    }
  }

  // If didn't find a position and list isn't full yet
  if (pos == -1 && *count < MAX_HIGH_SCORES) {
    pos = *count;
    (*count)++;
  }

  // If found a position to insert
  if (pos != -1) {
    // Shift scores down to make room
    for (i = MAX_HIGH_SCORES - 1; i > pos; i--) {
      if (i - 1 >= 0) {
        scores[i] = scores[i - 1];
      }
    }
    // Insert the new score
    new_score.this_run = 1;
    scores[pos] = new_score;
  }
}

// Update high scores at the end of the game
void update_high_scores(int rows, int cols) {
  int num_players = NUM_PLAYERS;
  HighScore best_scores[MAX_HIGH_SCORES] = {
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0}};
  HighScore worst_scores[MAX_HIGH_SCORES] = {
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0},
      {"                               ", 0, 0, 0, 0, 0, 0}};
  int count = 0;

  // Read existing high scores
  count = read_high_scores(best_scores, worst_scores);

  // Get the current date/time
  time_t current_time = time(NULL);

  // Process players who finished the race
  for (int i = 0; i < num_players; i++) {
    if (players[i].reached_goal && players[i].finished_rank == 1) {
      // Create a new score entry
      HighScore new_score;

      // Use the bot name based on player ID 
      // (subtract 1 for 0-based array index)
      int name_index = players[i].id - 1;
      if (name_index >= 0 && name_index < NUM_PLAYERS) {
        strncpy(new_score.name, BOT_NAMES[name_index + 1],
                sizeof(new_score.name) - 1);
        new_score.name[sizeof(new_score.name) - 1] =
            '\0'; // Ensure null termination
      } else {
        // invalid ID
        exit_game("invalid name_index %d\n", name_index);
      }

      new_score.player_id = players[i].id;
      new_score.score = calculate_score(players[i].moves, cols, rows);
      new_score.battles_won = players[i].battles_won;
      new_score.strength = players[i].strength;
      new_score.date = current_time;

      // Try to insert into best scores
      insert_high_score(best_scores, &count, new_score, 1);

      // Also track the worst scores for fun
      // (only if they actually finished though)
      insert_high_score(worst_scores, &count, new_score, 0);
    }
  }

  // Display the high score window
  display_high_scores_window(count, best_scores, worst_scores);

  // Save updated high scores
  save_high_scores(best_scores, worst_scores, count > 0 ? count : 0);
}

// Display high scores in a ncurses window
void display_high_scores_window(int count, HighScore best_scores[], HighScore worst_scores[]) {
  if (count == 0 || ShowWindows == 0) {
    // No high scores yet
    pauseForUser();
    return;
  }

  char date_str[20];

  // Save current state to restore later
  int old_curs = curs_set(0); // Hide cursor

  // Calculate window dimensions
  int height = MIN_ROWS -4;
  int width  = MIN_COLS;

  // Create a new window centered on screen
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  int y_pos =  max_y == MIN_ROWS ? 0 : 1;

  WINDOW *high_score_win = newwin(height, width, y_pos, (max_x - width) / 2);
  box(high_score_win, 0, 0);

  // Add a title
  wattron(high_score_win, A_BOLD);
  mvwprintw(high_score_win, 1, (width - 27) / 2, "*** BEST & WORST SCORES ***");
  wattroff(high_score_win, A_BOLD);

  // Display best scores section title
  wattron(high_score_win, A_UNDERLINE);
  mvwprintw(high_score_win, 3, 3, "BEST (FASTEST) SCORES");
  wattroff(high_score_win, A_UNDERLINE);

  // Display header
  mvwprintw(high_score_win, 4, 3, "# Name   Scor Ba St Date");

  // Display best scores
  int displayed_count = MIN(count, 5); // Limit displayed scores
  for (int i = 0; i < displayed_count; i++) {
    struct tm *tm_info = localtime(&best_scores[i].date);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
    // Highlight the new high score
    if (best_scores[i].this_run) {
      wattron(high_score_win,
              COLOR_PAIR(players[best_scores[i].player_id - 1].color_pair) |
                  A_BOLD);
    }
                                  // # Name   Score Ba St Date
    mvwprintw(high_score_win, 5 + i, 3, "%1d %-6s %4d %2d %2d %s", i + 1,
              best_scores[i].name, best_scores[i].score,
              best_scores[i].battles_won, best_scores[i].strength, date_str);

    if (best_scores[i].this_run) {
      wattroff(high_score_win,
               COLOR_PAIR(players[best_scores[i].player_id - 1].color_pair) |
                   A_BOLD);
    }
  }

  // Display worst scores section title
  wattron(high_score_win, A_UNDERLINE);
  mvwprintw(high_score_win, 11, 3, "WORST (SLOWEST) SCORES");
  wattroff(high_score_win, A_UNDERLINE);

  // Display header
  mvwprintw(high_score_win, 12, 3, "# Name   Scor Ba St Date");

  // Display worst scores
  for (int i = 0; i < displayed_count; i++) {
    struct tm *tm_info = localtime(&worst_scores[i].date);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
    // Highlight the new high score
    if (worst_scores[i].this_run) {
      wattron(high_score_win,
              COLOR_PAIR(players[worst_scores[i].player_id - 1].color_pair) |
                  A_BOLD);
    }

    mvwprintw(high_score_win, 13 + i, 3, "%1d %-6s %4d %2d %2d %s", i + 1,
              worst_scores[i].name, worst_scores[i].score,
              worst_scores[i].battles_won, worst_scores[i].strength, date_str);

    if (worst_scores[i].this_run) {
      wattroff(high_score_win,
               COLOR_PAIR(players[worst_scores[i].player_id - 1].color_pair) |
                   A_BOLD);
    }
  }

  // If we have a new best score, add some congratulatory text
  if (best_scores[0].this_run) {
    wattron(high_score_win, A_BOLD);
    mvwprintw(high_score_win, (height - 2), (width - 34) / 2,
              "** NEW CHAMPION! %s **", BOT_NAMES[best_scores[0].player_id]);
    wattroff(high_score_win, A_BOLD);
  }

  // If we have a new worst score, add some congratulatory text
  if (worst_scores[0].this_run) {
    wattron(high_score_win, A_BOLD);
    mvwprintw(high_score_win, (height - 2), (width - 34) / 2,
              "** NEW WORST :( %s **", BOT_NAMES[worst_scores[0].player_id]);
    wattroff(high_score_win, A_BOLD);
  }

  // Refresh and show the window
  wrefresh(high_score_win);

  // Wait for user input
  pauseForUser();

  // Clean up
  delwin(high_score_win);
  curs_set(old_curs); // Restore cursor state

  // Refresh the screen to remove the window
  wrefresh(stdscr);
}

#define OPTIMAL_PATH_FACTOR 1.5f
#define SCORE_MULTIPLIER 100.0f
#define COMPLEXITY_ADJUSTMENT_FACTOR 10.0f

int calculate_score(int moves, int width, int height) {
  if (moves <= 0 || width <= 0 || height <= 0) {
    return -1; // Error code for invalid input
  }

  // Calculate the effective maze dimensions (in cells, not walls)
  int effective_width = (int)((float)(width - 1) / 2.0f);
  int effective_height = (int)((float)(height - 1) / 2.0f);

  // Estimate optimal solution length
  float estimated_optimal_path =
      OPTIMAL_PATH_FACTOR * (float)(effective_width + effective_height);

  // Maze complexity factor
  float maze_complexity = (float)(effective_width * effective_height);

  // Calculate score (lower is better)
  // Base score is the actual moves normalized by estimated optimal path
  float efficiency_factor = (float)moves / estimated_optimal_path;

  // Apply a small adjustment based on maze complexity
  // This makes larger mazes slightly easier to score well on
  float complexity_adjustment =
      logf(maze_complexity) / COMPLEXITY_ADJUSTMENT_FACTOR;

  // Final score - lower is better
  int score = (int)roundf(SCORE_MULTIPLIER *
                          (efficiency_factor - complexity_adjustment));

  // Ensure score is always positive
  return score > 0 ? score : 1;
}

void pauseForUser() {
  read_keyboard();
  if (WaitForKey) {
    pauseGame();
  } else {
    update_status_line(DELAY_MSG);
    mysleep(2000);
  }
  move(maze->rows + 5, 0);
  wclrtoeol(stdscr);
  read_keyboard();
}

void calc_game_speed() {
  double min_speed = 1.0;    // slowest
  double max_speed = 100.0;  // fastest
  if (Game_speed < min_speed) {
    Game_speed = DEF_GAME_SPEED;
  }
  if (Game_speed > max_speed) {
    Game_speed = DEF_GAME_SPEED;
  }
  // Map Game_speed logarithmically to [0, 1]
  double normalized_speed =
      (log(Game_speed) - log(min_speed)) / (log(max_speed) - log(min_speed));
  // Calculate delay (inverse relationship with normalized_speed)
  Game_delay =
      static_cast<int>(round((1.0 - normalized_speed) * MAX_GAME_DELAY));
}

void exit_game(const char *format, ...) {
  tcflush(STDIN_FILENO, TCIFLUSH);
  endwin();
  va_list args;
  va_start(args, format);
  vprintf(format, args);  // Forward the varargs to printf
  va_end(args);
  exit(0);
}

void update_status_line(const char *format, ...) {
  int direction = 0;
  char buffer[STATUS_LINE_MAX];
  char upchar[5] = " ";

  if (isdigit(format[0])) {
    // a key
    direction = atoi(format);
  } else {
    // a regular string
    direction = 0;
  }
  // Case 1: Add a new message to history
  if (direction == 0) {
    // get message
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if(strstr(buffer, LOST_MSG)) {
      // Shift all messages down in the array
      for (int i = STATUS_LINE_HISTORY - 1; i > 1; i--) {
        strncpy(status_lines[i].msg, status_lines[i-1].msg, STATUS_LINE_MAX);
        status_lines[i].move = status_lines[i-1].move;
      }
      // Insert new message at the front
      strncpy(status_lines[1].msg, buffer, STATUS_LINE_MAX);
      status_lines[1].move = Game_moves;
    }
    // Reset viewing index to current message
    current_status_index = 0;
  }
  // Case 2: Navigate to previous (older) message
  else if (direction == KEY_UP) {
    // Move up in history if there's a valid message
    if (
      current_status_index < STATUS_LINE_HISTORY - 1 && 
      strlen(status_lines[current_status_index + 1].msg) > 0
    ) {
      current_status_index++;
    }
  }
  // Case 3: Navigate to next (newer) message
  else if (direction == KEY_DOWN) {
    // Move down in history if not already at newest
    if (current_status_index > 0) {
      current_status_index--;
    }
  } else {
    exit_game("Illegal value for direction %d\n", direction);
  }
  
  attron(COLOR_PAIR(11) | A_BOLD);
  if (current_status_index == 0) { 
    // Current message - display normally
    if (direction == 0) {
      LastSLupdate = Game_moves;
      mvwprintw(stdscr, maze->rows + 5, 0, "%s", buffer);
    } else {
      mvwprintw(stdscr, maze->rows + 5, 0, "%s", PAUSE_MSG);
    }
  } else {
    // Past message - show indicator
    if (
      current_status_index < STATUS_LINE_HISTORY - 1 && 
      strlen(status_lines[current_status_index + 1].msg) > 0
    ) {
      strcpy(upchar , "↑");
    }        
    mvwprintw(stdscr, maze->rows + 5, 0, "%s↓[%d] %s", upchar, status_lines[current_status_index].move, status_lines[current_status_index].msg);
  }
  wclrtoeol(stdscr);
  attroff(COLOR_PAIR(11) | A_BOLD);
  wrefresh(stdscr);  
}

__attribute__((no_instrument_function))
void read_keyboard() {
  int ch;
  
  if(in_read_keyboard) return;
  in_read_keyboard =1;
  nodelay(stdscr, TRUE);
  ch = getch();
  nodelay(stdscr, FALSE);
  switch(ch) {
  // PAUSE with SPACE
  case 32:
    pauseGame();
    break;
  // SLOW DOWN with MINUS
  case 95:
  case 45:
    update_status_line("Slowing down...");
    Game_speed--;
    calc_game_speed();
    break;
  // SPEED UP with PLUS
  case 61:
  case 43:
    update_status_line("Speeding up...");
    Game_speed++;
    calc_game_speed();
    break;
  // EXIT with ESCAPE
  case 113:
  case 81:
  case 27:
    exit_game("User ended game early\n");
    break;
  case KEY_RESIZE:
    break;        
  case ERR:
    break;  
  case KEY_MOUSE:
//    MEVENT event;
//    if (getmouse(&event) == OK) {
//        // Check if it's a mousewheel event
//        if (event.bstate & BUTTON4_PRESSED) {
//            if (event.bstate & BUTTON_CTRL) {
//              ;
//            }
//        }
//        else if (event.bstate & BUTTON5_PRESSED) {
//            if (event.bstate & BUTTON_CTRL) {
//              ;
//            }
//        }
//    }          
    break;  
  default:
    update_status_line("Unrecognized key %d", ch);
  }
  in_read_keyboard =0;
}

void pauseGame() {
  char key[5];
  int paused;
  int ch;

    current_status_index = 0; // Reset to current message when pausing
    update_status_line(PAUSE_MSG);
    tcflush(STDIN_FILENO, TCIFLUSH);
    paused = 1;
    
    // Enter pause mode loop
    while (paused) {
      ch = getch();
      
      // Handle arrow keys for status history browsing
      if (ch == KEY_UP || ch == 259) {
        // Move to older message
        sprintf(key, "%d", KEY_UP);
        update_status_line(key);
      } 
      else if (ch == KEY_DOWN || ch == 258) {
        // Move to newer message
        sprintf(key, "%d", KEY_DOWN);
        update_status_line(key);
      }
      // Exit game early
      else if(ch == 113 || ch == 81 || ch == 27) {
        exit_game("User ended game early\n");
      }
      // Leave pause mode on any other key press (that's printable)
      else if (ch >= 32 && ch <= 126) {
        paused = 0;
      }
    }
    // Reset to current message when unpausing
    current_status_index = 0;
    update_status_line("Continuing...");
    tcflush(STDIN_FILENO, TCIFLUSH);
}

// for debugging
void logMessage(const char *format, ...) {
#ifdef DEBUG
  FILE *logFile = fopen(LOG_FILENAME, "a");

  if (logFile != NULL) {
    time_t now = time(NULL);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
             localtime(&now));

    fprintf(logFile, "[%s] ", timestamp);

    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fclose(logFile);
  } else {
    fprintf(stderr, "Error: Could not open log file: %s\n", LOG_FILENAME);
  }
#else
  format = format;
#endif
}

__attribute__((no_instrument_function))
void sleep_millis(long ms) {
    struct timespec req, rem;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000L;

    while (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR) {
            req = rem;
        } else {
            perror("nanosleep");
            break;
        }
    }
}

__attribute__((no_instrument_function))
void delay_with_polling(long total_delay_ms) {
    struct timespec start, end;
    long read_duration_ms;
    long sleep_time_ms;
    long remaining_time_ms = total_delay_ms;

    do {
        if(remaining_time_ms > POLL_INTERVAL_MS) {
          // do read keyboard
          clock_gettime(CLOCK_MONOTONIC, &start);
          read_keyboard();
          clock_gettime(CLOCK_MONOTONIC, &end);
          // calc how long that took
          read_duration_ms =
              (end.tv_sec - start.tv_sec) * 1000 +
              (end.tv_nsec - start.tv_nsec) / 1000000;
          // calc time to next poll
          sleep_time_ms = POLL_INTERVAL_MS - read_duration_ms;
          // don't overshoot remaining time
          if (sleep_time_ms > remaining_time_ms) {
              sleep_time_ms = remaining_time_ms;
          }
        } else {
          sleep_time_ms = remaining_time_ms;
        }
        // do the sleep
        if (sleep_time_ms > 0) {
            sleep_millis(sleep_time_ms);
        }
        // calc remaining sleep time
        remaining_time_ms -= sleep_time_ms;
    } while (remaining_time_ms > 0);
}

__attribute__((no_instrument_function))
void mysleep(long total_delay_ms) {
  if(in_mysleep) return;
  in_mysleep = 1;  
  if (total_delay_ms <= POLL_INTERVAL_MS) {
      sleep_millis(total_delay_ms);
  } else {
      delay_with_polling(total_delay_ms);
  }
  in_mysleep = 0;
}

// function to mark the solution path for a player
void highlight_player_solution_path(int p) {
  // Now mark each position in the path with solution char
  char solution_char = get_player_solution_char(p+1);
  Position end = players[p].current;
  int x = end.x, y = end.y;

  while (!(x == players[p].start.x && y == players[p].start.y)) {
    // Only overwrite certain cells (don't overwrite special cells)
    if (maze->grid[y][x] != END1 &&
        maze->grid[y][x] != END2 &&
        maze->grid[y][x] != END3 &&
        maze->grid[y][x] != END4 &&
        maze->grid[y][x] != TELEPORTER &&
        maze->grid[y][x] != MONSTER &&
        maze->grid[y][x] != DEFEATED_MONSTER
    ) {
      maze->grid[y][x] = solution_char;
    }    int px = parent_map[p][y][x].x;
    int py = parent_map[p][y][x].y;
    x = px;
    y = py;
  }
  print_maze();
  display_player_stats();
}

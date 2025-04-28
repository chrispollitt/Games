// CONSTANTS /////////////////////////////////////////////////////

// UTF-8 characters for maze elements
const CURRENT_CHAR1 = "①";
const CURRENT_CHAR2 = "②";
const CURRENT_CHAR3 = "③";
const CURRENT_CHAR4 = "④";
const VISITED_CHAR1 = "◇"; // RED
const VISITED_CHAR2 = "◇"; // CYAN
const VISITED_CHAR3 = "◇"; // YELLOW
const VISITED_CHAR4 = "◇"; // GREEN
const SOLUTION_CHAR1 = "◆";
const SOLUTION_CHAR2 = "◆";
const SOLUTION_CHAR3 = "◆";
const SOLUTION_CHAR4 = "◆";
const END_CHAR1 = "R"; // raspberry
const END_CHAR2 = "B"; // blueberry
const END_CHAR3 = "L"; // lemon
const END_CHAR4 = "K"; // kiwi
const TELEPORTER_CHAR = "◎";
const MONSTER_CHAR = "☠";
const DEFEATED_MONSTER_CHAR = "†";
const WALL_CHAR = "▒";
const PATH_CHAR = " ";

// Internal representations
const CURRENT1 = 'a';
const CURRENT2 = 'b';
const CURRENT3 = 'c';
const CURRENT4 = 'd';
const VISITED1 = 'e';
const VISITED2 = 'f';
const VISITED3 = 'g';
const VISITED4 = 'h';
const SOLUTION1 = 'i';
const SOLUTION2 = 'j';
const SOLUTION3 = 'k';
const SOLUTION4 = 'l';
const END1 = '1';
const END2 = '2';
const END3 = '3';
const END4 = '4';
const TELEPORTER = 'T';
const MONSTER = 'M';
const DEFEATED_MONSTER = 'N';
const WALL = '#';
const PATH = ' ';

// mins, maxes, etc.
const MIN_ROWS = 25;
const MIN_COLS = 40;
const MAX_ROWS = 1024;
const MAX_COLS = 1024;
const DEF_GAME_SPEED = 70;
const MAX_GAME_DELAY = 200;
const DEF_TELEPORTER_DENSITY = 1000;
const MAX_TELEPORTERS = 10;
const DEF_MONSTER_DENSITY = 500;
const MAX_MONSTERS = 26;
const DEF_MONSTER_STRENGTH = 10;
const MAX_MONSTER_STRENGTH = 15;
const DEF_PLAYER_STRENGTH  = 6;
const MAX_ATTEMPTS = 100; // Maximum attempts for placing teleporters/monsters
const POLL_INTERVAL_MS = 333;
const NUM_PLAYERS = 4;
const MAX_HIGH_SCORES = 10;
const HIGH_SCORE_FILENAME = "maze4_high_scores.dat";
const LOG_FILENAME = "maze4_logfile.txt";
const STATUS_LINE_HISTORY = 20;
const STATUS_LINE_MAX = 80;

// messages
const DELAY_MSG = "** Delaying for you to read **";
const LOST_MSG = "LOST the battle";
const PAUSE_MSG = "↑ ** Paused, press any key to continue **";

// CLASSES AND TYPE DEFINITIONS ////////////////////////////////////////////

class Coord {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }
}

class HighScore {
    constructor(name = "", player_id = 0, score = 0, battles_won = 0, strength = 0, this_run = 0, date = 0) {
        this.name = name;
        this.player_id = player_id;
        this.score = score;
        this.battles_won = battles_won;
        this.strength = strength;
        this.this_run = this_run;
        this.date = date;
    }
}

class Maze {
    constructor(rows, cols) {
        this.rows = rows;
        this.cols = cols;
        this.grid = Array(rows).fill().map(() => Array(cols).fill(WALL));
        this.visited = Array(NUM_PLAYERS).fill().map(() => 
            Array(rows).fill().map(() => Array(cols).fill(0)));
    }
}

class Position {
    constructor(x = 0, y = 0, parentX = -1, parentY = -1) {
        this.x = x;
        this.y = y;
        this.parentX = parentX;
        this.parentY = parentY;
    }
}

class Teleporter {
    constructor(x1 = 0, y1 = 0, x2 = 0, y2 = 0) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
    }
}

class Monster {
    constructor() {
        this.x = 0;
        this.y = 0;
        this.dx = 0;
        this.dy = 0;
        this.patrol_length = 0;
        this.steps = 0;
        this.strength = 0;
        this.defeated = 0;
        this.recovery_turns = 0;
    }
}

class Player {
    constructor() {
        this.id = 0;
        this.start = new Position();
        this.end = new Position();
        this.current = new Position();
        this.strength = 0;
        this.battles_won = 0;
        this.battles_lost = 0;
        this.recovery_turns = 0;
        this.justTeleported = 0;
        this.moves = 0;
        this.reached_goal = 0;
        this.finished_rank = 0;
        this.abandoned_race = 0;
        this.visited_char = '';
        this.solution_char = '';
        this.color_pair = 0;
        this.dx = [0, 0, 0, 0];
        this.dy = [0, 0, 0, 0];
    }
}

class Node {
    constructor(pos, next = null) {
        this.pos = pos;
        this.next = next;
    }
}

class Status {
    constructor(move = 0, msg = "") {
        this.move = move;
        this.msg = msg;
    }
}

// GLOBAL VARIABLES //////////////////////////////////////////////////

// Bot names
const BOT_NAMES = [
    "(n/a)",
    "RapRas", // "Rapid Raspberry", 
    "BusBlu", // "Bussin' Blueberry",
    "LigLem", // "Lightening Lemon", 
    "KwiKiw" // "Kwick Kiwi"
];

const MONSTER_NAMES = [
    "Abyssal Artichoke", "Brutal Broccoli", "Creeping Carrot",
    "Dreadful Daikon", "Eerie Eggplant", "Fiendish Fennel",
    "Ghastly Garlic", "Horrid Horseradish", "Infernal Iceberg",
    "Jagged Jicama", "Killer Kale", "Lurking Leek",
    "Mean Mushroom", "Nightmare Nori", "Ominous Onion",
    "Petrifying Potato", "Quagmire Quinoa", "Ravaging Radish",
    "Sinister Spinach", "Terror Tomato", "Unholy Ube",
    "Vile Vine Spinach", "Wicked Wasabi", "Xenophobic Xigua",
    "Yawning Yam", "Zealous Zucchini"
];

// Misc globals
let battle_win;
let Base_dx = [0, 1, 0, -1]; // Up, Right, Down, Left
let Base_dy = [-1, 0, 1, 0];

// Global arrays for teleporters and monsters
let teleporters = new Array(MAX_TELEPORTERS);
let Num_teleporters = -1;
let monsters = new Array(MAX_MONSTERS);
let Num_monsters = -1;
let Liv_monsters = 0;
let Max_monster_strength = -1;

// Array of players
let players = new Array(NUM_PLAYERS);
let parent_map = Array(NUM_PLAYERS).fill().map(() => 
    Array(MAX_ROWS).fill().map(() => 
        Array(MAX_COLS).fill().map(() => ({ x: 0, y: 0 }))));

// Game state
let Abort_run_round = 0;
let LastUpdate = Date.now();
let Players_finished = 0;
let Game_finished = 0;
let Game_moves = 0;
let Game_speed = -1;
let Game_delay = 0;
let Game_rounds = -1;
let Game_roundsB = -1;
let ShowWindows = -1;
let WaitForKey = -1;
let LastSLupdate = 0;
let current_status_index = 0;
let Screen_reduced = 0;
let status_lines = new Array(STATUS_LINE_HISTORY);
let old_rows = 0;
let old_cols = 0;
let in_read_keyboard = 0;
let in_mysleep = 0;
let in_pausegame = 0;
let pauseTime = -1;
let Maze_rows = 0;
let Help_shown_this_session = 0;

// maze state
let maze = null;

// FUNCTION IMPLEMENTATIONS ///////////////////////////////////////////////

// Initialize objects and setup
async function init() {
    // Initialize status_lines
    for (let i = 0; i < STATUS_LINE_HISTORY; i++) {
        status_lines[i] = new Status();
    }
    
    // Initialize teleporters
    for (let i = 0; i < MAX_TELEPORTERS; i++) {
        teleporters[i] = new Teleporter();
    }
    
    // Initialize monsters
    for (let i = 0; i < MAX_MONSTERS; i++) {
        monsters[i] = new Monster();
    }
    
    // Initialize players
    for (let i = 0; i < NUM_PLAYERS; i++) {
        players[i] = new Player();
    }
    
    // Initialize ncurses (simulated)
    initscr();
//    cbreak();
//    noecho();
//    keypad(stdscr, true);
    curs_set(0);
    start_color();
    use_default_colors();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, null);
    
    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_BLACK);            // Path
    init_pair(2, COLOR_RED, COLOR_BLACK);              // Player 1
    init_pair(3, COLOR_CYAN, COLOR_BLACK);             // Player 2
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);           // Player 3
    init_pair(5, COLOR_GREEN, COLOR_BLACK);            // Player 4
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);          // Current position
    init_pair(7, COLOR_CYAN, COLOR_BLUE);     // Teleporter
    init_pair(8, COLOR_WHITE, COLOR_MAGENTA); // Monster
    init_pair(9, COLOR_MAGENTA, COLOR_BLACK); // Defeated monster
    init_pair(10, COLOR_BLACK, COLOR_WHITE);  // Game stats
    init_pair(11, COLOR_BLACK, COLOR_YELLOW); // Alert
    
    init_pair(12, COLOR_RED, COLOR_BLACK);
    init_pair(13, COLOR_WHITE, COLOR_BLACK);
    init_pair(14, COLOR_YELLOW, COLOR_BLACK);
    init_pair(15, COLOR_BLUE, COLOR_BLACK);
    init_pair(16, COLOR_CYAN, COLOR_BLACK);
    
    await calc_game_speed();
    
    if (Game_rounds < 0) {
        Game_rounds = 10;
    }
    Game_roundsB = Game_rounds;
    if (WaitForKey < 0) {
        WaitForKey = 0;
    }
    if (ShowWindows < 0) {
        ShowWindows = 1;
    }
    if (pauseTime < 0) {
        pauseTime = 2 * 1000;
    }
    
    // Get terminal size
    let size = get_terminal_size();
    resizeterm(size.rows, size.cols);
    old_rows = size.rows;
    old_cols = size.cols;
    await logMessage(`Init term size is ${size.rows}, ${size.cols}`);
}

// Main function
async function main() {
    // Set the random seed
    Math.seedrandom(Date.now());
    
    // Initialize ncurses and settings
    await init();

    // If no high scores and help not yet shown, show help
    if (!Help_shown_this_session) {
        let best_scores = [], worst_scores = [];
    
        for (let i = 0; i < MAX_HIGH_SCORES; i++) {
            best_scores.push(new HighScore("", 0, 0, 0, 0, 0, 0));
            worst_scores.push(new HighScore("", 0, 0, 0, 0, 0, 0));
        }
    
        let high_score_count = await read_high_scores(best_scores, worst_scores);
    
        if (high_score_count === 0) {
            await show_help_window();
            Help_shown_this_session = 1;
        }
    }
    
    // Run game rounds
    for (; Game_rounds > 0; Game_rounds--) {
        await logMessage(`Starting round ${Game_rounds}`);
				try {
          await run_round();
				} catch(error) {
					// about the round
					Abort_run_round = 1;
					ungetch(0);
          while(Date.now() - LastUpdate < 2000) {
						await new Promise((resolve) => setTimeout(resolve, 500));
						console.log("waiting for run_round to abort");
					}
					delwin(battle_win);
					Abort_run_round = 0;
					// handle error
					if (error.message === EXIT_PROG) {
						throw new Error(EXIT_PROG);
					} else {
					  console.log("ignoring run_round() error: ", error);
					}
				}
        if (Screen_reduced) {
            Screen_reduced = 0;
            Game_rounds++;
        }
        
        // Make screen match reported size
        let size = get_terminal_size();
        resizeterm(size.rows, size.cols);
        old_rows = size.rows;
        old_cols = size.cols;
    }
    
    // Leave game
    exit_game("Game over\n");
}

// Run a round of the game
async function run_round() {
    let rows, cols, maze_area;
    
    // Initialize players
    await initialize_players(0);
    
    // Get terminal dimensions
    let termSize = getmaxyx(stdscr);
    rows = termSize.rows;
    cols = termSize.cols;
    maze_area = rows * cols;
    
    // Calculate default max values based on screen size if not provided
    if (Num_teleporters < 0) {
        // 1 teleporter per 100 cells, with a minimum of 1 and maximum of MAX_TELEPORTERS
        Num_teleporters = Math.floor(maze_area / DEF_TELEPORTER_DENSITY);
    }
    Num_teleporters = Num_teleporters < 0 ? 0 : (Num_teleporters > MAX_TELEPORTERS ? MAX_TELEPORTERS : Num_teleporters);
    
    if (Num_monsters < 0) {
        // 1 monster per 20 cells, with a minimum of 1 and maximum of MAX_MONSTERS
        Num_monsters = Math.floor(maze_area / DEF_MONSTER_DENSITY);
    }
    Num_monsters = Num_monsters < 0 ? 0 : (Num_monsters > MAX_MONSTERS ? MAX_MONSTERS : Num_monsters);
    
    if (Max_monster_strength < 0) {
        Max_monster_strength = DEF_MONSTER_STRENGTH; // Default maximum strength
    }
    Max_monster_strength = Max_monster_strength < 0 ? 0 : (Max_monster_strength > MAX_MONSTER_STRENGTH ? MAX_MONSTER_STRENGTH : Max_monster_strength);
    
    // Ensure minimum maze size
    if (rows < MIN_ROWS || cols < MIN_COLS) {
        exit_game(`Screen too small, min ${MIN_ROWS} rows, ${MIN_COLS} cols\n`);
    }
    
    // Ensure maximum maze size
    if (rows > MAX_ROWS || cols > MAX_COLS) {
        exit_game(`Screen too big, max ${MAX_ROWS} rows, ${MAX_COLS} cols\n`);
    }
    
    // Adjust for maze walls and borders
    rows = Math.floor((rows - 6) / 2) * 2 - 1; // Reserve space for messages and player stats
    cols = Math.floor((cols - 2) / 2) * 2 - 1;
    
    // Ensure rows and cols are odd, even if initial calculation is even.
    if (rows % 2 === 0) rows--;
    if (cols % 2 === 0) cols--;
    
    // Create and initialize maze
    await create_maze(rows, cols);
    if (!maze) {
        exit_game("Failed to allocate memory for maze\n");
    }
    
    // Generate maze using enhanced DFS for paths between corners
    await generate_maze();
    
    // Implement and call ensure_path_between_corners
    await ensure_path_between_corners();
    
    // Place teleporters and monsters
    await place_teleporters();
    await place_monsters();
    Liv_monsters = Num_monsters;
    
    // Finish initializing players
    await initialize_players(1);
    
    // Print initial maze
    clear();
    await print_maze();
    await display_player_stats();
    await pauseForUser();
    await update_status_line("The race is ON!");
    LastSLupdate = 1;
    
    // Solve maze concurrently for all players
    await solve_maze_multi();
    
    // highscore
    if (!Screen_reduced) await update_high_scores(rows, cols);
    
    // Clean up
    await free_maze();
}

async function initialize_players(stage) {
    // Stage 0
    if (stage === 0) {
        // Global vars
        for (let i = 0; i < MAX_TELEPORTERS; i++) {
            teleporters[i] = new Teleporter();
        }
        
        for (let i = 0; i < MAX_MONSTERS; i++) {
            monsters[i] = new Monster();
        }
        
        Liv_monsters = 0;
        
        for (let i = 0; i < NUM_PLAYERS; i++) {
            players[i] = new Player();
        }
        
        // Reset parent_map
        for (let p = 0; p < NUM_PLAYERS; p++) {
            for (let i = 0; i < MAX_ROWS; i++) {
                for (let j = 0; j < MAX_COLS; j++) {
                    parent_map[p][i][j] = { x: 0, y: 0 };
                }
            }
        }
        
        Players_finished = 0;
        Game_finished = 0;
        Game_moves = 0;
        maze = null;
        
        // Clear all status line entries
        for (let i = 0; i < STATUS_LINE_HISTORY; i++) {
            status_lines[i] = new Status();
        }
        
        // Copy the base direction arrays to start
        let dx_copy = [...Base_dx];
        let dy_copy = [...Base_dy];
        
        // Initialize player 1
        players[0].id = 1;
        players[0].strength = DEF_PLAYER_STRENGTH;
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
        
        // Set direction preferences - Right, Down, Left, Up (good for top-left to bottom-right)
        players[0].dx[0] = 1; // Right
        players[0].dy[0] = 0;
        players[0].dx[1] = 0; // Down
        players[0].dy[1] = 1;
        players[0].dx[2] = -1; // Left
        players[0].dy[2] = 0;
        players[0].dx[3] = 0; // Up
        players[0].dy[3] = -1;
        
        // Initialize player 2
        players[1].id = 2;
        players[1].strength = DEF_PLAYER_STRENGTH;
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
        
        // Set direction preferences - Left, Down, Right, Up (good for top-right to bottom-left)
        players[1].dx[0] = -1; // Left
        players[1].dy[0] = 0;
        players[1].dx[1] = 0; // Down
        players[1].dy[1] = 1;
        players[1].dx[2] = 1; // Right
        players[1].dy[2] = 0;
        players[1].dx[3] = 0; // Up
        players[1].dy[3] = -1;
        
        // Initialize player 3
        players[2].id = 3;
        players[2].strength = DEF_PLAYER_STRENGTH;
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
        
        // Set direction preferences - Right, Up, Left, Down (good for bottom-left to top-right)
        players[2].dx[0] = 1; // Right
        players[2].dy[0] = 0;
        players[2].dx[1] = 0; // Up
        players[2].dy[1] = -1;
        players[2].dx[2] = -1; // Left
        players[2].dy[2] = 0;
        players[2].dx[3] = 0; // Down
        players[2].dy[3] = 1;
        
        // Initialize player 4
        players[3].id = 4;
        players[3].strength = DEF_PLAYER_STRENGTH;
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
        
        // Set direction preferences - Left, Up, Right, Down (good for bottom-right to top-left)
        players[3].dx[0] = -1; // Left
        players[3].dy[0] = 0;
        players[3].dx[1] = 0; // Up
        players[3].dy[1] = -1;
        players[3].dx[2] = 1; // Right
        players[3].dy[2] = 0;
        players[3].dx[3] = 0; // Down
        players[3].dy[3] = 1;
        
        // Now randomize each player's directions slightly for variety
        for (let i = 0; i < NUM_PLAYERS; i++) {
            await shuffle_directions_for_player(i);
        }
    } 
    // Stage 1
    else {
        // Set player start and end positions to corners
        players[0].start.x = 1;
        players[0].start.y = 1;
        players[0].end.x = maze.cols - 2;
        players[0].end.y = maze.rows - 2;
        maze.grid[players[0].end.y][players[0].end.x] = END1;
        
        players[1].start.x = maze.cols - 2;
        players[1].start.y = 1;
        players[1].end.x = 1;
        players[1].end.y = maze.rows - 2;
        maze.grid[players[1].end.y][players[1].end.x] = END2;
        
        players[2].start.x = 1;
        players[2].start.y = maze.rows - 2;
        players[2].end.x = maze.cols - 2;
        players[2].end.y = 1;
        maze.grid[players[2].end.y][players[2].end.x] = END3;
        
        players[3].start.x = maze.cols - 2;
        players[3].start.y = maze.rows - 2;
        players[3].end.x = 1;
        players[3].end.y = 1;
        maze.grid[players[3].end.y][players[3].end.x] = END4;
        
        // Initialize player current positions
        for (let i = 0; i < NUM_PLAYERS; i++) {
            players[i].current = { ...players[i].start };
        }
    }
}

// Shuffle the directions for a specific player
async function shuffle_directions_for_player(idx) {
    // Apply a Fisher-Yates shuffle but only to the last 2 directions
    // This preserves the main direction preference but adds some randomness
    for (let i = 3; i >= 2; i--) {
        let j = Math.floor(Math.random() * (i + 1));
        
        // Swap dx[i] with dx[j]
        let temp = players[idx].dx[i];
        players[idx].dx[i] = players[idx].dx[j];
        players[idx].dx[j] = temp;
        
        // Swap dy[i] with dy[j]
        temp = players[idx].dy[i];
        players[idx].dy[i] = players[idx].dy[j];
        players[idx].dy[j] = temp;
    }
}

// Helper function to get the current character for a player
async function get_player_current_char(player_id) {
    switch (player_id) {
        case 1: return CURRENT1;
        case 2: return CURRENT2;
        case 3: return CURRENT3;
        case 4: return CURRENT4;
        default: return '!';
    }
}

// Helper function to get the visited character for a player
async function get_player_visited_char(player_id) {
    switch (player_id) {
        case 1: return VISITED1;
        case 2: return VISITED2;
        case 3: return VISITED3;
        case 4: return VISITED4;
        default: return '!';
    }
}

// Helper function to get the solution character for a player
async function get_player_solution_char(player_id) {
    switch (player_id) {
        case 1: return SOLUTION1;
        case 2: return SOLUTION2;
        case 3: return SOLUTION3;
        case 4: return SOLUTION4;
        default: return '!';
    }
}

// Check if a position is a player's position
async function is_player_position(x, y, current_pos) {
    for (let i = 0; i < NUM_PLAYERS; i++) {
        if (current_pos[i].x === x && current_pos[i].y === y) {
            return i + 1; // Return player id (1-based)
        }
    }
    return 0; // Not a player position
}

// Check if a position is a dead end
async function is_dead_end(x, y) {
    // Don't consider special positions as dead ends
    if (
        maze.grid[y][x] === END1 ||
        maze.grid[y][x] === END2 ||
        maze.grid[y][x] === END3 ||
        maze.grid[y][x] === END4
    ) {
        return 0;
    }
    
    // Only check PATH cells
    if (maze.grid[y][x] !== PATH) {
        return 0;
    }
    
    // Count adjacent PATH/START*/END* cells
    let path_neighbors = 0;
    for (let dir = 0; dir < 4; dir++) {
        let newX = x + Base_dx[dir];
        let newY = y + Base_dy[dir];
        
        if (newX >= 0 && newX < maze.cols && newY >= 0 && newY < maze.rows) {
            let cell = maze.grid[newY][newX];
            if (cell === PATH ||
                cell === END1 ||
                cell === END2 ||
                cell === END3 ||
                cell === END4) {
                path_neighbors++;
            }
        }
    }
    
    // If only one PATH neighbor, it's a dead end
    return (path_neighbors === 1) ? 1 : 0;
}

// Display player stats with combined battles column and status column
async function display_player_stats() {
    let base_row = maze.rows + 1;
    
    mvwprintw(stdscr, base_row - 1, 0, "  NAME | ST | BATS  | MOVES | STATUS");
    wclrtoeol(stdscr);
    
    for (let i = 0; i < NUM_PLAYERS; i++) {
        attron(COLOR_PAIR(players[i].color_pair) | A_BOLD);
        mvwprintw(stdscr, base_row + i, 0, `${BOT_NAMES[players[i].id].padStart(6)} | ${String(players[i].strength).padStart(2)} | ${String(players[i].battles_won).padStart(2)}/${String(players[i].battles_lost).padStart(2)} |  ${String(players[i].moves).padStart(4)} | `);
        
        // Add status column showing player's solve status
        if (players[i].reached_goal) {
            wprintw(stdscr, `Finished #${players[i].finished_rank}!`);
        } else if (players[i].abandoned_race === 1) {
            wprintw(stdscr, "DNF: Trapped");
        } else if (players[i].abandoned_race === 2) {
            wprintw(stdscr, "DNF: Loser");
        } else if (Game_moves === 0) {
            wprintw(stdscr, "Ready To Start");
        } else {
            wprintw(stdscr, "Solving");
        }
        wclrtoeol(stdscr);
        
        attroff(COLOR_PAIR(players[i].color_pair) | A_BOLD);
    }
    wnoutrefresh(stdscr);
}

// Display player alert with variable parameters
// player_index -2=abandoned, -1=out of moves, >0 is rank
async function display_player_alert(p_idx, rank) {
    Game_finished++;
    if (ShowWindows === 0) {
        if (Game_finished !== NUM_PLAYERS)
            await pauseForUser();
        return;
    }
    
    // Save current window and create a larger battle screen
    // Calculate window dimensions
    let height = MIN_ROWS - 2;
    let width = MIN_COLS;
    
    // Create a new window centered on screen
    let max_y, max_x;
    let termSize = getmaxyx(stdscr);
    max_y = termSize.rows;
    max_x = termSize.cols;
    
    battle_win = newwin(height, width, Math.floor((max_y - height) / 2), Math.floor((max_x - width) / 2));
    box(battle_win, 0, 0);
    
    // Set up colors for battle screen
    wbkgd(battle_win, COLOR_PAIR(11));
    
    // Display battle title based on battle type
    wattron(battle_win, COLOR_PAIR(players[p_idx - 1].color_pair) | A_BOLD);
    
    if (rank > 0) {
        mvwprintw(battle_win, 2, 2, "PLAYER REACHED GOAL!");
        // figlet goal
        mvwprintw(battle_win, 16, 1, String.raw`        RANK = ${rank}!                    `);
        mvwprintw(battle_win, 17, 1, String.raw` _____ _       _     _              _ `);
        mvwprintw(battle_win, 18, 1, String.raw`|  ___(_)_ __ (_)___| |__   ___  __| |`);
        mvwprintw(battle_win, 19, 1, String.raw`| |_  | | '_ \| / __| '_ \ / _ \/ _\ |`);
        mvwprintw(battle_win, 20, 1, String.raw`|  _| | | | | | \__ \ | | |  __/ (_| |`);
        mvwprintw(battle_win, 21, 1, String.raw`|_|   |_|_| |_|_|___/_| |_|\___|\__,_|`);
    } else if (rank === -1) {
        mvwprintw(battle_win, 2, 2, "PLAYER ABANDONED RACE");
        // figlet abandoned
        mvwprintw(battle_win, 16, 1, String.raw` _____  OUT OF MOVES!               _ `);
        mvwprintw(battle_win, 17, 1, String.raw`|_   _| __ __ _ _ __ _ __   ___  __| |`);
        mvwprintw(battle_win, 18, 1, String.raw`  | || '__/ _\ | '_ | '_ \ / _ \/ _\ |`);
        mvwprintw(battle_win, 19, 1, String.raw`  | || | | (_| | |_)| |_) |  __/ (_| |`);
        mvwprintw(battle_win, 20, 1, String.raw`  |_||_|  \__,_| .__| .__/ \___|\__,_|`);
        mvwprintw(battle_win, 21, 1, String.raw`               |_|  |_|               `);
    } else if (rank === -2) {
        mvwprintw(battle_win, 2, 2, "PLAYER ABANDONED RACE");
        // figlet abandoned
        mvwprintw(battle_win, 16, 1, String.raw`        TOO MANY LOSSES!        `);
        mvwprintw(battle_win, 17, 1, String.raw` _     ___  ____  _____ ____  _ `);
        mvwprintw(battle_win, 18, 1, String.raw`| |   / _ \/ ___|| ____|  _ \| |`);
        mvwprintw(battle_win, 19, 1, String.raw`| |  | | | \___ \|  _| | |_) | |`);
        mvwprintw(battle_win, 20, 1, String.raw`| |__| |_| |___) | |___|  _ <|_|`);
        mvwprintw(battle_win, 21, 1, String.raw`|____|\___/|____/|_____|_| \_(_)`);
    }
    
    await show_battle_art(1, 1, 1, 1, p_idx - 1, -1);
    wattroff(battle_win, COLOR_PAIR(players[p_idx - 1].color_pair) | A_BOLD);
    wnoutrefresh(battle_win);
    await pauseForUser();
    
    // Clean up battle window
    delwin(battle_win);
    battle_win = null; // Good practice to avoid dangling pointers
    
    // Redraw the main screen
    await print_maze();
    await display_player_stats();
}

// Place teleporters at dead ends
async function place_teleporters() {
    // Arrays to store dead end positions
    let dead_ends_x = new Array(maze.rows * maze.cols);
    let dead_ends_y = new Array(maze.rows * maze.cols);
    let dead_end_count = 0;
    
    // Find all dead ends
    for (let y = 1; y < maze.rows - 1; y++) {
        for (let x = 1; x < maze.cols - 1; x++) {
            if (await is_dead_end(x, y)) {
                dead_ends_x[dead_end_count] = x;
                dead_ends_y[dead_end_count] = y;
                dead_end_count++;
            }
        }
    }
    
    // If we don't have enough dead ends for at least one teleporter, exit
    if (dead_end_count < 2) {
        return;
    }
    
    // Adjust number of teleporters based on available dead ends
    let max_possible_teleporters = Math.floor(dead_end_count / 2);
    Num_teleporters = (max_possible_teleporters < Num_teleporters) ? max_possible_teleporters : Num_teleporters;
    
    // Place teleporters at randomly selected dead ends
    for (let i = 0; i < Num_teleporters; i++) {
        let idx1 = Math.floor(Math.random() * dead_end_count);
        let x1 = dead_ends_x[idx1];
        let y1 = dead_ends_y[idx1];
        
        // Remove this dead end from the pool
        dead_ends_x[idx1] = dead_ends_x[dead_end_count - 1];
        dead_ends_y[idx1] = dead_ends_y[dead_end_count - 1];
        dead_end_count--;
        
        let idx2 = Math.floor(Math.random() * dead_end_count);
        let x2 = dead_ends_x[idx2];
        let y2 = dead_ends_y[idx2];
        
        // Remove this dead end too
        dead_ends_x[idx2] = dead_ends_x[dead_end_count - 1];
        dead_ends_y[idx2] = dead_ends_y[dead_end_count - 1];
        dead_end_count--;
        
        // Skip corners (player start/end positions)
        if ((x1 <= 2 && y1 <= 2) ||
            (x1 >= maze.cols - 3 && y1 <= 2) ||
            (x1 <= 2 && y1 >= maze.rows - 3) ||
            (x1 >= maze.cols - 3 && y1 >= maze.rows - 3)
        ) {
            i--;
            continue;
        }
        if ((x2 <= 2 && y2 <= 2) ||
            (x2 >= maze.cols - 3 && y2 <= 2) ||
            (x2 <= 2 && y2 >= maze.rows - 3) ||
            (x2 >= maze.cols - 3 && y2 >= maze.rows - 3)
        ) {
            i--;
            continue;
        }
        
        // Place teleporters
        teleporters[i].x1 = x1;
        teleporters[i].y1 = y1;
        teleporters[i].x2 = x2;
        teleporters[i].y2 = y2;
        
        maze.grid[y1][x1] = TELEPORTER;
        maze.grid[y2][x2] = TELEPORTER;
    }
}

// Place monsters at random locations, avoiding teleporters and corners
async function place_monsters() {
    for (let i = 0; i < Num_monsters; i++) {
        // Find a random empty space
        let x, y;
        let attempts = 0;
        
        do {
            x = Math.floor(Math.random() * (maze.cols - 4)) + 2; // Avoid edges and corners
            y = Math.floor(Math.random() * (maze.rows - 4)) + 2;
            attempts++;
            
            // Skip if we can't find a spot after many attempts
            if (attempts > MAX_ATTEMPTS) {
                Num_monsters = i;
                return;
            }
            
            // Skip corners (player start/end positions)
            if ((x <= 2 && y <= 2) || (x >= maze.cols - 3 && y <= 2) ||
                (x <= 2 && y >= maze.rows - 3) ||
                (x >= maze.cols - 3 && y >= maze.rows - 3)) {
                continue;
            }
        } while (maze.grid[y][x] !== PATH || await is_dead_end(x, y));
        
        // Place monster
        monsters[i].x = x;
        monsters[i].y = y;
        
        // Random direction
        let dir = Math.floor(Math.random() * 4);
        monsters[i].dx = Base_dx[dir];
        monsters[i].dy = Base_dy[dir];
        
        // Random patrol length
        monsters[i].patrol_length = Math.floor(Math.random() * 10) + 5;
        monsters[i].steps = 0;
        
        // Random strength (1-10)
        monsters[i].strength = Math.floor(Math.random() * Max_monster_strength) + 1;
        
        // Recovery after fight
        monsters[i].recovery_turns = 0;
        
        // Not defeated initially
        monsters[i].defeated = 0;
        
        maze.grid[y][x] = MONSTER;
    }
}

// Update monster positions
async function update_monsters() {
    for (let i = 0; i < Num_monsters; i++) {
        // Skip defeated monsters
        if (monsters[i].defeated) {
            continue;
        }
        
        // decrement recovery_turns
        if (monsters[i].recovery_turns)
            monsters[i].recovery_turns--;
        
        // Clear current position
        maze.grid[monsters[i].y][monsters[i].x] = PATH;
        
        // Update position
        monsters[i].steps++;
        if (monsters[i].steps >= monsters[i].patrol_length) {
            // Change direction
            let dir = Math.floor(Math.random() * 4);
            monsters[i].dx = Base_dx[dir];
            monsters[i].dy = Base_dy[dir];
            monsters[i].steps = 0;
        }
        
        // Try to move
        let newX = monsters[i].x + monsters[i].dx;
        let newY = monsters[i].y + monsters[i].dy;
        let cell = maze.grid[newY][newX];
        
        // Check if the new position is valid and not a special cell
        if (newX > 0 && newX < maze.cols - 1 && newY > 0 &&
            newY < maze.rows - 1 && (cell === PATH || cell === MONSTER)) {
            monsters[i].x = newX;
            monsters[i].y = newY;
        } else {
            // Change direction if blocked
            let dir = Math.floor(Math.random() * 4);
            monsters[i].dx = Base_dx[dir];
            monsters[i].dy = Base_dy[dir];
            monsters[i].steps = 0;
        }
        
        // Mark new position
        maze.grid[monsters[i].y][monsters[i].x] = MONSTER;
    }
    
    // Check for monster vs monster collisions
    for (let i = 0; i < Num_monsters; i++) {
        // Skip defeated monsters
        if (monsters[i].defeated) {
            continue;
        }
        
        for (let j = i + 1; j < Num_monsters; j++) {
            // Skip defeated monsters
            if (monsters[j].defeated) {
                continue;
            }
            
            // Check if monsters are in the same cell
            if (monsters[i].x === monsters[j].x && monsters[i].y === monsters[j].y) {
                // Battle the monsters
                await battle_monsters(i, j);
                
                // After battle, the winner stays at the position, loser is already
                // marked as defeated so no additional position updates needed
            }
        }
    }
}

// Check if position is a teleporter and get destination
async function check_teleporter(x, y, newPos) {
    for (let i = 0; i < Num_teleporters; i++) {
        if (teleporters[i].x1 === x && teleporters[i].y1 === y) {
            newPos.x = teleporters[i].x2;
            newPos.y = teleporters[i].y2;
            return 1;
        } else if (teleporters[i].x2 === x && teleporters[i].y2 === y) {
            newPos.x = teleporters[i].x1;
            newPos.y = teleporters[i].y1;
            return 1;
        }
    }
    return 0;
}

// Check if position has a monster and return its index
async function check_monster(x, y) {
    for (let i = 0; i < Num_monsters; i++) {
        if (monsters[i].x === x && monsters[i].y === y && !monsters[i].defeated) {
            return i + 1; // Return monster index + 1 (so 0 means no monster)
        }
    }
    return 0; // No monster
}

// Show battle art
//                              winner         battle
//                 art/figlet, left/right,      type, player/monster  left-index
//                 right-index
async function show_battle_art(aart, leftside, type, player, left_idx, right_idx) {
    // Set the recovery time (in turns) for both winner and loser
    const WINNER_RECOVERY_TURNS = 3;
    const LOSER_RECOVERY_TURNS = 6;
    
    // Show ascii-art of battler /////////////////////////////////////////////
    if (aart) {
        if (ShowWindows === 0) return;
        let col = leftside ? 2 : 23;
        let idx = leftside ? left_idx : right_idx;
        
        if (player) { // Player Bot
            wattron(battle_win, COLOR_PAIR(players[idx].color_pair) | A_BOLD);
            mvwprintw(battle_win, 4, col, `${BOT_NAMES[idx + 1]}`);
            mvwprintw(battle_win, 5, col, `STR: ${players[idx].strength}  WINS: ${players[idx].battles_won}`);
            
            // Bot ASCII Art
            mvwprintw(battle_win,  6, col, String.raw`               `);
            mvwprintw(battle_win,  7, col, String.raw`     ____      `);
            mvwprintw(battle_win,  8, col, String.raw`    /    \     `);
            mvwprintw(battle_win,  9, col, String.raw`   | o  o |    `);
            mvwprintw(battle_win, 10, col, String.raw`   | ____ |    `);
            mvwprintw(battle_win, 11, col, String.raw`   ||____||    `);
            mvwprintw(battle_win, 12, col, String.raw`    \____/     `);
            mvwprintw(battle_win, 13, col, String.raw`               `);
            wattroff(battle_win, COLOR_PAIR(players[idx].color_pair) | A_BOLD);
        } else { // Monster
            wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
            mvwprintw(battle_win, 4, col, `${MONSTER_NAMES[idx]}`);
            mvwprintw(battle_win, 5, col, `Strength: ${monsters[idx].strength}    `);
            
            // Monster ASCII Art
            mvwprintw(battle_win,  6, col, String.raw`    .----,     `);
            mvwprintw(battle_win,  7, col, String.raw`   /      \    `);
            mvwprintw(battle_win,  8, col, String.raw`  |  O  O  |   `);
            mvwprintw(battle_win,  9, col, String.raw`  | .vvvv. |   `);
            mvwprintw(battle_win, 10, col, String.raw`  / |    | \   `);
            mvwprintw(battle_win, 11, col, String.raw` /   ^^^^ \ \  `);
            mvwprintw(battle_win, 12, col, String.raw`/  /|     |\ \.`);
            mvwprintw(battle_win, 13, col, String.raw`\_/ .~~~~~. \_/`);
            wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
        }
    } 
    // update stats for winner and loser /////////////////////////////////////
    else {
        let widx, lidx;
        let winner_is_player = ((type === 0 && leftside) || (type === 1));
        let loser_is_player = ((type === 0 && !leftside) || (type === 1));
        
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
            await update_status_line(`${BOT_NAMES[lidx + 1]} ${LOST_MSG}!`);
        } else {
            // loser is monster
            Liv_monsters--;
            monsters[lidx].defeated = 1;
            maze.grid[monsters[lidx].y][monsters[lidx].x] = DEFEATED_MONSTER;
            await update_status_line(`${MONSTER_NAMES[lidx]} ${LOST_MSG}!`);
        }
        
        if (ShowWindows !== 0) {
            // X out eyes of loser
            let los_col = leftside ? 23 : 2;
            if (loser_is_player) {
                // bot
                mvwprintw(battle_win, 9, los_col + 2, ` | x  x | `);
            } else {
                // monster
                mvwprintw(battle_win, 8, los_col + 1, ` |  X  X  | `);
            }
            
            // Show "Wins!" text with correct positioning ////////////////////////
            let win_col = leftside ? 1 : 23;
            mvwprintw(battle_win, 17, win_col, String.raw` _    _ _       `);
            mvwprintw(battle_win, 18, win_col, String.raw`| |  | (_)_ __  `);
            mvwprintw(battle_win, 19, win_col, String.raw`| |/\| | | '_ \ `);
            mvwprintw(battle_win, 20, win_col, String.raw`|  __  | | | | |`);
            mvwprintw(battle_win, 21, win_col, String.raw`|_/  \_|_|_| |_|`);
        }
    }
}

// Unified battle function that handles all battle scenarios
// Type: 0 = player vs monster, 1 = player vs player, 2 = monster vs monster
async function battle_unified(combatant1_idx, combatant2_idx, type) {
    // Get player indices if needed (for player vs monster or player vs player)
    let p1_idx = (type === 0 || type === 1) ? combatant1_idx - 1 : -1; // Adjust for 0-based indexing
    let p2_idx = (type === 1) ? combatant2_idx - 1 : -1; // Adjust for 0-based indexing
    
    // Get monster indices if needed (for player vs monster or monster vs monster)
    let m1_idx = (type === 0) ? combatant2_idx : (type === 2 ? combatant1_idx : -1);
    let m2_idx = (type === 2) ? combatant2_idx : -1;
    
    // Check if any combatant is in recovery mode - if so, skip battle entirely
    if ((type === 0 || type === 1) && players[p1_idx].recovery_turns > 0) {
        // Player 1 is recovering, no battle
        return -1; // Special code indicating battle was skipped
    }
    
    if (type === 1 && players[p2_idx].recovery_turns > 0) {
        // Player 2 is recovering, no battle
        return -1; // Special code indicating battle was skipped
    }
    
    if (type === 0 && !monsters[m1_idx].defeated && monsters[m1_idx].recovery_turns > 0) {
        // Monster is recovering, no battle
        return -1;
    }
    
    if (type === 2) {
        if (monsters[m1_idx].recovery_turns > 0 || monsters[m2_idx].recovery_turns > 0) {
            // One of the monsters is recovering, no battle
            return -1;
        }
    }
    
    // show battle spot
    let be_x = (type !== 2) ? players[p1_idx].current.x : monsters[m1_idx].x;
    let be_y = (type !== 2) ? players[p1_idx].current.y : monsters[m1_idx].y;
    await animate_bullseye(be_y, be_x, 5, parseInt(pauseTime / 40), 0);
    
    if (ShowWindows !== 0) {
        // Save current window and create a larger battle screen
        // Calculate window dimensions
        let height = MIN_ROWS - 2;
        let width = MIN_COLS;
        
        // Create a new window centered on screen
        let max_y, max_x;
        let termSize = getmaxyx(stdscr);
        max_y = termSize.rows;
        max_x = termSize.cols;
        
        battle_win = newwin(height, width, Math.floor((max_y - height) / 2), Math.floor((max_x - width) / 2));
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
        wattroff(battle_win, A_BOLD);
        
        // ASCII art for left combatant
        if (type === 0 || type === 1) { // Player Bot
            await show_battle_art(1, 1, type, 1, p1_idx, type === 0 ? m1_idx : p2_idx);
        } else { // Monster
            await show_battle_art(1, 1, type, 0, m1_idx, m2_idx);
        }
        
        // ASCII art for right combatant
        if (type === 1) { // Player Bot
            await show_battle_art(1, 0, type, 1, p1_idx, p2_idx);
        } else { // Monster
            await show_battle_art(1, 0, type, 0, type === 0 ? p1_idx : m1_idx, type === 0 ? m1_idx : m2_idx);
        }
        
        // Display vs text in the middle
        wattron(battle_win, A_BOLD);
        mvwprintw(battle_win, 9, 19, "VS");
        wattroff(battle_win, A_BOLD);
        
        // Display roll boxes
        mvwprintw(battle_win, 14, 5, "╔═════════╗        ╔═════════╗");
        mvwprintw(battle_win, 15, 5, "║         ║        ║         ║");
        mvwprintw(battle_win, 16, 5, "╚═════════╝        ╚═════════╝");
        
        wnoutrefresh(battle_win);
    }
    
    // Calculate battle rolls based on combatant types
    let roll1, roll2;
    
    // Left combatant roll
    if (type === 0 || type === 1) { // Player
        roll1 = players[p1_idx].strength + Math.floor(Math.random() * 6);
    } else { // Monster
        roll1 = monsters[m1_idx].strength + Math.floor(Math.random() * 6);
    }
    
    // Right combatant roll
    if (type === 1) { // Player
        roll2 = players[p2_idx].strength + Math.floor(Math.random() * 6);
    } else { // Monster
        roll2 = (type === 0 ? monsters[m1_idx].strength : monsters[m2_idx].strength) + Math.floor(Math.random() * 6);
    }
    
    if (ShowWindows !== 0) {
        // Display die rolls
        if (type === 0 || type === 1) { // Left is player
            wattron(battle_win, COLOR_PAIR(players[p1_idx].color_pair) | A_BOLD);
            mvwprintw(battle_win, 15, 10, `${roll1}`);
            wattroff(battle_win, COLOR_PAIR(players[p1_idx].color_pair) | A_BOLD);
        } else { // Left is monster
            wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
            mvwprintw(battle_win, 15, 10, `${roll1}`);
            wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
        }
        
        if (type === 1) { // Right is player
            wattron(battle_win, COLOR_PAIR(players[p2_idx].color_pair) | A_BOLD);
            mvwprintw(battle_win, 15, 29, `${roll2}`);
            wattroff(battle_win, COLOR_PAIR(players[p2_idx].color_pair) | A_BOLD);
        } else { // Right is monster
            wattron(battle_win, COLOR_PAIR(8) | A_BOLD);
            mvwprintw(battle_win, 15, 29, `${roll2}`);
            wattroff(battle_win, COLOR_PAIR(8) | A_BOLD);
        }
        
        wnoutrefresh(battle_win);
    }
    
    // Determine winner and loser
    let left_wins = 0;
    
    // For player vs monster, player wins on tie
    if (type === 0) {
        left_wins = (roll1 >= roll2) ? 1 : 0;
    }
    // For player vs player or monster vs monster, higher roll wins
    // In case of tie, first combatant wins (arbitrary rule)
    else {
        left_wins = (roll1 > roll2 ||
            (roll1 === roll2 &&
                (type === 1 ? combatant1_idx > combatant2_idx
                    : monsters[m1_idx].strength >=
                    monsters[m2_idx].strength))) ? 1 : 0;
    }
    
    // Display result and update stats based on battle type
    if (left_wins) {
        // Left combatant wins
        if (type === 0) {
            // Player vs monster - player wins
            await show_battle_art(0, 1, type, 1, p1_idx, m1_idx);
        } else if (type === 1) {
            // Player vs player - left player wins
            await show_battle_art(0, 1, type, 1, p1_idx, p2_idx);
        } else {
            // Monster vs monster - left monster wins
            await show_battle_art(0, 1, type, 0, m1_idx, m2_idx);
        }
    } else {
        // Right combatant wins
        if (type === 0) {
            // Player vs monster - monster wins
            await show_battle_art(0, 0, type, 0, p1_idx, m1_idx);
        } else if (type === 1) {
            // Player vs player - right player wins
            await show_battle_art(0, 0, type, 1, p1_idx, p2_idx);
        } else {
            // Monster vs monster - right monster wins
            await show_battle_art(0, 0, type, 0, m1_idx, m2_idx);
        }
    }
    
    if (ShowWindows !== 0) {
        wnoutrefresh(battle_win);
        await pauseForUser();
        
        // Clean up battle window
        delwin(battle_win);
        battle_win = null; // Good practice to avoid dangling pointers
    }
    
    // Redraw the main screen
    await print_maze();
    await display_player_stats();
    
    // Return battle result
    if (type === 0) {
        return left_wins; // For player vs monster: 1 if player won, 0 if monster won
    } else if (type === 1) {
        return left_wins ? combatant1_idx : combatant2_idx; // Return winner's ID
    } else {
        return left_wins ? combatant1_idx : combatant2_idx; // Return winner monster index
    }
}

// Convenience wrapper for player vs monster battles
async function battle_bot_monster(monster_index, player_id) {
    // For player vs monster, combatant1 is player, combatant2 is monster
    return await battle_unified(player_id, monster_index, 0);
}

// Convenience wrapper for player vs player battles
async function battle_bots(player1_id, player2_id) {
    return await battle_unified(player1_id, player2_id, 1);
}

// Convenience wrapper for monster vs monster battles
async function battle_monsters(monster1_idx, monster2_idx) {
    await battle_unified(monster1_idx, monster2_idx, 2);
    return 0; // Return value not used for monster vs monster
}

async function create_maze(rows, cols) {
    Maze_rows = rows;
    maze = new Maze(rows, cols);
    
    // Initialize with walls
    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            maze.grid[i][j] = WALL;
            
            // Initialize visited grid for each player
            for (let p = 0; p < NUM_PLAYERS; p++) {
                maze.visited[p][i][j] = 0;
            }
        }
    }
    
    return maze;
}

async function free_maze() {
    maze = null;
}

// Implement ensure_path_between_corners to guarantee connectivity
async function ensure_path_between_corners() {
    // Define the four corners
    let corners = [
        new Position(1, 1),                 // Top-left
        new Position(maze.cols - 2, 1),     // Top-right
        new Position(1, maze.rows - 2),     // Bottom-left
        new Position(maze.cols - 2, maze.rows - 2)  // Bottom-right
    ];
    
    // For each pair of corners, ensure a path exists
    for (let i = 0; i < 4; i++) {
        for (let j = i + 1; j < 4; j++) {
            // Create a temporary grid to track visited cells for this pathfinding attempt
            let visited = Array(maze.rows).fill().map(() => Array(maze.cols).fill(0));
            
            // Use BFS to find a path from corner i to corner j
            let queue = [];
            queue.push(corners[i]);
            visited[corners[i].y][corners[i].x] = 1;
            
            let path_found = 0;
            
            while (queue.length > 0 && !path_found) {
                let current = queue.shift();
                
                // Check if we've reached the destination corner
                if (current.x === corners[j].x && current.y === corners[j].y) {
                    path_found = 1;
                    break;
                }
                
                // Try all four directions
                for (let dir = 0; dir < 4; dir++) {
                    let newX = current.x + Base_dx[dir];
                    let newY = current.y + Base_dy[dir];
                    
                    // Check if valid cell and not visited and not a wall
                    if (newX > 0 && newX < maze.cols - 1 && newY > 0 &&
                        newY < maze.rows - 1 && !visited[newY][newX] &&
                        maze.grid[newY][newX] !== WALL) {
                        
                        visited[newY][newX] = 1;
                        let newPos = new Position(newX, newY, current.x, current.y);
                        queue.push(newPos);
                    }
                }
            }
            
            // If no path found, create one
            if (!path_found) {
                // Start at corner i, carve a path toward corner j
                let x = corners[i].x;
                let y = corners[i].y;
                let destX = corners[j].x;
                let destY = corners[j].y;
                
                while (x !== destX || y !== destY) {
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
                    maze.grid[y][x] = PATH;
                }
            }
        }
    }
}

// generate_maze to ensure paths between all corners
async function generate_maze() {
    // First, create a basic maze using DFS
    // Initialize stack for DFS
    let stack = [];
    let start = new Position(1, 1);
    stack.push(start);
    
    maze.grid[start.y][start.x] = PATH;
    
    while (stack.length > 0) {
        let current = stack.pop();
        
        // Get unvisited neighbors
        let unvisited = [];
        
        for (let dir = 0; dir < 4; dir++) {
            let newX = current.x + Base_dx[dir] * 2;
            let newY = current.y + Base_dy[dir] * 2;
            
            if (newX > 0 && newX < maze.cols - 1 && newY > 0 &&
                newY < maze.rows - 1 && maze.grid[newY][newX] === WALL) {
                unvisited.push(dir);
            }
        }
        
        if (unvisited.length > 0) {
            // Push current cell back onto stack
            stack.push(current);
            
            // Choose random unvisited neighbor
            let randDir = unvisited[Math.floor(Math.random() * unvisited.length)];
            let newX = current.x + Base_dx[randDir] * 2;
            let newY = current.y + Base_dy[randDir] * 2;
            
            // Remove wall between current cell and chosen cell
            maze.grid[current.y + Base_dy[randDir]][current.x + Base_dx[randDir]] = PATH;
            
            // Mark the chosen cell as part of the path
            maze.grid[newY][newX] = PATH;
            
            // Push chosen cell onto stack
            let newPos = new Position(newX, newY);
            stack.push(newPos);
        }
    }
}

// function to use player-specific direction arrays
async function solve_maze_multi() {
    // Create separate stacks for each player to ensure fairness
    let stacks = [[], [], [], []];
    
    // This is for tracking player collisions
    let previous_positions = [];
    for (let i = 0; i < NUM_PLAYERS; i++) {
        // Initialize with invalid positions
        previous_positions.push({ x: -1, y: -1 });
    }
    
    // Push each player's starting position onto their stack
    for (let p = 0; p < NUM_PLAYERS; p++) {
        stacks[p].push(players[p].start);
        // Mark start position as visited
        maze.visited[p][players[p].start.y][players[p].start.x] = 1;
    }
    
    //////////////////////////////////////////////////
    // Start Main solve loop - continue until all players finish or all stacks are empty
    while (Players_finished < NUM_PLAYERS) {
        // Check if all stacks are empty (no more moves for any player)
        let all_empty = 1;
        for (let p = 0; p < NUM_PLAYERS; p++) {
            if (stacks[p].length > 0 && !players[p].reached_goal &&
                !players[p].abandoned_race) {
                all_empty = 0;
                break;
            }
        }
        
        // After checking if all stacks are empty:
        if (all_empty) {
            // Before breaking, mark any players who didn't finish as having abandoned
            for (let p = 0; p < NUM_PLAYERS; p++) {
                if (!players[p].reached_goal && !players[p].abandoned_race) {
                    players[p].abandoned_race = 1;
                    await highlight_player_solution_path(p);
                    await display_player_alert(p + 1, -1);
                }
            }
            break;
        }
        
        /////////////////////////////////////////
        // Start Rotate through players, giving each a turn to move one step
        for (let p = 0; p < NUM_PLAYERS; p++) {
            // Skip players who have already reached their goal or abandoned
            if (players[p].reached_goal || players[p].abandoned_race)
                continue;
            
            // Check if this player's stack is empty
            if (stacks[p].length === 0) {
                // Mark player as abandoned if their stack is empty and they haven't reached their goal
                if (!players[p].abandoned_race && !players[p].reached_goal) {
                    players[p].abandoned_race = 1;
                    await highlight_player_solution_path(p);
                    await display_player_alert(p + 1, -1);
                }
                continue;
            }
            
            // update current position
            let player_id = p + 1;
            let current = stacks[p].pop();
            players[p].current = current;
            
            // Check for player vs player collision
            for (let other_p = 0; other_p < NUM_PLAYERS; other_p++) {
                // Skip self and players who've reached their goal or abandoned
                if (other_p === p || players[other_p].reached_goal ||
                    players[other_p].abandoned_race) {
                    continue;
                }
                
                // Check if players collide
                if (players[p].current.x === players[other_p].current.x &&
                    players[p].current.y === players[other_p].current.y) {
                    
                    // Initiate player vs player battle
                    let winner_id = await battle_bots(player_id, other_p + 1);
                    
                    // Loser must retreat to previous position
                    if (winner_id === player_id) {
                        // Other player retreats to previous position
                        if (previous_positions[other_p].x !== -1) {
                            players[other_p].current = previous_positions[other_p];
                        }
                        // Check if player has lost too many battles
                        if (players[other_p].battles_lost >= 3) {
                            players[other_p].abandoned_race = 2;
                            await highlight_player_solution_path(other_p);
                            await display_player_alert(other_p + 1, -2);
                            
                            // Clear the stack to stop the player's exploration
                            stacks[other_p] = [];
                        }
                        continue;
                        
                    } else if (winner_id < 0) {
                        ; // no battle took place
                    } else {
                        // Current player retreats to parent position
                        players[p].current.x = current.parentX;
                        players[p].current.y = current.parentY;
                        
                        // If no parent position, stay at current but mark as visited to avoid revisiting
                        if (players[p].current.x === -1 || players[p].current.y === -1) {
                            players[p].current = current;
                            maze.visited[p][current.y][current.x] = 1;
                        }
                        // Check if player has lost too many battles
                        if (players[p].battles_lost >= 3) {
                            players[p].abandoned_race = 2;
                            await highlight_player_solution_path(p);
                            await display_player_alert(player_id, -2);
                            
                            // Clear the stack to stop the player's exploration
                            stacks[p] = [];
                        }
                        continue;
                    }
                    
                    // Update display after battle
                    await print_maze();
                    await display_player_stats();
                }
            }
            
            // Store current position for potential retreat
            previous_positions[p].x = players[p].current.x;
            previous_positions[p].y = players[p].current.y;
            players[p].moves++;
            
            // Check if reached the end
            if (current.x === players[p].end.x && current.y === players[p].end.y) {
                players[p].reached_goal = 1;
                Players_finished++;
                players[p].finished_rank = Players_finished;
                
                // Mark as finished in UI
                await highlight_player_solution_path(p);
                await display_player_alert(player_id, players[p].finished_rank);
                continue;
            }
            
            // Check for teleporter
            let newPos = { x: 0, y: 0 };
            let teleporter_idx = -1;
            for (let i = 0; i < Num_teleporters; i++) {
                if ((teleporters[i].x1 === current.x &&
                    teleporters[i].y1 === current.y) ||
                    (teleporters[i].x2 === current.x &&
                        teleporters[i].y2 === current.y)) {
                    teleporter_idx = i;
                    break;
                }
            }
            
            if (!players[p].justTeleported && teleporter_idx >= 0 &&
                await check_teleporter(current.x, current.y, newPos)) {
                
                // Record teleportation
                players[p].justTeleported = Game_moves + 2;
                
                let teleported = new Position(newPos.x, newPos.y, current.x, current.y);
                stacks[p].push(teleported);
                parent_map[p][newPos.y][newPos.x].x = current.x;
                parent_map[p][newPos.y][newPos.x].y = current.y;
                
                // Mark destination as visited
                maze.visited[p][newPos.y][newPos.x] = 1;
                
                // Visualize teleportation
                for (let i = 0; i < 5; i++) { // Flicker for 3 cycles
                    mvprintw(newPos.y, newPos.x, TELEPORTER_CHAR);
                    mvprintw(current.y, current.x, TELEPORTER_CHAR);
                    wnoutrefresh(stdscr);
                    await mysleep(parseInt(pauseTime / 40));           // Short delay
                    
                    attron(A_REVERSE);
                    mvprintw(newPos.y, newPos.x, TELEPORTER_CHAR);
                    mvprintw(current.y, current.x, TELEPORTER_CHAR);
                    attroff(A_REVERSE);
                    wnoutrefresh(stdscr);
                    await mysleep(parseInt(pauseTime / 40));            // Another delay
                }
                continue;
            }
            
            // Check for monster
            let monster_idx = await check_monster(current.x, current.y);
            if (monster_idx > 0) {
                monster_idx--; // Adjust index (monster_idx was returned +1)
                let battle_result = await battle_bot_monster(monster_idx, player_id);
                
                if (battle_result === 0) {
                    // Player lost battle, mark position as visited in player's array
                    maze.visited[p][current.y][current.x] = 1;
                    
                    // Also update the visualization
                    let visited_char = await get_player_visited_char(player_id);
                    if (
                        maze.grid[current.y][current.x] !== END1 &&
                        maze.grid[current.y][current.x] !== END2 &&
                        maze.grid[current.y][current.x] !== END3 &&
                        maze.grid[current.y][current.x] !== END4 &&
                        maze.grid[current.y][current.x] !== TELEPORTER) {
                        maze.grid[current.y][current.x] = visited_char;
                    }
                    
                    // Check if player has lost too many battles
                    if (players[p].battles_lost >= 3) {
                        players[p].abandoned_race = 2;
                        await highlight_player_solution_path(p);
                        await display_player_alert(player_id, -2);
                        
                        // Clear the stack to stop the player's exploration
                        stacks[p] = [];
                    }
                    
                    continue;
                } else if (battle_result < 0) {
                    ; // no battle took place
                }
            }
            
            // Mark as visited in player's separate visited array
            maze.visited[p][current.y][current.x] = 1;
            
            // Also update the visualization in the shared grid
            let visited_char = await get_player_visited_char(player_id);
            if (
                maze.grid[current.y][current.x] !== END1 &&
                maze.grid[current.y][current.x] !== END2 &&
                maze.grid[current.y][current.x] !== END3 &&
                maze.grid[current.y][current.x] !== END4 &&
                maze.grid[current.y][current.x] !== TELEPORTER &&
                maze.grid[current.y][current.x] !== MONSTER &&
                maze.grid[current.y][current.x] !== DEFEATED_MONSTER) {
                maze.grid[current.y][current.x] = visited_char;
            }
            
            // start Try all possible directions using player-specific direction arrays
            for (let i = 0; i < 4; i++) {
                let dir_idx = i;
                let nextX = current.x + players[p].dx[dir_idx];
                let nextY = current.y + players[p].dy[dir_idx];
                
                // Don't go back to parent
                if (nextX === current.parentX && nextY === current.parentY) {
                    continue;
                }
                
                if (nextX >= 0 && nextX < maze.cols && nextY >= 0 && nextY < maze.rows) {
                    let cell = maze.grid[nextY][nextX];
                    
                    // Allow exploring if not a wall and not visited by THIS player
                    if (cell !== WALL && maze.visited[p][nextY][nextX] === 0) {
                        maze.visited[p][nextY][nextX] = 1; // Mark as visited for this player
                        
                        let nextPos = new Position(nextX, nextY, current.x, current.y);
                        stacks[p].push(nextPos);
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
        if (Game_moves % 5 === 0) {
            await update_monsters();
        }
        
        // Visualize exploration after each player's move
        await print_maze();
        await display_player_stats();
        await read_keyboard();
        if (LastSLupdate && LastSLupdate + 25 < Game_moves) {
            move(Maze_rows + 5, 0);
            wclrtoeol(stdscr);
        }
        await mysleep(Game_delay);
        
        // Update game moves
        Game_moves++;
        for (let i = 0; i < NUM_PLAYERS; i++) {
            // prevent teleport loops
            if (players[i].justTeleported < Game_moves)
                players[i].justTeleported = 0;
            // decrement recovery_turns
            if (players[i].recovery_turns)
                players[i].recovery_turns--;
        }
        
				//  check for abort
				if(Abort_run_round) {
					Screen_reduced = 1;
					Game_finished = NUM_PLAYERS;
					break;
				}
				LastUpdate = Date.now();
				
        // Check for window resize
        if (1) {
            let size = get_terminal_size();
            
            if (old_rows !== size.rows || old_cols !== size.cols) {
                await update_status_line(`Caught term resize to ${size.rows}, ${size.cols}`);
                await logMessage(`Caught term resize to ${size.rows}, ${size.cols}`);
                if (old_rows > size.rows || old_cols > size.cols) {
                    // abort current solve as screen has reduced in size
                    Screen_reduced = 1;
                    old_rows = size.rows;
                    old_cols = size.cols;
                    // spin wheels in lieu of debounce
                    await mysleep(2000);
                    // break
                    break;
                }
                old_rows = size.rows;
                old_cols = size.cols;
            }
        }
        doupdate();
    }
    // end Main solve loop
    //////////////////////////////////////////////////
    
    // Show final results
    if (!Screen_reduced) { 
		  await display_player_stats();
      doupdate();
		}
}

async function print_char(i, j, ichar) {
    move(i, j);
    // Otherwise, display the appropriate cell character
    switch (ichar) {
        // Player 1 cells
        case CURRENT1:
            attron(COLOR_PAIR(2) | A_BOLD);
            addstr(CURRENT_CHAR1);
            attroff(COLOR_PAIR(2) | A_BOLD);
            break;
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
        case CURRENT2:
            attron(COLOR_PAIR(3) | A_BOLD);
            addstr(CURRENT_CHAR2);
            attroff(COLOR_PAIR(3) | A_BOLD);
            break;
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
        case CURRENT3:
            attron(COLOR_PAIR(4) | A_BOLD);
            addstr(CURRENT_CHAR3);
            attroff(COLOR_PAIR(4) | A_BOLD);
            break;
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
        case CURRENT4:
            attron(COLOR_PAIR(5) | A_BOLD);
            addstr(CURRENT_CHAR4);
            attroff(COLOR_PAIR(5) | A_BOLD);
            break;
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
        case WALL:
            addstr(WALL_CHAR);
            break;
        case PATH:
            attron(COLOR_PAIR(1));
            addstr(PATH_CHAR);
            attroff(COLOR_PAIR(1));
            break;
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

// print_maze to display all players, teleporters, and monsters
async function print_maze() {
    // Get current terminal dimensions
    let termSize = getmaxyx(stdscr);
    let term_rows = termSize.rows;
    let term_cols = termSize.cols;
    
    // Ensure we don't write outside the terminal
    let visible_rows = (maze.rows < term_rows - 6) ? maze.rows : term_rows - 6;
    let visible_cols = (maze.cols < term_cols) ? maze.cols : term_cols - 1;
    
    let current_positions = [];
    for (let i = 0; i < NUM_PLAYERS; i++) {
        current_positions.push(players[i].current);
    }
    
    for (let i = 0; i < visible_rows; i++) {
        for (let j = 0; j < visible_cols; j++) {
            let ichar;
            let player = await is_player_position(j, i, current_positions);
            if (player > 0) {
                ichar = await get_player_current_char(player);
            } else {
                ichar = maze.grid[i][j];
            }
            await print_char(i, j, ichar);
        }
    }
    
    // print game info
    attron(COLOR_PAIR(11) | A_BOLD);
    mvwprintw(stdscr, 0, 1,
        `[Maze Game] ${Game_speed} s, ${Num_teleporters} t, ${Liv_monsters}/${Num_monsters} [${Max_monster_strength}] m, ${Game_rounds}/${Game_roundsB} r`);
    attroff(COLOR_PAIR(11) | A_BOLD);
}

async function push_stack(stack, pos) {
    stack.push(pos);
}

async function pop_stack(stack) {
    if (stack.length === 0) {
        return new Position(-1, -1, -1, -1);
    }
    
    return stack.pop();
}

async function is_empty(stack) {
    return stack.length === 0;
}

async function clear_stack(stack) {
    stack.length = 0;
}

async function animate_bullseye(y, x, max_radius, delay_ms, direction) {
    // UTF-8 characters for different rings of the bullseye
    const ring_chars = [
        "●", // Filled circle
        "◉", // Bullseye
        "◎", // Circle with dot
        "○", // Empty circle
        "·"  // Small dot
    ];
    const num_rings = ring_chars.length;
    
    // Save current colors
    let old_pair = PAIR_NUMBER(getattrs(stdscr));
    
    // Initialize color pairs if they haven't been already
    if (!has_colors()) {
        wprintw(stdscr, "Your terminal does not support colors\n");
        return;
    }
    
    // Clear any previous content
    wnoutrefresh(stdscr);
    
    let start, end, inc;
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
    for (let current_radius = start;
        (inc > 0) ? (current_radius <= end) : (current_radius >= end);
        current_radius += inc) {
        
        // Draw each ring of the bullseye
        for (let r = current_radius; r >= 0; r--) {
            // Select color based on ring position
            let color_pair = (r % 5) + 1;
            attron(COLOR_PAIR(color_pair));
            
            // Select character based on position in the bullseye
            let ring_char = ring_chars[r % num_rings];
            
            // For the innermost ring (when r=0), always use the filled circle
            if (r === 0) {
                ring_char = ring_chars[0];
                attron(COLOR_PAIR(1)); // Red for the center
            }
            
            // Draw a complete ring at radius r
            for (let i = 0; i <= 360; i += 10) {
                let angle = i * Math.PI / 180.0;
                let ring_y = y + Math.floor(r * Math.sin(angle));
                let ring_x = x + Math.floor(r * 2 * Math.cos(angle)); // Multiply by 2 to account for character aspect ratio
                
                // Make sure we don't draw outside the screen
                if (ring_y >= 0 && ring_y < LINES && ring_x >= 0 && ring_x < COLS - 4) {
                    // Use wide character functions for UTF-8
                    mvaddwstr(ring_y, ring_x, ring_char);
                }
            }
            
            // Reset attributes
            attroff(COLOR_PAIR(color_pair));
        }
        
        // Refresh the screen
        wnoutrefresh(stdscr);
        
        // Delay between frames
        await mysleep(delay_ms);
    }
    
    // Restore original color pair
    attron(COLOR_PAIR(old_pair));
}

// Read high scores from localStorage
async function read_high_scores(best_scores, worst_scores) {
    try {
        // Attempt to retrieve scores from localStorage
        const bestScoresData = localStorage.getItem('bestScores');
        const worstScoresData = localStorage.getItem('worstScores');
        const countData = localStorage.getItem('scoresCount');
        
        // Parse count or default to 0
        let count = parseInt(countData) || 0;
        
        if (bestScoresData) {
            // Parse and populate best_scores array
            const bestScoresArray = JSON.parse(bestScoresData);
            bestScoresArray.forEach((scoreData, index) => {
                best_scores[index] = new HighScore(
                    scoreData.name,
                    scoreData.player_id,
                    scoreData.score,
                    scoreData.battles_won,
                    scoreData.strength,
                    scoreData.this_run,
                    scoreData.date
                );
            });
        }
        
        if (worstScoresData) {
            // Parse and populate worst_scores array
            const worstScoresArray = JSON.parse(worstScoresData);
            worstScoresArray.forEach((scoreData, index) => {
                worst_scores[index] = new HighScore(
                    scoreData.name,
                    scoreData.player_id,
                    scoreData.score,
                    scoreData.battles_won,
                    scoreData.strength,
                    scoreData.this_run,
                    scoreData.date
                );
            });
        }
        
        return count;
    } catch (error) {
        console.error("Error reading high scores:", error);
        return 0; // Return 0 on error
    }
}

// Save high scores to localStorage
async function save_high_scores(best_scores, worst_scores, count) {
    try {
        // Convert best_scores array to plain objects for storage
        const bestScoresArray = best_scores.map(score => ({
            name: score.name,
            player_id: score.player_id,
            score: score.score,
            battles_won: score.battles_won,
            strength: score.strength,
            this_run: 0,
            date: score.date
        }));
        
        // Convert worst_scores array to plain objects for storage
        const worstScoresArray = worst_scores.map(score => ({
            name: score.name,
            player_id: score.player_id,
            score: score.score,
            battles_won: score.battles_won,
            strength: score.strength,
            this_run: 0,
            date: score.date
        }));
        
        // Store in localStorage
        localStorage.setItem('bestScores', JSON.stringify(bestScoresArray));
        localStorage.setItem('worstScores', JSON.stringify(worstScoresArray));
        localStorage.setItem('scoresCount', count.toString());
        
        return true; // Indicate successful save
    } catch (error) {
        console.error("Error saving high scores:", error);
        return false; // Indicate failed save
    }
}

// Insert a score into the high score lists
async function insert_high_score(scores, count, new_score, is_best) {
    let i, pos = -1;
    
    // Determine the position to insert based on whether it's best or worst list
    for (i = 0; i < count; i++) {
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
    if (pos === -1 && count < MAX_HIGH_SCORES) {
        pos = count;
        count++;
    }
    
    // If found a position to insert
    if (pos !== -1) {
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
    
    return count;
}

// Update high scores at the end of the game
async function update_high_scores(rows, cols) {
    let num_players = NUM_PLAYERS;
    let best_scores = [];
    let worst_scores = [];
    
    for (let i = 0; i < MAX_HIGH_SCORES; i++) {
        best_scores.push(new HighScore("", 0, 0, 0, 0, 0, 0));
        worst_scores.push(new HighScore("", 0, 0, 0, 0, 0, 0));
    }
    
    let count = 0 ;
    
    // Read existing high scores
    count = await read_high_scores(best_scores, worst_scores);
    
    // Get the current date/time
    let current_time = Math.floor(Date.now() / 1000);
    
    // Process players who finished the race
    for (let i = 0; i < num_players; i++) {
        if (players[i].reached_goal && players[i].finished_rank === 1) {
            // Create a new score entry
            let new_score = new HighScore();
            
            // Use the bot name based on player ID
            let name_index = players[i].id - 1;
            if (name_index >= 0 && name_index < NUM_PLAYERS) {
                new_score.name = BOT_NAMES[name_index + 1];
            } else {
                // invalid ID
                exit_game(`invalid name_index ${name_index}\n`);
            }
            
            new_score.player_id = players[i].id;
            new_score.score = await calculate_score(players[i].moves, cols, rows);
            new_score.battles_won = players[i].battles_won;
            new_score.strength = players[i].strength;
            new_score.date = current_time;
            
            // Try to insert into best scores
            count = await insert_high_score(best_scores, count, new_score, 1);
            
            // Also track the worst scores for fun
            // (only if they actually finished though)
            count = await insert_high_score(worst_scores, count, new_score, 0);
        }
    }
    
    // Display the high score window
    await display_high_scores_window(count, best_scores, worst_scores);
    
    // Save updated high scores
    await save_high_scores(best_scores, worst_scores, count > 0 ? count : 0);
}

// Display high scores in a ncurses window
async function display_high_scores_window(count, best_scores, worst_scores) {
    if (count === 0 || ShowWindows === 0) {
        // No high scores yet
        await pauseForUser();
        return;
    }
    
    // Save current state to restore later
    let old_curs = curs_set(0); // Hide cursor
    
    // Calculate window dimensions
    let height = MIN_ROWS - 4;
    let width = MIN_COLS;
    
    // Create a new window centered on screen
    let max_y, max_x;
    let termSize = getmaxyx(stdscr);
    max_y = termSize.rows;
    max_x = termSize.cols;
    let y_pos = max_y === MIN_ROWS ? 0 : 1;
    
    let high_score_win = newwin(height, width, y_pos, Math.floor((max_x - width) / 2));
    box(high_score_win, 0, 0);
    
    // Add a title
    wattron(high_score_win, A_BOLD);
    mvwprintw(high_score_win, 1, Math.floor((width - 27) / 2), "*** BEST & WORST SCORES ***");
    wattroff(high_score_win, A_BOLD);
    
    // Display best scores section title
    wattron(high_score_win, A_UNDERLINE);
    mvwprintw(high_score_win, 3, 3, "BEST (FASTEST) SCORES");
    wattroff(high_score_win, A_UNDERLINE);
    
    // Display header
    mvwprintw(high_score_win, 4, 3, "# Name   Scor Ba St Date");
    
    // Display best scores
    let displayed_count = Math.min(count, 5); // Limit displayed scores
    for (let i = 0; i < displayed_count; i++) {
        let date_str = new Date(best_scores[i].date * 1000).toISOString().split('T')[0];
        
        // Highlight the new high score
        if (best_scores[i].this_run) {
            wattron(high_score_win, COLOR_PAIR(players[best_scores[i].player_id - 1].color_pair) | A_BOLD);
        }
        
        mvwprintw(high_score_win, 5 + i, 3, 
            `${i + 1} ${best_scores[i].name.padEnd(6)} ${String(best_scores[i].score).padStart(4)} ${String(best_scores[i].battles_won).padStart(2)} ${String(best_scores[i].strength).padStart(2)} ${date_str}`);
        
        if (best_scores[i].this_run) {
            wattroff(high_score_win, COLOR_PAIR(players[best_scores[i].player_id - 1].color_pair) | A_BOLD);
        }
    }
    
    // Display worst scores section title
    wattron(high_score_win, A_UNDERLINE);
    mvwprintw(high_score_win, 11, 3, "WORST (SLOWEST) SCORES");
    wattroff(high_score_win, A_UNDERLINE);
    
    // Display header
    mvwprintw(high_score_win, 12, 3, "# Name   Scor Ba St Date");
    
    // Display worst scores
    for (let i = 0; i < displayed_count; i++) {
        let date_str = new Date(worst_scores[i].date * 1000).toISOString().split('T')[0];
        
        // Highlight the new high score
        if (worst_scores[i].this_run) {
            wattron(high_score_win, COLOR_PAIR(players[worst_scores[i].player_id - 1].color_pair) | A_BOLD);
        }
        
        mvwprintw(high_score_win, 13 + i, 3,
            `${i + 1} ${worst_scores[i].name.padEnd(6)} ${String(worst_scores[i].score).padStart(4)} ${String(worst_scores[i].battles_won).padStart(2)} ${String(worst_scores[i].strength).padStart(2)} ${date_str}`);
        
        if (worst_scores[i].this_run) {
            wattroff(high_score_win, COLOR_PAIR(players[worst_scores[i].player_id - 1].color_pair) | A_BOLD);
        }
    }
    
    // If we have a new best score, add some congratulatory text
    if (best_scores[0].this_run) {
        wattron(high_score_win, A_BOLD);
        mvwprintw(high_score_win, (height - 2), Math.floor((width - 34) / 2),
            `** NEW CHAMPION! ${BOT_NAMES[best_scores[0].player_id]} **`);
        wattroff(high_score_win, A_BOLD);
    }
    
    // If we have a new worst score, add some congratulatory text
    if (worst_scores[0].this_run) {
        wattron(high_score_win, A_BOLD);
        mvwprintw(high_score_win, (height - 2), Math.floor((width - 34) / 2),
            `** NEW WORST :( ${BOT_NAMES[worst_scores[0].player_id]} **`);
        wattroff(high_score_win, A_BOLD);
    }
    
    // Refresh and show the window
    wnoutrefresh(high_score_win);
    
    // Wait for user input
    await pauseForUser();
    
    // Clean up
    delwin(high_score_win);
    curs_set(old_curs); // Restore cursor state
    
    // Refresh the screen to remove the window
    wnoutrefresh(stdscr);
}

const OPTIMAL_PATH_FACTOR = 1.5;
const SCORE_MULTIPLIER = 100.0;
const COMPLEXITY_ADJUSTMENT_FACTOR = 10.0;

async function calculate_score(moves, width, height) {
    if (moves <= 0 || width <= 0 || height <= 0) {
        return -1; // Error code for invalid input
    }
    
    // Calculate the effective maze dimensions (in cells, not walls)
    let effective_width = Math.floor((width - 1) / 2.0);
    let effective_height = Math.floor((height - 1) / 2.0);
    
    // Estimate optimal solution length
    let estimated_optimal_path = OPTIMAL_PATH_FACTOR * (effective_width + effective_height);
    
    // Maze complexity factor
    let maze_complexity = effective_width * effective_height;
    
    // Calculate score (lower is better)
    // Base score is the actual moves normalized by estimated optimal path
    let efficiency_factor = moves / estimated_optimal_path;
    
    // Apply a small adjustment based on maze complexity
    // This makes larger mazes slightly easier to score well on
    let complexity_adjustment = Math.log(maze_complexity) / COMPLEXITY_ADJUSTMENT_FACTOR;
    
    // Final score - lower is better
    let score = Math.round(SCORE_MULTIPLIER * (efficiency_factor - complexity_adjustment));
    
    // Ensure score is always positive
    return score > 0 ? score : 1;
}

async function pauseForUser() {
    await read_keyboard();
    if (WaitForKey) {
        await pauseGame();  // calls doupdate()
    } else {
        await update_status_line(DELAY_MSG);
        await mysleep(pauseTime); // calls doupdate()
    }
    move(Maze_rows + 5, 0);
    wclrtoeol(stdscr);
    await read_keyboard();
    doupdate();
}

async function calc_game_speed() {
    let min_speed = 1.0;    // slowest
    let max_speed = 100.0;  // fastest
    if (Game_speed < min_speed) {
        Game_speed = DEF_GAME_SPEED;
    }
    if (Game_speed > max_speed) {
        Game_speed = DEF_GAME_SPEED;
    }
    // Map Game_speed logarithmically to [0, 1]
    let normalized_speed = (Math.log(Game_speed) - Math.log(min_speed)) / (Math.log(max_speed) - Math.log(min_speed));
    // Calculate delay (inverse relationship with normalized_speed)
    Game_delay = Math.round((1.0 - normalized_speed) * MAX_GAME_DELAY);
}

function exit_game(format, ...args) {
    try {
      wnoutrefresh(stdscr);
      doupdate();
      curs_set(1);
      endwin();
      console.log(format, ...args);
    } catch (error) {
      console.log("ignoring exit_game() error: ", error);
    }
    showGameOverScreen();  
    throw new Error(EXIT_PROG);
}

async function update_status_line(format, ...args) {
    let direction = 0;
    let buffer = "";
    let upchar = " ";
    
    if (!isNaN(parseInt(format))) {
        // a key
        direction = parseInt(format);
    } else {
        // a regular string
        direction = 0;
    }
    
    // Case 1: Add a new message to history
    if (direction === 0) {
        // Format the message (simplified in JS)
        buffer = format;
        if (args.length > 0) {
            // Simple string formatting simulation
            for (let arg of args) {
                buffer = buffer.replace(/%[sdif]/, arg);
            }
        }
        
        if (buffer.includes(LOST_MSG)) {
            // Shift all messages down in the array
            for (let i = STATUS_LINE_HISTORY - 1; i > 1; i--) {
                status_lines[i].msg = status_lines[i - 1].msg;
                status_lines[i].move = status_lines[i - 1].move;
            }
            // Insert new message at the front
            status_lines[1].msg = buffer;
            status_lines[1].move = Game_moves;
        }
        // Reset viewing index to current message
        current_status_index = 0;
    }
    // Case 2: Navigate to previous (older) message
    else if (direction === KEY_UP) {
        // Move up in history if there's a valid message
        if (
            current_status_index < STATUS_LINE_HISTORY - 1 &&
            status_lines[current_status_index + 1].msg.length > 0
        ) {
            current_status_index++;
        }
    }
    // Case 3: Navigate to next (newer) message
    else if (direction === KEY_DOWN) {
        // Move down in history if not already at newest
        if (current_status_index > 0) {
            current_status_index--;
        }
    } else {
        exit_game(`Illegal value for direction ${direction}\n`);
    }
    
    attron(COLOR_PAIR(11) | A_BOLD);
    if (current_status_index === 0) {
        // Current message - display normally
        if (direction === 0) {
            LastSLupdate = Game_moves;
            mvwprintw(stdscr, Maze_rows + 5, 0, buffer);
        } else {
            mvwprintw(stdscr, Maze_rows + 5, 0, PAUSE_MSG);
        }
    } else {
        // Past message - show indicator
        if (
            current_status_index < STATUS_LINE_HISTORY - 1 &&
            status_lines[current_status_index + 1].msg.length > 0
        ) {
            upchar = "↑";
        }
        mvwprintw(stdscr, Maze_rows + 5, 0, `${upchar}↓[${status_lines[current_status_index].move}] ${status_lines[current_status_index].msg}`);
    }
    wclrtoeol(stdscr);
    attroff(COLOR_PAIR(11) | A_BOLD);
    wnoutrefresh(stdscr);
}

async function read_keyboard() {
    let ch;
    let was_paused = 0;
    
    if (in_read_keyboard) return 0;
    in_read_keyboard = 1;
    nodelay(stdscr, true);
    ch = await getch();
    nodelay(stdscr, false);
    
    switch (ch) {
        // PAUSE with SPACE
        case 32:
            await pauseGame();
            was_paused = 1;
            break;
        // Toggle WaitForKey with K
        case 75:
        case 107:
            await update_status_line("Toggled WaitForKey");
            WaitForKey = 1 - WaitForKey;
            break;
        // Toggle ShowWindows with W
        case 87:
        case 119:
            await update_status_line("Toggled ShowWindows");
            ShowWindows = 1 - ShowWindows;
            break;
        // SLOW DOWN with MINUS
        case 95:
        case 45:
            await update_status_line("Slowing down...");
            Game_speed--;
            await calc_game_speed();
            break;
        // SPEED UP with PLUS
        case 61:
        case 43:
            await update_status_line("Speeding up...");
            Game_speed++;
            await calc_game_speed();
            break;
        // EXIT with ESCAPE or q/Q
        case 113:
        case 81:
        case 27:
            exit_game("User ended game early\n");
            break;
        // HELP with h or ?
        case 104:
        case 63:
            await show_help_window();
            break;
        // EXTENDED HELP with H
        case 72:  // 'H'
            await show_extended_help_window();
            break;
        case KEY_RESIZE:
            break;
        case ERR:
            break;
        case KEY_MOUSE:
            break;
        default:
            await update_status_line(`Unrecognized key ${ch}`);
    }
    in_read_keyboard = 0;
    return was_paused;
}

async function pauseGame() {
    let key = "";
    let paused;
    let ch;
    
    if (in_pausegame) return;
    in_pausegame = 1;
    
    current_status_index = 0; // Reset to current message when pausing
    await update_status_line(PAUSE_MSG);
    tcflush(STDIN_FILENO, TCIFLUSH);
    paused = 1;
    
    // Enter pause mode loop
    while (paused) {
        doupdate();
        ch = await getch();
        
        // Handle arrow keys for status history browsing
        if (ch === KEY_UP || ch === 259) {
            // Move to older message
            await update_status_line(String(KEY_UP));
        }
        else if (ch === KEY_DOWN || ch === 258) {
            // Move to newer message
            await update_status_line(String(KEY_DOWN));
        }
        // Exit game early
        else if (ch === 113 || ch === 81 || ch === 27) {
            exit_game("User ended game early\n");
        }
        // Leave pause mode on any other key press (that's printable)
        else if (ch >= 32 && ch <= 126) {
            paused = 0;
        }
    }
    
    // Reset to current message when unpausing
    current_status_index = 0;
    await update_status_line("Continuing...");
    tcflush(STDIN_FILENO, TCIFLUSH);
    doupdate();
    in_pausegame = 0;
}

// For debugging
async function logMessage(format, ...args) {
    // In a real implementation this would log to a file or console
    console.log(format, ...args);
}

async function sleep_millis(ms) {
    await new Promise(resolve => setTimeout(resolve, ms));
    return;
}

async function delay_with_polling(total_delay_ms) {
    let start, end;
    let read_duration_ms;
    let sleep_time_ms;
    let remaining_time_ms = total_delay_ms;
    
    do {
			  if(Abort_run_round) {
					break;
				}
        if (remaining_time_ms > POLL_INTERVAL_MS) {
            // do read keyboard
            start = Date.now();
            if (await read_keyboard()) break;
            end = Date.now();
            
            // calc how long that took
            read_duration_ms = end - start;
            
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
            await sleep_millis(sleep_time_ms);
            remaining_time_ms -= sleep_time_ms;
        } else {
          break;
        }        
        // calc remaining sleep time
    } while (remaining_time_ms > 0);
}

async function mysleep(total_delay_ms) {
    if (in_mysleep) return;
    in_mysleep = 1;
    doupdate();
    
    if (total_delay_ms <= POLL_INTERVAL_MS) {
        await sleep_millis(total_delay_ms);
    } else {
        await delay_with_polling(total_delay_ms);
    }
    
    in_mysleep = 0;
}

// function to mark the solution path for a player
async function highlight_player_solution_path(p) {
    // Now mark each position in the path with solution char
    let solution_char = await get_player_solution_char(p + 1);
    let current_char = await get_player_current_char(p + 1);
    let end = players[p].current;
    let x = end.x, y = end.y;
    
    while (!(x === players[p].start.x && y === players[p].start.y)) {
        // Only overwrite certain cells (don't overwrite special cells)
        if (maze.grid[y][x] !== END1 &&
            maze.grid[y][x] !== END2 &&
            maze.grid[y][x] !== END3 &&
            maze.grid[y][x] !== END4 &&
            maze.grid[y][x] !== TELEPORTER &&
            maze.grid[y][x] !== MONSTER &&
            maze.grid[y][x] !== DEFEATED_MONSTER
        ) {
            maze.grid[y][x] = solution_char;
        }
        
        let px = parent_map[p][y][x].x;
        let py = parent_map[p][y][x].y;
        x = px;
        y = py;
    }
    
    await print_maze();
    await display_player_stats();
    
    // Visualize end point
    for (let i = 0; i < 5; i++) { // Flicker for 3 cycles
        await print_char(end.y, end.x, current_char);
        wnoutrefresh(stdscr);
        await mysleep(parseInt(pauseTime / 40));           // Short delay (50ms)
        
        attron(A_REVERSE);
        await print_char(end.y, end.x, current_char);
        attroff(A_REVERSE);
        wnoutrefresh(stdscr);
        await mysleep(parseInt(pauseTime / 40));            // Another delay
    }
}

async function show_help_window() {
    if (ShowWindows === 0) return;
    
    let height = 20;
    let width = 60;
    let max_y, max_x;
    let termSize = getmaxyx(stdscr);
    max_y = termSize.rows;
    max_x = termSize.cols;
    
    let help_win = newwin(height, width, Math.floor((max_y - height) / 2), Math.floor((max_x - width) / 2));
    box(help_win, 0, 0);
    
    wattron(help_win, A_BOLD);
    mvwprintw(help_win, 1, 2, "Maze4 Game Help");
    wattroff(help_win, A_BOLD);
    
    mvwprintw(help_win, 3, 2, "Controls:");
    mvwprintw(help_win, 4, 4, "[Space]   - Pause/Continue");
    mvwprintw(help_win, 5, 4, "[+/-]     - Speed Up / Slow Down");
    mvwprintw(help_win, 6, 4, "[h/?]     - Show this Help");
    mvwprintw(help_win, 7, 4, "[H]       - Show Extended Help.");
    mvwprintw(help_win, 8, 4, "[q/Q/ESC] - Quit Game");
    mvwprintw(help_win, 9, 4, "[k]       - Toggle WaitForKey");
    mvwprintw(help_win,10, 4, "[w]       - Toggle ShowWindows");
    mvwprintw(help_win,12, 2, "During Pause:");
    mvwprintw(help_win,13, 4, "[Up/Down] - Scroll Status Messages");
    
    mvwprintw(help_win,15, 2, "Press any key to close...");
    
    wnoutrefresh(help_win);
    doupdate();
    
    await getch(); // Wait for any key
    delwin(help_win);
    wnoutrefresh(stdscr);
}

async function show_extended_help_window() {
    if (ShowWindows === 0) return;
    
    let height = 24;
    let width = 70;
    let max_y, max_x;
    let termSize = getmaxyx(stdscr);
    max_y = termSize.rows;
    max_x = termSize.cols;
    
    let help_win = newwin(height, width, Math.floor((max_y - height) / 2), Math.floor((max_x - width) / 2));
    box(help_win, 0, 0);
    
    wattron(help_win, A_BOLD);
    mvwprintw(help_win, 1, 2, "Maze4 Extended Help");
    wattroff(help_win, A_BOLD);
    
    mvwprintw(help_win, 3, 2, "Story:");
    mvwprintw(help_win, 4, 4, "In the Maze of Munch, four Fruitbots —");
    mvwprintw(help_win, 5, 4, "Rapid Raspberry, Bussin' Blueberry,");
    mvwprintw(help_win, 6, 4, "Lightning Lemon, and Kwick Kiwi — race");
    mvwprintw(help_win, 7, 4, "for glory against deadly Veggie Monsters.");
    
    mvwprintw(help_win, 9, 2, "Goal:");
    mvwprintw(help_win,10, 4, "Reach your goal first! Battle monsters,");
    mvwprintw(help_win,11, 4, "survive ambushes, and outmaneuver rivals.");
    mvwprintw(help_win,12, 4, "Three battle losses = system failure!");
    
    mvwprintw(help_win,14, 2, "Inspiration:");
    mvwprintw(help_win,15, 4, "Inspired by classic maze screensavers");
    mvwprintw(help_win,16, 4, "and the chaotic fun of marble races.");
    mvwprintw(help_win,17, 4, "All game logic written with AI assistance.");
    
    mvwprintw(help_win,19, 2, "Press any key to close...");
    
    wnoutrefresh(help_win);
    doupdate();
    
    await getch(); // Wait for any key
    delwin(help_win);
    wnoutrefresh(stdscr);
}

///// HTML VERSION ONLY ///////////////////////

function showGameOverScreen() {
    // Create a semi-transparent overlay for the whole screen
    const overlay = document.createElement('div');
    overlay.style.position = 'absolute';
    overlay.style.top = '0';
    overlay.style.left = '0';
    overlay.style.width = '100%';
    overlay.style.height = '100%';
    overlay.style.backgroundColor = 'rgba(0, 0, 0, 0.7)';
    overlay.style.zIndex = '1000';
    overlay.style.display = 'flex';
    overlay.style.flexDirection = 'column';
    overlay.style.justifyContent = 'center';
    overlay.style.alignItems = 'center';
    overlay.style.color = '#fff';
    overlay.style.fontFamily = 'Courier New, monospace';
    
    // Create the game over message
    const gameOverBox = document.createElement('div');
    gameOverBox.style.backgroundColor = '#000';
    gameOverBox.style.border = '2px solid #f00';
    gameOverBox.style.padding = '20px';
    gameOverBox.style.borderRadius = '5px';
    gameOverBox.style.textAlign = 'center';
    gameOverBox.style.maxWidth = '80%';
    
    // Add game over text
    const title = document.createElement('h2');
    title.textContent = '*** GAME OVER! ***';
    title.style.color = '#f00';
    title.style.marginBottom = '20px';
    
    // Add play again button
    const playAgainBtn = document.createElement('a');
    playAgainBtn.href = '#';
    playAgainBtn.textContent = '[Play Again]';
    playAgainBtn.style.color = '#ff0';
    playAgainBtn.style.textDecoration = 'underline';
    playAgainBtn.style.cursor = 'pointer';
    playAgainBtn.style.padding = '10px';
    playAgainBtn.style.fontSize = '1.2em';
    playAgainBtn.onclick = function() {
        location.reload();
        return false;
    };
    
    // Assemble the game over box
    gameOverBox.appendChild(title);
    gameOverBox.appendChild(playAgainBtn);
    
    // Add game over box to overlay
    overlay.appendChild(gameOverBox);
    
    // Add to document
    document.body.appendChild(overlay);
    
    // Focus on the play again button
    playAgainBtn.focus();
    
    return overlay; // Return reference in case you need to remove it programmatically
}
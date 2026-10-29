// maze4.cpp - Fully refactored C++ version

// Define UTF-8 characters for maze elements
#define NCURSES_WIDECHAR 1
#define _GNU_SOURCE 1

#include <algorithm>
#include <array>
#include <chrono>
#include <clocale>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <memory>
#include <ncurses.h>
#include <random>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

// UTF-8 characters for maze elements
const std::string CURRENT_CHAR1 = "①";
const std::string CURRENT_CHAR2 = "②";
const std::string CURRENT_CHAR3 = "③";
const std::string CURRENT_CHAR4 = "④";
const std::string VISITED_CHAR1 = "◇"; // RED
const std::string VISITED_CHAR2 = "◇"; // CYAN
const std::string VISITED_CHAR3 = "◇"; // YELLOW
const std::string VISITED_CHAR4 = "◇"; // GREEN
const std::string SOLUTION_CHAR1 = "◆";
const std::string SOLUTION_CHAR2 = "◆";
const std::string SOLUTION_CHAR3 = "◆";
const std::string SOLUTION_CHAR4 = "◆";
const std::string END_CHAR1 = "R"; // raspberry
const std::string END_CHAR2 = "B"; // blueberry
const std::string END_CHAR3 = "L"; // lemon
const std::string END_CHAR4 = "K"; // kiwi
const std::string TELEPORTER_CHAR = "◎";
const std::string MONSTER_CHAR = "☠";
const std::string DEFEATED_MONSTER_CHAR = "†";
const std::string WALL_CHAR = "▒";
const std::string PATH_CHAR = " ";

// Internal representations
constexpr char CURRENT1 = 'a';
constexpr char CURRENT2 = 'b';
constexpr char CURRENT3 = 'c';
constexpr char CURRENT4 = 'd';
constexpr char VISITED1 = 'e';
constexpr char VISITED2 = 'f';
constexpr char VISITED3 = 'g';
constexpr char VISITED4 = 'h';
constexpr char SOLUTION1 = 'i';
constexpr char SOLUTION2 = 'j';
constexpr char SOLUTION3 = 'k';
constexpr char SOLUTION4 = 'l';
constexpr char END1 = '1';
constexpr char END2 = '2';
constexpr char END3 = '3';
constexpr char END4 = '4';
constexpr char TELEPORTER = 'T';
constexpr char MONSTER = 'M';
constexpr char DEFEATED_MONSTER = 'N';
constexpr char WALL = '#';
constexpr char PATH = ' ';

// Constants
constexpr int MIN_ROWS = 25;
constexpr int MIN_COLS = 40;
constexpr int MAX_ROWS = 1024;
constexpr int MAX_COLS = 1024;
constexpr int DEF_GAME_SPEED = 50;
constexpr int MAX_GAME_DELAY = 200;
constexpr int DEF_TELEPORTER_DENSITY = 1000;
constexpr int MAX_TELEPORTERS = 10;
constexpr int DEF_MONSTER_DENSITY = 500;
constexpr int MAX_MONSTERS = 26;
constexpr int DEF_MONSTER_STRENGTH = 10;
constexpr int MAX_MONSTER_STRENGTH = 15;
constexpr int DEF_PLAYER_STRENGTH = 6;
constexpr int MAX_ATTEMPTS = 100; // Maximum attempts for placing teleporters/monsters
constexpr int POLL_INTERVAL_MS = 333;
constexpr int NUM_PLAYERS = 4;
constexpr int MAX_HIGH_SCORES = 10;
constexpr int STATUS_LINE_HISTORY = 20;
constexpr int STATUS_LINE_MAX = 80;

// Messages
const std::string DELAY_MSG = "** Delaying for you to read **";
const std::string LOST_MSG = "LOST the battle";
const std::string PAUSE_MSG = "↑ ** Paused, press any key to continue **";
const std::string HIGH_SCORE_FILENAME = "maze4pp_high_scores.dat";
const std::string LOG_FILENAME = "maze4pp.log";

// Forward declarations of classes
class Position;
class Logger;
class StatusLine;
class HighScore;
class HighScoreManager;
class Entity;
class Player;
class Monster;
class Teleporter;
class MazeGrid;
class UI;
class Battle;
class Game;

// Global game instance (needed for signal handlers)
Game* g_gameInstance;
extern int g_argc;
extern char** g_argv;

// Helper classes
class Position {
public:
    int x, y;
    int parentX, parentY;

    Position(int x = -1, int y = -1, int parentX = -1, int parentY = -1)
        : x(x), y(y), parentX(parentX), parentY(parentY) {}

    bool operator==(const Position& other) const;

    bool isValid() const;
};

// Node for path-finding
class Node {
public:
    Position pos;
    std::unique_ptr<Node> next;

    Node(const Position& position) : pos(position), next(nullptr) {}
};

// Path-finding stack
class PathStack {
private:
    std::unique_ptr<Node> head;

public:
    PathStack() : head(nullptr) {}
		
		~PathStack() {
        clear(); // this will avoid recursion
    }

    bool isEmpty() const; 

    void push(const Position& pos);

    Position pop();

    void clear();
};

// Status message
class StatusMessage {
public:
    int move;
    std::string msg;

    StatusMessage() : move(0), msg("") {}
};

// Status line manager
class StatusLine {
private:
    std::vector<StatusMessage> messages;
    int currentIndex;
    int lastUpdate;
    int rows;

public:
    StatusLine(int historySize, int rowsInMaze) 
        : messages(historySize), currentIndex(0), lastUpdate(0), rows(rowsInMaze) {}

    void update(const std::string& format, ...); 

    void navigateHistory(int direction);

    void display(int direction);

    bool needsClear(int currentTime) const;

    void resetIndex();
};

// Logger class for debug messages
class Logger {
public:
    static void log(const std::string& format, ...);
};

// High score entry
class HighScore {
public:
    char name[32];
    int playerId;
    int score;
    int battlesWon;
    int strength;
    bool thisRun;
    time_t date;

    HighScore() : playerId(0), score(0), battlesWon(0), strength(0), thisRun(false), date(0) {
        strcpy(name, "");
    }
};

// High score manager
class HighScoreManager {
private:
    std::vector<HighScore> bestScores;
    std::vector<HighScore> worstScores;
    int count;

public:
    HighScoreManager() : bestScores(MAX_HIGH_SCORES), worstScores(MAX_HIGH_SCORES), count(0) {
        readScores();
    }

    void readScores();

    void saveScores();

    void insertScore(HighScore& newScore, bool isBest);

    void display(UI& ui);

    int getCount() const;

    const std::vector<HighScore>& getBestScores() const;

    const std::vector<HighScore>& getWorstScores() const;
};

// Base entity class for players and monsters
class Entity {
protected:
    Position position;
    int strength;
    int battlesWon;
    int battlesLost;
    int recoveryTurns;

public:
    Entity(int x, int y, int strength)
        : position(x, y), strength(strength), battlesWon(0), battlesLost(0), recoveryTurns(0) {}

    virtual ~Entity() = default;

    Position getPosition() const;
    void setPosition(const Position& pos);
    
    int getStrength() const;
    void setStrength(int str);
    void incrementStrength();
    
    int getBattlesWon() const;
    void incrementBattlesWon();
    
    int getBattlesLost() const;
    void incrementBattlesLost();
    
    int getRecoveryTurns() const;
    void setRecoveryTurns(int turns);
    void decrementRecoveryTurns();
    
    bool isRecovering() const;

    virtual void update() = 0;
    virtual char getCharacter() const = 0;
};

// Player class
class Player : public Entity {
private:
    int id;  // 1-4
    Position start;
    Position end;
    int moves;
    bool reachedGoal;
    int finishedRank;
    int abandonedRace;  // 0=no, 1=trapped, 2=too many losses
    bool justTeleported;
    int justTeleportedTime;
    char visitedChar;
    char solutionChar;
    int colorPair;
    std::array<int, 4> dx;
    std::array<int, 4> dy;
    std::vector<std::vector<Position>> parentMap;
    std::vector<std::vector<char>> visited;

public:
    Player(int id, int x, int y, int strength, int rows, int cols)
        : Entity(x, y, strength), id(id), moves(0), reachedGoal(false), 
          finishedRank(0), abandonedRace(0), justTeleported(false), justTeleportedTime(0) {
        
        // Set visited and solution chars based on player ID
        switch (id) {
            case 1: visitedChar = VISITED1; solutionChar = SOLUTION1; colorPair = 2; break;
            case 2: visitedChar = VISITED2; solutionChar = SOLUTION2; colorPair = 3; break;
            case 3: visitedChar = VISITED3; solutionChar = SOLUTION3; colorPair = 4; break;
            case 4: visitedChar = VISITED4; solutionChar = SOLUTION4; colorPair = 5; break;
        }

        // Initialize direction preferences based on player ID
        if (id == 1) {
            // Player 1: Right, Down, Left, Up
            dx = {1, 0, -1, 0};
            dy = {0, 1, 0, -1};
        } else if (id == 2) {
            // Player 2: Left, Down, Right, Up
            dx = {-1, 0, 1, 0};
            dy = {0, 1, 0, -1};
        } else if (id == 3) {
            // Player 3: Right, Up, Left, Down
            dx = {1, 0, -1, 0};
            dy = {0, -1, 0, 1};
        } else { // Player 4
            // Player 4: Left, Up, Right, Down
            dx = {-1, 0, 1, 0};
            dy = {0, -1, 0, 1};
        }

        // Randomize directions slightly for variety
        shuffleDirections();

        // Initialize parent map and visited grid
        parentMap.resize(rows, std::vector<Position>(cols, Position()));
        visited.resize(rows, std::vector<char>(cols, 0));
    }

    void shuffleDirections();

    void setStartPosition(const Position& pos);
    void setEndPosition(const Position& pos);
    Position getStartPosition() const;
    Position getEndPosition() const;

    int getId() const;
    int getMoves() const;
    void incrementMoves();

    bool hasReachedGoal() const;
    void setReachedGoal(bool reached);

    int getFinishedRank() const;
    void setFinishedRank(int rank);

    int getAbandonedRace() const;
    void setAbandonedRace(int reason);

    bool hasJustTeleported() const;
    void setJustTeleported(bool teleported, int time);

    void checkTeleportStatus(int currentTime);

    char getVisitedChar() const;
    char getSolutionChar() const;
    char getCurrentChar() const;

    int getColorPair() const;

    const std::array<int, 4>& getDx() const;
    const std::array<int, 4>& getDy() const;

    bool isVisited(int y, int x) const;
    void markVisited(int y, int x);

    Position getParentAt(int y, int x) const;
    void setParentAt(int y, int x, const Position& parent);

    virtual void update() override;

    virtual char getCharacter() const override;
};

// Monster class /////////////////////////////////////////////////
class Monster : public Entity {
private:
    int dx, dy;
    int patrolLength;
    int steps;
    bool defeated;

public:
    Monster(int x, int y, int strength, int dx, int dy, int patrolLength)
        : Entity(x, y, strength), dx(dx), dy(dy), patrolLength(patrolLength), 
          steps(0), defeated(false) {}

    bool isDefeated() const { return defeated; }
    void setDefeated(bool isDefeated) { defeated = isDefeated; }

    int getDx() const { return dx; }
    int getDy() const { return dy; }
    void setDirection(int newDx, int newDy) { dx = newDx; dy = newDy; }

    int getSteps() const { return steps; }
    void incrementSteps() { steps++; }
    void resetSteps() { steps = 0; }

    int getPatrolLength() const { return patrolLength; }

    virtual void update() override {
        decrementRecoveryTurns();
    }

    virtual char getCharacter() const override {
        return defeated ? DEFEATED_MONSTER : MONSTER;
    }
};

// Teleporter class
class Teleporter {
private:
    Position pos1;
    Position pos2;

public:
    Teleporter(int x1, int y1, int x2, int y2) 
        : pos1(x1, y1), pos2(x2, y2) {}

    Position getFirst() const { return pos1; }
    Position getSecond() const { return pos2; }

    bool matches(int x, int y) const {
        return (pos1.x == x && pos1.y == y) || (pos2.x == x && pos2.y == y);
    }

    Position getDestination(int x, int y) const {
        if (pos1.x == x && pos1.y == y) {
            return pos2;
        }
        return pos1;
    }
};

// Maze grid class
class MazeGrid {
private:
    int rows;
    int cols;
    std::vector<std::vector<char>> grid;

public:
    MazeGrid(int r, int c);

    int getRows() const;
		
    int getCols() const;

    char getCell(int y, int x) const __attribute__((no_instrument_function));

    void setCell(int y, int x, char value);

    bool isWall(int y, int x) const;

    bool isPath(int y, int x) const;

    bool isDeadEnd(int y, int x) const;

    void generate();

    void ensurePathsBetweenCorners();

    std::vector<Position> findDeadEnds() const;
};

// UI class for rendering the game
class UI {
private:
    std::string statsHeader;
    const std::array<std::string, 5> botNames;
    const std::array<std::string, 5> botNamesLong;
    std::array<std::string, 5> displayBotNames;
    std::array<std::string, 26> monsterNames;
    std::array<std::string, 26> shuffledMonsterNames;

public:
    UI() : botNames({"(n/a)", "RapRas", "BusBlu", "LigLem", "KwiKiw"}),
           botNamesLong({"(n/a)", "Rapid Raspberry", "Bussin' Blubery", "Light'nin Lemon", "Kwick Kiwi     "}) {
        // Initialize monster names
        monsterNames = {
            "Abyssal Articho", "Brutal Broccoli", "Creeping Carrot",
            "Dreaded Daikon ", "Eerie Eggplant ", "Fiendish Fennel",
            "Ghastly Garlic ", "Horrid Habanero", "Irate Ivy-Gourd",
            "Jagged Jicama  ", "Killer Kale    ", "Lurking Leek   ",
            "Mean Mushroom  ", "Nightmare Nori ", "Ominous Onion  ",
            "Putrid Potato  ", "Quagmire Quinoa", "Ravager Radish ",
            "Sinister Spinch", "Terror Tomato  ", "Unholy Ube     ",
            "Vile Verdolaga ", "Wicked Wasabi  ", "Xyresic Xà-Lách",
            "Yucky Yam      ", "Zealous Zuke   "
        };
        
        // Shuffle monster names
        shuffleMonsterNames();
    }

    void shuffleMonsterNames();

    void setupBotNames(int cols);

    std::string getBotName(int id) const;

    std::string getBotNameLong(int id) const;

    std::string getMonsterName(int index) const;

    void printChar(int y, int x, char character) const  __attribute__((no_instrument_function));

    void printMaze(const MazeGrid& maze, const std::vector<Player>& players) const;

    void displayPlayerStats(const std::vector<Player>& players) const;

    void animateBullseye(int y, int x, int maxRadius, int delayMs, int direction) const;

    void displayPlayerAlert(const Player& player, int rank) const;

    void showHelpWindow() const;

    void showExtendedHelpWindow() const;
};

// Battle class
class Battle {
private:
    static constexpr int WINNER_RECOVERY_TURNS = 3;
    static constexpr int LOSER_RECOVERY_TURNS = 6;
    WINDOW* battleWin;
    UI& ui;

public:
    Battle(UI& ui) : battleWin(nullptr), ui(ui) {}

    void showBattleArt(bool leftSide, bool isPlayer, int leftIdx, int rightIdx);

    void updateBattleResults(bool leftWins, int type, int leftIdx, int rightIdx);

    int battle(int combatant1Idx, int combatant2Idx, int type);

    int battlePlayerMonster(int playerId, int monsterIdx);

    int battlePlayers(int player1Id, int player2Id);

    int battleMonsters(int monster1Idx, int monster2Idx);
};

// Singleton for the maze instance
class maze {
private:
    static MazeGrid* instance_;

    // Private constructor
    //maze(int rows, int cols) : mazeGrid(rows, cols) {}

public:
    static MazeGrid& instance() __attribute__((no_instrument_function));
    static void initialize(int rows, int cols);
    static void generate();
    static int getRows();
    static int getCols();
};

// Main game class
class Game {
private:
    UI ui;
    Battle battle;
    HighScoreManager highScores;
    bool screenReduced;
    bool inReadKeyboard;
    bool inMySleep;
    bool inPauseGame;
    int oldRows;
    int oldCols;

public:
    // Static game state variables
    static int gameSpeed;
    static int gameDelay;
    static int gameRounds;
    static int gameRoundsTotal;
    static int gameTime;
    static bool showWindows;
    static bool waitForKey;
    static int pauseTime;
    static int numTeleporters;
    static int numMonsters;
    static int liveMonsters;
    static int maxMonsterStrength;
    static int playersFinished;
    static int gameFinished;
    static StatusLine status;
    static std::vector<Player> players;
    static std::vector<Monster> monsters;
    static std::vector<Teleporter> teleporters;

    Game() 
        : battle(ui), screenReduced(false), 
          inReadKeyboard(false), inMySleep(false), inPauseGame(false),
          oldRows(0), oldCols(0) {
		    Logger::log("in Game constructor");
        g_gameInstance = this;
    }

    ~Game();
		
    void init();

    void run();

    void runRound();

    void initializePlayers(bool stage2); 

    void placeTeleporters(); 

    void placeMonsters();

    void updateMonsters();

    bool checkTeleporter(int x, int y, Position& dest);

    int checkMonster(int x, int y);

    void solveMaze();

    void highlightPlayerSolutionPath(Player& player);

    void updateHighScores(int rows, int cols); 

    int calculateScore(int moves, int width, int height);

    static void pauseForUser(); 

    void calculateGameSpeed();

    static void exitGame(const char* format, ...);

    static int readKeyboard();

    int readKeyboardImpl();

    static void pauseGame();

    void pauseGameImpl();

    static void mysleep(long ms);

    void sleepImpl(long ms);
};

std::string get_stacktrace(int max_frames = 32);

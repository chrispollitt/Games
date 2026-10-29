// maze4.cpp - Fully refactored C++ version

#ifndef __CYGWIN__
#include <execinfo.h>
#include <sstream>
#endif

#include "maze4pp.hh"

// Method Implementations /////////////////////////////
void HighScoreManager::display(UI& ui) {
    if (count == 0 || !Game::showWindows) {
        Game::pauseForUser();
        return;
    }

    char dateStr[20];
    int placed = 0;

    // Save current state to restore later
    int oldCurs = curs_set(0); // Hide cursor

    // Calculate window dimensions
    int height = MIN_ROWS - 4;
    int width = MIN_COLS;

    // Create a new window centered on screen
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int yPos = maxY == MIN_ROWS ? 0 : 1;

    WINDOW* highScoreWin = newwin(height, width, yPos, (maxX - width) / 2);
    box(highScoreWin, 0, 0);

    // Add a title
    wattron(highScoreWin, A_BOLD);
    mvwprintw(highScoreWin, 1, (width - 27) / 2, "*** BEST & WORST SCORES ***");
    wattroff(highScoreWin, A_BOLD);

    // Display best scores section title
    wattron(highScoreWin, A_UNDERLINE);
    mvwprintw(highScoreWin, 3, 3, "BEST (FASTEST) SCORES");
    wattroff(highScoreWin, A_UNDERLINE);

    // Display header
    mvwprintw(highScoreWin, 4, 3, "# Name   Scor Ba St Date");

    // Display best scores
    int displayedCount = std::min(count, 5); // Limit displayed scores
    for (int i = 0; i < displayedCount; i++) {
        struct tm* tmInfo = localtime(&bestScores[i].date);
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tmInfo);
        
        // Highlight the new high score
        if (bestScores[i].thisRun) {
            placed = i;
            wattron(highScoreWin, COLOR_PAIR(bestScores[i].playerId + 1) | A_BOLD);
        } else {
            wattron(highScoreWin, COLOR_PAIR(bestScores[i].playerId + 1));
        }
        
        mvwprintw(highScoreWin, 5 + i, 3, "%1d %-6s %4d %2d %2d %s", i + 1,
                bestScores[i].name, bestScores[i].score,
                bestScores[i].battlesWon, bestScores[i].strength, dateStr);

        if (bestScores[i].thisRun) {
            wattroff(highScoreWin, COLOR_PAIR(bestScores[i].playerId + 1) | A_BOLD);
        } else {
            wattroff(highScoreWin, COLOR_PAIR(bestScores[i].playerId + 1));
        }
    }

    // Display worst scores section title
    wattron(highScoreWin, A_UNDERLINE);
    mvwprintw(highScoreWin, 11, 3, "WORST (SLOWEST) SCORES");
    wattroff(highScoreWin, A_UNDERLINE);

    // Display header
    mvwprintw(highScoreWin, 12, 3, "# Name   Scor Ba St Date");

    // Display worst scores
    for (int i = 0; i < displayedCount; i++) {
        struct tm* tmInfo = localtime(&worstScores[i].date);
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tmInfo);
        
        // Highlight the new high score
        if (worstScores[i].thisRun) {
            placed = i * -1;
            wattron(highScoreWin, COLOR_PAIR(worstScores[i].playerId + 1) | A_BOLD);
        } else {
            wattron(highScoreWin, COLOR_PAIR(worstScores[i].playerId + 1));
        }

        mvwprintw(highScoreWin, 13 + i, 3, "%1d %-6s %4d %2d %2d %s", i + 1,
                worstScores[i].name, worstScores[i].score,
                worstScores[i].battlesWon, worstScores[i].strength, dateStr);

        if (worstScores[i].thisRun) {
            wattroff(highScoreWin, COLOR_PAIR(worstScores[i].playerId + 1) | A_BOLD);
        } else {
            wattroff(highScoreWin, COLOR_PAIR(worstScores[i].playerId + 1));
        }
    }

    // Display special messages based on placement
    if (bestScores[0].thisRun) {
        wattron(highScoreWin, A_BOLD);
        mvwprintw(highScoreWin, (height - 2), (width - 34) / 2,
            "** NEW CHAMPION! %s **", ui.getBotNameLong(bestScores[0].playerId).c_str());
        wattroff(highScoreWin, A_BOLD);
    } else if(placed > 0) {
        mvwprintw(highScoreWin, (height - 2), (width - 34) / 2,
            "** Fast PLACED %d %s **", placed+1, ui.getBotNameLong(bestScores[placed].playerId).c_str());
    }

    // If we have a new worst score, add some congratulatory text
    if (worstScores[0].thisRun) {
        wattron(highScoreWin, A_BOLD);
        mvwprintw(highScoreWin, (height - 2), (width - 34) / 2,
            "** NEW WORST :( %s **", ui.getBotNameLong(worstScores[0].playerId).c_str());
        wattroff(highScoreWin, A_BOLD);
    } else if(placed < 0) {
        placed = abs(placed);
        mvwprintw(highScoreWin, (height - 2), (width - 34) / 2,
            "** Slow PLACED %d %s **", placed+1, ui.getBotNameLong(worstScores[placed].playerId).c_str());
    }

    // Refresh and show the window
    wrefresh(highScoreWin);

    // Wait for user input
    Game::pauseForUser();

    // Clean up
    delwin(highScoreWin);
    curs_set(oldCurs); // Restore cursor state

    // Refresh the screen to remove the window
    refresh();
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::isValid() const {
    return x >= 0 && y >= 0;
}

bool PathStack::isEmpty() const {
    return head == nullptr;
}

void PathStack::push(const Position& pos) {
    auto node = std::make_unique<Node>(pos);
    node->next = std::move(head);
    head = std::move(node);
}

Position PathStack::pop() {
    if (isEmpty()) {
        return Position(-1, -1, -1, -1);
    }

    Position pos = head->pos;
    
    // Create a temporary to hold the node we're removing
    // This ensures proper destruction
    auto temp = std::move(head);
    head = std::move(temp->next);
    
    // temp will be destroyed when this function returns, 
    // but without triggering the recursive destruction
    
    return pos;
}

void PathStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

void StatusLine::update(const std::string& format, ...) {
    char buffer[STATUS_LINE_MAX];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
    va_end(args);

    StatusMessage newMsg;
    newMsg.msg = buffer;
    newMsg.move = Game::gameTime;

    // If it's a battle lost message, add to history
    if (newMsg.msg.find(LOST_MSG) != std::string::npos) {
        // Shift all messages down in the array
        for (int i = static_cast<int>(messages.size()) - 1; i > 1; i--) {
            messages[i].msg = messages[i-1].msg;
            messages[i].move = messages[i-1].move;
        }
        // Insert new message at index 1 (after current)
        messages[1] = newMsg;
    }

    // Reset viewing index to current message
    currentIndex = 0;
    messages[currentIndex] = newMsg;
    lastUpdate = Game::gameTime;

    // Display the message
    display(0);
}

void StatusLine::navigateHistory(int direction) {
    if (direction == KEY_UP) {
        // Move up in history if there's a valid message
        if (currentIndex < static_cast<int>(messages.size()) - 1 && !messages[currentIndex + 1].msg.empty()) {
            currentIndex++;
        }
    } else if (direction == KEY_DOWN) {
        // Move down in history if not already at newest
        if (currentIndex > 0) {
            currentIndex--;
        }
    }
    display(direction);
}

void StatusLine::display(int direction) {
    attron(COLOR_PAIR(11) | A_BOLD);
    if (currentIndex == 0) { 
        // Current message - display normally
        if (direction == 0) {
            lastUpdate = Game::gameTime;
            mvprintw(maze::getRows() + 5, 0, "%s", messages[currentIndex].msg.c_str());
        } else {
            mvprintw(maze::getRows() + 5, 0, "%s", PAUSE_MSG.c_str());
        }
    } else {
        // Past message - show indicator
        std::string upChar = " ";
        if (currentIndex < static_cast<int>(messages.size()) - 1 && !messages[currentIndex + 1].msg.empty()) {
            upChar = "↑";
        }        
        mvprintw(maze::getRows() + 5, 0, "%s↓[%d] %s", upChar.c_str(), messages[currentIndex].move, 
                 messages[currentIndex].msg.c_str());
    }
    clrtoeol();
    attroff(COLOR_PAIR(11) | A_BOLD);
    refresh();
}

bool StatusLine::needsClear(int currentTime) const {
    return lastUpdate > 0 && lastUpdate + 25 < currentTime;
}

void StatusLine::resetIndex() {
    currentIndex = 0;
}

void Logger::log(const std::string& format, ...) {
#ifdef DEBUG
    std::ofstream logFile(LOG_FILENAME, std::ios::app);
    if (logFile.is_open()) {
        time_t now = time(nullptr);
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        logFile << "[" << timestamp << "] ";
        
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
        va_end(args);
        
        logFile << buffer << std::endl;
        logFile.close();
    }
#else
    format.c_str();
#endif
}

// High score manager
void HighScoreManager::readScores() {
    std::ifstream file(HIGH_SCORE_FILENAME, std::ios::binary);
    if (!file.is_open()) {
        return; // File doesn't exist yet
    }

    file.read(reinterpret_cast<char*>(&count), sizeof(int));
    if (count > MAX_HIGH_SCORES) {
        count = MAX_HIGH_SCORES; // Safety check
    }

    file.read(reinterpret_cast<char*>(bestScores.data()), count * sizeof(HighScore));
    file.read(reinterpret_cast<char*>(worstScores.data()), count * sizeof(HighScore));
    file.close();
}

void HighScoreManager::saveScores() {
    std::ofstream file(HIGH_SCORE_FILENAME, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    // Clear this_run flags before saving
    for (int i = 0; i < count; i++) {
        bestScores[i].thisRun = false;
        worstScores[i].thisRun = false;
    }

    file.write(reinterpret_cast<const char*>(&count), sizeof(int));
    file.write(reinterpret_cast<const char*>(bestScores.data()), count * sizeof(HighScore));
    file.write(reinterpret_cast<const char*>(worstScores.data()), count * sizeof(HighScore));
    file.close();
}

void HighScoreManager::insertScore(HighScore& newScore, bool isBest) {
    std::vector<HighScore>& scores = isBest ? bestScores : worstScores;
    int pos = -1;

    // Determine the position to insert
    for (int i = 0; i < count; i++) {
        if (isBest) {
            // For best scores, lower score is better
            if (newScore.score < scores[i].score) {
                pos = i;
                break;
            }
        } else {
            // For worst scores, higher score is "better" for the worst list
            if (newScore.score > scores[i].score) {
                pos = i;
                break;
            }
        }
    }

    // If didn't find a position and list isn't full yet
    if (pos == -1 && count < MAX_HIGH_SCORES) {
        pos = count;
        count++;
    }

    // If found a position to insert
    if (pos != -1) {
        // Shift scores down to make room
        for (int i = MAX_HIGH_SCORES - 1; i > pos; i--) {
            if (i - 1 >= 0) {
                scores[i] = scores[i - 1];
            }
        }
        // Insert the new score
        newScore.thisRun = true;
        scores[pos] = newScore;
    }
}

int HighScoreManager::getCount() const { return count; }
const std::vector<HighScore>& HighScoreManager::getBestScores() const { return bestScores; }
const std::vector<HighScore>& HighScoreManager::getWorstScores() const { return worstScores; }

// Base entity class for players and monsters
Position Entity::getPosition() const { return position; }
void Entity::setPosition(const Position& pos) { position = pos; }

int Entity::getStrength() const { return strength; }
void Entity::setStrength(int str) { strength = str; }
void Entity::incrementStrength() { strength++; }

int Entity::getBattlesWon() const { return battlesWon; }
void Entity::incrementBattlesWon() { battlesWon++; }

int Entity::getBattlesLost() const { return battlesLost; }
void Entity::incrementBattlesLost() { battlesLost++; }

int Entity::getRecoveryTurns() const { return recoveryTurns; }
void Entity::setRecoveryTurns(int turns) { recoveryTurns = turns; }
void Entity::decrementRecoveryTurns() { if (recoveryTurns > 0) recoveryTurns--; }

bool Entity::isRecovering() const { return recoveryTurns > 0; }

// Player class
void Player::shuffleDirections() {
    // Apply a Fisher-Yates shuffle but only to the last 2 directions
    for (int i = 3; i >= 2; i--) {
        int j = rand() % (i + 1);
        std::swap(dx[i], dx[j]);
        std::swap(dy[i], dy[j]);
    }
}

void Player::setStartPosition(const Position& pos) { start = pos; }
void Player::setEndPosition(const Position& pos) { end = pos; }
Position Player::getStartPosition() const { return start; }
Position Player::getEndPosition() const { return end; }

int Player::getId() const { return id; }
int Player::getMoves() const { return moves; }
void Player::incrementMoves() { moves++; }

bool Player::hasReachedGoal() const { return reachedGoal; }
void Player::setReachedGoal(bool reached) { reachedGoal = reached; }

int Player::getFinishedRank() const { return finishedRank; }
void Player::setFinishedRank(int rank) { finishedRank = rank; }

int Player::getAbandonedRace() const { return abandonedRace; }
void Player::setAbandonedRace(int reason) { abandonedRace = reason; }

bool Player::hasJustTeleported() const { return justTeleported; }
void Player::setJustTeleported(bool teleported, int time) { 
    justTeleported = teleported; 
    if (teleported) {
        justTeleportedTime = time + 2;
    }
}

void Player::checkTeleportStatus(int currentTime) {
    if (justTeleportedTime < currentTime) {
        justTeleported = false;
    }
}

char Player::getVisitedChar() const { return visitedChar; }
char Player::getSolutionChar() const { return solutionChar; }
char Player::getCurrentChar() const {
    switch (id) {
        case 1: return CURRENT1;
        case 2: return CURRENT2;
        case 3: return CURRENT3;
        case 4: return CURRENT4;
        default: return '!';
    }
}

int Player::getColorPair() const { return colorPair; }

const std::array<int, 4>& Player::getDx() const { return dx; }
const std::array<int, 4>& Player::getDy() const { return dy; }

bool Player::isVisited(int y, int x) const { return visited[y][x] != 0; }
void Player::markVisited(int y, int x) { visited[y][x] = 1; }

Position Player::getParentAt(int y, int x) const { return parentMap[y][x]; }
void Player::setParentAt(int y, int x, const Position& parent) { parentMap[y][x] = parent; }

void Player::update() {
    decrementRecoveryTurns();
}

char Player::getCharacter() const {
    return getCurrentChar();
}

// Main game class
Game::~Game() {
    g_gameInstance = nullptr;
}

void Game::init() {
	  Logger::log("Game::init() called");
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

    calculateGameSpeed();

    if (gameRounds < 0) {
        gameRounds = 1;
    }
    gameRoundsTotal = gameRounds;
    if (pauseTime < 0) {
        pauseTime = 2 * 1000;
    }

    // Make screen match reported size
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); 
    resizeterm(size.ws_row, size.ws_col);
    oldRows = size.ws_row;
    oldCols = size.ws_col;
    Logger::log("Init term size is %d, %d", size.ws_row, size.ws_col);
}

void Game::run() {
	  Logger::log("Game::run() called");
    // Process command-line arguments
    int opt;
    while ((opt = getopt(g_argc, g_argv, "t:m:s:g:r:p:kwh")) != -1) {
        switch (opt) {
        case 't':
            numTeleporters = atoi(optarg);
            break;
        case 'm':
            numMonsters = atoi(optarg);
            break;
        case 's':
            maxMonsterStrength = atoi(optarg);
            break;
        case 'g':
            gameSpeed = atoi(optarg);
            break;
        case 'r':
            gameRounds = atoi(optarg);
            break;
        case 'k':
            waitForKey = true;
            break;
        case 'w':
            showWindows = true;
            break;
        case 'p':
            pauseTime = static_cast<int>(atof(optarg) * 1000.0);
            break;
        case 'h':
            printf("Usage: %s [options]\n", g_argv[0]);
            printf("Options:\n");
            printf("  -t N    Set number of teleporter pairs (default: "
                "auto-calculated)\n");
            printf("  -m N    Set number of monsters (default: auto-calculated)\n");
            printf("  -s N    Set maximum monster strength (1-%d, default: %d)\n",
                MAX_MONSTER_STRENGTH, DEF_MONSTER_STRENGTH);
            printf("  -g N    Set game speed (1-100, default: %d)\n", DEF_GAME_SPEED);
            printf("  -p N    Set user pause seconds (default: %d)\n", 2);
            printf("  -k      Wait for key-press to continue (default: No)\n");
            printf("  -w      Show battle windows (default: No)\n");
            printf("  -h      Display this help message\n");
            exit(0);
        default:
            fprintf(stderr, "Try '%s -h' for more information.\n", g_argv[0]);
            exit(1);
        }
    }

    // Set the random seed
    srand(static_cast<unsigned int>(time(NULL)));

    // Initialize the game
    init();

    // Loop for the game rounds
    for (; gameRounds > 0; gameRounds--) {
        Logger::log("Starting round %d", gameRounds);
        runRound();
        if (screenReduced) {
            screenReduced = false;
            gameRounds++;
        }
        // Make screen match reported size
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); 
        resizeterm(size.ws_row, size.ws_col);
        oldRows = size.ws_row;
        oldCols = size.ws_col;
    }

    // Exit the game
    exitGame("Game over\n");
}

void Game::runRound() {
    // Reset game state
    gameTime = 0;
    playersFinished = 0;
    gameFinished = 0;
    
    // Initialize players
    initializePlayers(false);

    // Get terminal dimensions
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int mazeArea = rows * cols;

    // Calculate default max values based on screen size if not provided
    if (numTeleporters < 0) {
        // 1 teleporter per 100 cells, with a minimum of 1 and maximum of MAX_TELEPORTERS
        numTeleporters = mazeArea / DEF_TELEPORTER_DENSITY;
    }
    numTeleporters = std::max(0, std::min(numTeleporters, MAX_TELEPORTERS));
    
    if (numMonsters < 0) {
        // 1 monster per 20 cells, with a minimum of 1 and maximum of MAX_MONSTERS
        numMonsters = mazeArea / DEF_MONSTER_DENSITY;
    }
    numMonsters = std::max(0, std::min(numMonsters, MAX_MONSTERS));
    
    if (maxMonsterStrength < 0) {
        maxMonsterStrength = DEF_MONSTER_STRENGTH; // Default maximum strength
    }
    maxMonsterStrength = std::max(0, std::min(maxMonsterStrength, MAX_MONSTER_STRENGTH));
    
    // Ensure minimum maze size
    if (rows < MIN_ROWS || cols < MIN_COLS) {
        exitGame("Screen too small, min %d rows, %d cols\n", MIN_ROWS, MIN_COLS);
    }
    // Ensure maximum maze size
    if (rows > MAX_ROWS || cols > MAX_COLS) {
        exitGame("Screen too big, max %d rows, %d cols\n", MAX_ROWS, MAX_COLS);
    }
    
    // Adjust for maze walls and borders
    rows = (rows - 6) / 2 * 2 - 1; // Reserve space for messages and player stats
    cols = (cols - 2) / 2 * 2 - 1;
    // Ensure rows and cols are odd, even if initial calculation is even.
    if (rows % 2 == 0) rows--;
    if (cols % 2 == 0) cols--;

    // Create and initialize maze
    maze::initialize(rows, cols);
    
    // Generate maze
    maze::generate();
    
    // Finish initializing players with start/end positions
    initializePlayers(true);
    
    // Place teleporters and monsters
    placeTeleporters();
    placeMonsters();
    liveMonsters = numMonsters;

    // Print initial maze
    clear();
    ui.printMaze(maze::instance(), players);
    ui.displayPlayerStats(players);
    pauseForUser();
    status.update("The race is ON!");

    // Solve maze concurrently for all players
    solveMaze();

    // Show high scores
    if (!screenReduced) {
        updateHighScores(rows, cols);
    }
}

void Game::initializePlayers(bool stage2) {
    if (!stage2) {
        // Initialize player list and reset game state
        players.clear();
        monsters.clear();
        teleporters.clear();
        liveMonsters = 0;
    } else {
        // Create players
        for (int i = 0; i < NUM_PLAYERS; i++) {
            int strength = DEF_PLAYER_STRENGTH;
            players.push_back(Player(i + 1, 0, 0, strength, maze::getRows(), maze::getCols()));
        }
        // Set up bot names based on screen width
        ui.setupBotNames(maze::getCols());

        // Set player start and end positions to corners
        players[0].setStartPosition(Position(1, 1));
        players[0].setEndPosition(Position(maze::getCols() - 2, maze::getRows() - 2));
        maze::instance().setCell(players[0].getEndPosition().y, players[0].getEndPosition().x, END1);

        players[1].setStartPosition(Position(maze::getCols() - 2, 1));
        players[1].setEndPosition(Position(1, maze::getRows() - 2));
        maze::instance().setCell(players[1].getEndPosition().y, players[1].getEndPosition().x, END2);

        players[2].setStartPosition(Position(1, maze::getRows() - 2));
        players[2].setEndPosition(Position(maze::getCols() - 2, 1));
        maze::instance().setCell(players[2].getEndPosition().y, players[2].getEndPosition().x, END3);

        players[3].setStartPosition(Position(maze::getCols() - 2, maze::getRows() - 2));
        players[3].setEndPosition(Position(1, 1));
        maze::instance().setCell(players[3].getEndPosition().y, players[3].getEndPosition().x, END4);

        // Initialize player current positions
        for (int i = 0; i < NUM_PLAYERS; i++) {
            players[i].setPosition(players[i].getStartPosition());
        }
    }
}

void Game::placeTeleporters() {
    // Find all dead ends
    std::vector<Position> deadEnds = maze::instance().findDeadEnds();

    // If we don't have enough dead ends for at least one teleporter, exit
    if (deadEnds.size() < 2) {
        return;
    }

    // Adjust number of teleporters based on available dead ends
    int maxPossibleTeleporters = static_cast<int>(deadEnds.size()) / 2;
    numTeleporters = std::min(maxPossibleTeleporters, numTeleporters);

    // Shuffle the dead ends to select random pairs
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deadEnds.begin(), deadEnds.end(), g);

    // Place teleporters at randomly selected dead ends
    for (int i = 0; i < numTeleporters; i++) {
        // Get two dead ends
        Position pos1 = deadEnds[i * 2];
        Position pos2 = deadEnds[i * 2 + 1];

        // Skip corners (player start/end positions)
        if ((pos1.x <= 2 && pos1.y <= 2) ||
            (pos1.x >= maze::getCols() - 3 && pos1.y <= 2) ||
            (pos1.x <= 2 && pos1.y >= maze::getRows() - 3) ||
            (pos1.x >= maze::getCols() - 3 && pos1.y >= maze::getRows() - 3)) {
            i--;
            continue;
        }
        if ((pos2.x <= 2 && pos2.y <= 2) ||
            (pos2.x >= maze::getCols() - 3 && pos2.y <= 2) ||
            (pos2.x <= 2 && pos2.y >= maze::getRows() - 3) ||
            (pos2.x >= maze::getCols() - 3 && pos2.y >= maze::getRows() - 3)) {
            i--;
            continue;
        }

        // Create teleporter and add to list
        teleporters.push_back(Teleporter(pos1.x, pos1.y, pos2.x, pos2.y));

        // Mark teleporters in maze
        maze::instance().setCell(pos1.y, pos1.x, TELEPORTER);
        maze::instance().setCell(pos2.y, pos2.x, TELEPORTER);
    }
}

void Game::placeMonsters() {
    static const int dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
    static const int dy[] = {-1, 0, 1, 0};

    for (int i = 0; i < numMonsters; i++) {
        // Find a random empty space
        int x, y;
        int attempts = 0;

        do {
            x = rand() % (maze::getCols() - 4) + 2; // Avoid edges and corners
            y = rand() % (maze::getRows() - 4) + 2;
            attempts++;

            // Skip if we can't find a spot after many attempts
            if (attempts > MAX_ATTEMPTS) {
                numMonsters = i;
                return;
            }

            // Skip corners (player start/end positions)
            if ((x <= 2 && y <= 2) || (x >= maze::getCols() - 3 && y <= 2) ||
                (x <= 2 && y >= maze::getRows() - 3) ||
                (x >= maze::getCols() - 3 && y >= maze::getRows() - 3)) {
                continue;
            }
        } while (maze::instance().getCell(y, x) != PATH || maze::instance().isDeadEnd(y, x));

        // Random direction
        int dir = rand() % 4;
        int monsterDx = dx[dir];
        int monsterDy = dy[dir];

        // Random patrol length
        int patrolLength = rand() % 10 + 5;

        // Random strength
        int strength = rand() % maxMonsterStrength + 1;

        // Create and add monster
        monsters.push_back(Monster(x, y, strength, monsterDx, monsterDy, patrolLength));
        maze::instance().setCell(y, x, MONSTER);
    }
}

void Game::updateMonsters() {
    for (int i = 0; i < numMonsters; i++) {
        Monster& monster = monsters[i];
        
        // Skip defeated monsters
        if (monster.isDefeated()) {
            continue;
        }

        // Update recovery status
        monster.update();

        // Clear current position
        Position pos = monster.getPosition();
        maze::instance().setCell(pos.y, pos.x, PATH);

        // Update position
        monster.incrementSteps();
        if (monster.getSteps() >= monster.getPatrolLength()) {
            // Change direction randomly
            static const int dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
            static const int dy[] = {-1, 0, 1, 0};
            int dir = rand() % 4;
            monster.setDirection(dx[dir], dy[dir]);
            monster.resetSteps();
        }

        // Try to move
        int newX = pos.x + monster.getDx();
        int newY = pos.y + monster.getDy();
        char cell = maze::instance().getCell(newY, newX);

        // Check if the new position is valid and not a special cell
        if (newX > 0 && newX < maze::getCols() - 1 && newY > 0 &&
            newY < maze::getRows() - 1 && (cell == PATH || cell == MONSTER)) {
            monster.setPosition(Position(newX, newY));
        } else {
            // Change direction if blocked
            static const int dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
            static const int dy[] = {-1, 0, 1, 0};
            int dir = rand() % 4;
            monster.setDirection(dx[dir], dy[dir]);
            monster.resetSteps();
        }

        // Mark new position
        maze::instance().setCell(monster.getPosition().y, monster.getPosition().x, MONSTER);
    }

    // Check for monster vs monster collisions
    for (int i = 0; i < numMonsters; i++) {
        // Skip defeated monsters
        if (monsters[i].isDefeated()) {
            continue;
        }

        for (int j = i + 1; j < numMonsters; j++) {
            // Skip defeated monsters
            if (monsters[j].isDefeated()) {
                continue;
            }

            // Check if monsters are in the same cell
            if (monsters[i].getPosition() == monsters[j].getPosition()) {
                // Battle the monsters
                battle.battleMonsters(i, j);
            }
        }
    }
}

bool Game::checkTeleporter(int x, int y, Position& dest) {
    for (const auto& teleporter : teleporters) {
        if (teleporter.matches(x, y)) {
            dest = teleporter.getDestination(x, y);
            return true;
        }
    }
    return false;
}

int Game::checkMonster(int x, int y) {
    for (int i = 0; i < numMonsters; i++) {
        if (monsters[i].getPosition().x == x && 
            monsters[i].getPosition().y == y && 
            !monsters[i].isDefeated()) {
            return i; // Return monster index
        }
    }
    return -1; // No monster
}

void Game::solveMaze() {
    // Create separate stacks for each player to ensure fairness
    std::vector<PathStack> stacks(NUM_PLAYERS);

    // This is for tracking player collisions
    std::vector<Position> previousPositions(NUM_PLAYERS, Position(-1, -1));

    // Push each player's starting position onto their stack
    for (int p = 0; p < NUM_PLAYERS; p++) {
        stacks[p].push(players[p].getStartPosition());
        players[p].markVisited(players[p].getStartPosition().y, players[p].getStartPosition().x);
    }

    // Start Main solve loop - continue until all players finish or all stacks are empty
    while (playersFinished < NUM_PLAYERS) {
        // Check if all stacks are empty (no more moves for any player)
        bool allEmpty = true;
        for (int p = 0; p < NUM_PLAYERS; p++) {
            if (!stacks[p].isEmpty() && !players[p].hasReachedGoal() && players[p].getAbandonedRace() == 0) {
                allEmpty = false;
                break;
            }
        }

        // After checking if all stacks are empty:
        if (allEmpty) {
            // Before breaking, mark any players who didn't finish as having abandoned
            for (int p = 0; p < NUM_PLAYERS; p++) {
                if (!players[p].hasReachedGoal() && players[p].getAbandonedRace() == 0) {
                    players[p].setAbandonedRace(1);
                    highlightPlayerSolutionPath(players[p]);
                    ui.displayPlayerAlert(players[p], -1);
                }
            }
            break;
        }

        // Rotate through players, giving each a turn to move one step
        for (int p = 0; p < NUM_PLAYERS; p++) {
            // Skip players who have already reached their goal or abandoned
            if (players[p].hasReachedGoal() || players[p].getAbandonedRace() != 0)
                continue;

            // Check if this player's stack is empty
            if (stacks[p].isEmpty()) {
                // Mark player as abandoned if their stack is empty and they haven't reached their goal
                if (players[p].getAbandonedRace() == 0 && !players[p].hasReachedGoal()) {
                    players[p].setAbandonedRace(1);
                    highlightPlayerSolutionPath(players[p]);
                    ui.displayPlayerAlert(players[p], -1);
                }
                continue;
            }

            // Update current position
            int playerId = p + 1;
            Position current = stacks[p].pop();
            players[p].setPosition(current);

            // Check for player vs player collision
            for (int otherP = 0; otherP < NUM_PLAYERS; otherP++) {
                // Skip self and players who've reached their goal or abandoned
                if (otherP == p || players[otherP].hasReachedGoal() ||
                    players[otherP].getAbandonedRace() != 0) {
                    continue;
                }

                // Check if players collide
                if (players[p].getPosition() == players[otherP].getPosition()) {
                    // Initiate player vs player battle
                    int winnerId = battle.battlePlayers(playerId, otherP + 1);

                    // Loser must retreat to previous position
                    if (winnerId == playerId) {
                        // Other player retreats to previous position
                        if (previousPositions[otherP].isValid()) {
                            players[otherP].setPosition(previousPositions[otherP]);
                        }
                        // Check if player has lost too many battles
                        if (players[otherP].getBattlesLost() >= 3) {
                            players[otherP].setAbandonedRace(2);
                            highlightPlayerSolutionPath(players[otherP]);
                            ui.displayPlayerAlert(players[otherP], -2);

                            // Clear the stack to stop the player's exploration
                            stacks[otherP].clear();
                        }
                        continue;
                    } else if (winnerId < 0) {
                        ; // no battle took place
                    } else {
                        // Current player retreats to parent position
                        if (current.parentX >= 0 && current.parentY >= 0) {
                            players[p].setPosition(Position(current.parentX, current.parentY));
                        } else {
                            // If no parent position, stay at current but mark as visited
                            players[p].markVisited(current.y, current.x);
                        }
                        
                        // Check if player has lost too many battles
                        if (players[p].getBattlesLost() >= 3) {
                            players[p].setAbandonedRace(2);
                            highlightPlayerSolutionPath(players[p]);
                            ui.displayPlayerAlert(players[p], -2);

                            // Clear the stack to stop the player's exploration
                            stacks[p].clear();
                        }
                        continue;
                    }

                    // Update display after battle
                    ui.printMaze(maze::instance(), players);
                    ui.displayPlayerStats(players);
                }
            }

            // Store current position for potential retreat
            previousPositions[p] = players[p].getPosition();
            players[p].incrementMoves();

            // Check if reached the end
            if (current.x == players[p].getEndPosition().x && current.y == players[p].getEndPosition().y) {
                players[p].setReachedGoal(true);
                playersFinished++;
                players[p].setFinishedRank(playersFinished);

                // Mark as finished in UI
                highlightPlayerSolutionPath(players[p]);
                ui.displayPlayerAlert(players[p], players[p].getFinishedRank());
                continue;
            }

            // Check for teleporter
            Position teleportDest;
            if (!players[p].hasJustTeleported() && 
                checkTeleporter(current.x, current.y, teleportDest)) {

                // Record teleportation
                players[p].setJustTeleported(true, gameTime);

                Position teleported(teleportDest.x, teleportDest.y, current.x, current.y);
                stacks[p].push(teleported);
                players[p].setParentAt(teleportDest.y, teleportDest.x, Position(current.x, current.y));

                // Mark destination as visited
                players[p].markVisited(teleportDest.y, teleportDest.x);

                // Visualize teleportation
                for (int i = 0; i < 5; i++) { // Flicker for 3 cycles
                    ui.printChar(teleportDest.y, teleportDest.x, TELEPORTER);
                    ui.printChar(current.y, current.x, TELEPORTER);
                    refresh();
                    mysleep(pauseTime/40);

                    attron(A_REVERSE);
                    ui.printChar(teleportDest.y, teleportDest.x, TELEPORTER);
                    ui.printChar(current.y, current.x, TELEPORTER);
                    attroff(A_REVERSE);
                    refresh();
                    mysleep(pauseTime/40);
                }
                continue;
            }

            // Check for monster
            int monsterIdx = checkMonster(current.x, current.y);
            if (monsterIdx >= 0) {
                int battleResult = battle.battlePlayerMonster(playerId, monsterIdx);

                if (battleResult == 0) {
                    // Player lost battle, mark position as visited
                    players[p].markVisited(current.y, current.x);

                    // Update the visualization
                    char visitedChar = players[p].getVisitedChar();
                    char cell = maze::instance().getCell(current.y, current.x);
                    if (cell != END1 && cell != END2 && cell != END3 && cell != END4 &&
                        cell != TELEPORTER) {
                        maze::instance().setCell(current.y, current.x, visitedChar);
                    }

                    // Check if player has lost too many battles
                    if (players[p].getBattlesLost() >= 3) {
                        players[p].setAbandonedRace(2);
                        highlightPlayerSolutionPath(players[p]);
                        ui.displayPlayerAlert(players[p], -2);

                        // Clear the stack to stop the player's exploration
                        stacks[p].clear();
                    }

                    continue;
                } else if (battleResult < 0) {
                    ; // no battle took place
                }
            }

            // Mark as visited in player's separate visited array
            players[p].markVisited(current.y, current.x);

            // Also update the visualization in the shared grid
            char visitedChar = players[p].getVisitedChar();
            char cell = maze::instance().getCell(current.y, current.x);
            if (cell != END1 && cell != END2 && cell != END3 && cell != END4 &&
                cell != TELEPORTER && cell != MONSTER && cell != DEFEATED_MONSTER) {
                maze::instance().setCell(current.y, current.x, visitedChar);
            }

            // Try all possible directions using player-specific direction arrays
            const auto& dx = players[p].getDx();
            const auto& dy = players[p].getDy();
            for (int i = 0; i < 4; i++) {
                int nextX = current.x + dx[i];
                int nextY = current.y + dy[i];

                // Don't go back to parent
                if (nextX == current.parentX && nextY == current.parentY) {
                    continue;
                }

                // Check bounds and maze cells
                if (nextX >= 0 && nextX < maze::getCols() && nextY >= 0 && nextY < maze::getRows()) {
                    char nextCell = maze::instance().getCell(nextY, nextX);

                    // Allow exploring if not a wall and not visited by THIS player
                    if (nextCell != WALL && !players[p].isVisited(nextY, nextX)) {
                        players[p].markVisited(nextY, nextX);

                        Position nextPos(nextX, nextY, current.x, current.y);
                        stacks[p].push(nextPos);
                        players[p].setParentAt(nextY, nextX, Position(current.x, current.y));
                    }
                }
            }
        }

        // Update monsters occasionally
        if (gameTime % 5 == 0) {
            updateMonsters();
        }

        // Visualize exploration after each player's move
        ui.printMaze(maze::instance(), players);
        ui.displayPlayerStats(players);
        readKeyboard();
        if (status.needsClear(gameTime)) {
            move(maze::getRows() + 5, 0);
            clrtoeol();
        }
        mysleep(gameDelay);

        // Update game time
        gameTime++;
        
        // Update player teleport status
        for (auto& player : players) {
            player.checkTeleportStatus(gameTime);
            player.update();
        }

        // Check for window resize
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); // Get new size
        if (oldRows != size.ws_row || oldCols != size.ws_col) {
            status.update("Caught term resize to %d, %d", size.ws_row, size.ws_col);
            Logger::log("Caught term resize to %d, %d", size.ws_row, size.ws_col);
            if (oldRows > size.ws_row || oldCols > size.ws_col) {
                // abort current solve as screen has reduced in size
                screenReduced = true;
                oldRows = size.ws_row;
                oldCols = size.ws_col; 
                // spin wheels in lieu of debounce
                mysleep(2000);
                // break
                break;
            }
            oldRows = size.ws_row;
            oldCols = size.ws_col;
        }
        
        doupdate();
    }

    // Show final results
    if (!screenReduced) {
        ui.displayPlayerStats(players);
    }
    doupdate();
}

void Game::highlightPlayerSolutionPath(Player& player) {
    // Now mark each position in the path with solution char
    char solutionChar = player.getSolutionChar();
    char currentChar = player.getCurrentChar();
    Position end = player.getPosition();
    int x = end.x, y = end.y;

    while (!(x == player.getStartPosition().x && y == player.getStartPosition().y)) {
        // Only overwrite certain cells (don't overwrite special cells)
        char cell = maze::instance().getCell(y, x);
        if (cell != END1 && cell != END2 && cell != END3 && cell != END4 &&
            cell != TELEPORTER && cell != MONSTER && cell != DEFEATED_MONSTER) {
            maze::instance().setCell(y, x, solutionChar);
        }    
        
        Position parent = player.getParentAt(y, x);
        x = parent.x;
        y = parent.y;
    }
    
    ui.printMaze(maze::instance(), players);
    ui.displayPlayerStats(players);
    
    // Visualize end point
    for (int i = 0; i < 5; i++) { // Flicker for 3 cycles
        ui.printChar(end.y, end.x, currentChar);      
        refresh();
        mysleep(pauseTime/40);
    
        attron(A_REVERSE);
        ui.printChar(end.y, end.x, currentChar);      
        attroff(A_REVERSE);
        refresh();
        mysleep(pauseTime/40);
    }
}

void Game::updateHighScores(int rows, int cols) {
    // Process players who finished the race
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].hasReachedGoal() && players[i].getFinishedRank() == 1) {
            // Create a new score entry
            HighScore newScore;

            // Use the bot name based on player ID
            strncpy(newScore.name, ui.getBotName(players[i].getId()).c_str(), sizeof(newScore.name) - 1);
            newScore.name[sizeof(newScore.name) - 1] = '\0'; // Ensure null termination

            newScore.playerId = players[i].getId();
            newScore.score = calculateScore(players[i].getMoves(), cols, rows);
            newScore.battlesWon = players[i].getBattlesWon();
            newScore.strength = players[i].getStrength();
            newScore.date = time(nullptr);

            // Try to insert into best scores
            highScores.insertScore(newScore, true);

            // Also track the worst scores for fun
            // (only if they actually finished though)
            highScores.insertScore(newScore, false);
        }
    }

    // Display the high score window
    highScores.display(ui);

    // Save updated high scores
    highScores.saveScores();
}

int Game::calculateScore(int moves, int width, int height) {
    constexpr float OPTIMAL_PATH_FACTOR = 1.5f;
    constexpr float SCORE_MULTIPLIER = 100.0f;
    constexpr float COMPLEXITY_ADJUSTMENT_FACTOR = 10.0f;

    if (moves <= 0 || width <= 0 || height <= 0) {
        return -1; // Error code for invalid input
    }

    // Calculate the effective maze dimensions (in cells, not walls)
    int effectiveWidth  = static_cast<int>( (width  - 1) / 2 );
    int effectiveHeight = static_cast<int>( (height - 1) / 2 );

    // Estimate optimal solution length
    float estimatedOptimalPath = OPTIMAL_PATH_FACTOR * (float)(effectiveWidth + effectiveHeight);

    // Maze complexity factor
    float mazeComplexity = (float)(effectiveWidth * effectiveHeight);

    // Calculate score (lower is better)
    // Base score is the actual moves normalized by estimated optimal path
    float efficiencyFactor = (float)moves / estimatedOptimalPath;

    // Apply a small adjustment based on maze complexity
    float complexityAdjustment = logf(mazeComplexity) / COMPLEXITY_ADJUSTMENT_FACTOR;

    // Final score - lower is better
    int score = (int)roundf(SCORE_MULTIPLIER * (efficiencyFactor - complexityAdjustment));

    // Ensure score is always positive
    return score > 0 ? score : 1;
}

void Game::pauseForUser() {
    readKeyboard();
    if (waitForKey) {
        pauseGame();  // calls doupdate()
    } else {
        status.update(DELAY_MSG);
        mysleep(pauseTime); // calls doupdate()
    }
    move(maze::getRows() + 5, 0);
    clrtoeol();
    readKeyboard();
    doupdate();
}

void Game::calculateGameSpeed() {
    constexpr double minSpeed = 1.0;    // slowest
    constexpr double maxSpeed = 100.0;  // fastest
    if (gameSpeed < minSpeed) {
        gameSpeed = DEF_GAME_SPEED;
    }
    if (gameSpeed > maxSpeed) {
        gameSpeed = DEF_GAME_SPEED;
    }
    // Map Game_speed logarithmically to [0, 1]
    double normalizedSpeed = (log(gameSpeed) - log(minSpeed)) / (log(maxSpeed) - log(minSpeed));
    // Calculate delay (inverse relationship with normalized_speed)
    gameDelay = static_cast<int>(round((1.0 - normalizedSpeed) * MAX_GAME_DELAY));
}

void Game::exitGame(const char* format, ...) {
    doupdate();
    tcflush(STDIN_FILENO, TCIFLUSH);
    endwin();
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // Forward the varargs to printf
    va_end(args);
    exit(0);
}

int Game::readKeyboard() {
    if (g_gameInstance) {
        return g_gameInstance->readKeyboardImpl();
    }
    return 0;
}

int Game::readKeyboardImpl() {
    if (inReadKeyboard) return 0;
    inReadKeyboard = true;
    
    int ch;
    int wasPaused = 0;
    
    nodelay(stdscr, TRUE);
    ch = getch();
    nodelay(stdscr, FALSE);
    
    switch(ch) {
    // PAUSE with SPACE
    case ' ':
        pauseGame();
        wasPaused = 1;
        break;
    // Toggle WaitForKey with K
    case 'K':
    case 'k':
        status.update("Toggled WaitForKey");
        waitForKey = !waitForKey;
        break;
    // Toggle ShowWindows with W
    case 'W':
    case 'w':
        status.update("Toggled ShowWindows");
        showWindows = !showWindows;
        break;
    // SLOW DOWN with MINUS
    case '_':
    case '-':
        status.update("Slowing down...");
        gameSpeed--;
        calculateGameSpeed();
        break;
    // SPEED UP with PLUS
    case '=':
    case '+':
        status.update("Speeding up...");
        gameSpeed++;
        calculateGameSpeed();
        break;
    // EXIT with ESCAPE or Q
    case 'q':
    case 'Q':
    case 27:
        exitGame("User ended game early\n");
        break;
    // HELP with h or ?
    case 'h':
    case '?':
        ui.showHelpWindow();
        break;
    // EXTENDED HELP with H
    case 'H':
        ui.showExtendedHelpWindow();
        break;
    case KEY_RESIZE:
        break;        
    case ERR:
        break;  
    case KEY_MOUSE:
        break;  
    default:
        status.update("Unrecognized key %d", ch);
    }
    
    inReadKeyboard = false;
    return wasPaused;
}

void Game::pauseGame() {
    if (g_gameInstance) {
        g_gameInstance->pauseGameImpl();
    }
}

void Game::pauseGameImpl() {
    if (inPauseGame) return;
    inPauseGame = true;
    
    status.resetIndex(); // Reset to current message when pausing
    status.update(PAUSE_MSG);
    tcflush(STDIN_FILENO, TCIFLUSH);
    bool paused = true;
    
    // Enter pause mode loop
    while (paused) {
        doupdate();
        int ch = getch();
        
        // Handle arrow keys for status history browsing
        if (ch == KEY_UP || ch == 259) {
            // Move to older message
            status.navigateHistory(KEY_UP);
        } 
        else if (ch == KEY_DOWN || ch == 258) {
            // Move to newer message
            status.navigateHistory(KEY_DOWN);
        }
        // Exit game early
        else if (ch == 'q' || ch == 'Q' || ch == 27) {
            exitGame("User ended game early\n");
        }
        // Leave pause mode on any other key press (that's printable)
        else if (ch >= 32 && ch <= 126) {
            paused = false;
        }
    }
    
    // Reset to current message when unpausing
    status.resetIndex();
    status.update("Continuing...");
    tcflush(STDIN_FILENO, TCIFLUSH);
    doupdate();
    inPauseGame = false;
}

void Game::mysleep(long ms) {
    if (g_gameInstance) {
        g_gameInstance->sleepImpl(ms);
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void Game::sleepImpl(long ms) {
    if (inMySleep) return;
    inMySleep = true;
    
    doupdate();
    if (ms <= POLL_INTERVAL_MS) {
        // For short sleeps, just sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    } else {
        // For longer sleeps, poll for keyboard input
        long remainingTime = ms;
        
        while (remainingTime > 0) {
            auto start = std::chrono::steady_clock::now();
            
            // Check for keyboard input
            if (readKeyboardImpl()) {
                break;
            }
            
            auto end = std::chrono::steady_clock::now();
            long readDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            // Calculate sleep time until next poll
            long sleepTime = std::min(POLL_INTERVAL_MS - readDuration, remainingTime);
            
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                remainingTime -= sleepTime;
            } else {
                break;
            }
        }
    }
    
    inMySleep = false;
}

// Maze grid class
MazeGrid::MazeGrid(int r, int c) : rows(r), cols(c) {
    grid.resize(rows, std::vector<char>(cols, WALL));
}

int MazeGrid::getRows() const { return rows; }
int MazeGrid::getCols() const { return cols; }

char MazeGrid::getCell(int y, int x) const {
    if (y >= 0 && y < rows && x >= 0 && x < cols) {
        return grid[y][x];
    }
    return WALL; // Default to wall for out-of-bounds
}

void MazeGrid::setCell(int y, int x, char value) {
    if (y >= 0 && y < rows && x >= 0 && x < cols) {
        grid[y][x] = value;
    }
}

bool MazeGrid::isWall(int y, int x) const {
    return getCell(y, x) == WALL;
}

bool MazeGrid::isPath(int y, int x) const {
    return getCell(y, x) == PATH;
}

bool MazeGrid::isDeadEnd(int y, int x) const {
    // Don't consider special positions as dead ends
    char cell = getCell(y, x);
    if (cell == END1 || cell == END2 || cell == END3 || cell == END4) {
        return false;
    }

    // Only check PATH cells
    if (cell != PATH) {
        return false;
    }

    // Count adjacent PATH/END* cells
    int pathNeighbors = 0;
    static const int dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
    static const int dy[] = {-1, 0, 1, 0};

    for (int dir = 0; dir < 4; dir++) {
        int newX = x + dx[dir];
        int newY = y + dy[dir];
        char neighborCell = getCell(newY, newX);
        
        if (neighborCell == PATH || 
            neighborCell == END1 || 
            neighborCell == END2 || 
            neighborCell == END3 ||
            neighborCell == END4) {
            pathNeighbors++;
        }
    }

    // If only one PATH neighbor, it's a dead end
    return (pathNeighbors == 1);
}

void MazeGrid::generate() {
    // First, create a basic maze using DFS
    PathStack stack;
    Position start(1, 1, -1, -1);
    stack.push(start);

    setCell(start.y, start.x, PATH);

    std::array<int, 4> dx = {0, 1, 0, -1}; // Up, Right, Down, Left
    std::array<int, 4> dy = {-1, 0, 1, 0};

    while (!stack.isEmpty()) {
        Position current = stack.pop();

        // Get unvisited neighbors
        std::vector<int> unvisited;
        for (int dir = 0; dir < 4; dir++) {
            int newX = current.x + dx[dir] * 2;
            int newY = current.y + dy[dir] * 2;

            if (newX > 0 && newX < cols - 1 && newY > 0 && newY < rows - 1 && grid[newY][newX] == WALL) {
                unvisited.push_back(dir);
            }
        }

        if (!unvisited.empty()) {
            // Push current cell back onto stack
            stack.push(current);

            // Choose random unvisited neighbor
            int randDir = unvisited[rand() % unvisited.size()];
            int newX = current.x + dx[randDir] * 2;
            int newY = current.y + dy[randDir] * 2;

            // Remove wall between current cell and chosen cell
            setCell(current.y + dy[randDir], current.x + dx[randDir], PATH);

            // Mark the chosen cell as part of the path
            setCell(newY, newX, PATH);

            // Push chosen cell onto stack
            stack.push(Position(newX, newY, -1, -1));
        }
    }

    // Ensure paths between corners
    ensurePathsBetweenCorners();
}

void MazeGrid::ensurePathsBetweenCorners() {
    // Define the four corners
    std::array<Position, 4> corners = {
        Position(1, 1, -1, -1),                  // Top-left
        Position(cols - 2, 1, -1, -1),          // Top-right
        Position(1, rows - 2, -1, -1),          // Bottom-left
        Position(cols - 2, rows - 2, -1, -1)    // Bottom-right
    };

    // For each pair of corners, ensure a path exists
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            // Create a temporary grid to track visited cells for this pathfinding attempt
            std::vector<std::vector<char>> visited(rows, std::vector<char>(cols, 0));

            // Use BFS to find a path from corner i to corner j
            PathStack queue;
            queue.push(corners[i]);
            visited[corners[i].y][corners[i].x] = 1;

            bool pathFound = false;
            Position current;

            while (!queue.isEmpty() && !pathFound) {
                current = queue.pop();

                // Check if we've reached the destination corner
                if (current.x == corners[j].x && current.y == corners[j].y) {
                    pathFound = true;
                    break;
                }

                // Try all four directions
                static const int dx[] = {0, 1, 0, -1}; // Up, Right, Down, Left
                static const int dy[] = {-1, 0, 1, 0};

                for (int dir = 0; dir < 4; dir++) {
                    int newX = current.x + dx[dir];
                    int newY = current.y + dy[dir];

                    // Check if valid cell and not visited and not a wall
                    if (newX > 0 && newX < cols - 1 && newY > 0 && newY < rows - 1 && 
                        !visited[newY][newX] && grid[newY][newX] != WALL) {

                        visited[newY][newX] = 1;
                        queue.push(Position(newX, newY, current.x, current.y));
                    }
                }
            }

            // If no path found, create one
            if (!pathFound) {
                // Start at corner i, carve a path toward corner j
                int x = corners[i].x;
                int y = corners[i].y;
                int destX = corners[j].x;
                int destY = corners[j].y;

                while (x != destX || y != destY) {
                    // Move in the direction of the destination
                    if (x < destX) x++;
                    else if (x > destX) x--;

                    if (y < destY) y++;
                    else if (y > destY) y--;

                    // Carve path
                    grid[y][x] = PATH;
                }
            }
        }
    }
}

std::vector<Position> MazeGrid::findDeadEnds() const {
    std::vector<Position> deadEnds;
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < cols - 1; x++) {
            if (isDeadEnd(y, x)) {
                deadEnds.push_back(Position(x, y));
            }
        }
    }
    return deadEnds;
}

// UI class for rendering the game
void UI::shuffleMonsterNames() {
    // Copy original names
    shuffledMonsterNames = monsterNames;
    
    // Fisher-Yates shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = static_cast<int>(shuffledMonsterNames.size()) - 1; i > 0; i--) {
        std::uniform_int_distribution<> dist(0, i);
        int j = dist(g);
        std::swap(shuffledMonsterNames[i], shuffledMonsterNames[j]);
    }
}

void UI::setupBotNames(int cols) {
    if (cols < MIN_COLS + 10) {
        displayBotNames = botNames;
        statsHeader = "  NAME | ST | BATS  | MOVES | STATUS";
    } else {
        displayBotNames = botNamesLong;
        statsHeader = "  NAME          | ST | BATS  | MOVES | STATUS";
    }
}

std::string UI::getBotName(int id) const {
    if (id >= 1 && id <= 4) {
        return displayBotNames[id];
    }
    return "Unknown";
}

std::string UI::getBotNameLong(int id) const {
    if (id >= 1 && id <= 4) {
        return botNamesLong[id];
    }
    return "Unknown";
}

std::string UI::getMonsterName(int index) const {
    if (index >= 0 && index < static_cast<int>(shuffledMonsterNames.size())) {
        return shuffledMonsterNames[index];
    }
    return "Unknown Monster";
}

void UI::printChar(int y, int x, char character) const {
    move(y, x);
    // Otherwise, display the appropriate cell character
    switch (character) {  
    // Player 1 cells
    case CURRENT1:
        attron(COLOR_PAIR(2) | A_BOLD);
        addstr(CURRENT_CHAR1.c_str());
        attroff(COLOR_PAIR(2) | A_BOLD);
        break;
    case VISITED1:
        attron(COLOR_PAIR(2));
        addstr(VISITED_CHAR1.c_str());
        attroff(COLOR_PAIR(2));
        break;
    case SOLUTION1:
        attron(COLOR_PAIR(2) | A_BOLD);
        addstr(SOLUTION_CHAR1.c_str());
        attroff(COLOR_PAIR(2) | A_BOLD);
        break;
    case END1:
        attron(COLOR_PAIR(2) | A_BOLD);
        addstr(END_CHAR1.c_str());
        attroff(COLOR_PAIR(2) | A_BOLD);
        break;
    
    // Player 2 cells
    case CURRENT2:
        attron(COLOR_PAIR(3) | A_BOLD);
        addstr(CURRENT_CHAR2.c_str());
        attroff(COLOR_PAIR(3) | A_BOLD);
        break;
    case VISITED2:
        attron(COLOR_PAIR(3));
        addstr(VISITED_CHAR2.c_str());
        attroff(COLOR_PAIR(3));
        break;
    case SOLUTION2:
        attron(COLOR_PAIR(3) | A_BOLD);
        addstr(SOLUTION_CHAR2.c_str());
        attroff(COLOR_PAIR(3) | A_BOLD);
        break;
    case END2:
        attron(COLOR_PAIR(3) | A_BOLD);
        addstr(END_CHAR2.c_str());
        attroff(COLOR_PAIR(3) | A_BOLD);
        break;
    
    // Player 3 cells
    case CURRENT3:
        attron(COLOR_PAIR(4) | A_BOLD);
        addstr(CURRENT_CHAR3.c_str());
        attroff(COLOR_PAIR(4) | A_BOLD);
        break;
    case VISITED3:
        attron(COLOR_PAIR(4));
        addstr(VISITED_CHAR3.c_str());
        attroff(COLOR_PAIR(4));
        break;
    case SOLUTION3:
        attron(COLOR_PAIR(4) | A_BOLD);
        addstr(SOLUTION_CHAR3.c_str());
        attroff(COLOR_PAIR(4) | A_BOLD);
        break;
    case END3:
        attron(COLOR_PAIR(4) | A_BOLD);
        addstr(END_CHAR3.c_str());
        attroff(COLOR_PAIR(4) | A_BOLD);
        break;
    
    // Player 4 cells
    case CURRENT4:
        attron(COLOR_PAIR(5) | A_BOLD);
        addstr(CURRENT_CHAR4.c_str());
        attroff(COLOR_PAIR(5) | A_BOLD);
        break;
    case VISITED4:
        attron(COLOR_PAIR(5));
        addstr(VISITED_CHAR4.c_str());
        attroff(COLOR_PAIR(5));
        break;
    case SOLUTION4:
        attron(COLOR_PAIR(5) | A_BOLD);
        addstr(SOLUTION_CHAR4.c_str());
        attroff(COLOR_PAIR(5) | A_BOLD);
        break;
    case END4:
        attron(COLOR_PAIR(5) | A_BOLD);
        addstr(END_CHAR4.c_str());
        attroff(COLOR_PAIR(5) | A_BOLD);
        break;
    
    // Special cells
    case WALL:
        addstr(WALL_CHAR.c_str());
        break;
    case PATH:
        attron(COLOR_PAIR(1));
        addstr(PATH_CHAR.c_str());
        attroff(COLOR_PAIR(1));
        break;
    case TELEPORTER:
        attron(COLOR_PAIR(7) | A_BOLD);
        addstr(TELEPORTER_CHAR.c_str());
        attroff(COLOR_PAIR(7) | A_BOLD);
        break;
    case MONSTER:
        attron(COLOR_PAIR(8) | A_BOLD);
        addstr(MONSTER_CHAR.c_str());
        attroff(COLOR_PAIR(8) | A_BOLD);
        break;
    case DEFEATED_MONSTER:
        attron(COLOR_PAIR(9) | A_BOLD);
        addstr(DEFEATED_MONSTER_CHAR.c_str());
        attroff(COLOR_PAIR(9) | A_BOLD);
        break;
    default:
        attron(COLOR_PAIR(8));
        addstr("!");
        attroff(COLOR_PAIR(8));
        break;
    }
}

void UI::printMaze(const MazeGrid& maze, const std::vector<Player>& players) const {
    // Get current terminal dimensions
    int termRows, termCols;
    getmaxyx(stdscr, termRows, termCols);

    int rows = maze.getRows();
    int cols = maze.getCols();

    // Ensure we don't write outside the terminal
    int visibleRows = (rows < termRows - 6) ? rows : termRows - 6;
    int visibleCols = (cols < termCols) ? cols : termCols - 1;

    // Get player positions for current state
    std::vector<Position> playerPositions;
    for (const auto& player : players) {
        playerPositions.push_back(player.getPosition());
    }

    for (int i = 0; i < visibleRows; i++) {
        for (int j = 0; j < visibleCols; j++) {
            // Check if any player is at this position
            bool isPlayerPosition = false;
            for (size_t p = 0; p < playerPositions.size(); p++) {
                if (playerPositions[p].x == j && playerPositions[p].y == i) {
                    printChar(i, j, players[p].getCurrentChar());
                    isPlayerPosition = true;
                    break;
                }
            }

            if (!isPlayerPosition) {
                // Otherwise, print the maze cell
                printChar(i, j, maze.getCell(i, j));
            }
        }
    }

    // Print game info header
    attron(COLOR_PAIR(11) | A_BOLD);
    mvprintw(0, 1, "[Maze Game] %d s, %d t, %d/%d [%d] m, %d/%d r",
            Game::gameSpeed, Game::numTeleporters, 
            Game::liveMonsters, Game::numMonsters,
            Game::maxMonsterStrength, 
            Game::gameRounds, Game::gameRoundsTotal);
    attroff(COLOR_PAIR(11) | A_BOLD);
}

void UI::displayPlayerStats(const std::vector<Player>& players) const {
    int baseRow = maze::getRows() + 1;

    mvprintw(baseRow - 1, 0, "%s", statsHeader.c_str());
    clrtoeol();
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        const Player& player = players[i];
        attron(COLOR_PAIR(player.getColorPair()) | A_BOLD);
        mvprintw(baseRow + i, 0, "%6s | %2d | %2d/%-2d |  %4d | ",
                getBotName(player.getId()).c_str(), player.getStrength(),
                player.getBattlesWon(), player.getBattlesLost(), player.getMoves());

        // Add status column showing player's solve status
        if (player.hasReachedGoal()) {
            char rankn[4];
            switch (player.getFinishedRank()) {
            case 1: strcpy(rankn, "1st"); break;
            case 2: strcpy(rankn, "2nd"); break;
            case 3: strcpy(rankn, "3rd"); break;
            case 4: strcpy(rankn, "4th"); break;
            }
            printw("Finished %s!", rankn);
        } else if (player.getAbandonedRace() == 1) {
            printw("DNF: Trapped");
        } else if (player.getAbandonedRace() == 2) {
            printw("DNF: Loser");
        } else if (Game::gameTime == 0) {
            printw("Ready To Start");
        } else {
            printw("Solving");
        }
        clrtoeol();

        attroff(COLOR_PAIR(player.getColorPair()) | A_BOLD);
    }
    refresh();
}

void UI::animateBullseye(int y, int x, int maxRadius, int delayMs, int direction) const {
    // UTF-8 characters for different rings of the bullseye
    const std::vector<std::wstring> ringChars = {
        L"●", // Filled circle
        L"◉", // Bullseye
        L"◎", // Circle with dot
        L"○", // Empty circle
        L"·"  // Small dot
    };
    const int numRings = static_cast<int>(ringChars.size());

    // Save current colors
    int oldPair = PAIR_NUMBER(getattrs(stdscr));

    // Clear any previous content
    refresh();

    int start, end, inc;
    if (direction) {
        start = maxRadius;
        end = 0;
        inc = -1;
    } else {
        start = 0;
        end = maxRadius;
        inc = 1;
    }

    // Animate the bullseye
    for (int currentRadius = start;
         (inc > 0) ? (currentRadius <= end) : (currentRadius >= end);
         currentRadius += inc) {

        // Draw each ring of the bullseye
        for (int r = currentRadius; r >= 0; r--) {
            // Select color based on ring position
            int colorPair = (r % 5) + 1;
            attron(COLOR_PAIR(colorPair));

            // Select character based on position in the bullseye
            const std::wstring& ringChar = ringChars[r % numRings];

            // For the innermost ring (when r=0), always use the filled circle
            if (r == 0) {
                attron(COLOR_PAIR(1)); // Red for the center
            }

            // Draw a complete ring at radius r
            for (int i = 0; i <= 360; i += 10) {
                double angle = i * M_PI / 180.0;
                int ringY = y + static_cast<int>(r * sin(angle));
                int ringX = x + static_cast<int>(r * 2 * cos(angle)); // Multiply by 2 for aspect ratio

                // Make sure we don't draw outside the screen
                if (ringY >= 0 && ringY < LINES && ringX >= 0 && ringX < COLS-4) {
                    // Use wide character functions for UTF-8
                    mvaddwstr(ringY, ringX, ringChar.c_str());
                }
            }

            // Reset attributes
            attroff(COLOR_PAIR(colorPair));
        }

        // Refresh the screen
        refresh();

        // Delay between frames
        Game::mysleep(delayMs);
    }

    // Restore original color pair
    attron(COLOR_PAIR(oldPair));
}

void UI::displayPlayerAlert(const Player& player, int rank) const {
    if (!Game::showWindows && Game::gameFinished != NUM_PLAYERS) {
        Game::pauseForUser();
        return;
    }

    // Calculate window dimensions
    int height = MIN_ROWS - 2;
    int width = MIN_COLS;

    // Create a new window centered on screen
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    WINDOW* battleWin = newwin(height, width, (maxY - height) / 2, (maxX - width) / 2);
    box(battleWin, 0, 0);

    // Set up colors for battle screen
    wbkgd(battleWin, COLOR_PAIR(11));

    // Display battle title based on battle type
    wattron(battleWin, COLOR_PAIR(player.getColorPair()) | A_BOLD);

    if (rank > 0) {
        mvwprintw(battleWin, 2, 2, "PLAYER REACHED GOAL!");
        // figlet goal
        mvwprintw(battleWin, 16, 1,   "        RANK = %d!                    ", rank);
        mvwprintw(battleWin, 17, 1, R"( _____ _       _     _              _ )");
        mvwprintw(battleWin, 18, 1, R"(|  ___(_)_ __ (_)___| |__   ___  __| |)");
        mvwprintw(battleWin, 19, 1, R"(| |_  | | '_ \| / __| '_ \ / _ \/ _` |)");
        mvwprintw(battleWin, 20, 1, R"(|  _| | | | | | \__ \ | | |  __/ (_| |)");
        mvwprintw(battleWin, 21, 1, R"(|_|   |_|_| |_|_|___/_| |_|\___|\__,_|)");
    } else if (rank == -1) {
        mvwprintw(battleWin, 2, 2, "PLAYER ABANDONED RACE");
        // figlet abandoned
        mvwprintw(battleWin, 16, 1, R"( _____  OUT OF MOVES!               _ )");
        mvwprintw(battleWin, 17, 1, R"(|_   _| __ __ _ _ __ _ __   ___  __| |)");
        mvwprintw(battleWin, 18, 1, R"(  | || '__/ _` | '_ | '_ \ / _ \/ _` |)");
        mvwprintw(battleWin, 19, 1, R"(  | || | | (_| | |_)| |_) |  __/ (_| |)");
        mvwprintw(battleWin, 20, 1, R"(  |_||_|  \__,_| .__| .__/ \___|\__,_|)");
        mvwprintw(battleWin, 21, 1, R"(               |_|  |_|               )");
    } else if (rank == -2) {
        mvwprintw(battleWin, 2, 2, "PLAYER ABANDONED RACE");
        // figlet abandoned
        mvwprintw(battleWin, 16, 1,   "        TOO MANY LOSSES!        ");
        mvwprintw(battleWin, 17, 1, R"( _     ___  ____  _____ ____  _ )");
        mvwprintw(battleWin, 18, 1, R"(| |   / _ \/ ___|| ____|  _ \| |)");
        mvwprintw(battleWin, 19, 1, R"(| |  | | | \___ \|  _| | |_) | |)");
        mvwprintw(battleWin, 20, 1, R"(| |__| |_| |___) | |___|  _ <|_|)");
        mvwprintw(battleWin, 21, 1, R"(|_____\___/|____/|_____|_| \_(_))");
    }

    // Draw player battle art
    mvwprintw(battleWin, 4, 2, "%s", getBotNameLong(player.getId()).c_str());
    mvwprintw(battleWin, 5, 2, "STR: %d  WINS: %d", player.getStrength(), player.getBattlesWon());

    // Bot ASCII Art
    mvwprintw(battleWin,  6, 2, R"(               )");
    mvwprintw(battleWin,  7, 2, R"(     ____      )");
    mvwprintw(battleWin,  8, 2, R"(    /    \     )");
    mvwprintw(battleWin,  9, 2, R"(   | o  o |    )");
    mvwprintw(battleWin, 10, 2, R"(   | ____ |    )");
    mvwprintw(battleWin, 11, 2, R"(   ||____||    )");
    mvwprintw(battleWin, 12, 2, R"(    \____/     )");
    mvwprintw(battleWin, 13, 2, R"(               )");

    wattroff(battleWin, COLOR_PAIR(player.getColorPair()) | A_BOLD);
    wrefresh(battleWin);
    Game::pauseForUser();

    // Clean up battle window
    delwin(battleWin);

    // Redraw the main screen
    //clear();
}

void UI::showHelpWindow() const {
    int height = 20;
    int width = 60;
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    WINDOW* helpWin = newwin(height, width, (maxY - height) / 2, (maxX - width) / 2);
    box(helpWin, 0, 0);
    
    wattron(helpWin, A_BOLD);
    mvwprintw(helpWin, 1, 2, "Maze4 Game Help");
    wattroff(helpWin, A_BOLD);
    
    mvwprintw(helpWin, 3, 2, "Controls:");
    mvwprintw(helpWin, 4, 4, "[Space]   - Pause/Continue");
    mvwprintw(helpWin, 5, 4, "[+/-]     - Speed Up / Slow Down");
    mvwprintw(helpWin, 6, 4, "[h/?]     - Show this Help");
    mvwprintw(helpWin, 7, 4, "[H]       - Show Extended Help.");
    mvwprintw(helpWin, 8, 4, "[q/Q/ESC] - Quit Game");
    mvwprintw(helpWin, 9, 4, "[k]       - Toggle WaitForKey");
    mvwprintw(helpWin,10, 4, "[w]       - Toggle ShowWindows");
    mvwprintw(helpWin,12, 2, "During Pause:");
    mvwprintw(helpWin,13, 4, "[Up/Down] - Scroll Status Messages");
    
    mvwprintw(helpWin,15, 2, "Press any key to close...");
    
    wrefresh(helpWin);
    
    getch(); // Wait for any key
    delwin(helpWin);
    refresh();
}

void UI::showExtendedHelpWindow() const {
    int height = 24;
    int width = 70;
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    WINDOW* helpWin = newwin(height, width, (maxY - height) / 2, (maxX - width) / 2);
    box(helpWin, 0, 0);
    
    wattron(helpWin, A_BOLD);
    mvwprintw(helpWin, 1, 2, "Maze4 Extended Help");
    wattroff(helpWin, A_BOLD);
    
    mvwprintw(helpWin, 3, 2, "Story:");
    mvwprintw(helpWin, 4, 4, "In the Maze of Munch, four Fruitbots —");
    mvwprintw(helpWin, 5, 4, "Rapid Raspberry, Bussin' Blueberry,");
    mvwprintw(helpWin, 6, 4, "Lightning Lemon, and Kwick Kiwi — race");
    mvwprintw(helpWin, 7, 4, "for glory against deadly Veggie Monsters.");
    
    mvwprintw(helpWin, 9, 2, "Goal:");
    mvwprintw(helpWin,10, 4, "Reach your goal first! Battle monsters,");
    mvwprintw(helpWin,11, 4, "survive ambushes, and outmaneuver rivals.");
    mvwprintw(helpWin,12, 4, "Three battle losses = system failure!");
    
    mvwprintw(helpWin,14, 2, "Inspiration:");
    mvwprintw(helpWin,15, 4, "Inspired by classic maze screensavers");
    mvwprintw(helpWin,16, 4, "and the chaotic fun of marble races.");
    mvwprintw(helpWin,17, 4, "All game logic written with AI assistance.");
    
    mvwprintw(helpWin,19, 2, "Press any key to close...");
    
    wrefresh(helpWin);
    
    getch(); // Wait for any key
    delwin(helpWin);
    refresh();
}

// Battle class
void Battle::showBattleArt(bool leftSide, bool isPlayer, int leftIdx, int rightIdx) {
    // Set the recovery time (in turns) for both winner and loser
    if (battleWin == nullptr || !Game::showWindows) {
        return;
    }

    int col = leftSide ? 2 : 23;
    int idx = leftSide ? leftIdx : rightIdx;
    if (isPlayer) { // Player Bot
        wattron(battleWin, COLOR_PAIR(idx + 2) | A_BOLD);
        mvwprintw(battleWin, 4, col, "%s", ui.getBotNameLong(idx + 1).c_str());
        mvwprintw(battleWin, 5, col, "STR: %d  WINS: %d", 
                  Game::players[idx].getStrength(), Game::players[idx].getBattlesWon());

        // Bot ASCII Art
        mvwprintw(battleWin,  6, col, R"(               )");
        mvwprintw(battleWin,  7, col, R"(     ____      )");
        mvwprintw(battleWin,  8, col, R"(    /    \     )");
        mvwprintw(battleWin,  9, col, R"(   | o  o |    )");
        mvwprintw(battleWin, 10, col, R"(   | ____ |    )");
        mvwprintw(battleWin, 11, col, R"(   ||____||    )");
        mvwprintw(battleWin, 12, col, R"(    \____/     )");
        mvwprintw(battleWin, 13, col, R"(               )");
        wattroff(battleWin, COLOR_PAIR(idx + 2) | A_BOLD);
    } else { // Monster
        wattron(battleWin, COLOR_PAIR(8) | A_BOLD);
        mvwprintw(battleWin, 4, col, "%s", ui.getMonsterName(idx).c_str());
        mvwprintw(battleWin, 5, col, "Strength: %d    ", Game::monsters[idx].getStrength());

        // Monster ASCII Art
        mvwprintw(battleWin,  6, col, R"(    .----,     )");
        mvwprintw(battleWin,  7, col, R"(   /      \    )");
        mvwprintw(battleWin,  8, col, R"(  |  O  O  |   )");
        mvwprintw(battleWin,  9, col, R"(  | .vvvv. |   )");
        mvwprintw(battleWin, 10, col, R"(  / |    | \   )");
        mvwprintw(battleWin, 11, col, R"( /  `^^^^'  \  )");
        mvwprintw(battleWin, 12, col, R"(/  /|     |\ \.)");
        mvwprintw(battleWin, 13, col, R"(\_/ .~~~~~. \_/)");
        wattroff(battleWin, COLOR_PAIR(8) | A_BOLD);
    }
}

void Battle::updateBattleResults(bool leftWins, int type, int leftIdx, int rightIdx) {
    int winnerIdx, loserIdx;
    bool winnerIsPlayer, loserIsPlayer;

    if (leftWins) {
        winnerIdx = leftIdx;
        loserIdx = rightIdx;
        winnerIsPlayer = (type == 0 || type == 1);
        loserIsPlayer = (type == 1);
    } else {
        winnerIdx = rightIdx;
        loserIdx = leftIdx;
        winnerIsPlayer = (type == 1);
        loserIsPlayer = (type == 0 || type == 1);
    }

    // Handle winner
    if (winnerIsPlayer) {
        // Winner is player
        Game::players[winnerIdx].incrementStrength();
        Game::players[winnerIdx].incrementBattlesWon();
        Game::players[winnerIdx].setRecoveryTurns(WINNER_RECOVERY_TURNS);
    } else {
        // Winner is monster
        Game::monsters[winnerIdx].incrementStrength();
        Game::monsters[winnerIdx].setRecoveryTurns(WINNER_RECOVERY_TURNS);
    }

    // Handle loser
    if (loserIsPlayer) {
        // Loser is player
        Game::players[loserIdx].incrementBattlesLost();
        Game::players[loserIdx].setRecoveryTurns(LOSER_RECOVERY_TURNS);
        Game::status.update("%s %s!", ui.getBotName(loserIdx + 1).c_str(), LOST_MSG.c_str());
    } else {
        // Loser is monster
        Game::liveMonsters--;
        Game::monsters[loserIdx].setDefeated(true);
        maze::instance().setCell(Game::monsters[loserIdx].getPosition().y, 
                              Game::monsters[loserIdx].getPosition().x, 
                              DEFEATED_MONSTER);
        Game::status.update("%s %s!", ui.getMonsterName(loserIdx).c_str(), LOST_MSG.c_str());
    }

    if (Game::showWindows) {
        // X out eyes of loser
        int losCol = leftWins ? 23 : 2;
        if (loserIsPlayer) {
            // Bot
            mvwprintw(battleWin, 9, losCol + 2, R"( | x  x | )");
        } else {
            // Monster
            mvwprintw(battleWin, 8, losCol + 1, R"( |  X  X  | )");
        }
        
        // Show "Wins!" text with correct positioning
        int winCol = leftWins ? 1 : 23;
        mvwprintw(battleWin, 17, winCol, R"( _    _ _       )");
        mvwprintw(battleWin, 18, winCol, R"(| |  | (_)_ __  )");
        mvwprintw(battleWin, 19, winCol, R"(| |/\| | | '_ \ )");
        mvwprintw(battleWin, 20, winCol, R"(|  __  | | | | |)");
        mvwprintw(battleWin, 21, winCol, R"(|_/  \_|_|_| |_|)");
    }
}

int Battle::battle(int combatant1Idx, int combatant2Idx, int type) {
    // Get player indices if needed (for player vs monster or player vs player)
    int p1Idx = (type == 0 || type == 1) ? combatant1Idx - 1 : -1; // Adjust for 0-based indexing
    int p2Idx = (type == 1) ? combatant2Idx - 1 : -1; // Adjust for 0-based indexing

    // Get monster indices if needed (for player vs monster or monster vs monster)
    int m1Idx = (type == 0) ? combatant2Idx : (type == 2 ? combatant1Idx : -1);
    int m2Idx = (type == 2) ? combatant2Idx : -1;

    // Check if any combatant is in recovery mode - if so, skip battle entirely
    if ((type == 0 || type == 1) && Game::players[p1Idx].isRecovering()) {
        // Player 1 is recovering, no battle
        return -1; // Special code indicating battle was skipped
    }

    if (type == 1 && Game::players[p2Idx].isRecovering()) {
        // Player 2 is recovering, no battle
        return -1; // Special code indicating battle was skipped
    }

    if (type == 0 && !Game::monsters[m1Idx].isDefeated() && Game::monsters[m1Idx].isRecovering()) {
        // Monster is recovering, no battle
        return -1;
    }

    if (type == 2) {
        if (Game::monsters[m1Idx].isRecovering() || Game::monsters[m2Idx].isRecovering()) {
            // One of the monsters is recovering, no battle
            return -1;
        }
    }

    // Show battle spot
    int beX = (type != 2) ? Game::players[p1Idx].getPosition().x : Game::monsters[m1Idx].getPosition().x;
    int beY = (type != 2) ? Game::players[p1Idx].getPosition().y : Game::monsters[m1Idx].getPosition().y;
    ui.animateBullseye(beY, beX, 5, Game::pauseTime/40, 0);

    if (Game::showWindows) {
        // Calculate window dimensions
        int height = MIN_ROWS - 2;
        int width = MIN_COLS;
        
        // Create a new window centered on screen
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        battleWin = newwin(height, width, (maxY - height) / 2, (maxX - width) / 2);
        box(battleWin, 0, 0);
        
        // Set up colors for battle screen
        wbkgd(battleWin, COLOR_PAIR(10));
        
        // Display battle title based on battle type
        wattron(battleWin, A_BOLD);
        switch (type) {
        case 0:
            mvwprintw(battleWin, 2, 2, "PLAYER VS MONSTER BATTLE");
            break;
        case 1:
            mvwprintw(battleWin, 2, 2, "PLAYER VS PLAYER BATTLE");
            break;
        case 2:
            mvwprintw(battleWin, 2, 2, "MONSTER VS MONSTER BATTLE");
            break;
        }
        wattroff(battleWin, A_BOLD);
        
        // ASCII art for left combatant
        if (type == 0 || type == 1) { // Player Bot
            showBattleArt(true, true, p1Idx, type == 0 ? m1Idx : p2Idx);
        } else { // Monster
            showBattleArt(true, false, m1Idx, m2Idx);
        }
        
        //       .... for  right combatant
        if (type == 1) { // Player Bot
            showBattleArt(false, true, p1Idx, p2Idx);
        } else { // Monster
            showBattleArt(false, false, type == 0 ? p1Idx : m1Idx,type == 0 ? m1Idx : m2Idx);
        }
        
        // Display vs text in the middle
        wattron(battleWin, A_BOLD);
        mvwprintw(battleWin, 9, 19, "VS");
        wattroff(battleWin, A_BOLD);
        
        // Display roll boxes
        mvwprintw(battleWin, 14, 5, "╔═════════╗        ╔═════════╗");
        mvwprintw(battleWin, 15, 5, "║         ║        ║         ║");
        mvwprintw(battleWin, 16, 5, "╚═════════╝        ╚═════════╝");
        
        wrefresh(battleWin);
    }

    // Calculate battle rolls based on combatant types
    int roll1, roll2;

    // Left combatant roll
    if (type == 0 || type == 1) { // Player
        roll1 = Game::players[p1Idx].getStrength() + (rand() % 6);
    } else { // Monster
        roll1 = Game::monsters[m1Idx].getStrength() + (rand() % 6);
    }

    // Right combatant roll
    if (type == 1) { // Player
        roll2 = Game::players[p2Idx].getStrength() + (rand() % 6);
    } else { // Monster
        roll2 = (type == 0 ? Game::monsters[m1Idx].getStrength() : Game::monsters[m2Idx].getStrength()) +
                (rand() % 6);
    }

    if (Game::showWindows) {
        // Display die rolls
        if (type == 0 || type == 1) { // Left is player
            wattron(battleWin, COLOR_PAIR(Game::players[p1Idx].getColorPair()) | A_BOLD);
            mvwprintw(battleWin, 15, 10, "%d", roll1);
            wattroff(battleWin, COLOR_PAIR(Game::players[p1Idx].getColorPair()) | A_BOLD);
        } else { // Left is monster
            wattron(battleWin, COLOR_PAIR(8) | A_BOLD);
            mvwprintw(battleWin, 15, 10, "%d", roll1);
            wattroff(battleWin, COLOR_PAIR(8) | A_BOLD);
        }
        
        if (type == 1) { // Right is player
            wattron(battleWin, COLOR_PAIR(Game::players[p2Idx].getColorPair()) | A_BOLD);
            mvwprintw(battleWin, 15, 29, "%d", roll2);
            wattroff(battleWin, COLOR_PAIR(Game::players[p2Idx].getColorPair()) | A_BOLD);
        } else { // Right is monster
            wattron(battleWin, COLOR_PAIR(8) | A_BOLD);
            mvwprintw(battleWin, 15, 29, "%d", roll2);
            wattroff(battleWin, COLOR_PAIR(8) | A_BOLD);
        }
        
        wrefresh(battleWin);
    }

    // Determine winner and loser
    bool leftWins = false;

    // For player vs monster, player wins on tie
    if (type == 0) {
        leftWins = (roll1 >= roll2);
    }
    // For player vs player or monster vs monster, higher roll wins
    // In case of tie, first combatant wins (arbitrary rule)
    else {
        leftWins = (roll1 > roll2 ||
                    (roll1 == roll2 && 
                    (type == 1 ? combatant1Idx > combatant2Idx
                                : Game::monsters[m1Idx].getStrength() >=
                                  Game::monsters[m2Idx].getStrength())));
    }

    // Display result and update stats based on battle type
    if (leftWins) {
        // Left combatant wins
        if (type == 0) {
            // Player vs monster - player wins
            updateBattleResults(true, type, p1Idx, m1Idx);
        } else if (type == 1) {
            // Player vs player - left player wins
            updateBattleResults(true, type, p1Idx, p2Idx);
        } else {
            // Monster vs monster - left monster wins
            updateBattleResults(true, type, m1Idx, m2Idx);
        }
    } else {
        // Right combatant wins
        if (type == 0) {
            // Player vs monster - monster wins
            updateBattleResults(false, type, p1Idx, m1Idx);
        } else if (type == 1) {
            // Player vs player - right player wins
            updateBattleResults(false, type, p1Idx, p2Idx);
        } else {
            // Monster vs monster - right monster wins
            updateBattleResults(false, type, m1Idx, m2Idx);
        }
    }

    if (Game::showWindows) {
        wrefresh(battleWin);
        Game::pauseForUser();
        
        // Clean up battle window
        delwin(battleWin);
        battleWin = nullptr;
    }

    // Redraw the main screen
    ui.printMaze(maze::instance(), Game::players);
    ui.displayPlayerStats(Game::players);

    // Return battle result
    if (type == 0) {
        return leftWins ? 1 : 0; // For player vs monster: 1 if player won, 0 if monster won
    } else if (type == 1) {
        return leftWins ? combatant1Idx : combatant2Idx; // Return winner's ID
    } else {
        return leftWins ? combatant1Idx : combatant2Idx; // Return winner monster index
    }
}

int Battle::battlePlayerMonster(int playerId, int monsterIdx) {
    return battle(playerId, monsterIdx, 0);
}

int Battle::battlePlayers(int player1Id, int player2Id) {
    return battle(player1Id, player2Id, 1);
}

int Battle::battleMonsters(int monster1Idx, int monster2Idx) {
    battle(monster1Idx, monster2Idx, 2);
    return 0; // Return value not used for monster vs monster
}

// Singleton for the maze instance
MazeGrid& maze::instance() {
    //Logger::log("maze::instance() called");
    static bool inInstance = false;
    
    if (inInstance) {
        // We're already inside instance(), avoid recursion
        static MazeGrid dummyMaze(MIN_ROWS, MIN_COLS);
        return dummyMaze;
    }
    
    inInstance = true;
    
    if (!instance_) {
        Logger::log("FATAL: maze::instance() called before initialization!\n%s",
            get_stacktrace().c_str());
				abort();
    }
    
    inInstance = false;
    return *instance_; 
}

void maze::initialize(int rows, int cols) {
	  Logger::log("maze::initialize() called");
    delete instance_;
    instance_ = new MazeGrid(rows, cols);
}
void maze::generate() { instance().generate(); }
int maze::getRows() { return instance().getRows(); }
int maze::getCols() { return instance().getCols(); }

std::string get_stacktrace(int max_frames) {
#ifndef __CYGWIN__
    void* buffer[32];
    int nptrs = backtrace(buffer, max_frames);
    char** symbols = backtrace_symbols(buffer, nptrs);

    std::ostringstream oss;
    oss << "Stack trace:\n";
    for (int i = 0; i < nptrs; ++i) {
        oss << "  #" << i << " " << symbols[i] << '\n';
    }
    free(symbols);
    return oss.str();
#else
	  max_frames = max_frames;
    return "Not supported on CYGWIN";
#endif
}

// Global arguments for command line processing ////////////////////////

// Static member initialization
int Game::gameSpeed = DEF_GAME_SPEED;
int Game::gameDelay = 0;
int Game::gameRounds = 1;
int Game::gameRoundsTotal = 1;
int Game::gameTime = 0;
bool Game::showWindows = false;
bool Game::waitForKey = false;
int Game::pauseTime = 2000;
int Game::numTeleporters = -1;
int Game::numMonsters = -1;
int Game::liveMonsters = 0;
int Game::maxMonsterStrength = -1;
int Game::playersFinished = 0;
int Game::gameFinished = 0;
std::vector<Player> Game::players;
std::vector<Monster> Game::monsters;
std::vector<Teleporter> Game::teleporters;

int g_argc = 0;
char** g_argv = nullptr;
StatusLine Game::status(STATUS_LINE_HISTORY, 0);
MazeGrid* maze::instance_ = nullptr;

// Main function //////////////////////////////////////////
int main(int argc, char *argv[]) {
    Logger::log("main() called");

    // Store arguments globally
    g_argc = argc;
    g_argv = argv;
    
    // Create and run the game
    Game game;
    game.run();
    
    return 0;
}

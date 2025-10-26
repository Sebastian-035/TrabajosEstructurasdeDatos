#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <random>
#include <conio.h>
#include <queue>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#undef max
#undef min
#endif

using namespace std;

#define SAVE_PATH "C:\\Users\\T14 GEN1\\Desktop\\Trabajos\\ProyectosdeVS\\CazadorDeTesoros\\Guardado.txt"

const int MAP_SIZE = 20;
const int INVENTORY_SIZE = 5;

// Función para limpiar la pantalla sin parpadeos
void clearScreen() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    DWORD count;
    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    COORD homeCoords = { 0, 0 };
    FillConsoleOutputCharacter(hStdOut, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hStdOut, homeCoords);
#else
    // En sistemas Unix/Linux, usar secuencia de escape ANSI
    cout << "\033[2J\033[H";
#endif
}

// Clase para manejar el buffer de pantalla
class ScreenBuffer {
private:
    vector<string> buffer;
    int width, height;

public:
    ScreenBuffer(int w, int h) : width(w), height(h) {
        buffer.resize(h, string(w, ' '));
    }

    void clear() {
        for (auto& line : buffer) {
            fill(line.begin(), line.end(), ' ');
        }
    }

    void set(int x, int y, char c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y][x] = c;
        }
    }

    void setString(int x, int y, const string& str) {
        for (size_t i = 0; i < str.length() && x + (int)i < width; i++) {
            set(x + (int)i, y, str[i]);
        }
    }

    void display() {
        clearScreen();
        for (const auto& line : buffer) {
            cout << line << '\n';
        }
        cout.flush();
    }
};

class Player {
public:
    int x, y;
    int lives;
    int energy;
    bool hasKey;
    int movesWithoutCrash;
    int totalTreasures;
    int score;
    vector<int> inventory;

    Player() : x(MAP_SIZE / 2), y(MAP_SIZE / 2), lives(40), energy(4),
        hasKey(false), movesWithoutCrash(0), totalTreasures(0), score(0) {
        inventory.resize(0);
    }
};

class Map {
public:
    vector<vector<char>> grid;
    int key_x, key_y;
    int door_x, door_y;
    vector<pair<int, int>> treasures;
    vector<pair<int, int>> traps;
    vector<vector<bool>> revealed;
    pair<int, int> valid_start_pos;
    bool isGenerated;

    Map() : key_x(0), key_y(0), door_x(0), door_y(0), isGenerated(false) {
        grid.resize(MAP_SIZE, vector<char>(MAP_SIZE, '?'));
        revealed.resize(MAP_SIZE, vector<bool>(MAP_SIZE, false));
        initializeBorders();
    }

    void initializeBorders() {
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1) {
                    grid[i][j] = '|';
                    revealed[i][j] = true;
                }
            }
        }
    }

    void generate(int level) {
        if (isGenerated) return;

        for (int i = 1; i < MAP_SIZE - 1; i++) {
            for (int j = 1; j < MAP_SIZE - 1; j++) {
                grid[i][j] = '?';
                revealed[i][j] = false;
            }
        }

        createPaths(1, 1);

        for (int i = 1; i < MAP_SIZE - 1; i++) {
            for (int j = 1; j < MAP_SIZE - 1; j++) {
                if (grid[i][j] == '?') {
                    grid[i][j] = '|';
                }
            }
        }

        findValidRespawn();
        int startX = valid_start_pos.first;
        int startY = valid_start_pos.second;

        placeDoor(startX, startY);
        placeKey(startX, startY);
        placeObjects(startX, startY, level);

        isGenerated = true;
    }

    bool isAccessible(int startX, int startY, int targetX, int targetY) const {
        if (startX < 0 || startX >= MAP_SIZE || startY < 0 || startY >= MAP_SIZE ||
            targetX < 0 || targetX >= MAP_SIZE || targetY < 0 || targetY >= MAP_SIZE) {
            return false;
        }

        vector<vector<bool>> visited(MAP_SIZE, vector<bool>(MAP_SIZE, false));
        queue<pair<int, int>> q;

        q.push({ startX, startY });
        visited[startX][startY] = true;

        vector<pair<int, int>> directions = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current.first == targetX && current.second == targetY) {
                return true;
            }

            for (const auto& dir : directions) {
                int nextX = current.first + dir.first;
                int nextY = current.second + dir.second;

                if (nextX >= 0 && nextX < MAP_SIZE && nextY >= 0 && nextY < MAP_SIZE &&
                    !visited[nextX][nextY] && grid[nextX][nextY] != '|') {

                    visited[nextX][nextY] = true;
                    q.push({ nextX, nextY });
                }
            }
        }
        return false;
    }

    pair<int, int> findValidRespawn() {
        int startX = MAP_SIZE / 2;
        int startY = MAP_SIZE / 2;

        if (grid[startX][startY] == ' ') {
            valid_start_pos = { startX, startY };
            return valid_start_pos;
        }

        for (int radius = 1; radius < MAP_SIZE / 2; radius++) {
            for (int i = startX - radius; i <= startX + radius; i++) {
                for (int j = startY - radius; j <= startY + radius; j++) {
                    if (i >= 1 && i < MAP_SIZE - 1 && j >= 1 && j < MAP_SIZE - 1 &&
                        grid[i][j] == ' ') {
                        valid_start_pos = { i, j };
                        return valid_start_pos;
                    }
                }
            }
        }

        for (int i = 1; i < MAP_SIZE - 1; ++i) {
            for (int j = 1; j < MAP_SIZE - 1; ++j) {
                if (grid[i][j] == ' ') {
                    valid_start_pos = { i, j };
                    return valid_start_pos;
                }
            }
        }

        valid_start_pos = { 1, 1 };
        return valid_start_pos;
    }

    void createPaths(int startX, int startY) {
        vector<pair<int, int>> directions = { { -1,0 }, { 1,0 }, { 0,-1 }, { 0,1 } };
        vector<pair<int, int>> stack;
        stack.push_back({ startX, startY });
        grid[startX][startY] = ' ';

        unsigned seed = static_cast<unsigned>(chrono::steady_clock::now().time_since_epoch().count());
        default_random_engine e(seed);

        while (!stack.empty()) {
            auto current = stack.back();
            stack.pop_back();

            shuffle(directions.begin(), directions.end(), e);

            for (auto dir : directions) {
                int newX = current.first + dir.first * 2;
                int newY = current.second + dir.second * 2;

                if (newX > 0 && newX < MAP_SIZE - 1 && newY > 0 && newY < MAP_SIZE - 1 && grid[newX][newY] == '?') {
                    grid[newX][newY] = ' ';
                    grid[current.first + dir.first][current.second + dir.second] = ' ';
                    stack.push_back({ newX, newY });
                }
            }
        }
    }

    void placeDoor(int startX, int startY) {
        bool placed = false;

        vector<pair<int, int>> potentialDoors;

        for (int i = 1; i < MAP_SIZE - 1; i++) {
            for (int j = 1; j < MAP_SIZE - 1; j++) {
                if (grid[i][j] == '|') {
                    vector<pair<int, int>> adjacentSpaces;
                    if (i > 0 && grid[i - 1][j] == ' ') adjacentSpaces.push_back({ i - 1, j });
                    if (i < MAP_SIZE - 1 && grid[i + 1][j] == ' ') adjacentSpaces.push_back({ i + 1, j });
                    if (j > 0 && grid[i][j - 1] == ' ') adjacentSpaces.push_back({ i, j - 1 });
                    if (j < MAP_SIZE - 1 && grid[i][j + 1] == ' ') adjacentSpaces.push_back({ i, j + 1 });

                    for (const auto& space : adjacentSpaces) {
                        if (isAccessible(startX, startY, space.first, space.second)) {
                            potentialDoors.push_back({ i, j });
                            break;
                        }
                    }
                }
            }
        }

        if (!potentialDoors.empty()) {
            shuffle(potentialDoors.begin(), potentialDoors.end(),
                default_random_engine(static_cast<unsigned>(chrono::steady_clock::now().time_since_epoch().count())));

            door_x = potentialDoors[0].first;
            door_y = potentialDoors[0].second;
            grid[door_x][door_y] = '#';
            placed = true;
        }

        if (!placed) {
            for (int i = 1; i < MAP_SIZE - 1; i++) {
                for (int j = 1; j < MAP_SIZE - 1; j++) {
                    if (grid[i][j] == '|') {
                        door_x = i;
                        door_y = j;
                        grid[door_x][door_y] = '#';
                        placed = true;
                        break;
                    }
                }
                if (placed) break;
            }
        }
    }

    void placeKey(int startX, int startY) {
        bool placed = false;

        vector<pair<int, int>> potentialSpots;
        for (int i = 1; i < MAP_SIZE - 1; i++) {
            for (int j = 1; j < MAP_SIZE - 1; j++) {
                if (grid[i][j] == ' ' && isAccessible(startX, startY, i, j)) {
                    int distToStart = abs(i - startX) + abs(j - startY);
                    int distToDoor = abs(i - door_x) + abs(j - door_y);
                    if (distToStart > 3 && distToStart < 15 && distToDoor > 3) {
                        potentialSpots.push_back({ i, j });
                    }
                }
            }
        }

        if (!potentialSpots.empty()) {
            shuffle(potentialSpots.begin(), potentialSpots.end(),
                default_random_engine(static_cast<unsigned>(chrono::steady_clock::now().time_since_epoch().count())));

            key_x = potentialSpots[0].first;
            key_y = potentialSpots[0].second;
            grid[key_x][key_y] = 'K';
            placed = true;
        }

        if (!placed) {
            for (int i = 1; i < MAP_SIZE - 1; ++i) {
                for (int j = 1; j < MAP_SIZE - 1; ++j) {
                    if (grid[i][j] == ' ' && isAccessible(startX, startY, i, j) &&
                        (i != door_x || j != door_y)) {
                        key_x = i;
                        key_y = j;
                        grid[i][j] = 'K';
                        return;
                    }
                }
            }
        }
    }

    void placeObjects(int startX, int startY, int level) {
        auto placeObject = [&](char objectChar, int count) {
            for (int i = 0; i < count; i++) {
                bool placed = false;
                int x, y;

                vector<pair<int, int>> availableSpots;
                for (int i = 1; i < MAP_SIZE - 1; i++) {
                    for (int j = 1; j < MAP_SIZE - 1; j++) {
                        if (grid[i][j] == ' ' && isAccessible(startX, startY, i, j) &&
                            (i != key_x || j != key_y) && (i != door_x || j != door_y)) {
                            availableSpots.push_back({ i, j });
                        }
                    }
                }

                if (!availableSpots.empty()) {
                    shuffle(availableSpots.begin(), availableSpots.end(),
                        default_random_engine(static_cast<unsigned>(chrono::steady_clock::now().time_since_epoch().count())));

                    x = availableSpots[0].first;
                    y = availableSpots[0].second;
                    grid[x][y] = objectChar;
                    if (objectChar == 'T') treasures.push_back(make_pair(x, y));
                    if (objectChar == 'X') traps.push_back(make_pair(x, y));
                    placed = true;
                }
            }
            };

        int treasureCount = 4 + level;
        int trapCount = 8 + level * 2;

        placeObject('T', treasureCount);
        placeObject('X', trapCount);
    }

    void revealCross(int x, int y) {
        for (int i = max(0, x - 1); i <= min(MAP_SIZE - 1, x + 1); i++) {
            for (int j = max(0, y - 1); j <= min(MAP_SIZE - 1, y + 1); j++) {
                revealed[i][j] = true;
            }
        }
    }

    void print(ScreenBuffer& screen, int player_x, int player_y) {
        int screenX = 0;
        int screenY = 6; // Empezar después de la información del juego

        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                char displayChar;
                if (i == player_x && j == player_y) {
                    displayChar = 'P';
                }
                else if (revealed[i][j]) {
                    displayChar = grid[i][j];
                }
                else {
                    displayChar = '?';
                }
                screen.set(screenX + j * 2, screenY + i, displayChar);
                screen.set(screenX + j * 2 + 1, screenY + i, ' ');
            }
        }
    }

    // Guardar el estado base del mapa (sin información de revelado)
    int getBaseNumericCode(int x, int y, bool hasKey) const {
        char cell = grid[x][y];

        switch (cell) {
        case ' ':
            return 1;
        case '|':
            return 7;
        case 'T':
            return 3;
        case 'X':
            return 4;
        case 'K':
            return (hasKey && x == key_x && y == key_y) ? 1 : 5;
        case '#':
            return 6;
        default:
            return 1;
        }
    }

    // Guardar el estado de revelado
    int getRevealedCode(int x, int y) const {
        return revealed[x][y] ? 1 : 0;
    }

    void setGridFromCode(int code, int x, int y, bool hasKey) {
        switch (code) {
        case 1:
            grid[x][y] = ' ';
            break;
        case 7:
            grid[x][y] = '|';
            break;
        case 3:
            grid[x][y] = 'T';
            treasures.push_back({ x, y });
            break;
        case 4:
            grid[x][y] = 'X';
            traps.push_back({ x, y });
            break;
        case 5:
            key_x = x;
            key_y = y;
            grid[x][y] = 'K';
            break;
        case 6:
            door_x = x;
            door_y = y;
            grid[x][y] = '#';
            break;
        }

        if ((x == 0 || x == MAP_SIZE - 1 || y == 0 || y == MAP_SIZE - 1) && code != 6) {
            grid[x][y] = '|';
        }
    }

    void setRevealedFromCode(int code, int x, int y) {
        revealed[x][y] = (code == 1);
    }

    // Función para cargar un mapa completo desde datos guardados
    void loadFromCodes(const vector<vector<int>>& baseCodes, const vector<vector<int>>& revealedCodes, bool hasKey) {
        treasures.clear();
        traps.clear();

        for (int i = 0; i < MAP_SIZE; ++i) {
            for (int j = 0; j < MAP_SIZE; ++j) {
                setGridFromCode(baseCodes[i][j], i, j, hasKey);
                setRevealedFromCode(revealedCodes[i][j], i, j);
            }
        }
        isGenerated = true;
    }
};

class Game {
private:
    vector<Map> maps;
    int currentLevel;
    Player player;
    chrono::steady_clock::time_point startTime;
    bool gameRunning;
    vector<pair<string, pair<int, pair<int, int>>>> ranking;
    ScreenBuffer screen;

public:
    Game() : currentLevel(0), gameRunning(true), screen(80, 30) {
        maps.resize(3);
        maps[0].generate(1);
        startTime = chrono::steady_clock::now();
        loadRanking();

        if (currentLevel < maps.size()) {
            player.x = maps[0].valid_start_pos.first;
            player.y = maps[0].valid_start_pos.second;
            maps[currentLevel].revealCross(player.x, player.y);
        }
    }

    void generateNextLevel() {
        if (currentLevel < maps.size()) {
            maps[currentLevel].generate(currentLevel + 1);
        }
    }

    void play() {
        while (gameRunning) {
            if (currentLevel < maps.size()) {
                generateNextLevel();
            }

            Map& currentMap = maps[currentLevel];

            // Limpiar y dibujar en el buffer
            screen.clear();
            displayGameInfo();
            currentMap.print(screen, player.x, player.y);
            screen.display();

            handleInput();

            if (player.lives <= 0) {
                clearScreen();
                cout << "Game Over! Te quedaste sin vidas." << endl;
                this_thread::sleep_for(chrono::milliseconds(2000));
                gameRunning = false;
            }

            checkLevelCompletion();

            if (currentLevel >= 3) {
                break;
            }
        }

        if (currentLevel >= 3) {
            completeGame();
        }
    }

    void displayGameInfo() {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(now - startTime);

        screen.setString(0, 0, "=== NIVEL " + to_string(currentLevel + 1) + " ===");
        screen.setString(0, 1, "Tiempo: " + to_string(elapsed.count()) + "s | Vidas: " + to_string(player.lives));
        screen.setString(0, 2, "Energia: " + to_string(player.energy) + " | Puntuacion: " + to_string(player.score));
        screen.setString(0, 3, "Llave: " + string(player.hasKey ? "Si" : "No"));
        screen.setString(0, 4, "Controles: WASD-mover, I-inventario, Q-salvar y salir");
        screen.setString(0, 5, "=========================");
    }

    void handleInput() {
        int input;

#ifdef _WIN32
        input = _getch();
#else
        system("stty raw");
        input = getchar();
        system("stty cooked");
#endif

        input = toupper(input);

        switch (input) {
        case 'W': movePlayer(-1, 0); break;
        case 'S': movePlayer(1, 0); break;
        case 'A': movePlayer(0, -1); break;
        case 'D': movePlayer(0, 1); break;
        case 'I': manageInventory(); break;
        case 'Q':
            saveGame();
            gameRunning = false;
            break;
        default:
            break;
        }
    }

    void movePlayer(int dx, int dy) {
        int newX = player.x + dx;
        int newY = player.y + dy;

        if (newX < 0 || newX >= MAP_SIZE || newY < 0 || newY >= MAP_SIZE) {
            handleWallCollision(newX, newY);
            return;
        }

        char targetCell = maps[currentLevel].grid[newX][newY];

        if (targetCell == '|') {
            handleWallCollision(newX, newY);
            return;
        }

        if (targetCell == '#') {
            if (player.hasKey) {
                // Permitir movimiento
            }
            else {
                // Mostrar mensaje en el buffer sin interrumpir el flujo
                screen.clear();
                displayGameInfo();
                maps[currentLevel].print(screen, player.x, player.y);
                screen.setString(0, 26, "La puerta esta cerrada. Necesitas la llave 'K'.");
                screen.display();
                this_thread::sleep_for(chrono::milliseconds(1000));
                return;
            }
        }

        player.x = newX;
        player.y = newY;
        player.movesWithoutCrash++;

        // Revelar el área alrededor de la nueva posición
        maps[currentLevel].revealCross(player.x, player.y);

        if (player.movesWithoutCrash >= 3 && player.energy < 4) {
            player.energy++;
            player.movesWithoutCrash = 0;
            screen.clear();
            displayGameInfo();
            maps[currentLevel].print(screen, player.x, player.y);
            screen.setString(0, 26, "Recuperaste 1 de energia!");
            screen.display();
            this_thread::sleep_for(chrono::milliseconds(800));
        }

        switch (targetCell) {
        case 'K':
            player.hasKey = true;
            maps[currentLevel].grid[newX][newY] = ' ';
            screen.clear();
            displayGameInfo();
            maps[currentLevel].print(screen, player.x, player.y);
            screen.setString(0, 26, "Has encontrado la llave!");
            screen.display();
            this_thread::sleep_for(chrono::milliseconds(1000));
            break;

        case 'T':
            collectTreasure(newX, newY);
            break;

        case 'X':
            hitTrap(newX, newY);
            break;

        case '#':
            break;
        }
    }

    void handleWallCollision(int wallX, int wallY) {
        if (wallX >= 0 && wallX < MAP_SIZE && wallY >= 0 && wallY < MAP_SIZE) {
            maps[currentLevel].revealed[wallX][wallY] = true;
        }

        player.energy--;
        player.movesWithoutCrash = 0;

        screen.clear();
        displayGameInfo();
        maps[currentLevel].print(screen, player.x, player.y);
        screen.setString(0, 26, "Crash! Chocaste contra un muro. Pierdes 1 de energia.");
        screen.display();
        this_thread::sleep_for(chrono::milliseconds(800));

        if (player.energy <= 0) {
            player.lives--;
            player.energy = 4;
            screen.clear();
            displayGameInfo();
            maps[currentLevel].print(screen, player.x, player.y);
            screen.setString(0, 26, "Te quedaste sin energia y has perdido una vida!");
            screen.display();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }

    void collectTreasure(int x, int y) {
        int baseValue = rand() % 100 + 50;

        auto& treasures_list = maps[currentLevel].treasures;
        treasures_list.erase(
            remove_if(treasures_list.begin(), treasures_list.end(),
                [&](const pair<int, int>& p) { return p.first == x && p.second == y; }),
            treasures_list.end()
        );

        int actualValue = baseValue + (currentLevel * 50);

        if (player.inventory.size() < INVENTORY_SIZE) {
            player.inventory.push_back(actualValue);
            sort(player.inventory.rbegin(), player.inventory.rend());
            player.totalTreasures++;
            player.score += actualValue;

            screen.clear();
            displayGameInfo();
            maps[currentLevel].print(screen, player.x, player.y);
            screen.setString(0, 26, "Tesoro encontrado! Valor: " + to_string(actualValue) +
                " (Inventario: " + to_string(player.inventory.size()) + "/" + to_string(INVENTORY_SIZE) + ")");
            screen.display();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        else {
            manageInventoryFull(actualValue);
        }

        maps[currentLevel].grid[x][y] = ' ';
    }

    void hitTrap(int x, int y) {
        int damage = 1 + currentLevel;
        player.lives -= damage;
        player.movesWithoutCrash = 0;

        auto& traps_list = maps[currentLevel].traps;
        traps_list.erase(
            remove_if(traps_list.begin(), traps_list.end(),
                [&](const pair<int, int>& p) { return p.first == x && p.second == y; }),
            traps_list.end()
        );

        maps[currentLevel].grid[x][y] = ' ';

        screen.clear();
        displayGameInfo();
        maps[currentLevel].print(screen, player.x, player.y);
        screen.setString(0, 26, "Trampa activada! Has perdido " + to_string(damage) + " vidas. La trampa se ha desactivado.");
        screen.display();
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    void manageInventoryFull(int newValue) {
        clearScreen();
        cout << "=== INVENTARIO LLENO ===" << endl;
        cout << "Has encontrado un tesoro valor: " << newValue << endl;
        cout << "Tu inventario esta lleno. Max: " << INVENTORY_SIZE << endl;

        for (size_t i = 0; i < player.inventory.size(); i++) {
            cout << (i + 1) << ". Tesoro valor: " << player.inventory[i] << endl;
        }

        cout << "Quieres reemplazar un tesoro? (S/N): ";
        char choice;
        cin >> choice;

        if (toupper(choice) == 'S') {
            replaceTreasure(newValue);
        }
        else {
            cout << "Tesoro no recogido." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void replaceTreasure(int newValue) {
        cout << "Elige el numero del tesoro a reemplazar (1-" << player.inventory.size() << ", 0 para cancelar): ";
        int choice;

        while (!(cin >> choice) || choice < 0 || static_cast<size_t>(choice) > player.inventory.size()) {
            cout << "Entrada invalida. Elige un numero entre 1 y " << player.inventory.size() << " o 0: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice > 0 && static_cast<size_t>(choice) <= player.inventory.size()) {
            player.score -= player.inventory[choice - 1];
            player.inventory[choice - 1] = newValue;
            player.score += newValue;
            sort(player.inventory.rbegin(), player.inventory.rend());
            cout << "Tesoro reemplazado!" << endl;
        }
        else {
            cout << "Operacion cancelada. Tesoro no recogido." << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    void manageInventory() {
        clearScreen();
        cout << "=== INVENTARIO ===" << endl;
        if (player.inventory.empty()) {
            cout << "Inventario vacio." << endl;
        }
        else {
            for (size_t i = 0; i < player.inventory.size(); i++) {
                cout << (i + 1) << ". Tesoro valor: " << player.inventory[i] << endl;
            }
        }
        cout << "==================" << endl;
        cout << "Presiona cualquier tecla para continuar...";

        int input;
#ifdef _WIN32
        input = _getch();
#else
        system("stty raw");
        input = getchar();
        system("stty cooked");
#endif
    }

    void checkLevelCompletion() {
        if (player.hasKey && player.x == maps[currentLevel].door_x && player.y == maps[currentLevel].door_y) {
            currentLevel++;
            if (currentLevel < 3) {
                generateNextLevel();

                player.lives += 1;
                player.energy = 4;
                player.hasKey = false;
                player.movesWithoutCrash = 0;

                Map& nextMap = maps[currentLevel];
                player.x = nextMap.valid_start_pos.first;
                player.y = nextMap.valid_start_pos.second;

                nextMap.revealCross(player.x, player.y);

                screen.clear();
                displayGameInfo();
                nextMap.print(screen, player.x, player.y);
                screen.setString(0, 26, "Nivel completado! Avanzas al nivel " + to_string(currentLevel + 1) + " y ganas 1 vida.");
                screen.display();
                this_thread::sleep_for(chrono::milliseconds(2000));
            }
        }
    }

    void completeGame() {
        clearScreen();
        auto endTime = chrono::steady_clock::now();
        auto totalTime = chrono::duration_cast<chrono::seconds>(endTime - startTime);

        cout << "FELICIDADES! Has completado todos los niveles." << endl;
        cout << "Tiempo total: " << totalTime.count() << " segundos" << endl;
        cout << "Vidas restantes: " << player.lives << endl;
        cout << "Puntuacion total: " << player.score << endl;
        cout << "Tesoros recolectados: " << player.totalTreasures << endl;

        string playerName;
        cout << "Ingresa tu nombre para el ranking: ";
        cin >> playerName;

        int timeSeconds = static_cast<int>(totalTime.count());

        ranking.emplace_back(playerName, make_pair(timeSeconds, make_pair(player.lives, player.score)));

        sort(ranking.begin(), ranking.end(), [](const auto& a, const auto& b) {
            return a.second.second.second > b.second.second.second;
            });

        saveRanking();
        displayRanking();
    }

    void displayRanking() {
        clearScreen();
        cout << "=== RANKING ===" << endl;

        if (ranking.empty()) {
            cout << "No hay jugadores en el ranking." << endl;
        }
        else {
            cout << left << setw(15) << "Nombre" << setw(10) << "Tiempo" << setw(8) << "Vidas" << setw(10) << "Puntuacion" << endl;
            cout << "---------------------------------" << endl;

            for (const auto& entry : ranking) {
                cout << left << setw(15) << entry.first
                    << setw(10) << entry.second.first
                    << setw(8) << entry.second.second.first
                    << setw(10) << entry.second.second.second << endl;
            }
        }

        cout << "Presiona cualquier tecla para continuar...";

        int input;
#ifdef _WIN32
        input = _getch();
#else
        system("stty raw");
        input = getchar();
        system("stty cooked");
#endif
    }

    void saveGame() {
        ofstream file(SAVE_PATH);
        if (file.is_open()) {
            file << "CURRENT_LEVEL: " << currentLevel << endl;
            file << "PLAYER_POS: " << player.x << " " << player.y << endl;
            file << "PLAYER_STATS: " << player.lives << " " << player.energy << " " << player.hasKey << " " << player.movesWithoutCrash << " " << player.totalTreasures << " " << player.score << endl;

            file << "INVENTORY_SIZE: " << player.inventory.size() << endl;
            file << "INVENTORY_ITEMS:";
            for (int val : player.inventory) {
                file << " " << val;
            }
            file << endl;

            auto currentTime = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
            file << "ELAPSED_TIME: " << elapsed.count() << endl;

            // Guardar el mapa actual - BASE y REVEALED por separado
            Map& currentMap = maps[currentLevel];
            file << "MAP_BASE:" << endl;
            for (int i = 0; i < MAP_SIZE; ++i) {
                for (int j = 0; j < MAP_SIZE; ++j) {
                    int code = currentMap.getBaseNumericCode(i, j, player.hasKey);
                    file << code << " ";
                }
                file << endl;
            }

            file << "MAP_REVEALED:" << endl;
            for (int i = 0; i < MAP_SIZE; ++i) {
                for (int j = 0; j < MAP_SIZE; ++j) {
                    int revealed = currentMap.getRevealedCode(i, j);
                    file << revealed << " ";
                }
                file << endl;
            }

            cout << "Juego guardado exitosamente en: " << SAVE_PATH << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
        else {
            cout << "ERROR: No se pudo abrir o crear el archivo de guardado en: " << SAVE_PATH << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
    }

    void loadGame() {
        ifstream file(SAVE_PATH);
        if (!file.is_open()) {
            cout << "ERROR: No se encontró partida guardada en: " << SAVE_PATH << ". Iniciando partida nueva." << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            *this = Game();
            return;
        }

        string tag;
        file >> tag >> currentLevel;
        file >> tag >> player.x >> player.y;
        file >> tag >> player.lives >> player.energy >> player.hasKey >> player.movesWithoutCrash >> player.totalTreasures >> player.score;

        size_t inv_size;
        file >> tag >> inv_size;
        file >> tag;
        player.inventory.resize(inv_size);
        for (size_t i = 0; i < inv_size; i++) {
            file >> player.inventory[i];
        }

        int savedTime;
        file >> tag >> savedTime;
        startTime = chrono::steady_clock::now() - chrono::seconds(savedTime);
        gameRunning = true;

        // Inicializar todos los mapas
        maps.clear();
        maps.resize(3);

        // Cargar el mapa base
        file >> tag; // "MAP_BASE:"

        vector<vector<int>> baseCodes(MAP_SIZE, vector<int>(MAP_SIZE));
        for (int i = 0; i < MAP_SIZE; ++i) {
            for (int j = 0; j < MAP_SIZE; ++j) {
                if (!(file >> baseCodes[i][j])) {
                    cout << "Error leyendo datos del mapa base." << endl;
                    return;
                }
            }
        }

        // Cargar el estado de revelado
        file >> tag; // "MAP_REVEALED:"

        vector<vector<int>> revealedCodes(MAP_SIZE, vector<int>(MAP_SIZE));
        for (int i = 0; i < MAP_SIZE; ++i) {
            for (int j = 0; j < MAP_SIZE; ++j) {
                if (!(file >> revealedCodes[i][j])) {
                    cout << "Error leyendo datos de revelado." << endl;
                    return;
                }
            }
        }

        // Cargar el mapa actual desde los códigos
        Map& currentMap = maps[currentLevel];
        currentMap.loadFromCodes(baseCodes, revealedCodes, player.hasKey);

        // Asegurar que la posición del jugador sea un camino
        currentMap.grid[player.x][player.y] = ' ';

        // Revelar el área alrededor del jugador
        currentMap.revealCross(player.x, player.y);

        cout << "Partida cargada exitosamente." << endl;
        this_thread::sleep_for(chrono::milliseconds(1500));
    }

    void loadRanking() {
        ifstream file("ranking.dat");
        if (file) {
            ranking.clear();
            string name;
            int time, lives, score;

            while (file >> name >> time >> lives >> score) {
                ranking.emplace_back(name, make_pair(time, make_pair(lives, score)));
            }
        }
    }

    void saveRanking() {
        ofstream file("ranking.dat");
        if (file) {
            for (const auto& entry : ranking) {
                file << entry.first << " " << entry.second.first << " "
                    << entry.second.second.first << " " << entry.second.second.second << endl;
            }
        }
    }

    void showMenu() {
        while (true) {
            clearScreen();
            cout << "=== TESORO EN EL LABERINTO ===" << endl;
            cout << "1. Nueva Partida" << endl;
            cout << "2. Cargar Partida" << endl;
            cout << "3. Ver Ranking" << endl;
            cout << "4. Salir" << endl;
            cout << "Selecciona una opcion: ";

            int choice;

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Intenta de nuevo." << endl;
                this_thread::sleep_for(chrono::milliseconds(1000));
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1:
                *this = Game();
                play();
                break;
            case 2:
                loadGame();
                if (gameRunning) {
                    play();
                }
                break;
            case 3:
                displayRanking();
                break;
            case 4:
                return;
            default:
                cout << "Opcion invalida!" << endl;
                this_thread::sleep_for(chrono::milliseconds(1000));
                break;
            }
        }
    }
};

int main() {
    unsigned seed = static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count());
    srand(seed);

    Game game;
    game.showMenu();
    return 0;
}
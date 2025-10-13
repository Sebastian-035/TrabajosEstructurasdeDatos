#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 20;
char mapaVisible[SIZE][SIZE];
char mapaReal[SIZE][SIZE];

int playerX, playerY;
int salidaX, salidaY;
int vidas = 4;
int energia = 4;
int pasosSinChocar = 0;
int tesorosRecolectados = 0;

// Función para limpiar pantalla
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Función para obtener entrada sin esperar Enter
char getch() {
    char c;
    system("stty raw");
    c = getchar();
    system("stty cooked");
    return c;
}

// Función para colocar elementos aleatoriamente en el mapa
void colocarElementos(int nivel) {
    int trampasColocadas = 0;
    int tesorosColocados = 0;
    
    srand(time(0) + nivel); // Semilla diferente para cada nivel
    
    while (trampasColocadas < 8) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        
        // Colocar trampa solo en caminos vacíos, no en posición inicial ni salida
        if (mapaReal[y][x] == ' ' && !(x == playerX && y == playerY) && 
            !(x == salidaX && y == salidaY)) {
            mapaReal[y][x] = 'X';
            trampasColocadas++;
        }
    }
    
    while (tesorosColocados < 5) { // 5 tesoros por nivel
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        
        // Colocar tesoro solo en caminos vacíos
        if (mapaReal[y][x] == ' ' && !(x == playerX && y == playerY) && 
            !(x == salidaX && y == salidaY)) {
            mapaReal[y][x] = 'T';
            tesorosColocados++;
        }
    }
}

void inicializarMapa(int nivel) {
    // Laberintos predefinidos para 3 niveles
    const char* niveles[3][SIZE] = {
        {   // Nivel 1 - Laberinto simple
            "| | | | | | | | | |",
            "|P      |        #|", 
            "| | | | | | | | | |",
            "|       | |      | |",
            "| | | |   | | | | |",
            "|   |     |      | |",
            "| | | | | | | | | |",
            "|     |   | |    | |",
            "| | | | |   | | | |",
            "|   |     |      | |",
            "| | | | | | | | | |",
            "|     |   | |    | |",
            "| | | | |   | | | |",
            "|   |     |      | |",
            "| | | | | | | | | |",
            "|     |   | |    | |",
            "| | | | |   | | | |",
            "|   |     |      | |",
            "| | | | | | | | | |",
            "| | | | | | | | | |"
        },
        {   // Nivel 2 - Laberinto medio
            "| | | | | | | | | |",
            "|P|        | |    |",
            "| | | | |  | | | | |",
            "|   |    |        #|",
            "| | | | | | | | | |",
            "|      |    |     | |",
            "| | | | | | | | | |",
            "| |     |  | |   | |",
            "| | | |    | | | | |",
            "|    | | |        | |",
            "| | | | | | | | | |",
            "|      |    |     | |",
            "| | | | | | | | | |",
            "| |     |  | |   | |",
            "| | | |    | | | | |",
            "|    | | |        | |",
            "| | | | | | | | | |",
            "|      |    |     | |",
            "| | | | | | | | | |",
            "| | | | | | | | | |"
        },
        {   // Nivel 3 - Laberinto complejo
            "| | | | | | | | | |",
            "|P|  |    | |     |",
            "| | | | | | | | |#|",
            "|   |       |    | |",
            "| | | | | | | | | |",
            "|    |  |      |  | |",
            "| | | | | | | | | |",
            "| |    |   | |   | |",
            "| | | | | | | | | |",
            "|     | |      |  | |",
            "| | | | | | | | | |",
            "|    |  |      |  | |",
            "| | | | | | | | | |",
            "| |    |   | |   | |",
            "| | | | | | | | | |",
            "|     | |      |  | |",
            "| | | | | | | | | |",
            "|    |  |      |  | |",
            "| | | | | | | | | |",
            "| | | | | | | | | |"
        }
    };

    // Reiniciar estadísticas para nuevo nivel
    vidas = 4;
    energia = 4;
    pasosSinChocar = 0;
    tesorosRecolectados = 0;

    // Copiar el mapa del nivel seleccionado
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            mapaReal[i][j] = niveles[nivel][i][j];
            if(mapaReal[i][j] == 'P') {
                playerX = j;
                playerY = i;
            }
            if(mapaReal[i][j] == '#') {
                salidaX = j;
                salidaY = i;
            }
        }
    }

    // Colocar trampas y tesoros
    colocarElementos(nivel);

    // Inicializar mapa visible (todo oscuro)
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            mapaVisible[i][j] = '*';
        }
    }
    mapaVisible[playerY][playerX] = 'P';
}

void dibujarMapa() {
    clearScreen();
    cout << "=== LABERINTO DEL TESORO ===" << endl;
    cout << "Nivel completado: " << tesorosRecolectados << "/5 tesoros" << endl;
    cout << "Vidas: " << vidas << " | Energía: " << energia << endl;
    cout << "Controles: W (arriba), S (abajo), A (izquierda), D (derecha), Q (salir)" << endl << endl;
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            cout << mapaVisible[i][j];
        }
        cout << endl;
    }
    
    cout << endl << "Leyenda: P(Jugador) | (Pared) X(Trampa) T(Tesoro) #(Salida)" << endl;
}

bool moverJugador(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    if(newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE) {
        char celdaDestino = mapaReal[newY][newX];
        
        // Verificar si es una pared
        if(celdaDestino == '|') {
            energia--; // Pierde energía al chocar con pared
            pasosSinChocar = 0; // Reinicia contador de pasos sin chocar
            
            if(energia <= 0) {
                vidas--;
                energia = 4; // Recupera energía pero pierde vida
                if(vidas <= 0) {
                    cout << "¡Game Over! Te quedaste sin vidas." << endl;
                    return true; // Termina el juego
                }
            }
            return false; // No se puede mover a una pared
        }
        
        // Revelar el camino en la posición actual
        if(mapaReal[playerY][playerX] != 'P') {
            mapaVisible[playerY][playerX] = mapaReal[playerY][playerX];
        }

        // Mover al jugador
        playerX = newX;
        playerY = newY;
        
        // Verificar trampas
        if(celdaDestino == 'X') {
            vidas--;
            mapaVisible[playerY][playerX] = 'P';
            cout << "¡Trampa! Pierdes una vida. Vidas restantes: " << vidas << endl;
            
            if(vidas <= 0) {
                cout << "¡Game Over! Te quedaste sin vidas." << endl;
                return true;
            }
        }
        // Verificar tesoros
        else if(celdaDestino == 'T') {
            tesorosRecolectados++;
            mapaReal[playerY][playerX] = ' '; // Remover tesoro
            mapaVisible[playerY][playerX] = 'P';
            cout << "¡Tesoro encontrado! (" << tesorosRecolectados << "/5)" << endl;
        }
        else {
            mapaVisible[playerY][playerX] = 'P';
        }

        // Incrementar pasos sin chocar y verificar energía
        pasosSinChocar++;
        if(pasosSinChocar >= 3) {
            if(energia < 4) {
                energia++;
                cout << "¡Energía recuperada! (+1)" << endl;
            }
            pasosSinChocar = 0;
        }

        // Verificar si llegó a la salida
        if(playerX == salidaX && playerY == salidaY) {
            return true;
        }
    }
    return false;
}

int main() {
    int nivelActual = 0;
    bool juegoActivo = true;

    cout << "BIENVENIDO AL LABERINTO DEL TESORO" << endl;
    cout << "Encuentra 5 tesoros y llega a la salida (#) en cada nivel." << endl;
    cout << "¡Cuidado con las trampas (X) y las paredes (|)!" << endl;
    cout << "Presiona cualquier tecla para comenzar..." << endl;
    getch();

    while(juegoActivo && nivelActual < 3) {
        inicializarMapa(nivelActual);
        bool nivelCompletado = false;

        while(!nivelCompletado && vidas > 0) {
            dibujarMapa();
            cout << "\nNivel " << (nivelActual + 1) << " - Encuentra la salida y 5 tesoros!" << endl;

            char tecla = getch();
            bool salidaAlcanzada = false;

            switch(tecla) {
                case 'w': // Arriba
                    salidaAlcanzada = moverJugador(0, -1);
                    break;
                case 's': // Abajo
                    salidaAlcanzada = moverJugador(0, 1);
                    break;
                case 'a': // Izquierda
                    salidaAlcanzada = moverJugador(-1, 0);
                    break;
                case 'd': // Derecha
                    salidaAlcanzada = moverJugador(1, 0);
                    break;
                case 'q': // Salir del juego
                    juegoActivo = false;
                    nivelCompletado = true;
                    cout << "\nJuego terminado." << endl;
                    break;
            }

            if(salidaAlcanzada && tesorosRecolectados >= 5) {
                nivelCompletado = true;
                dibujarMapa();
                cout << "\n¡Felicidades! Completaste el nivel " << (nivelActual + 1) << "!" << endl;
                cout << "Encontraste todos los tesoros y llegaste a la salida." << endl;
                cout << "Presiona cualquier tecla para continuar..." << endl;
                getch();
            }
            else if(salidaAlcanzada && tesorosRecolectados < 5) {
                cout << "¡Falta encontrar " << (5 - tesorosRecolectados) << " tesoros!" << endl;
            }
        }
        
        if(vidas <= 0) {
            juegoActivo = false;
            cout << "¡Game Over! No completaste el laberinto." << endl;
        }
        else {
            nivelActual++;
        }
    }

    if(nivelActual == 3) {
        cout << "\n¡FELICIDADES! Has completado todos los niveles del Laberinto del Tesoro!" << endl;
        cout << "Eres un verdadero maestro buscador de tesoros!" << endl;
    }

    return 0;
}
#include <iostream>
using namespace std;

const int SIZE = 20;
char mapaVisible[SIZE][SIZE];
char mapaReal[SIZE][SIZE];

int playerX, playerY;
int salidaX, salidaY;


char getch() {
    char c;
    system("stty raw");    // Set terminal to raw mode
    c = getchar();
    system("stty cooked"); // Restore terminal to normal mode
    return c;
}


void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen
}

void inicializarMapa(int nivel) {
    // Patrones de mapas para los 3 niveles - corregidos para tamaño exacto
    const char* niveles[3][SIZE] = {
        {   // Nivel 1
            "*P~~~~~~~~~~~~~~~~~",  // 19 caracteres
            "*~***************~~",
            "*~~~~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~***********~~",
            "****~###########~~",
            "*****************"
        },
        {   // Nivel 2
            "********P~~~~~~~~~",
            "*********~********",
            "*********~********",
            "*********~********",
            "*********~********",
            "*********~********",
            "~~~~~~~~#########~",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************",
            "*****************"
        },
        {   // Nivel 3
            "P~~~~~~~~~~~~~~~~~",
            "~~~~~~~~~~~~~~~~~~",
            "~~~**********~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~*~~~~~~~~*~~~~~",
            "~~~##########~~~~~",
            "*****************"
        }
    };

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

    // Inicializar mapa visible
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            mapaVisible[i][j] = '*';
        }
    }
    mapaVisible[playerY][playerX] = 'P';
}

void dibujarMapa() {
    clearScreen();
    cout << "Nivel de Tesoros - Usa WASD para moverte" << endl;
    cout << "Llega a la salida (#) revelando el camino (~)" << endl << endl;
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            cout << mapaVisible[i][j];
        }
        cout << endl;
    }
}

bool moverJugador(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    if(newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE) {
        // Revelar el camino en la posición actual
        if(mapaReal[playerY][playerX] != 'P') {
            mapaVisible[playerY][playerX] = mapaReal[playerY][playerX];
        }

        // Mover al jugador
        playerX = newX;
        playerY = newY;
        mapaVisible[playerY][playerX] = 'P';

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

    while(juegoActivo && nivelActual < 3) {
        inicializarMapa(nivelActual);
        bool nivelCompletado = false;

        while(!nivelCompletado) {
            dibujarMapa();
            cout << "\nNivel " << nivelActual + 1 << " - Encuentra la salida!" << endl;
            cout << "Controles: W (arriba), S (abajo), A (izquierda), D (derecha), Q (salir)" << endl;

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

            if(salidaAlcanzada) {
                nivelCompletado = true;
                dibujarMapa();
                cout << "\n¡Felicidades! Encontraste la salida del nivel " << nivelActual + 1 << "!" << endl;
                cout << "Presiona cualquier tecla para continuar..." << endl;
                getch();
            }
        }
        nivelActual++;
    }

    if(nivelActual == 3) {
        cout << "\n¡Has completado todos los niveles! Eres un maestro buscador de tesoros!" << endl;
    }

    return 0;
}
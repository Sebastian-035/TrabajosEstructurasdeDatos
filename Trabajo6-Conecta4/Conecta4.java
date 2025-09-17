import java.util.Scanner;

public class Conecta4 {
    private static final int FILAS = 6;
    private static final int COLUMNAS = 7;
    private static final char VACIO = ' ';
    private static final char JUGADOR1 = 'X';
    private static final char JUGADOR2 = 'O';
    
    private char[][] tablero;
    private boolean turnoJugador1;
    
    public Conecta4() {
        tablero = new char[FILAS][COLUMNAS];
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS; j++) {
                tablero[i][j] = VACIO;
            }
        }
        turnoJugador1 = true;
    }
    
    public void jugar() {
        Scanner scanner = new Scanner(System.in);
        boolean juegoTerminado = false;
        
        while (!juegoTerminado) {
            mostrarTablero();
            char jugadorActual = turnoJugador1 ? JUGADOR1 : JUGADOR2;
            System.out.println("Turno del Jugador " + (turnoJugador1 ? "1 (X)" : "2 (O)"));
            
            int columna;
            do {
                System.out.print("Elige columna (1-7): ");
                columna = scanner.nextInt() - 1;
            } while (!columnaValida(columna) || !columnaDisponible(columna));
            
            insertarFicha(columna, jugadorActual);
            
            if (hayGanador(jugadorActual)) {
                mostrarTablero();
                System.out.println("¡Jugador " + (turnoJugador1 ? "1" : "2") + " gana!");
                juegoTerminado = true;
            } else if (tableroLleno()) {
                mostrarTablero();
                System.out.println("¡Empate!");
                juegoTerminado = true;
            }
            
            turnoJugador1 = !turnoJugador1;
        }
        scanner.close();
    }
    
    private boolean columnaValida(int columna) {
        return columna >= 0 && columna < COLUMNAS;
    }
    
    private boolean columnaDisponible(int columna) {
        return tablero[0][columna] == VACIO;
    }
    
    private void insertarFicha(int columna, char jugador) {
        for (int i = FILAS - 1; i >= 0; i--) {
            if (tablero[i][columna] == VACIO) {
                tablero[i][columna] = jugador;
                break;
            }
        }
    }
    
    private boolean hayGanador(char jugador) {
        // Verificar horizontal
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS - 3; j++) {
                if (tablero[i][j] == jugador &&
                    tablero[i][j+1] == jugador &&
                    tablero[i][j+2] == jugador &&
                    tablero[i][j+3] == jugador) {
                    return true;
                }
            }
        }
        
        // Verificar vertical
        for (int i = 0; i < FILAS - 3; i++) {
            for (int j = 0; j < COLUMNAS; j++) {
                if (tablero[i][j] == jugador &&
                    tablero[i+1][j] == jugador &&
                    tablero[i+2][j] == jugador &&
                    tablero[i+3][j] == jugador) {
                    return true;
                }
            }
        }
        
        // Verificar diagonal ascendente
        for (int i = 3; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS - 3; j++) {
                if (tablero[i][j] == jugador &&
                    tablero[i-1][j+1] == jugador &&
                    tablero[i-2][j+2] == jugador &&
                    tablero[i-3][j+3] == jugador) {
                    return true;
                }
            }
        }
        
        // Verificar diagonal descendente
        for (int i = 0; i < FILAS - 3; i++) {
            for (int j = 0; j < COLUMNAS - 3; j++) {
                if (tablero[i][j] == jugador &&
                    tablero[i+1][j+1] == jugador &&
                    tablero[i+2][j+2] == jugador &&
                    tablero[i+3][j+3] == jugador) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    private boolean tableroLleno() {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[0][j] == VACIO) {
                return false;
            }
        }
        return true;
    }
    
    private void mostrarTablero() {
        System.out.println("\n  1   2   3   4   5   6   7");
        for (int i = 0; i < FILAS; i++) {
            System.out.print("| ");
            for (int j = 0; j < COLUMNAS; j++) {
                System.out.print(tablero[i][j] + " | ");
            }
            System.out.println();
        }
        System.out.println("-----------------------------");
    }
    
    public static void main(String[] args) {
        Conecta4 juego = new Conecta4();
        juego.jugar();
    }
}
public class Main {
    static final int FILAS = 3;
    static final int COLUMNAS = 4;
    
    public static void main(String[] args) {
        int[][] matriz = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}
        };
        
        // Recorrer por columnas
        System.out.println("Recorrido por columnas:");
        for (int col = 0; col < COLUMNAS; col++) {
            System.out.print("Columna " + col + ": ");
            for (int fila = 0; fila < FILAS; fila++) {
                System.out.print(matriz[fila][col] + " ");
            }
            System.out.println();
        }
    }
}
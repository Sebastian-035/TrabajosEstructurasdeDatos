public class Main {
    public static void main(String[] args) {
        int[] arreglo = {12,23,57,45,34, 67, 89, 34};
        int valorBuscado = 34;
        boolean encontrado = false;

        for (int i = 0; i < arreglo.length; i++) {
            if (arreglo[i] == valorBuscado) {
                System.out.println("Valor encontrado en el índice " + i);
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            System.out.println("Valor no encontrado en el arreglo.");
        }
    }
}
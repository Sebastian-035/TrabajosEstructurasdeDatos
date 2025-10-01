import java.util.Arrays;

public class QuickSort {

    // Función de utilidad para intercambiar dos elementos
    static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    // Esta función toma el último elemento como pivote
    static int partition(int[] arr, int low, int high) {
        // Se elige el último elemento como pivote
        int pivot = arr[high];

        // Índice del elemento más pequeño
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            // Si el elemento actual es menor que el pivote
            if (arr[j] < pivot) {
                i++;
                swap(arr, i, j);
            }
        }
        // Coloca el pivote en su posición correcta
        swap(arr, i + 1, high);
        return (i + 1);
    }

    /*
     * La función principal que implementa QuickSort
     * arr[] --> Array a ordenar,
     * low --> Índice de inicio,
     * high --> Índice final.
     */
    static void quickSort(int[] arr, int low, int high) {
        if (low < high) {
            // pi es el índice de partición
            int pi = partition(arr, low, high);

            // Ordena recursivamente los sub-arrays
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    // Ejemplo de uso
    public static void main(String[] args) {
        int[] arr = {10, 7, 8, 9, 1, 5};
        quickSort(arr, 0, arr.length - 1);
        System.out.println("Array ordenado:");
        System.out.println(Arrays.toString(arr));
    }
}
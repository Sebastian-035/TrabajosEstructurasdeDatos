using System;

public class QuickSort
{
    // Función de utilidad para intercambiar dos elementos
    private static void Swap(int[] arr, int i, int j)
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    /*
     * Esta función toma el último elemento como pivote,
     * coloca el pivote en su posición correcta en el array
     * ordenado, y coloca todos los elementos más pequeños a la
     * izquierda del pivote y todos los elementos mayores a la
     * derecha del pivote.
     */
    private static int Partition(int[] arr, int low, int high)
    {
        // Se elige el último elemento como pivote
        int pivot = arr[high];
        
        // Índice del elemento más pequeño
        int i = (low - 1); 

        for (int j = low; j <= high - 1; j++)
        {
            // Si el elemento actual es menor que el pivote
            if (arr[j] < pivot)
            {
                i++;
                Swap(arr, i, j);
            }
        }
        // Coloca el pivote en su posición correcta
        Swap(arr, i + 1, high);
        return (i + 1);
    }

    /*
     * La función principal que implementa QuickSort()
     * arr[] --> Array a ordenar,
     * low --> Índice de inicio,
     * high --> Índice final.
     */
    public static void Sort(int[] arr, int low, int high)
    {
        if (low < high)
        {
            // pi es el índice de partición, arr[pi] está ahora en el lugar correcto
            int pi = Partition(arr, low, high);

            // Ordena recursivamente los elementos antes y después de la partición
            Sort(arr, low, pi - 1);
            Sort(arr, pi + 1, high);
        }
    }

    // Ejemplo de uso
    public static void Main(string[] args)
    {
        int[] arr = { 10, 7, 8, 9, 1, 5 };
        int n = arr.Length;
        Sort(arr, 0, n - 1);
        Console.WriteLine("Array ordenado:");
        Console.WriteLine(string.Join(" ", arr));
    }
}
using System;

class Program {
    static void OrdenamientoPorInsercion(int[] arr) {
        int n = arr.Length;
        for (int i = 1; i < n; i++) {
            int clave = arr[i];
            int j = i - 1;
            
            while (j >= 0 && arr[j] > clave) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = clave;
        }
    }
    
    static void Main() {
        int[] arreglo = {12, 11, 13, 5, 6};
        OrdenamientoPorInsercion(arreglo);
        
        Console.Write("Arreglo ordenado: ");
        foreach (int num in arreglo) {
            Console.Write(num + " ");
        }
    }
}

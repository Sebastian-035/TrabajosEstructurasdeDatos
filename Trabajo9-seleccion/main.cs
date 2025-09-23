using System;

class OrdenamientoSeleccion {
    
    static void OrdenamientoPorSeleccion(int[] arr) {
        int n = arr.Length;
        
        for (int i = 0; i < n - 1; i++) {
            int indiceMinimo = i;
            
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[indiceMinimo]) {
                    indiceMinimo = j;
                }
            }
            
            if (indiceMinimo != i) {
                int temp = arr[i];
                arr[i] = arr[indiceMinimo];
                arr[indiceMinimo] = temp;
            }
        }
    }
    
    static void Main() {
        int[] arreglo = {64, 25, 12, 22, 11};
        
        Console.Write("Arreglo original: ");
        foreach (int num in arreglo) {
            Console.Write(num + " ");
        }
        Console.WriteLine();
        
        OrdenamientoPorSeleccion(arreglo);
        
        Console.Write("Arreglo ordenado: ");
        foreach (int num in arreglo) {
            Console.Write(num + " ");
        }
        Console.WriteLine();
    }
}
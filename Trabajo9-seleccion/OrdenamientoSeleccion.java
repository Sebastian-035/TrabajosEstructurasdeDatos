public class OrdenamientoSeleccion {
    
    public static void ordenamientoPorSeleccion(int[] arr) {
        int n = arr.length;
        
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
    
    public static void main(String[] args) {
        int[] arreglo = {64, 25, 12, 22, 11};
        
        System.out.print("Arreglo original: ");
        for (int num : arreglo) {
            System.out.print(num + " ");
        }
        System.out.println();
        
        ordenamientoPorSeleccion(arreglo);
        
        System.out.print("Arreglo ordenado: ");
        for (int num : arreglo) {
            System.out.print(num + " ");
        }
        System.out.println();
    }
}
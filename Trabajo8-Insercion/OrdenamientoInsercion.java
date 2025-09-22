public class OrdenamientoInsercion {
    public static void ordenamientoInsercion(int[] arr) {
        int n = arr.length;
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
    
    public static void main(String[] args) {
        int[] arreglo = {12, 11, 13, 5, 6};
        ordenamientoInsercion(arreglo);
        
        System.out.print("Arreglo ordenado: ");
        for (int num : arreglo) {
            System.out.print(num + " ");
        }
    }
}
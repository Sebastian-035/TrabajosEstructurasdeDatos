public class ProgramaPrincipal {
    
    public static void main(String[] args) {
        // Declaración e inicialización de un arreglo de enteros con 6 valores
        int[] valores = {10, 20, 30, 40, 50, 60};
        
        // Modificación del valor en la posición 2 (índice 2) del arreglo
        valores[2] = 290;
        
        // Mostrar todo el contenido del arreglo en consola
        System.out.println("Contenido del arreglo:");
        for (int i = 0; i < valores.length; i++) {  
            // Imprime el índice y el valor almacenado en esa posición
            System.out.println("valores[" + i + "] = " + valores[i]);
        }
        
        // Se calcula la suma entre el primer elemento (índice 0) y el tercer elemento (índice 2)
        int suma = valores[0] + valores[2];
        
        // Se muestra el resultado de la suma en consola
        System.out.println("\nSuma de elementos: " + suma);
    }
}

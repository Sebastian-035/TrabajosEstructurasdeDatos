import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> numeros = new ArrayList<>();
        numeros.add(10);
        numeros.add(20);
        numeros.add(30);
        numeros.add(40);
        numeros.add(50);

        int valorAInsertar = 75;
        int indice = 2; // Insertar en la posición 2 (entre 20 y 30)

        if (indice >= 0 && indice <= numeros.size()) {
            numeros.add(indice, valorAInsertar);
        } else {
            System.out.println("Índice fuera de rango.");
        }

        // Mostrar el resultado
        System.out.print("Lista actualizada: ");
        for (int num : numeros) {
            System.out.print(num + " ");
        }
        System.out.println();
    }
} 

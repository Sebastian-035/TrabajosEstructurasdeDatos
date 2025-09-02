import java.util.Scanner;

class Persona {
    private String nombre;
    private String apellido;
    private String sexo;
    private int edad;

    // Constructor
    public Persona() {
        nombre = "";
        apellido = "";
        sexo = "";
        edad = 0;
    }

    public void pedirDatos() {
        Scanner scanner = new Scanner(System.in);

        System.out.println("\n=== INGRESO DE DATOS ===");
        System.out.print("Nombre: ");
        nombre = scanner.nextLine();

        System.out.print("Apellido: ");
        apellido = scanner.nextLine();

        System.out.print("Sexo (M/F): ");
        sexo = scanner.nextLine();

        System.out.print("Edad: ");
        edad = scanner.nextInt();
        scanner.nextLine(); // Limpiar el buffer del scanner
    }

    public void mostrarDatos() {
        System.out.println("\n=== DATOS DE LA PERSONA ===");
        System.out.println("Nombre: " + nombre);
        System.out.println("Apellido: " + apellido);
        System.out.println("Sexo: " + sexo);
        System.out.println("Edad: " + edad + " años");
        System.out.println("============================");
    }
}

public class Main {
    public static void main(String[] args) {
        Persona[] personas = new Persona[2];
        Scanner scanner = new Scanner(System.in);

        System.out.println("SISTEMA DE REGISTRO DE PERSONAS");
        System.out.println("Ingrese los datos de 2 personas:");

        // Pedir y mostrar datos
        for(int i = 0; i < 2; i++) {
            System.out.println("\nPersona " + (i + 1) + ":");
            personas[i] = new Persona();
            personas[i].pedirDatos();
            personas[i].mostrarDatos();
        }

        System.out.println("\nRegistro completado exitosamente!");
        scanner.close();
    }
}
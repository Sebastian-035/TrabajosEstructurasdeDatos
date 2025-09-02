using System;

public class Persona
{
    private string nombre, apellido, sexo;
    private uint edad;

    // Constructor
    public Persona()
    {
        nombre = "";
        apellido = "";
        sexo = "";
        edad = 0;
    }

    public void PedirDatos()
    {
        Console.WriteLine("\n=== INGRESO DE DATOS ===");
        
        Console.Write("Nombre: ");
        nombre = Console.ReadLine();

        Console.Write("Apellido: ");
        apellido = Console.ReadLine();

        Console.Write("Sexo (M/F): ");
        sexo = Console.ReadLine();

        Console.Write("Edad: ");
        string edadInput = Console.ReadLine();
        uint.TryParse(edadInput, out edad); // Conversión segura
    }

    public void MostrarDatos()
    {
        Console.WriteLine("\n=== DATOS DE LA PERSONA ===");
        Console.WriteLine("Nombre: " + nombre);
        Console.WriteLine("Apellido: " + apellido);
        Console.WriteLine("Sexo: " + sexo);
        Console.WriteLine("Edad: " + edad + " años");
        Console.WriteLine("============================");
    }
}

public class Program
{
    public static void Main(string[] args)
    {
        Persona[] personas = new Persona[2];

        Console.WriteLine("SISTEMA DE REGISTRO DE PERSONAS");
        Console.WriteLine("Ingrese los datos de 2 personas:");

        // Inicializar el array de personas
        for (int i = 0; i < personas.Length; i++)
        {
            personas[i] = new Persona();
        }

        // Pedir y mostrar datos
        for (int i = 0; i < personas.Length; i++)
        {
            Console.WriteLine("\nPersona " + (i + 1) + ":");
            personas[i].PedirDatos();
            personas[i].MostrarDatos();
        }

        Console.WriteLine("\nRegistro completado exitosamente!");
    }
}
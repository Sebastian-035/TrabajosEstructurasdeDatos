using System;

class Program
{
    static void Main()
    {
        // Declaración
        int[] numeros = new int[5];

        // Inicialización
        numeros[0] = 10;
        numeros[1] = 20;
        numeros[2] = 30;
        numeros[3] = 40;
        numeros[4] = 50;

        // Asignación
        numeros[2] = 100;

        // Prueba en pantalla
        Console.WriteLine("Contenido del arreglo:");
        for (int i = 0; i < numeros.Length; i++)
        {
            Console.WriteLine($"numeros[{i}] = {numeros[i]}");
        }

        // Suma de dos elementos
        int suma = numeros[0] + numeros[2];
        Console.WriteLine($"\nSuma de numeros[0] + numeros[2] = {suma}");
    }
}

using System;

class Program
{
    static void Main()
    {
        int[] arreglo = { 10, 20, 30, 40, 50 };
        int[] nuevoArreglo = new int[arreglo.Length + 1];
        int valor = 25;
        int posicion = 2;

        Console.WriteLine("Recorrido del arreglo:");
        for (int i = 0; i < arreglo.Length; i++)
        {
            Console.WriteLine($"Posición {i}: {arreglo[i]}");
        }
        // Copiar elementos hasta la posición deseada
        for (int i = 0; i < posicion; i++)
        {
            nuevoArreglo[i] = arreglo[i];
        }

        // Insertar nuevo valor
        nuevoArreglo[posicion] = valor;

        // Copiar el resto de elementos
        for (int i = posicion; i < arreglo.Length; i++)
        {
            nuevoArreglo[i + 1] = arreglo[i];
        }

        Console.WriteLine("Arreglo después de la inserción:");
        for (int i = 0; i < nuevoArreglo.Length; i++)
        {
            Console.WriteLine($"Posición {i}: {nuevoArreglo[i]}");
        }
    }
}
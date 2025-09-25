using System;
//Manuel Sebastian Castellon Gonzalez
class Program
{
    static void Main()
    {
        // Crear un array para los valores
        int[] numeros = { 10, 20, 30, 40, 50 };
        
        Console.WriteLine("En orden normal: ");
        MostrarFila(numeros);
        
        Console.WriteLine("En orden inverso: ");
        MostrarFilaInverso(numeros);
    }
    
    static void MostrarFila(int[] array)
    {
        for (int i = 0; i < array.Length; i++)
        {
            Console.Write(array[i]);
            if (i < array.Length - 1) // Para no poner coma al final
                Console.Write(", ");
        }
        Console.WriteLine(); // Salto de línea al final
    }
    
    static void MostrarFilaInverso(int[] array)
    {
        for (int i = array.Length - 1; i >= 0; i--)
        {
            Console.Write(array[i]);
            if (i > 0) // Para no poner coma al final
                Console.Write(", ");
        }
        Console.WriteLine(); // Salto de línea al final
    }
}
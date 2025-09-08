using System;

class Program
{
    const int FILAS = 3;
    const int COLUMNAS = 4;

    static void Main()
    {
        int[,] matriz = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}
        };
        
        // Recorrer por renglones (filas)
        Console.WriteLine("Recorrido por renglones:");
        for (int fila = 0; fila < FILAS; fila++)
        {
            Console.Write($"Renglón {fila}: ");
            for (int col = 0; col < COLUMNAS; col++)
            {
                Console.Write($"{matriz[fila, col]} ");
            }
            Console.WriteLine();
        }
    }
}
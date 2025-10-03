using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        int capas = 1;
        int filas = 10;
        int columnas =1;


        // Crear y llenar la matriz 
        int[,,] matriz = new int[capas, filas, columnas];
        LlenarMatrizConValores(matriz, capas, filas, columnas);

        Console.WriteLine("Matriz  de " + capas + "x" + filas + "x" + columnas + " creada\n");

        // Mostrar matriz original
        Console.WriteLine("Matriz original:");
        MostrarMatriz(matriz, capas, filas, columnas);

        // ELiminar duplicads
        Console.WriteLine("\nA1D Eliminar duplicados ");
        EliminarDuplicados(matriz, capas, filas, columnas);

    }

    static void LlenarMatrizConValores(int[,,] matriz, int capas, int filas, int columnas)
    {
        // Valores 
        int[] valoresBase = { 1, 2, 4, 4, 5, 6, 7, 2, 9, 10 };

        int index = 0;
        for (int c = 0; c < capas; c++)
        {
            for (int f = 0; f < filas; f++)
            {
                for (int col = 0; col < columnas; col++)
                {
                    // Si se acaban los valores, volver a empezar desde el principio
                    matriz[c, f, col] = valoresBase[index % valoresBase.Length];
                    index++;
                }
            }
        }
    }

    static void MostrarMatriz(int[,,] matriz, int capas, int filas, int columnas)
    {
        for (int c = 0; c < capas; c++)
        {
            Console.WriteLine($"Capa {c}:");
            for (int f = 0; f < filas; f++)
            {
                for (int col = 0; col < columnas; col++)
                {
                    Console.Write(matriz[c, f, col] + " ");
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }
    }

    static void EliminarDuplicados(int[,,] matriz, int capas, int filas, int columnas)
    {
        HashSet<int> valoresUnicos = new HashSet<int>();
        List<int> valoresSinDuplicados = new List<int>();

        for (int c = 0; c < capas; c++)
            for (int f = 0; f < filas; f++)
                for (int col = 0; col < columnas; col++)
                    if (valoresUnicos.Add(matriz[c, f, col]))
                        valoresSinDuplicados.Add(matriz[c, f, col]);

        Console.WriteLine("Valores únicos en la matriz: " + string.Join(", ", valoresSinDuplicados));
        Console.WriteLine("Total de valores únicos: " + valoresSinDuplicados.Count);
    }

    
}
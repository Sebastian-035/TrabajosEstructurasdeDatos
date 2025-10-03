using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        int capas = 3;
        int filas = 3;
        int columnas = 3;


        // Crear y llenar la matriz 3D
        int[,,] matriz = new int[capas, filas, columnas];
        LlenarMatrizConValores(matriz, capas, filas, columnas);

        Console.WriteLine("Matriz 3D de " + capas + "x" + filas + "x" + columnas + " creada\n");

        // Mostrar matriz original
        Console.WriteLine("Matriz original:");
        MostrarMatriz(matriz, capas, filas, columnas);

        // A.3D PROMEDIO POR CAPA
        Console.WriteLine("\nA3D Promedio por capa ");
        PromedioPorCapa(matriz, capas, filas, columnas);
    }

    static void LlenarMatrizConValores(int[,,] matriz, int capas, int filas, int columnas)
    {
        // Valores 
        int[] valoresBase = { 1, 2, 3, 4, 5, 4, 7, 8, 9, 2, 3, 1, 5, 6, 5, 8, 9, 7, 3, 1, 2, 6, 4, 6, 9, 7, 81 };

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

    static void PromedioPorCapa(int[,,] matriz, int capas, int filas, int columnas)
    {
        for (int c = 0; c < capas; c++)
        {
            int sumaCapa = 0;
            int totalElementos = filas * columnas;

            for (int f = 0; f < filas; f++)
                for (int col = 0; col < columnas; col++)
                    sumaCapa += matriz[c, f, col];

            double promedio = (double)sumaCapa / totalElementos;
            Console.WriteLine($"Capa {c}: Suma = {sumaCapa}, Elementos = {totalElementos}, Promedio = {promedio:F2}");
        }
    }
}
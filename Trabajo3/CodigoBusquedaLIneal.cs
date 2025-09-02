using System;

class Program
{
    static void Main()
    {
        int[] arreglo = { 10, 20, 30, 40, 50 };
        int elementoBuscado = 30;
        bool encontrado = false;
        int posicion = -1;

        Console.WriteLine("Recorrido del arreglo:");
        for (int i = 0; i < arreglo.Length; i++)
        {
            Console.WriteLine($"Posición {i}: {arreglo[i]}");
        }

        // Búsqueda lineal
        for (int i = 0; i < arreglo.Length; i++)
        {
            if (arreglo[i] == elementoBuscado)
            {
                encontrado = true;
                posicion = i;
                break;
            }
        }

        // Mostrar resultado
        if (encontrado)
        {
            Console.WriteLine($"Elemento {elementoBuscado} encontrado en la posición {posicion}");
        }
        else
        {
            Console.WriteLine($"Elemento {elementoBuscado} no encontrado en el arreglo");
        }
    }
}
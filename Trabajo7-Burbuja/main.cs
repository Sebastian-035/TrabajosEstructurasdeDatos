using System;

class BubbleSort
{
    static void BubbleSortArray(int[] arr)
    {
        int n = arr.Length;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    
    static void Main()
    {
        Console.Write("Ingrese la cantidad de números a ordenar: ");
        int n = Convert.ToInt32(Console.ReadLine());
        
        int[] arr = new int[n];
        
        Console.WriteLine($"Ingrese {n} números:");
        for (int i = 0; i < n; i++)
        {
            arr[i] = Convert.ToInt32(Console.ReadLine());
        }
        
        BubbleSortArray(arr);
        
        Console.Write("Arreglo ordenado: ");
        foreach (int num in arr)
        {
            Console.Write(num + " ");
        }
    }
}
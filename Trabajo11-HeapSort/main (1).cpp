#include <iostream>
#include <vector>
using namespace std;

// Función para hacer heapify en un subárbol con raíz en el índice i
void heapify(int arr[], int n, int i) {
    int largest = i; // Inicializar el más grande como raíz
    int left = 2 * i + 1; // hijo izquierdo
    int right = 2 * i + 2; // hijo derecho

    // Si el hijo izquierdo es más grande que la raíz
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Si el hijo derecho es más grande que el más grande hasta ahora
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Si el más grande no es la raíz
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursivamente heapify el subárbol afectado
        heapify(arr, n, largest);
    }
}

// Función principal para hacer heap sort
void heapSort(int arr[], int n) {
    // Construir el heap (reorganizar el array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extraer elementos uno por uno del heap
    for (int i = n - 1; i > 0; i--) {
        // Mover la raíz actual al final
        swap(arr[0], arr[i]);

        // Llamar heapify en el heap reducido
        heapify(arr, i, 0);
    }
}


// Función para imprimir el array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << "\n";
}

// Ejemplo de uso
int main() {
    // Usando array tradicional
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    cout << "Array original: ";
    printArray(arr, n);
    
    heapSort(arr, n);
    
    cout << "Array ordenado: ";
    printArray(arr, n);
    
    
    return 0;
}
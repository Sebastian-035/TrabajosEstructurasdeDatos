#include <iostream>
using namespace std;

void ordenamientoInsercion(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int clave = arr[i];
        int j = i - 1;
        
        // Mover los elementos mayores que la clave una posición adelante
        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

int main() {
    int arreglo[] = {12, 11, 13, 5, 6};
    int n = sizeof(arreglo) / sizeof(arreglo[0]);
    
    ordenamientoInsercion(arreglo, n);
    
    cout << "Arreglo ordenado: ";
    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }
    
    return 0;
}
#include <iostream>
using namespace std;

void ordenamientoPorSeleccion(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        // Encontrar el índice del elemento mínimo en el segmento no ordenado
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        
        // Intercambiar el elemento mínimo con el primer elemento del segmento
        if (indiceMinimo != i) {
            int temp = arr[i];
            arr[i] = arr[indiceMinimo];
            arr[indiceMinimo] = temp;
        }
    }
}

int main() {
    int arreglo[] = {64, 25, 12, 22, 11};
    int n = sizeof(arreglo) / sizeof(arreglo[0]);
    
    cout << "Arreglo original: ";
    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }
    cout << endl;
    
    ordenamientoPorSeleccion(arreglo, n);
    
    cout << "Arreglo ordenado: ";
    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }
    cout << endl;
    
    return 0;
}
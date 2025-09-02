#include <iostream>
using namespace std;

int main() {
    int arreglo[] = {12, 45, 23, 67, 89, 34};
    int tamaño = sizeof(arreglo) / sizeof(arreglo[0]);
    int valorBuscado = 67;
    bool encontrado = false;

    for (int i = 0; i < tamaño; i++) {
        if (arreglo[i] == valorBuscado) {
            cout << "Valor encontrado en el índice " << i << endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Valor no encontrado en el arreglo." << endl;
    }

    return 0;
}

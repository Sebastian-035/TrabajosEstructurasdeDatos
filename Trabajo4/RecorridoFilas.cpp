#include <iostream>
using namespace std;

const int FILAS = 3;
const int COLUMNAS = 4;

int main() {
    int matriz[FILAS][COLUMNAS] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    // Recorrer por renglones (filas)
    cout << "Recorrido por renglones:" << endl;
    for (int fila = 0; fila < FILAS; fila++) {
        cout << "Renglón " << fila << ": ";
        for (int col = 0; col < COLUMNAS; col++) {
            cout << matriz[fila][col] << " ";
        }
        cout << endl;
    }
    
    return 0;
}
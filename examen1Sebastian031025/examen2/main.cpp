#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <limits>
using namespace std;

#define DIM 1 // Cambia entre 1, 2 o 3

// Variables globales
int filas = 10, columnas = 0, capas = 0;
int totalElementos = 0;
int* arr = nullptr;

void creador(int modo, int minVal, int maxVal) {

    // Modo = 1 → aleatorio
    // Modo = 2 → manual
    // minVal, maxVal solo se usan si modo = 1

    srand(time(0)); // Inicializa semilla aleatoria

    if (DIM == 1) totalElementos = filas;
    else if (DIM == 2) totalElementos = filas * columnas;
    else if (DIM == 3) totalElementos = filas * columnas * capas;
    else {
        cout << "Definir DIM en 1, 2 o 3." << endl;
        totalElementos = 0;
        arr = nullptr;
        return;
    }

    arr = new int[totalElementos];

    for (int i = 0; i < totalElementos; i++) {
        if (modo == 1) arr[i] = minVal + rand() % (maxVal - minVal + 1);
        else cin >> arr[i];
    }
}

void impresor() {
    if (!arr || totalElementos == 0) return;

    if (DIM == 1) {
        for (int i = 0; i < totalElementos; i++) cout << arr[i] << " ";
        cout << endl;
    }
    else if (DIM == 2) {
        int idx = 0;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) cout << arr[idx++] << " ";
            cout << endl;
        }
    }
    else if (DIM == 3) {
        int idx = 0;
        for (int k = 0; k < capas; k++) {
            cout << "CAPA " << k + 1 << ":" << endl;
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) cout << arr[idx++] << " ";
                cout << endl;
            }
            cout << endl;
        }
    }
}

void aplanadora() {
    if (!arr || totalElementos == 0) return;

    // Si el arreglo ya está en 1D (DIM==1), no hace nada
    if (DIM == 1) return;

    // Creamos un array temporal para almacenar los elementos lineales
    int* plano = new int[totalElementos];
    int idx = 0;

    if (DIM == 2) {
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                plano[idx++] = arr[i * columnas + j]; // convertimos a lineal
    }
    else if (DIM == 3) {
        for (int k = 0; k < capas; k++)
            for (int i = 0; i < filas; i++)
                for (int j = 0; j < columnas; j++)
                    plano[idx++] = arr[k * filas * columnas + i * columnas + j];
    }

    // Reemplazamos el arreglo global por el plano lineal
    delete[] arr;
    arr = plano;
}

void bubbleSorting(int orden, int criterio, int alternancia) {
    // orden = 1 → ascendente, 2 → descendente
    // criterio = 1 → filas, 2 → columnas, 3 → subarreglos (capas 3D)
    // alternancia = 0 → no alterna, 1..10 → alterna cada N elementos
    if (!arr || totalElementos == 0) return;

    // Creamos un array temporal para manejar columnas si es necesario
    int* temp = new int[totalElementos];
    for (int i = 0; i < totalElementos; i++) temp[i] = arr[i];

    // Determinamos la orientación del orden
    // Para filas y subarreglos: orden normal en 1D
    // Para columnas: necesitamos simular lectura columna por columna
    if (criterio == 2 && DIM >= 2) {
        int idx = 0;
        for (int j = 0; j < columnas; j++) {
            for (int i = 0; i < filas; i++) {
                for (int k = 0; k < ((DIM==3)?capas:1); k++) {
                    int originalIdx = k*filas*columnas + i*columnas + j;
                    temp[idx++] = arr[originalIdx];
                }
            }
        }
    }

    // Bubble sort completo sobre temp
    for (int i = 0; i < totalElementos - 1; i++) {
        for (int j = 0; j < totalElementos - 1 - i; j++) {
            int actualOrden = orden;
            if (alternancia > 0) {
                int bloqueAlt = j / alternancia;
                if (bloqueAlt % 2 != 0) actualOrden = (orden == 1) ? 2 : 1;
            }
            if ((actualOrden == 1 && temp[j] > temp[j + 1]) ||
                (actualOrden == 2 && temp[j] < temp[j + 1])) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    // Reasignar a arr según criterio
    if (criterio == 1 || criterio == 3 || DIM == 1) {
        for (int i = 0; i < totalElementos; i++) arr[i] = temp[i];
    } else if (criterio == 2 && DIM >= 2) {
        int idx = 0;
        for (int j = 0; j < columnas; j++) {
            for (int i = 0; i < filas; i++) {
                for (int k = 0; k < ((DIM==3)?capas:1); k++) {
                    int originalIdx = k*filas*columnas + i*columnas + j;
                    arr[originalIdx] = temp[idx++];
                }
            }
        }
    }

    delete[] temp;
}

void estadistica(int criterio) {
    // criterio = 0 → todo el array
    // criterio = 1 → filas (subarreglos 1D)
    // criterio = 2 → columnas (subarreglos "verticales")
    // criterio = 3 → capas (subarreglos 2D)
    if (!arr || totalElementos == 0) return;

    std::vector<std::vector<int>> bloques;

    if (criterio == 0 || DIM == 1) {
        bloques.push_back(std::vector<int>(arr, arr + totalElementos));
    } else if (criterio == 1) { // filas
        for (int k = 0; k < ((DIM==3)?capas:1); k++) {
            for (int i = 0; i < filas; i++) {
                std::vector<int> fila;
                for (int j = 0; j < columnas; j++) {
                    int idx = k*filas*columnas + i*columnas + j;
                    fila.push_back(arr[idx]);
                }
                bloques.push_back(fila);
            }
        }
    } else if (criterio == 2) { // columnas
        for (int k = 0; k < ((DIM==3)?capas:1); k++) {
            for (int j = 0; j < columnas; j++) {
                std::vector<int> col;
                for (int i = 0; i < filas; i++) {
                    int idx = k*filas*columnas + i*columnas + j;
                    col.push_back(arr[idx]);
                }
                bloques.push_back(col);
            }
        }
    } else if (criterio == 3 && DIM==3) { // capas completas
        for (int k = 0; k < capas; k++) {
            std::vector<int> capa;
            for (int i = 0; i < filas*columnas; i++) {
                int idx = k*filas*columnas + i;
                capa.push_back(arr[idx]);
            }
            bloques.push_back(capa);
        }
    }

    for (int b = 0; b < bloques.size(); b++) {
        auto& sub = bloques[b];
        int suma = 0;
        for (int val : sub) suma += val;
        double promedio = (double)suma / sub.size();

        std::vector<int> sorted = sub;
        std::sort(sorted.begin(), sorted.end());

        // mediana
        double mediana;
        int n = sorted.size();
        if (n % 2 == 0) mediana = (sorted[n/2 -1] + sorted[n/2]) / 2.0;
        else mediana = sorted[n/2];

        // moda
        std::unordered_map<int,int> freq;
        for (int val : sub) freq[val]++;
        int moda = sub[0]; int maxF = 0;
        for (auto& p : freq) {
            if (p.second > maxF) { moda = p.first; maxF = p.second; }
        }

        std::cout << "Bloque " << b+1 << ": Suma=" << suma
                  << ", Promedio=" << promedio
                  << ", Mediana=" << mediana
                  << ", Moda=" << moda << "\n";
    }
}

void buscarDuplicados(int modo) {
    // modo = 1 → solo reportar duplicados
    // modo = 2 → eliminar segunda ocurrencia de cada número duplicado (mantener la primera)
    // modo = 3 → eliminar todas las ocurrencias de los duplicados
    if (!arr || totalElementos == 0) return;

    std::unordered_map<int, int> freq;

    // Contamos frecuencia de cada número
    for (int i = 0; i < totalElementos; i++) {
        freq[arr[i]]++;
    }

    if (modo == 1) {
        std::cout << "Duplicados encontrados:\n";
        for (auto& p : freq) {
            if (p.second > 1) {
                std::cout << "Número " << p.first << " aparece " << p.second << " veces\n";
            }
        }
    }
    else if (modo == 2) {
    std::unordered_map<int, bool> seen; // true si ya dejamos la primera aparición
    for (int i = 0; i < totalElementos; i++) {
        if (freq[arr[i]] > 1) { // es un número duplicado
            if (seen[arr[i]]) {
                // eliminar elemento duplicado
                for (int j = i; j < totalElementos - 1; j++) arr[j] = arr[j + 1];
                arr[totalElementos - 1] = 0; // poner cero al final
                totalElementos--;
                i--; // revisar el nuevo elemento que quedó en la misma posición
            } else {
                seen[arr[i]] = true; // mantenemos la primera aparición
                }
            }
        }
    }
    else if (modo == 3) {
        for (auto& p : freq) {
            if (p.second > 1) {
                int toRemove = p.second;
                for (int i = 0; i < totalElementos && toRemove > 0; ) {
                    if (arr[i] == p.first) {
                        // Corrimiento hacia la izquierda
                        for (int j = i; j < totalElementos - 1; j++) arr[j] = arr[j+1];
                        arr[totalElementos - 1] = 0; // poner cero al final
                        totalElementos--;
                        toRemove--;
                    } else i++;
                }
            }
        }
    }
}


int main(){
    creador(1, 10, 99);
    impresor(); cout << endl;
    buscarDuplicados(2);
    impresor();
}

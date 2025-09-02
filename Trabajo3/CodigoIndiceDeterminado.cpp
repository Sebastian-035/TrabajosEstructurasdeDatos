#include <iostream>
#include <vector>

int main() {
    std::vector<int> numeros = {10, 20, 30, 40, 50};
    int valorAInsertar = 25;
    int indice = 2; // Insertar en la posición 2 (entre 20 y 30)

    if (indice >= 0 && indice <= numeros.size()) {
        numeros.insert(numeros.begin() + indice, valorAInsertar);
    } else {
        std::cout << "Índice fuera de rango." << std::endl;
    }

    // Mostrar el resultado
    std::cout << "Vector actualizado: ";
    for (int num : numeros) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
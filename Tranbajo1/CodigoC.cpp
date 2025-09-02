#include <iostream>
int main() {
    //  Declaración
    int numeros[5];

    // Inicialización
    numeros[0] = 10;
    numeros[1] = 20;
    numeros[2] = 30;
    numeros[3] = 40;
    numeros[4] = 50;

    //  Asignación 
    numeros[2] = 100;  

    //   prueba en pantalla
    std::cout << "Contenido del arreglo:\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "numeros[" << i << "] = " << numeros[i] << std::endl;
    }

    //  suma de dos elementos
    int suma = numeros[0] + numeros[2];  
    std::cout << "\nSuma de numeros[0] + numeros[2] = " << suma << std::endl;

    return 0;
}
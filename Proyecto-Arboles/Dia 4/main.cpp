#include "arbol.h"
#include "consola.h"
#include "gestor_json.h"
#include <iostream>

int main() {
    std::cout << "=== PROYECTO 1: SISTEMA DE ARCHIVOS JERARQUICO ===\n";
    std::cout << "Implementacion: Dias 1-3 del cronograma\n";
    std::cout << "===================================================\n";
    
    // Crear el árbol
    Arbol arbol;
    
    // Cargar datos existentes si existen
    GestorJSON::cargarArbol(arbol, "datos.json");
    
    // Iniciar la interfaz de consola
    Consola consola(arbol);
    consola.ejecutar();
    
    // Guardar antes de salir
    GestorJSON::guardarArbol(arbol, "datos.json");
    
    std::cout << "Programa terminado.\n";
    return 0;
}
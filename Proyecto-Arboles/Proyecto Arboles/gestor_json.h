#ifndef GESTOR_JSON_H
#define GESTOR_JSON_H

#include "nodo.h"
#include "arbol.h"
#include <string>

class GestorJSON {
public:
    static bool guardarArbol(const Arbol& arbol, const std::string& nombreArchivo);
    static bool cargarArbol(Arbol& arbol, const std::string& nombreArchivo);
    
private:
    static void guardarNodoRecursivo(std::ofstream& archivo, Nodo* nodo, int nivel);
    static void cargarNodoRecursivo(Arbol& arbol, std::ifstream& archivo, Nodo* padre, int& proximoId);
};

#endif
#ifndef GESTOR_JSON_H
#define GESTOR_JSON_H

#include "nodo.h"
#include "arbol.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

class GestorJSON {
public:
    static bool guardarArbol(const Arbol& arbol, const std::string& nombreArchivo);
    static bool cargarArbol(Arbol& arbol, const std::string& nombreArchivo);
    
private:
    static std::string escapeJson(const std::string& str);
    static std::string unescapeJson(const std::string& str);
    static int parsearEntero(std::ifstream& archivo, const std::string& clave, const std::string& linea);
    static std::string parsearString(std::ifstream& archivo, const std::string& clave, const std::string& linea);
    static TipoNodo parsearTipoNodo(std::ifstream& archivo, const std::string& clave, const std::string& linea);
};

#endif
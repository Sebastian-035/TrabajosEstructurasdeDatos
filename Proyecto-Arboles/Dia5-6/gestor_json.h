#ifndef GESTOR_JSON_H
#define GESTOR_JSON_H

#include "nodo.h"
#include "arbol.h"
#include <string>
#include <map>

class GestorJSON {
public:
    static bool guardarArbol(const Arbol& arbol, const std::string& nombreArchivo);
    static bool cargarArbol(Arbol& arbol, const std::string& nombreArchivo);
    
private:
    static std::string escapeJson(const std::string& str);
    static std::string unescapeJson(const std::string& str);
};

#endif
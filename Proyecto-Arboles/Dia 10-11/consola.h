#ifndef CONSOLA_H
#define CONSOLA_H

#include "arbol_gestor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cctype>

class Consola {
private:
    Arbol& arbol;
    
    int obtenerId(const std::string& arg);
    void mostrarInfoNodo(Nodo* nodo) const;
    void mostrarAyuda() const;
    
    void procesarComando(const std::string& comando);
    
    void procesarMkdir(std::stringstream& ss);
    void procesarTouch(std::stringstream& ss);
    void procesarLs(std::stringstream& ss);
    void procesarCat(std::stringstream& ss);
    void procesarRm(std::stringstream& ss);
    void procesarMv(std::stringstream& ss);
    void procesarRename(std::stringstream& ss);
    void procesarPath(std::stringstream& ss);
    void procesarPreorden();
    void procesarFind(std::stringstream& ss);
    void procesarSearch(std::stringstream& ss);
    void procesarSuggest(std::stringstream& ss);
    void procesarSave(std::stringstream& ss);
    void procesarLoad(std::stringstream& ss);
    void procesarInfo();
    void procesarClear();
    void procesarTrash();
    void procesarRestore(std::stringstream& ss);
    void procesarClearTrash();

public:
    Consola(Arbol& arbol);
    void ejecutar();
};

#endif // CONSOLA_H
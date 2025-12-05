#ifndef CONSOLA_H
#define CONSOLA_H

#include "arbol.h"
#include <string>

class Consola {
private:
    Arbol& arbol;
    
    void mostrarMenu() const;
    void procesarComando(const std::string& comando);
    void mostrarAyuda() const;
    
public:
    Consola(Arbol& arbol);
    void ejecutar();
};

#endif
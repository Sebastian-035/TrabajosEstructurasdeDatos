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
    void mostrarResultadosBusqueda(const std::vector<Nodo*>& resultados) const;
    void mostrarAutocompletado(const std::vector<std::string>& sugerencias) const;

public:
    Consola(Arbol& arbol);
    void ejecutar();
};

#endif
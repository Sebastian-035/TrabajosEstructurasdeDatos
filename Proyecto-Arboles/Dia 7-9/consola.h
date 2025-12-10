#ifndef CONSOLA_H
#define CONSOLA_H

#include "arbol.h"
#include <string>
#include <sstream>

class Consola {
private:
    Arbol& arbol;

    void mostrarMenu() const;
    void procesarComando(const std::string& comando);
    void mostrarAyuda() const;

    // Métodos para procesar cada comando
    void procesarMkdir(std::stringstream& ss);
    void procesarTouch(std::stringstream& ss);
    void procesarLs(std::stringstream& ss);
    void procesarCat(std::stringstream& ss); // Nuevo: mostrar contenido
    void procesarRm(std::stringstream& ss);
    void procesarMv(std::stringstream& ss);
    void procesarRename(std::stringstream& ss);
    void procesarPath(std::stringstream& ss);
    void procesarPreorden();
    void procesarFind(std::stringstream& ss);
    void procesarSearch(std::stringstream& ss);
    void procesarSuggest(std::stringstream& ss);
    void procesarExists(std::stringstream& ss);
    void procesarSave(std::stringstream& ss);
    void procesarLoad(std::stringstream& ss);
    void procesarInfo();
    void procesarClear();

    //METODOS PARA PAPELERA
    void procesarTrash(); // listar papelera
    void procesarRestore(std::stringstream& ss);
    void procesarClearTrash(); // vaciar papelera

    // Métodos auxiliares
    int obtenerId(const std::string& arg);
    void mostrarInfoNodo(Nodo* nodo) const;

public:
    Consola(Arbol& arbol);
    void ejecutar();
};

#endif
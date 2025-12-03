#ifndef ARBOL_H
#define ARBOL_H

#include "nodo.h"
#include <vector>

class Arbol {
private:
    Nodo* raiz;
    int proximoId;
    
    // Métodos auxiliares privados
    Nodo* buscarNodoRecursivo(Nodo* nodo, int id) const;
    void eliminarRecursivo(Nodo* nodo);
    void recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const;
    
public:
    Arbol();
    ~Arbol();
    
    // Operaciones básicas
    Nodo* crearNodo(const std::string& nombre, TipoNodo tipo, const std::string& contenido = "", Nodo* padre = nullptr);
    bool eliminarNodo(int id);
    bool moverNodo(int idOrigen, int idDestino);
    bool renombrarNodo(int id, const std::string& nuevoNombre);
    
    // Búsquedas
    Nodo* buscarNodo(int id) const;
    Nodo* buscarNodoPorRuta(const std::string& ruta) const;
    
    // Recorridos
    std::vector<Nodo*> recorridoPreorden() const;
    
    // Métodos de información
    Nodo* getRaiz() const;
    int getAltura() const;
    int getTamano() const;
    int getProximoId() const;
    
    // Métodos auxiliares
    void listarHijos(int idPadre) const;
};

#endif
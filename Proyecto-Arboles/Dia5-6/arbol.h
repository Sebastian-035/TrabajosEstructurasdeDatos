#ifndef ARBOL_H
#define ARBOL_H

#include "nodo.h"
#include "trie.h"
#include <vector>
#include <unordered_map>

class Arbol {
private:
    Nodo* raiz;
    int proximoId;
    Trie trieNombres; // Trie para búsqueda y autocompletado
    std::unordered_map<int, Nodo*> mapaNodos; // Mapa hash para búsqueda rápida por ID

    // Métodos auxiliares privados
    Nodo* buscarNodoRecursivo(Nodo* nodo, int id) const;
    void eliminarRecursivo(Nodo* nodo);
    void recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const;
    void actualizarTrie(Nodo* nodo, bool agregar = true);
    void actualizarMapa(Nodo* nodo, bool agregar = true);

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

    // Búsqueda por nombre (usando Trie y mapa hash)
    std::vector<Nodo*> buscarPorNombreExacto(const std::string& nombre) const;
    std::vector<std::pair<std::string, std::vector<int>>> buscarPorPrefijo(const std::string& prefijo) const;
    std::vector<std::string> autocompletarNombres(const std::string& prefijo, int limite = 5) const;

    // Recorridos
    std::vector<Nodo*> recorridoPreorden() const;

    // Métodos de información
    Nodo* getRaiz() const;
    int getAltura() const;
    int getTamano() const;
    int getProximoId() const;

    // Métodos auxiliares
    void listarHijos(int idPadre) const;

    // Nuevo método para reconstruir el árbol (para carga de JSON)
    void reconstruirArbol(Nodo* nuevaRaiz, int nuevoProximoId);

    // Método para actualizar índices después de carga
    void reconstruirIndices();
};

#endif
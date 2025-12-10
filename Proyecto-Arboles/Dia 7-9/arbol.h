#ifndef ARBOL_H
#define ARBOL_H

#include "nodo.h"
#include "trie.h"
#include "error_handler.h"
#include <vector>
#include <unordered_map>
#include <queue> // Necesario para la restauración

class Arbol {
private:
    Nodo* raiz;
    int proximoId;
    Trie trieNombres;
    std::unordered_map<int, Nodo*> mapaNodos;
    
    // ESTRUCTURA PARA LA PAPELERA
    std::unordered_map<int, Nodo*> papelera; 

    // Métodos auxiliares privados
    Nodo* buscarNodoRecursivo(Nodo* nodo, int id) const;
    void eliminarRecursivo(Nodo* nodo); // Para el destructor
    void eliminarFisico(Nodo* nodo); // Para vaciar papelera (actualiza mapa/trie)
    void recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const;
    void actualizarTrie(Nodo* nodo, bool agregar = true);
    void actualizarMapa(Nodo* nodo, bool agregar = true);
    
    // Método privado para la carga desde JSON
    void addNodoToPapelera(Nodo* nodo);

    // Validaciones
    void validarCreacionNodo(Nodo* padre, const std::string& nombre) const;
    void validarMovimientoNodo(Nodo* origen, Nodo* destino) const;
    void validarRenombreNodo(Nodo* nodo, const std::string& nuevoNombre) const;
    void validarEliminacionNodo(Nodo* nodo) const;

public:
    Arbol();
    ~Arbol();

    // Operaciones básicas con manejo de excepciones
    Nodo* crearNodo(const std::string& nombre, TipoNodo tipo, const std::string& contenido = "", Nodo* padre = nullptr);
    bool eliminarNodo(int id); // Mueve a papelera
    bool moverNodo(int idOrigen, int idDestino);
    bool renombrarNodo(int id, const std::string& nuevoNombre);

    // NUEVAS OPERACIONES DE PAPELERA
    bool restaurarNodo(int id);
    bool vaciarPapelera();
    std::vector<Nodo*> listarPapelera() const;

    // Búsquedas con validación
    Nodo* buscarNodo(int id) const;
    Nodo* buscarNodoPorRuta(const std::string& ruta) const;

    // Búsqueda por nombre
    std::vector<Nodo*> buscarPorNombreExacto(const std::string& nombre) const;
    std::vector<std::pair<std::string, std::vector<int>>> buscarPorPrefijo(const std::string& prefijo) const;
    std::vector<std::string> autocompletarNombres(const std::string& prefijo, int limite = 5) const;

    // Recorridos
    std::vector<Nodo*> recorridoPreorden() const;

    // Métodos de información
    Nodo* getRaiz() const;
    int getAltura() const; // Pendiente de implementación, se devuelve un valor fijo
    int getTamano() const;
    int getProximoId() const;
    int getTamanoPapelera() const;

    // Métodos auxiliares
    void listarHijos(int idPadre) const;
    bool existeNodo(int id) const;
    bool existeNombreEnUbicacion(const std::string& nombre, int idPadre) const;
    std::string obtenerRutaNodo(int id) const;
    
    // Para la serialización JSON (incluye papelera)
    std::vector<Nodo*> getAllNodos() const; 

    // Solo para GestorJSON (permite la carga)
    friend class GestorJSON; 
};

#endif
#ifndef ARBOL_GESTOR_H
#define ARBOL_GESTOR_H

#include "error_handler.h"
#include "nodo_trie.h"
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// ==================== ARBOL ====================
class Arbol {
private:
    Nodo* raiz;
    int proximoId;
    std::map<int, Nodo*> mapaNodos;
    std::map<int, Nodo*> papelera;
    Trie trieNombres;

    void eliminarRecursivo(Nodo* nodo);
    void eliminarFisico(Nodo* nodo);
    void actualizarTrie(Nodo* nodo, bool agregar);
    void actualizarMapa(Nodo* nodo, bool agregar);
    void validarCreacionNodo(Nodo* padre, const std::string& nombre) const;
    void validarMovimientoNodo(Nodo* origen, Nodo* destino) const;
    void validarRenombreNodo(Nodo* nodo, const std::string& nuevoNombre) const;
    void validarEliminacionNodo(Nodo* nodo) const;
    void recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const;
    Nodo* buscarNodoRecursivo(Nodo* nodo, int id) const;
    void removeFromStructures(Nodo* nodo);
    void addToStructures(Nodo* nodo);

public:
    Arbol();
    ~Arbol();

    // Métodos principales
    Nodo* crearNodo(const std::string& nombre, TipoNodo tipo, 
                    const std::string& contenido = "", Nodo* padre = nullptr);
    bool eliminarNodo(int id);
    bool moverNodo(int idOrigen, int idDestino);
    bool renombrarNodo(int id, const std::string& nuevoNombre);
    bool restaurarNodo(int id);
    bool vaciarPapelera();

    // Búsqueda
    Nodo* buscarNodo(int id) const;
    Nodo* buscarNodoPorRuta(const std::string& ruta) const;
    std::vector<Nodo*> buscarPorNombreExacto(const std::string& nombre) const;
    std::vector<std::pair<std::string, std::vector<int>>> buscarPorPrefijo(const std::string& prefijo) const;
    std::vector<std::string> autocompletarNombres(const std::string& prefijo, int limite = 10) const;

    // Recorridos
    std::vector<Nodo*> recorridoPreorden() const;
    std::vector<Nodo*> getAllNodos() const;
    void listarHijos(int idPadre) const;

    // Información
    Nodo* getRaiz() const;
    int getAltura() const;
    int getTamano() const;
    int getProximoId() const;
    int getTamanoPapelera() const;
    std::vector<Nodo*> listarPapelera() const;
    bool existeNodo(int id) const;
    bool existeNombreEnUbicacion(const std::string& nombre, int idPadre) const;
    std::string obtenerRutaNodo(int id) const;

    // Para carga JSON
    void limpiarArbol();
    void establecerProximoId(int nuevoId);
    void agregarNodoCargado(Nodo* nodo);
    void reconstruirDesdeCarga(Nodo* nodoRaizCargado);
    void addNodoToPapelera(Nodo* nodo);
};

// ==================== GESTOR JSON ====================
class GestorJSON {
private:
    static std::string escapeJson(const std::string& str);
    static std::string unescapeJson(const std::string& str);

public:
    static bool guardarArbol(const Arbol& arbol, const std::string& nombreArchivo);
    static bool cargarArbol(Arbol& arbol, const std::string& nombreArchivo);
};

#endif // ARBOL_GESTOR_H
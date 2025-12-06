#include "arbol.h"
#include <iostream>
#include <queue>
#include <sstream>

Arbol::Arbol() : raiz(nullptr), proximoId(1) {
    // Crear nodo raíz por defecto
    raiz = new Nodo(proximoId++, "root", CARPETA);
    actualizarMapa(raiz, true);
    actualizarTrie(raiz, true);
}

Arbol::~Arbol() {
    eliminarRecursivo(raiz);
}

void Arbol::actualizarTrie(Nodo* nodo, bool agregar) {
    if (!nodo) return;

    if (agregar) {
        trieNombres.insertar(nodo->getNombre(), nodo->getId());
    }
    else {
        trieNombres.eliminar(nodo->getNombre(), nodo->getId());
    }
}

void Arbol::actualizarMapa(Nodo* nodo, bool agregar) {
    if (!nodo) return;

    if (agregar) {
        mapaNodos[nodo->getId()] = nodo;
    }
    else {
        mapaNodos.erase(nodo->getId());
    }
}

void Arbol::eliminarRecursivo(Nodo* nodo) {
    if (!nodo) return;

    // Actualizar índices para todos los nodos en el subárbol
    std::queue<Nodo*> cola;
    cola.push(nodo);

    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();

        // Actualizar índices
        actualizarTrie(actual, false);
        actualizarMapa(actual, false);

        // Agregar hijos a la cola
        for (Nodo* hijo : actual->getHijos()) {
            cola.push(hijo);
        }
    }

    // Ahora eliminar físicamente los nodos
    cola.push(nodo);

    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();

        for (Nodo* hijo : actual->getHijos()) {
            cola.push(hijo);
        }

        if (actual != nodo) { // No eliminar el nodo raíz aquí
            delete actual;
        }
    }

    if (nodo == raiz) {
        delete raiz;
        raiz = nullptr;
    }
}

Nodo* Arbol::crearNodo(const std::string& nombre, TipoNodo tipo, const std::string& contenido, Nodo* padre) {
    if (!padre) padre = raiz;

    // Verificar si ya existe un hijo con ese nombre
    if (padre->buscarHijoPorNombre(nombre)) {
        std::cout << "Error: Ya existe un nodo con el nombre '" << nombre << "' en esta ubicacion.\n";
        return nullptr;
    }

    Nodo* nuevoNodo = new Nodo(proximoId++, nombre, tipo, contenido);
    padre->agregarHijo(nuevoNodo);

    // Actualizar índices
    actualizarMapa(nuevoNodo, true);
    actualizarTrie(nuevoNodo, true);

    return nuevoNodo;
}

bool Arbol::eliminarNodo(int id) {
    if (id == raiz->getId()) {
        std::cout << "Error: No se puede eliminar la raiz.\n";
        return false;
    }

    Nodo* nodo = buscarNodo(id);
    if (!nodo) {
        std::cout << "Error: Nodo no encontrado.\n";
        return false;
    }

    Nodo* padre = nodo->getPadre();
    if (padre) {
        padre->eliminarHijo(nodo);

        // Eliminar del árbol y actualizar índices
        eliminarRecursivo(nodo);
        return true;
    }

    return false;
}

bool Arbol::moverNodo(int idOrigen, int idDestino) {
    if (idOrigen == idDestino) {
        std::cout << "Error: No se puede mover un nodo a si mismo.\n";
        return false;
    }

    Nodo* origen = buscarNodo(idOrigen);
    Nodo* destino = buscarNodo(idDestino);

    if (!origen || !destino) {
        std::cout << "Error: Nodo(s) no encontrado(s).\n";
        return false;
    }

    if (destino->getTipo() != CARPETA) {
        std::cout << "Error: El destino debe ser una carpeta.\n";
        return false;
    }

    // Verificar si el destino es descendiente del origen (evitar ciclos)
    Nodo* temp = destino;
    while (temp) {
        if (temp->getId() == origen->getId()) {
            std::cout << "Error: No se puede mover una carpeta dentro de si misma o sus descendientes.\n";
            return false;
        }
        temp = temp->getPadre();
    }

    // Verificar si ya existe un nodo con el mismo nombre en el destino
    if (destino->buscarHijoPorNombre(origen->getNombre())) {
        std::cout << "Error: Ya existe un nodo con el nombre '" << origen->getNombre() << "' en el destino.\n";
        return false;
    }

    // Remover del padre actual
    Nodo* padreActual = origen->getPadre();
    if (padreActual) {
        padreActual->eliminarHijo(origen);
    }

    // Agregar al nuevo padre
    destino->agregarHijo(origen);

    return true;
}

bool Arbol::renombrarNodo(int id, const std::string& nuevoNombre) {
    Nodo* nodo = buscarNodo(id);
    if (!nodo) {
        std::cout << "Error: Nodo no encontrado.\n";
        return false;
    }

    // Verificar si ya existe un hermano con el mismo nombre
    Nodo* padre = nodo->getPadre();
    if (padre && padre->buscarHijoPorNombre(nuevoNombre)) {
        std::cout << "Error: Ya existe un nodo con el nombre '" << nuevoNombre << "' en esta ubicacion.\n";
        return false;
    }

    // Actualizar Trie: eliminar el nombre antiguo y agregar el nuevo
    actualizarTrie(nodo, false); // Eliminar nombre antiguo
    nodo->setNombre(nuevoNombre);
    actualizarTrie(nodo, true); // Agregar nombre nuevo

    return true;
}

Nodo* Arbol::buscarNodoRecursivo(Nodo* nodo, int id) const {
    if (!nodo) return nullptr;

    if (nodo->getId() == id) {
        return nodo;
    }

    for (Nodo* hijo : nodo->getHijos()) {
        Nodo* encontrado = buscarNodoRecursivo(hijo, id);
        if (encontrado) return encontrado;
    }

    return nullptr;
}

Nodo* Arbol::buscarNodo(int id) const {
    // Usar el mapa hash para búsqueda O(1)
    auto it = mapaNodos.find(id);
    if (it != mapaNodos.end()) {
        return it->second;
    }
    return nullptr;
}

Nodo* Arbol::buscarNodoPorRuta(const std::string& ruta) const {
    if (ruta.empty() || ruta == "/") return raiz;

    std::stringstream ss(ruta);
    std::string segmento;
    Nodo* actual = raiz;

    // Ignorar el primer '/' si existe
    if (ruta[0] == '/') {
        getline(ss, segmento, '/');
    }

    while (getline(ss, segmento, '/')) {
        if (segmento.empty()) continue;

        actual = actual->buscarHijoPorNombre(segmento);
        if (!actual) return nullptr;
    }

    return actual;
}

// Nuevos métodos de búsqueda usando Trie

std::vector<Nodo*> Arbol::buscarPorNombreExacto(const std::string& nombre) const {
    std::vector<Nodo*> resultados;
    std::vector<int> ids = trieNombres.buscarExacto(nombre);

    for (int id : ids) {
        Nodo* nodo = buscarNodo(id);
        if (nodo) {
            resultados.push_back(nodo);
        }
    }

    return resultados;
}

std::vector<std::pair<std::string, std::vector<int>>> Arbol::buscarPorPrefijo(const std::string& prefijo) const {
    return trieNombres.buscarPrefijo(prefijo);
}

std::vector<std::string> Arbol::autocompletarNombres(const std::string& prefijo, int limite) const {
    return trieNombres.autocompletar(prefijo, limite);
}

void Arbol::recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const {
    if (!nodo) return;

    resultado.push_back(nodo);

    for (Nodo* hijo : nodo->getHijos()) {
        recorridoPreordenRecursivo(hijo, resultado);
    }
}

std::vector<Nodo*> Arbol::recorridoPreorden() const {
    std::vector<Nodo*> resultado;
    recorridoPreordenRecursivo(raiz, resultado);
    return resultado;
}

Nodo* Arbol::getRaiz() const {
    return raiz;
}

int Arbol::getAltura() const {
    if (!raiz) return 0;

    std::queue<std::pair<Nodo*, int>> cola;
    cola.push({ raiz, 1 });
    int alturaMaxima = 1;

    while (!cola.empty()) {
        auto [nodo, nivel] = cola.front();
        cola.pop();

        alturaMaxima = std::max(alturaMaxima, nivel);

        for (Nodo* hijo : nodo->getHijos()) {
            cola.push({ hijo, nivel + 1 });
        }
    }

    return alturaMaxima;
}

int Arbol::getTamano() const {
    return mapaNodos.size();
}

int Arbol::getProximoId() const {
    return proximoId;
}

void Arbol::listarHijos(int idPadre) const {
    Nodo* padre = buscarNodo(idPadre);
    if (!padre) {
        std::cout << "Nodo no encontrado.\n";
        return;
    }

    std::vector<Nodo*> hijos = padre->getHijos();
    if (hijos.empty()) {
        std::cout << "La carpeta esta vacia.\n";
        return;
    }

    std::cout << "Contenido de '" << padre->getNombre() << "':\n";
    for (Nodo* hijo : hijos) {
        std::cout << "  [" << (hijo->getTipo() == CARPETA ? "DIR" : "FILE") << "] "
            << hijo->getNombre() << " (ID: " << hijo->getId() << ")\n";
    }
}

void Arbol::reconstruirArbol(Nodo* nuevaRaiz, int nuevoProximoId) {
    // Limpiar índices actuales
    trieNombres.limpiar();
    mapaNodos.clear();

    // Establecer la nueva raíz
    raiz = nuevaRaiz;
    proximoId = nuevoProximoId;

    // Reconstruir índices
    reconstruirIndices();
}

void Arbol::reconstruirIndices() {
    if (!raiz) return;

    std::queue<Nodo*> cola;
    cola.push(raiz);

    while (!cola.empty()) {
        Nodo* nodo = cola.front();
        cola.pop();

        // Actualizar índices
        actualizarMapa(nodo, true);
        actualizarTrie(nodo, true);

        // Agregar hijos a la cola
        for (Nodo* hijo : nodo->getHijos()) {
            cola.push(hijo);
        }
    }
}
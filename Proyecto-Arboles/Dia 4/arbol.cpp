#include "arbol.h"
#include <iostream>
#include <queue>
#include <sstream>

Arbol::Arbol() : raiz(nullptr), proximoId(1) {
    // Crear nodo raíz por defecto
    raiz = new Nodo(proximoId++, "root", CARPETA);
}

Arbol::~Arbol() {
    eliminarRecursivo(raiz);
}

void Arbol::eliminarRecursivo(Nodo* nodo) {
    if (!nodo) return;
    
    for (Nodo* hijo : nodo->getHijos()) {
        eliminarRecursivo(hijo);
    }
    delete nodo;
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
    
    nodo->setNombre(nuevoNombre);
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
    return buscarNodoRecursivo(raiz, id);
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
    cola.push({raiz, 1});
    int alturaMaxima = 1;
    
    while (!cola.empty()) {
        auto [nodo, nivel] = cola.front();
        cola.pop();
        
        alturaMaxima = std::max(alturaMaxima, nivel);
        
        for (Nodo* hijo : nodo->getHijos()) {
            cola.push({hijo, nivel + 1});
        }
    }
    
    return alturaMaxima;
}

int Arbol::getTamano() const {
    if (!raiz) return 0;
    
    std::queue<Nodo*> cola;
    cola.push(raiz);
    int tamano = 0;
    
    while (!cola.empty()) {
        Nodo* nodo = cola.front();
        cola.pop();
        tamano++;
        
        for (Nodo* hijo : nodo->getHijos()) {
            cola.push(hijo);
        }
    }
    
    return tamano;
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
    // Eliminar el árbol actual
    if (raiz && raiz != nuevaRaiz) {
        eliminarRecursivo(raiz);
    }
    
    // Establecer la nueva raíz
    raiz = nuevaRaiz;
    proximoId = nuevoProximoId;
}
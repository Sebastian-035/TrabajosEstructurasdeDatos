#include "arbol.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <algorithm>

Arbol::Arbol() : raiz(nullptr), proximoId(1) {
    try {
        // ID 1 para la raíz
        raiz = new Nodo(proximoId++, "root", CARPETA); 
        actualizarMapa(raiz, true);
        actualizarTrie(raiz, true);
        ErrorHandler::mostrarInfo("Arbol inicializado correctamente");
    }
    catch (const std::bad_alloc& e) {
        ErrorHandler::mostrarError(FilesystemException("Error de memoria al inicializar el arbol"));
        throw;
    }
}

Arbol::~Arbol() {
    try {
        eliminarRecursivo(raiz);
        // Borrar nodos de la papelera
        for (auto& par : papelera) {
            delete par.second;
        }
        papelera.clear();
        ErrorHandler::mostrarInfo("Arbol y Papelera destruidos correctamente");
    }
    catch (...) {
        ErrorHandler::mostrarAdvertencia("Excepcion no manejada durante la destruccion del arbol");
    }
}

void Arbol::eliminarRecursivo(Nodo* nodo) {
    if (!nodo) return;

    for (Nodo* hijo : nodo->getHijos()) {
        eliminarRecursivo(hijo);
    }
    // Borrado físico
    delete nodo;
}

void Arbol::eliminarFisico(Nodo* nodo) {
    if (!nodo) return;

    // 1. Recorrer y eliminar hijos (Postorden)
    for (Nodo* hijo : nodo->getHijos()) {
        eliminarFisico(hijo);
    }

    // 2. Eliminar del Trie y MapaNodos
    actualizarTrie(nodo, false);
    actualizarMapa(nodo, false);
    
    // 3. Borrado físico
    delete nodo;
}

void Arbol::actualizarTrie(Nodo* nodo, bool agregar) {
    if (agregar) {
        trieNombres.insertar(nodo->getNombre(), nodo->getId());
    } else {
        trieNombres.eliminar(nodo->getNombre(), nodo->getId());
    }
}

void Arbol::actualizarMapa(Nodo* nodo, bool agregar) {
    if (agregar) {
        mapaNodos[nodo->getId()] = nodo;
    } else {
        mapaNodos.erase(nodo->getId());
    }
}

void Arbol::validarCreacionNodo(Nodo* padre, const std::string& nombre) const {
    if (!padre) {
        throw OperacionInvalidaException("crear nodo", "Padre no especificado");
    }

    if (nombre.empty()) {
        throw OperacionInvalidaException("crear nodo", "Nombre no puede estar vacio");
    }

    if (padre->buscarHijoPorNombre(nombre)) {
        throw NombreDuplicadoException(nombre, padre->getId());
    }
}

void Arbol::validarMovimientoNodo(Nodo* origen, Nodo* destino) const {
    if (!origen || !destino) {
        throw OperacionInvalidaException("mover", "Nodo de origen o destino no encontrado");
    }

    if (origen == raiz) {
        throw OperacionInvalidaException("mover", "No se puede mover el nodo raiz");
    }

    if (origen == destino) {
        throw OperacionInvalidaException("mover", "Origen y destino son el mismo nodo");
    }

    if (destino->getTipo() == ARCHIVO) {
        throw DestinoInvalidoException(destino->getId());
    }

    if (destino->buscarHijoPorNombre(origen->getNombre())) {
        throw NombreDuplicadoException(origen->getNombre(), destino->getId());
    }

    // Evitar mover un nodo a uno de sus propios descendientes
    Nodo* temp = destino;
    while (temp != nullptr) {
        if (temp == origen) {
            throw OperacionInvalidaException("mover", "No se puede mover un nodo a uno de sus propios descendientes");
        }
        temp = temp->getPadre();
    }
}

void Arbol::validarRenombreNodo(Nodo* nodo, const std::string& nuevoNombre) const {
    if (!nodo) {
        throw OperacionInvalidaException("renombrar", "Nodo no encontrado");
    }

    if (nodo == raiz) {
        throw OperacionInvalidaException("renombrar", "No se puede renombrar el nodo raiz");
    }

    if (nuevoNombre.empty()) {
        throw OperacionInvalidaException("renombrar", "El nuevo nombre no puede estar vacio");
    }

    Nodo* padre = nodo->getPadre();
    if (padre && padre->buscarHijoPorNombre(nuevoNombre)) {
        throw NombreDuplicadoException(nuevoNombre, padre->getId());
    }
}

void Arbol::validarEliminacionNodo(Nodo* nodo) const {
    if (!nodo) {
        throw OperacionInvalidaException("eliminar", "Nodo no encontrado");
    }

    if (nodo == raiz) {
        throw OperacionInvalidaException("eliminar", "No se puede eliminar el nodo raiz");
    }
}

Nodo* Arbol::crearNodo(const std::string& nombre, TipoNodo tipo, const std::string& contenido, Nodo* padre) {
    try {
        if (padre == nullptr) {
            padre = raiz;
        }

        validarCreacionNodo(padre, nombre);
        
        if (padre->getTipo() == ARCHIVO) {
            throw OperacionInvalidaException("crear", "No se pueden crear nodos dentro de un archivo");
        }

        Nodo* nuevoNodo = new Nodo(proximoId++, nombre, tipo, contenido);
        padre->agregarHijo(nuevoNodo);

        actualizarMapa(nuevoNodo, true);
        actualizarTrie(nuevoNodo, true);
        
        ErrorHandler::mostrarInfo("Nodo creado: ID " + std::to_string(nuevoNodo->getId()) + 
            " ('" + nombre + "') en " + padre->getRutaCompleta());
        
        return nuevoNodo;
    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return nullptr;
    }
    catch (const std::bad_alloc& e) {
        ErrorHandler::mostrarError(FilesystemException("Error de memoria al crear nodo"));
        return nullptr;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al crear nodo"));
        return nullptr;
    }
}

bool Arbol::eliminarNodo(int id) {
    try {
        Nodo* nodo = buscarNodo(id);
        validarEliminacionNodo(nodo);

        // 1. Eliminar de la lista de hijos del padre
        Nodo* padre = nodo->getPadre();
        if (padre) {
            padre->eliminarHijo(nodo);
        }

        std::queue<Nodo*> q;
        q.push(nodo);
        while (!q.empty()) {
            Nodo* actual = q.front();
            q.pop();

            actualizarMapa(actual, false); // Eliminar del mapa principal
            actualizarTrie(actual, false); // Eliminar del trie

            for (Nodo* hijo : actual->getHijos()) {
                q.push(hijo);
            }
        }

        // 3. Mover a la estructura de papelera
        papelera[nodo->getId()] = nodo;
        
        // 4. Quitar la referencia al padre para que no se liste en el árbol principal
        nodo->setPadre(nullptr); 

        ErrorHandler::mostrarInfo("Nodo movido a la papelera: ID " + std::to_string(id) + " ('" + nodo->getNombre() + "')");
        return true;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al eliminar nodo"));
        return false;
    }
}

bool Arbol::moverNodo(int idOrigen, int idDestino) {
    try {
        Nodo* origen = buscarNodo(idOrigen);
        Nodo* destino = buscarNodo(idDestino);
        validarMovimientoNodo(origen, destino);
        
        // 1. Quitar del padre original
        Nodo* padreOriginal = origen->getPadre();
        if (padreOriginal) {
            padreOriginal->eliminarHijo(origen);
        }
        
        // 2. Mover al nuevo destino (agregar como hijo)
        destino->agregarHijo(origen);
        
        // 3. Actualizar Trie (nombre puede ser el mismo, pero la ruta es nueva)
        
        ErrorHandler::mostrarInfo("Nodo movido: ID " + std::to_string(idOrigen) + 
            " a " + destino->getRutaCompleta());
        return true;
    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al mover nodo"));
        return false;
    }
}

bool Arbol::renombrarNodo(int id, const std::string& nuevoNombre) {
    try {
        Nodo* nodo = buscarNodo(id);
        validarRenombreNodo(nodo, nuevoNombre);
        
        // 1. Eliminar del Trie con el nombre antiguo
        actualizarTrie(nodo, false);
        
        // 2. Renombrar
        nodo->setNombre(nuevoNombre);
        
        // 3. Reinsertar en el Trie con el nombre nuevo
        actualizarTrie(nodo, true);
        
        ErrorHandler::mostrarInfo("Nodo ID " + std::to_string(id) + " renombrado a '" + nuevoNombre + "'");
        return true;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al renombrar nodo"));
        return false;
    }
}

bool Arbol::restaurarNodo(int id) {
    try {
        auto it = papelera.find(id);
        if (it == papelera.end()) {
            throw NodoNotFoundException(id);
        }

        Nodo* nodo = it->second;

        // Intentar restaurar al padre original si existe
        Nodo* padreOriginal = nullptr;
        
        Nodo* nuevoPadre = raiz;

        // Validar si el nombre ya existe en la ubicación de restauración (la raíz)
        if (existeNombreEnUbicacion(nodo->getNombre(), nuevoPadre->getId())) {
            // Renombrar el nodo restaurado para evitar duplicados
            std::string nuevoNombre = nodo->getNombre() + "_restaurado_" + std::to_string(id);
            ErrorHandler::mostrarAdvertencia("Nombre duplicado. Restaurando '" + nodo->getNombre() + "' como '" + nuevoNombre + "'");
            nodo->setNombre(nuevoNombre);
        }
        
        // 1. Restaurar al nuevo padre (la raíz)
        nuevoPadre->agregarHijo(nodo);
        nodo->setPadre(nuevoPadre); // Esto también establece el padre del nodo.

        // 2. Mover del mapa de papelera al mapa de nodos principal
        papelera.erase(it);
        
        // 3. Reinsertar en Trie y MapaNodos (recursivamente para todo el subárbol)
        std::queue<Nodo*> q;
        q.push(nodo);
        while (!q.empty()) {
            Nodo* actual = q.front();
            q.pop();

            actualizarMapa(actual, true); // Reinsertar en mapaNodos
            actualizarTrie(actual, true); // Reinsertar en trie

            for (Nodo* hijo : actual->getHijos()) {
                q.push(hijo);
            }
        }
        
        ErrorHandler::mostrarInfo("Nodo ID " + std::to_string(id) + " ('" + nodo->getNombre() + "') restaurado en '" + nuevoPadre->getRutaCompleta() + "'");
        return true;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al restaurar nodo"));
        return false;
    }
}

bool Arbol::vaciarPapelera() {
    if (papelera.empty()) {
        ErrorHandler::mostrarAdvertencia("La papelera ya está vacía.");
        return true;
    }

    if (!ErrorHandler::confirmarAccion("¿Está seguro de que desea ELIMINAR PERMANENTEMENTE todos los " + std::to_string(papelera.size()) + " elementos de la papelera?")) {
        ErrorHandler::mostrarInfo("Operación de vaciado cancelada.");
        return false;
    }

    try {
        for (auto const& [id, nodo] : papelera) {
            // Elminacion fisica recursiva (actualiza mapa/trie)
            eliminarFisico(nodo); 
        }
        papelera.clear();
        ErrorHandler::mostrarInfo("Papelera vaciada permanentemente. Nodos eliminados.");
        return true;
    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al vaciar la papelera"));
        return false;
    }
}

std::vector<Nodo*> Arbol::listarPapelera() const {
    std::vector<Nodo*> lista;
    for (auto const& [id, nodo] : papelera) {
        lista.push_back(nodo);
    }
    return lista;
}

void Arbol::addNodoToPapelera(Nodo* nodo) {
    if (nodo->getId() != raiz->getId()) {
        papelera[nodo->getId()] = nodo;
        // Se asume que el GestorJSON ya hizo la limpieza del mapa/trie si no era root
    }
}

Nodo* Arbol::buscarNodo(int id) const {
    try {
        auto it = mapaNodos.find(id);
        if (it != mapaNodos.end()) {
            return it->second;
        }

        throw NodoNotFoundException(id);

    }
    catch (const FilesystemException& e) {
        throw;
    }
    catch (...) {
        throw FilesystemException("Error desconocido al buscar nodo");
    }
}

Nodo* Arbol::buscarNodoPorRuta(const std::string& ruta) const {
    if (ruta.empty() || ruta == "/") {
        return raiz;
    }

    std::stringstream ss(ruta);
    std::string segmento;
    Nodo* actual = raiz;

    // Saltar el primer '/'
    if (ruta[0] == '/') {
        std::getline(ss, segmento, '/');
    }

    while (std::getline(ss, segmento, '/')) {
        if (segmento.empty()) continue; // Manejar //

        Nodo* siguiente = actual->buscarHijoPorNombre(segmento);
        if (siguiente == nullptr) {
            throw NodoNotFoundExceptionStr(ruta);
        }
        actual = siguiente;
    }

    return actual;
}

std::vector<Nodo*> Arbol::buscarPorNombreExacto(const std::string& nombre) const {
    std::vector<Nodo*> resultados;
    std::vector<int> ids = trieNombres.buscarExacto(nombre);

    for (int id : ids) {
        auto it = mapaNodos.find(id);
        if (it != mapaNodos.end()) {
            resultados.push_back(it->second);
        }
    }
    return resultados;
}

std::vector<std::pair<std::string, std::vector<int>>> Arbol::buscarPorPrefijo(const std::string& prefijo) const {
    // La búsqueda por prefijo del Trie ya devuelve los IDs
    return trieNombres.buscarPrefijo(prefijo);
}

std::vector<std::string> Arbol::autocompletarNombres(const std::string& prefijo, int limite) const {
    return trieNombres.autocompletar(prefijo, limite);
}

void Arbol::recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const {
    if (nodo == nullptr) {
        return;
    }
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

std::vector<Nodo*> Arbol::getAllNodos() const {
    std::vector<Nodo*> todos;
    
    // Agregar nodos del arbol principal (recorridoPreorden ya los obtiene)
    recorridoPreordenRecursivo(raiz, todos); 
    
    // Agregar nodos de la papelera
    for (auto const& [id, nodo] : papelera) {
        todos.push_back(nodo);
    }
    
    return todos;
}

Nodo* Arbol::getRaiz() const {
    return raiz;
}

int Arbol::getAltura() const {
    // Implementación simple, no se pide la implementación compleja.
    return 1; 
}

int Arbol::getTamano() const {
    return mapaNodos.size();
}

int Arbol::getProximoId() const {
    return proximoId;
}

int Arbol::getTamanoPapelera() const {
    return papelera.size();
}

void Arbol::listarHijos(int idPadre) const {
    try {
        Nodo* padre = buscarNodo(idPadre);
        if (padre->getTipo() == ARCHIVO) {
            ErrorHandler::mostrarAdvertencia("El nodo ID " + std::to_string(idPadre) + " es un archivo, no una carpeta.");
            return;
        }

        std::vector<Nodo*> hijos = padre->getHijos();

        std::cout << "\nContenido de '" << padre->getRutaCompleta() << "' (ID: " << padre->getId() << "): " << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        std::cout << std::left << std::setw(8) << "ID"
                  << std::left << std::setw(15) << "TIPO"
                  << "NOMBRE" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        if (hijos.empty()) {
            std::cout << "[Vacio]" << std::endl;
        } else {
            for (Nodo* hijo : hijos) {
                std::string tipoStr = (hijo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO");
                std::cout << std::left << std::setw(8) << hijo->getId()
                          << std::left << std::setw(15) << tipoStr
                          << hijo->getNombre() << std::endl;
            }
        }
        std::cout << std::string(70, '-') << std::endl;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al listar hijos"));
    }
}

bool Arbol::existeNodo(int id) const {
    return mapaNodos.find(id) != mapaNodos.end();
}

bool Arbol::existeNombreEnUbicacion(const std::string& nombre, int idPadre) const {
    try {
        Nodo* padre = buscarNodo(idPadre);
        return padre->buscarHijoPorNombre(nombre) != nullptr;
    }
    catch (...) {
        return false;
    }
}

std::string Arbol::obtenerRutaNodo(int id) const {
    try {
        Nodo* nodo = buscarNodo(id);
        return nodo->getRutaCompleta();
    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return "";
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error desconocido al obtener ruta"));
        return "";
    }
}
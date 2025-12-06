#include "trie.h"
#include <algorithm>
#include <cctype>

NodoTrie::NodoTrie() : esFinPalabra(false) {}

bool NodoTrie::tieneHijo(char c) const {
    return hijos.find(std::tolower(c)) != hijos.end();
}

std::shared_ptr<NodoTrie> NodoTrie::obtenerHijo(char c) const {
    auto it = hijos.find(std::tolower(c));
    if (it != hijos.end()) {
        return it->second;
    }
    return nullptr;
}

void NodoTrie::agregarHijo(char c, std::shared_ptr<NodoTrie> nodo) {
    hijos[std::tolower(c)] = nodo;
}

void NodoTrie::marcarFinPalabra(int idNodo) {
    esFinPalabra = true;
    // Añadir el ID si no está ya presente
    if (std::find(idsNodos.begin(), idsNodos.end(), idNodo) == idsNodos.end()) {
        idsNodos.push_back(idNodo);
    }
}

void NodoTrie::desmarcarFinPalabra(int idNodo) {
    // Eliminar el ID específico
    auto it = std::find(idsNodos.begin(), idsNodos.end(), idNodo);
    if (it != idsNodos.end()) {
        idsNodos.erase(it);
    }

    // Si no hay más IDs, ya no es fin de palabra
    if (idsNodos.empty()) {
        esFinPalabra = false;
    }
}

bool NodoTrie::esFinDePalabra() const {
    return esFinPalabra;
}

std::vector<int> NodoTrie::obtenerIds() const {
    return idsNodos;
}

std::vector<char> NodoTrie::obtenersCaracteresHijos() const {
    std::vector<char> caracteres;
    for (const auto& par : hijos) {
        caracteres.push_back(par.first);
    }
    return caracteres;
}

bool NodoTrie::tieneHijos() const {
    return !hijos.empty();
}

Trie::Trie() : raiz(std::make_shared<NodoTrie>()) {}

void Trie::insertar(const std::string& palabra, int idNodo) {
    std::shared_ptr<NodoTrie> actual = raiz;

    for (char c : palabra) {
        if (!actual->tieneHijo(c)) {
            actual->agregarHijo(c, std::make_shared<NodoTrie>());
        }
        actual = actual->obtenerHijo(c);
    }

    actual->marcarFinPalabra(idNodo);
}

bool Trie::eliminar(const std::string& palabra, int idNodo) {
    std::shared_ptr<NodoTrie> actual = raiz;

    // Primero, verificar que la palabra existe
    for (char c : palabra) {
        if (!actual->tieneHijo(c)) {
            return false; // La palabra no existe
        }
        actual = actual->obtenerHijo(c);
    }

    if (!actual->esFinDePalabra()) {
        return false; // La palabra no existe como palabra completa
    }

    // Eliminar el ID específico
    actual->desmarcarFinPalabra(idNodo);

    return true;
}

std::vector<int> Trie::buscarExacto(const std::string& palabra) const {
    std::shared_ptr<NodoTrie> actual = raiz;

    for (char c : palabra) {
        if (!actual->tieneHijo(c)) {
            return {}; // No encontrado
        }
        actual = actual->obtenerHijo(c);
    }

    if (actual->esFinDePalabra()) {
        return actual->obtenerIds();
    }

    return {};
}

void Trie::buscarPrefijoRecursivo(std::shared_ptr<NodoTrie> nodo,
    std::string prefijoActual,
    std::vector<std::pair<std::string, std::vector<int>>>& resultados) const {
    if (nodo->esFinDePalabra()) {
        resultados.push_back({ prefijoActual, nodo->obtenerIds() });
    }

    for (char c : nodo->obtenersCaracteresHijos()) {
        buscarPrefijoRecursivo(nodo->obtenerHijo(c), prefijoActual + c, resultados);
    }
}

std::vector<std::pair<std::string, std::vector<int>>> Trie::buscarPrefijo(const std::string& prefijo) const {
    std::vector<std::pair<std::string, std::vector<int>>> resultados;
    std::shared_ptr<NodoTrie> actual = raiz;

    // Navegar hasta el final del prefijo
    for (char c : prefijo) {
        if (!actual->tieneHijo(c)) {
            return resultados; // Prefijo no encontrado
        }
        actual = actual->obtenerHijo(c);
    }

    // Realizar búsqueda recursiva desde este nodo
    buscarPrefijoRecursivo(actual, prefijo, resultados);

    return resultados;
}

std::vector<std::string> Trie::autocompletar(const std::string& prefijo, int limite) const {
    std::vector<std::string> sugerencias;
    auto resultados = buscarPrefijo(prefijo);

    for (const auto& resultado : resultados) {
        if (sugerencias.size() >= static_cast<size_t>(limite)) {
            break;
        }
        sugerencias.push_back(resultado.first);
    }

    return sugerencias;
}

bool Trie::existePrefijo(const std::string& prefijo) const {
    std::shared_ptr<NodoTrie> actual = raiz;

    for (char c : prefijo) {
        if (!actual->tieneHijo(c)) {
            return false;
        }
        actual = actual->obtenerHijo(c);
    }

    return true;
}

void Trie::limpiar() {
    raiz = std::make_shared<NodoTrie>();
}
#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

class NodoTrie {
private:
    std::map<char, std::shared_ptr<NodoTrie>> hijos;
    bool esFinPalabra;
    std::vector<int> idsNodos; // IDs de nodos que terminan aquí

public:
    NodoTrie();

    bool tieneHijo(char c) const;
    std::shared_ptr<NodoTrie> obtenerHijo(char c) const;
    void agregarHijo(char c, std::shared_ptr<NodoTrie> nodo);
    void marcarFinPalabra(int idNodo);
    void desmarcarFinPalabra(int idNodo);
    bool esFinDePalabra() const;
    std::vector<int> obtenerIds() const;
    std::vector<char> obtenerCaracteresHijos() const;
    bool tieneHijos() const;
};

class Trie {
private:
    std::shared_ptr<NodoTrie> raiz;

    void buscarPrefijoRecursivo(std::shared_ptr<NodoTrie> nodo,
        std::string prefijoActual,
        std::vector<std::pair<std::string, std::vector<int>>>& resultados) const;

public:
    Trie();

    void insertar(const std::string& palabra, int idNodo);
    bool eliminar(const std::string& palabra, int idNodo);
    std::vector<int> buscarExacto(const std::string& palabra) const;
    std::vector<std::pair<std::string, std::vector<int>>> buscarPrefijo(const std::string& prefijo) const;
    std::vector<std::string> autocompletar(const std::string& prefijo, int limite = 5) const;
    bool existePrefijo(const std::string& prefijo) const;
    void limpiar();
};

#endif
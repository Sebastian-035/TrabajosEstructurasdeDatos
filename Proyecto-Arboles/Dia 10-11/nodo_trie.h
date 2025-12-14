#ifndef NODO_TRIE_H
#define NODO_TRIE_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <stdexcept>

// ==================== ENUMS ====================
enum TipoNodo { CARPETA, ARCHIVO };

// ==================== NODO ====================
class Nodo {
private:
    int id;
    std::string nombre;
    TipoNodo tipo;
    std::string contenido;
    Nodo* padre;
    std::vector<Nodo*> hijos;

    void construirRutaRecursiva(std::string& ruta) const;

public:
    Nodo(int id, const std::string& nombre, TipoNodo tipo,
         const std::string& contenido = "", Nodo* padre = nullptr);
    ~Nodo();

    // Métodos getters
    int getId() const;
    const std::string& getNombre() const;
    TipoNodo getTipo() const;
    const std::string& getContenido() const;
    Nodo* getPadre() const;
    const std::vector<Nodo*>& getHijos() const;
    int getCantidadHijos() const;
    std::string getRutaCompleta() const;
    std::string getTipoString() const;

    // Métodos setters
    void setNombre(const std::string& nuevoNombre);
    void setContenido(const std::string& nuevoContenido);
    void setPadre(Nodo* nuevoPadre);

    // Métodos para manejo de hijos
    void agregarHijo(Nodo* hijo);
    bool eliminarHijo(Nodo* hijo);
    bool eliminarHijoPorId(int id);
    bool eliminarHijoPorNombre(const std::string& nombre);

    // Búsqueda
    Nodo* buscarHijoPorId(int id) const;
    Nodo* buscarHijoPorNombre(const std::string& nombre) const;

    // Métodos auxiliares
    bool esRaiz() const;
    bool tieneHijos() const;
    void limpiarHijos();
};

// ==================== TRIE ====================
struct TrieNode {
    std::map<char, std::shared_ptr<TrieNode>> hijos;
    std::vector<int> ids;
    bool esFinal;
    TrieNode();
};

class Trie {
private:
    std::shared_ptr<TrieNode> raiz;
    
    void recorrerTrie(const std::shared_ptr<TrieNode>& nodo,
                     const std::string& prefijo,
                     std::vector<std::pair<std::string, std::vector<int>>>& resultados) const;
    
    void recorrerParaAutocompletar(const std::shared_ptr<TrieNode>& nodo,
                                  const std::string& prefijo,
                                  std::vector<std::string>& resultados,
                                  int& contador,
                                  int limite) const;
    
    std::shared_ptr<TrieNode> buscarNodo(const std::string& nombre) const;

public:
    Trie();
    void insertar(const std::string& nombre, int id);
    void eliminar(const std::string& nombre, int id);
    std::vector<int> buscarExacto(const std::string& nombre) const;
    std::vector<std::pair<std::string, std::vector<int>>> buscarPrefijo(const std::string& prefijo) const;
    std::vector<std::string> autocompletar(const std::string& prefijo, int limite = 10) const;
};

#endif // NODO_TRIE_H
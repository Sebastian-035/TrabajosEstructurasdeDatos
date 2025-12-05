#ifndef NODO_H
#define NODO_H

#include <string>
#include <vector>

enum TipoNodo {
    CARPETA,
    ARCHIVO
};

class Nodo {
private:
    int id;
    std::string nombre;
    TipoNodo tipo;
    std::string contenido;
    std::vector<Nodo*> hijos;
    Nodo* padre;

public:
    Nodo(int id, const std::string& nombre, TipoNodo tipo, const std::string& contenido = "");
    
    // Getters
    int getId() const;
    std::string getNombre() const;
    TipoNodo getTipo() const;
    std::string getContenido() const;
    Nodo* getPadre() const;
    std::vector<Nodo*> getHijos() const;
    
    // Setters
    void setNombre(const std::string& nuevoNombre);
    void setContenido(const std::string& nuevoContenido);
    void setPadre(Nodo* nuevoPadre);
    
    // Operaciones con hijos
    void agregarHijo(Nodo* hijo);
    void eliminarHijo(Nodo* hijo);
    Nodo* buscarHijoPorNombre(const std::string& nombre) const;
    Nodo* buscarHijoPorId(int id) const;
    
    // Métodos auxiliares
    std::string getRutaCompleta() const;
    bool esHoja() const;
    int getCantidadHijos() const;
};

#endif
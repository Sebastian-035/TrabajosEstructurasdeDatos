#include "nodo.h"
#include <algorithm>

Nodo::Nodo(int id, const std::string& nombre, TipoNodo tipo, const std::string& contenido)
    : id(id), nombre(nombre), tipo(tipo), contenido(contenido), padre(nullptr) {}

int Nodo::getId() const { return id; }
std::string Nodo::getNombre() const { return nombre; }
TipoNodo Nodo::getTipo() const { return tipo; }
std::string Nodo::getContenido() const { return contenido; }
Nodo* Nodo::getPadre() const { return padre; }
std::vector<Nodo*> Nodo::getHijos() const { return hijos; }

void Nodo::setNombre(const std::string& nuevoNombre) { nombre = nuevoNombre; }
void Nodo::setContenido(const std::string& nuevoContenido) { contenido = nuevoContenido; }
void Nodo::setPadre(Nodo* nuevoPadre) { padre = nuevoPadre; }

void Nodo::agregarHijo(Nodo* hijo) {
    hijos.push_back(hijo);
    hijo->setPadre(this);
}

void Nodo::eliminarHijo(Nodo* hijo) {
    auto it = std::find(hijos.begin(), hijos.end(), hijo);
    if (it != hijos.end()) {
        hijos.erase(it);
    }
}

Nodo* Nodo::buscarHijoPorNombre(const std::string& nombre) const {
    for (Nodo* hijo : hijos) {
        if (hijo->getNombre() == nombre) {
            return hijo;
        }
    }
    return nullptr;
}

Nodo* Nodo::buscarHijoPorId(int id) const {
    for (Nodo* hijo : hijos) {
        if (hijo->getId() == id) {
            return hijo;
        }
    }
    return nullptr;
}

std::string Nodo::getRutaCompleta() const {
    if (padre == nullptr) {
        return "/" + nombre;
    }
    return padre->getRutaCompleta() + "/" + nombre;
}

bool Nodo::esHoja() const {
    return hijos.empty();
}

int Nodo::getCantidadHijos() const {
    return hijos.size();
}
#include "gestor_json.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool GestorJSON::guardarArbol(const Arbol& arbol, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo para guardar.\n";
        return false;
    }
    
    archivo << "{\n";
    archivo << "  \"proximo_id\": " << arbol.getProximoId() << ",\n";
    archivo << "  \"nodos\": [\n";
    
    std::vector<Nodo*> nodos = arbol.recorridoPreorden();
    for (size_t i = 0; i < nodos.size(); ++i) {
        Nodo* nodo = nodos[i];
        archivo << "    {\n";
        archivo << "      \"id\": " << nodo->getId() << ",\n";
        archivo << "      \"nombre\": \"" << nodo->getNombre() << "\",\n";
        archivo << "      \"tipo\": \"" << (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO") << "\",\n";
        archivo << "      \"contenido\": \"" << nodo->getContenido() << "\",\n";
        archivo << "      \"padre_id\": " << (nodo->getPadre() ? nodo->getPadre()->getId() : 0) << "\n";
        archivo << "    }";
        
        if (i < nodos.size() - 1) {
            archivo << ",";
        }
        archivo << "\n";
    }
    
    archivo << "  ]\n";
    archivo << "}\n";
    
    archivo.close();
    std::cout << "Arbol guardado en '" << nombreArchivo << "'\n";
    return true;
}

bool GestorJSON::cargarArbol(Arbol& arbol, const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo para cargar.\n";
        return false;
    }
    
    // Leer todo el contenido
    std::stringstream buffer;
    buffer << archivo.rdbuf();
    std::string contenido = buffer.str();
    
    // Parseo simple (en una implementación real usaríamos una biblioteca JSON)
    // Por simplicidad, recrearemos el árbol manualmente
    
    std::cout << "Nota: La carga de JSON se implementara en el Dia 4 del cronograma.\n";
    std::cout << "Por ahora, se creara un arbol de ejemplo.\n";
    
    // Crear un árbol de ejemplo
    Nodo* raiz = arbol.getRaiz();
    
    // Crear algunas carpetas y archivos de ejemplo
    Nodo* documentos = arbol.crearNodo("Documentos", CARPETA, "", raiz);
    Nodo* musica = arbol.crearNodo("Musica", CARPETA, "", raiz);
    
    arbol.crearNodo("tarea.txt", ARCHIVO, "Contenido de la tarea", documentos);
    arbol.crearNodo("proyecto.cpp", ARCHIVO, "Codigo del proyecto", documentos);
    arbol.crearNodo("cancion1.mp3", ARCHIVO, "", musica);
    arbol.crearNodo("cancion2.mp3", ARCHIVO, "", musica);
    
    archivo.close();
    return true;
}
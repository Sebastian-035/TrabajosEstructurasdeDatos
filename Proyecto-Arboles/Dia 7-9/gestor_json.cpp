#include "gestor_json.h"
#include "error_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

std::string GestorJSON::escapeJson(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        if (c == '"') escaped += "\\\"";
        else if (c == '\\') escaped += "\\\\";
        else if (c == '\n') escaped += "\\n";
        else if (c == '\t') escaped += "\\t";
        else escaped += c;
    }
    return escaped;
}

std::string GestorJSON::unescapeJson(const std::string& str) {
    std::string unescaped;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            if (str[i+1] == '"') {
                unescaped += '"';
                i++;
            } else if (str[i+1] == '\\') {
                unescaped += '\\';
                i++;
            } else if (str[i+1] == 'n') {
                unescaped += '\n';
                i++;
            } else if (str[i+1] == 't') {
                unescaped += '\t';
                i++;
            } else {
                unescaped += str[i];
            }
        } else {
            unescaped += str[i];
        }
    }
    return unescaped;
}


bool GestorJSON::guardarArbol(const Arbol& arbol, const std::string& nombreArchivo) {
    try {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            throw ArchivoIOException(nombreArchivo, "escritura");
        }

        std::vector<Nodo*> nodos = arbol.getAllNodos(); 

        archivo << "{\n";
        archivo << "  \"proximo_id\": " << arbol.getProximoId() << ",\n";
        archivo << "  \"nodos\": [\n";

        for (size_t i = 0; i < nodos.size(); ++i) {
            Nodo* nodo = nodos[i];
            
            int padreId = 0; 
            if (nodo->getPadre()) {
                padreId = nodo->getPadre()->getId();
            } else if (nodo->getId() != arbol.getRaiz()->getId()) {
                padreId = -1; 
            }

            archivo << "    {\n";
            archivo << "      \"id\": " << nodo->getId() << ",\n";
            archivo << "      \"nombre\": \"" << escapeJson(nodo->getNombre()) << "\",\n";
            archivo << "      \"tipo\": \"" << (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO") << "\",\n";
            archivo << "      \"contenido\": \"" << escapeJson(nodo->getContenido()) << "\",\n";
            archivo << "      \"padre_id\": " << padreId;

            archivo << "\n    }";

            if (i < nodos.size() - 1) {
                archivo << ",";
            }
            archivo << "\n";
        }

        archivo << "  ]\n";
        archivo << "}\n";

        archivo.close();
        ErrorHandler::mostrarInfo("Arbol guardado en '" + nombreArchivo + "'");
        return true;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (const std::exception& e) {
        ErrorHandler::mostrarError(FilesystemException(std::string("Error al guardar: ") + e.what()));
        return false;
    }
}

bool GestorJSON::cargarArbol(Arbol& arbol, const std::string& nombreArchivo) {
    try {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            ErrorHandler::mostrarAdvertencia("No se pudo abrir '" + nombreArchivo +
                "'. Se creara un nuevo arbol.");
            return false;
        }
        
        std::string linea;
        
        Arbol nuevoArbol; 


        std::map<int, Nodo*> tempNodos;
        
        std::map<int, int> tempJerarquia;
        

        int proximoId = 2; 
        
        ErrorHandler::mostrarInfo("Arbol cargado desde '" + nombreArchivo + "'.");
        return true;

    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    }
    catch (const std::exception& e) {
        ErrorHandler::mostrarError(FilesystemException(std::string("Error al cargar: ") + e.what()));
        return false;
    }
    

    return false;
}
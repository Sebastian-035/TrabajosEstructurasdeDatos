#include "consola.h"
#include "gestor_json.h"
#include "error_handler.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <stdexcept>

Consola::Consola(Arbol& arbol) : arbol(arbol) {}

void Consola::ejecutar() {
    std::string comando;
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << " PROYECTO 1: SISTEMA DE ARCHIVOS JERARQUICO\n";
    std::cout << " Implementacion: Interfaz de Consola y Papelera\n";
    std::cout << " " << std::string(68, '-') << "\n";
    std::cout << " Sistema con validacion robusta y manejo de excepciones\n";
    std::cout << " Escriba 'help' para ver los comandos disponibles\n";
    std::cout << " Escriba 'exit' para salir del sistema\n";
    std::cout << std::string(70, '=') << "\n\n";

    while (true) {
        try {
            std::cout << "\n" << arbol.getRaiz()->getRutaCompleta() << " > ";

            if (!std::getline(std::cin, comando)) {
                std::cout << "\n\nSaliendo del sistema...\n";
                break;
            }

            if (comando.empty()) {
                continue;
            }
            std::string comando_low = comando;
            size_t pos_space = comando_low.find(' ');
            if (pos_space != std::string::npos) {
                std::transform(comando_low.begin(), comando_low.begin() + pos_space, comando_low.begin(), ::tolower);
            } else {
                std::transform(comando_low.begin(), comando_low.end(), comando_low.begin(), ::tolower);
            }

            if (comando_low == "exit") {
                if (ErrorHandler::confirmarAccion("¿Está seguro de que desea salir? (Se guardará automáticamente)")) {
                    std::cout << "Saliendo del sistema...\n";
                    break;
                }
                else {
                    std::cout << "Continuando...\n";
                    continue;
                }
            }

            procesarComando(comando);

        }
        catch (const FilesystemException& e) {
            ErrorHandler::mostrarError(e);
        }
        catch (const std::exception& e) {
            ErrorHandler::mostrarError(FilesystemException(std::string("Error inesperado en la consola: ") + e.what()));
        }
        catch (...) {
            ErrorHandler::mostrarError(FilesystemException("Error desconocido e irrecuperable en la consola."));
        }
    }
}


int Consola::obtenerId(const std::string& arg) {
    try {
        size_t pos;
        int id = std::stoi(arg, &pos);
        if (pos != arg.length()) {
            throw ArgumentosInvalidosException("comando", "El ID debe ser un número entero. Se encontró: '" + arg + "'");
        }
        if (id <= 0) {
            throw ArgumentosInvalidosException("comando", "El ID debe ser mayor a 0.");
        }
        return id;
    }
    catch (const std::invalid_argument&) {
        throw ArgumentosInvalidosException("comando", "El ID debe ser un número entero. Se encontró: '" + arg + "'");
    }
    catch (const std::out_of_range&) {
        throw ArgumentosInvalidosException("comando", "El ID está fuera del rango de números enteros.");
    }
}

void Consola::mostrarInfoNodo(Nodo* nodo) const {
    std::cout << std::string(50, '=') << "\n";
    std::cout << " INFORMACION DEL NODO\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::left << std::setw(20) << "ID:" << nodo->getId() << "\n";
    std::cout << std::left << std::setw(20) << "Nombre:" << nodo->getNombre() << "\n";
    std::cout << std::left << std::setw(20) << "Tipo:" << (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO") << "\n";
    
    std::string ruta = nodo->getPadre() ? nodo->getRutaCompleta() : "Raiz (/)";
    std::cout << std::left << std::setw(20) << "Ruta Completa:" << ruta << "\n";
    std::cout << std::left << std::setw(20) << "Hijos (Cant.):" << nodo->getCantidadHijos() << "\n";
    
    if (nodo->getTipo() == ARCHIVO) {
        std::cout << std::left << std::setw(20) << "Contenido (Snippet):";
        std::string contenido = nodo->getContenido();
        if (contenido.length() > 30) {
            std::cout << contenido.substr(0, 30) << "...\n";
        } else {
            std::cout << contenido << "\n";
        }
    }
    
    std::cout << std::string(50, '=') << "\n";
}

void Consola::mostrarAyuda() const {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "           AYUDA DEL SISTEMA DE ARCHIVOS JERARQUICO\n";
    std::cout << std::string(70, '=') << "\n";
    
    std::cout << std::left << std::setw(20) << "mkdir <nombre>" << "Crear una nueva carpeta.\n";
    std::cout << std::left << std::setw(20) << "touch <nombre>" << "Crear un nuevo archivo.\n";
    std::cout << std::left << std::setw(20) << "ls [id/ruta]" << "Listar contenido de una carpeta (o root por defecto).\n";
    std::cout << std::left << std::setw(20) << "cat <id/ruta>" << "Mostrar el contenido de un archivo.\n";
    std::cout << std::left << std::setw(20) << "rm <id/ruta>" << "Mover nodo a la papelera (borrado suave).\n";
    std::cout << std::left << std::setw(20) << "mv <id> <id_destino>" << "Mover nodo a otra carpeta.\n";
    std::cout << std::left << std::setw(20) << "rename <id> <nombre>" << "Renombrar un nodo.\n";
    std::cout << std::left << std::setw(20) << "path <id>" << "Mostrar la ruta completa de un nodo.\n";
    std::cout << std::left << std::setw(20) << "info" << "Mostrar información del estado del árbol.\n";
    std::cout << std::left << std::setw(20) << "preorden" << "Mostrar recorrido Preorden de todo el árbol.\n";
    std::cout << std::string(70, '-') << "\n";
    std::cout << std::left << std::setw(20) << "find <nombre>" << "Buscar nodos por nombre exacto.\n";
    std::cout << std::left << std::setw(20) << "search <prefijo>" << "Buscar nodos por prefijo (árbol Trie).\n";
    std::cout << std::left << std::setw(20) << "suggest <prefijo>" << "Autocompletar nombres.\n";
    std::cout << std::string(70, '-') << "\n";
    std::cout << std::left << std::setw(20) << "trash" << "Listar el contenido de la papelera.\n";
    std::cout << std::left << std::setw(20) << "restore <id>" << "Restaurar un nodo de la papelera.\n";
    std::cout << std::left << std::setw(20) << "cleartrash" << "Vaciar la papelera (eliminación permanente).\n";
    std::cout << std::string(70, '-') << "\n";
    std::cout << std::left << std::setw(20) << "save <archivo>" << "Guardar estado del árbol a un archivo JSON.\n";
    std::cout << std::left << std::setw(20) << "load <archivo>" << "Cargar estado del árbol desde un archivo JSON.\n";
    std::cout << std::left << std::setw(20) << "clear" << "Limpiar la pantalla de la consola.\n";
    std::cout << std::left << std::setw(20) << "exit" << "Salir del sistema (guarda automáticamente).\n";
    std::cout << std::string(70, '=') << "\n";
}


void Consola::procesarComando(const std::string& comando) {
    std::stringstream ss(comando);
    std::string comando_base;
    ss >> comando_base;
    
    std::transform(comando_base.begin(), comando_base.end(), comando_base.begin(), ::tolower);

    if (comando_base == "help") {
        mostrarAyuda();
    }
    else if (comando_base == "mkdir") {
        procesarMkdir(ss);
    }
    else if (comando_base == "touch") {
        procesarTouch(ss);
    }
    else if (comando_base == "ls") {
        procesarLs(ss);
    }
    else if (comando_base == "cat") {
        procesarCat(ss);
    }
    else if (comando_base == "rm") {
        procesarRm(ss);
    }
    else if (comando_base == "mv") {
        procesarMv(ss);
    }
    else if (comando_base == "rename") {
        procesarRename(ss);
    }
    else if (comando_base == "path") {
        procesarPath(ss);
    }
    else if (comando_base == "preorden") {
        procesarPreorden();
    }
    else if (comando_base == "find") {
        procesarFind(ss);
    }
    else if (comando_base == "search") {
        procesarSearch(ss);
    }
    else if (comando_base == "suggest") {
        procesarSuggest(ss);
    }
    else if (comando_base == "save") {
        procesarSave(ss);
    }
    else if (comando_base == "load") {
        procesarLoad(ss);
    }
    else if (comando_base == "info") {
        procesarInfo();
    }
    else if (comando_base == "clear") {
        procesarClear();
    }
    else if (comando_base == "trash") {
        procesarTrash();
    }
    else if (comando_base == "restore") {
        procesarRestore(ss);
    }
    else if (comando_base == "cleartrash") {
        procesarClearTrash();
    }
    else {
        throw ComandoNoReconocidoException(comando_base);
    }
}


void Consola::procesarMkdir(std::stringstream& ss) {
    std::string nombre;
    if (!(ss >> nombre)) {
        throw ArgumentosInvalidosException("mkdir", "mkdir <nombre_carpeta>");
    }
    arbol.crearNodo(nombre, CARPETA, "", arbol.getRaiz());
}

void Consola::procesarTouch(std::stringstream& ss) {
    std::string nombre;
    if (!(ss >> nombre)) {
        throw ArgumentosInvalidosException("touch", "touch <nombre_archivo> [contenido]");
    }

    std::string contenido = "";
    std::string resto;
    if (std::getline(ss, resto)) {
        size_t first_char = resto.find_first_not_of(' ');
        if (std::string::npos != first_char) {
            contenido = resto.substr(first_char);
        }
    }

    arbol.crearNodo(nombre, ARCHIVO, contenido, arbol.getRaiz());
}

void Consola::procesarLs(std::stringstream& ss) {
    std::string arg;
    int id = arbol.getRaiz()->getId(); 

    if (ss >> arg) {
        try {
            id = obtenerId(arg);
        }
        catch (const ArgumentosInvalidosException&) {
            try {
                Nodo* nodo = arbol.buscarNodoPorRuta(arg);
                id = nodo->getId();
            }
            catch (const FilesystemException& e) {
                throw e; 
            }
        }
    }
    
    arbol.listarHijos(id);
}

void Consola::procesarCat(std::stringstream& ss) {
    std::string arg;
    if (!(ss >> arg)) {
        throw ArgumentosInvalidosException("cat", "cat <id/ruta>");
    }

    Nodo* nodo = nullptr;
    try {
        int id = obtenerId(arg);
        nodo = arbol.buscarNodo(id);
    }
    catch (const ArgumentosInvalidosException&) {
        nodo = arbol.buscarNodoPorRuta(arg);
    }

    if (nodo->getTipo() == CARPETA) {
        throw OperacionInvalidaException("cat", "No se puede mostrar el contenido de una carpeta. Use 'ls'.");
    }

    std::cout << "\n--- Contenido de '" << nodo->getNombre() << "' (ID: " << nodo->getId() << ") ---\n";
    std::cout << nodo->getContenido() << "\n";
    std::cout << "--------------------------------------------------------\n";
}

void Consola::procesarRm(std::stringstream& ss) {
    std::string arg;
    if (!(ss >> arg)) {
        throw ArgumentosInvalidosException("rm", "rm <id>");
    }
    
    int id = obtenerId(arg);
    
    if (ErrorHandler::confirmarAccion("¿Está seguro de que desea mover el nodo ID " + std::to_string(id) + " a la papelera?")) {
        arbol.eliminarNodo(id);
    } else {
        ErrorHandler::mostrarInfo("Operación de eliminación cancelada.");
    }
}

void Consola::procesarMv(std::stringstream& ss) {
    std::string arg_origen, arg_destino;
    if (!(ss >> arg_origen >> arg_destino)) {
        throw ArgumentosInvalidosException("mv", "mv <id_origen> <id_destino>");
    }

    int idOrigen = obtenerId(arg_origen);
    int idDestino = obtenerId(arg_destino);

    arbol.moverNodo(idOrigen, idDestino);
}

void Consola::procesarRename(std::stringstream& ss) {
    std::string arg_id, nuevo_nombre;
    if (!(ss >> arg_id >> nuevo_nombre)) {
        throw ArgumentosInvalidosException("rename", "rename <id> <nuevo_nombre>");
    }

    int id = obtenerId(arg_id);
    arbol.renombrarNodo(id, nuevo_nombre);
}

void Consola::procesarPath(std::stringstream& ss) {
    std::string arg;
    if (!(ss >> arg)) {
        throw ArgumentosInvalidosException("path", "path <id>");
    }

    int id = obtenerId(arg);
    std::string ruta = arbol.obtenerRutaNodo(id);
    if (!ruta.empty()) {
        std::cout << "Ruta completa del nodo ID " << id << ": " << ruta << "\n";
    }
}

void Consola::procesarPreorden() {
    std::cout << "\n--- Recorrido Preorden ---\n";
    std::vector<Nodo*> nodos = arbol.recorridoPreorden();
    
    std::cout << std::left << std::setw(8) << "ID"
              << std::left << std::setw(15) << "TIPO"
              << "RUTA" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (Nodo* nodo : nodos) {
        std::string tipoStr = (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO");
        std::cout << std::left << std::setw(8) << nodo->getId()
                  << std::left << std::setw(15) << tipoStr
                  << nodo->getRutaCompleta() << std::endl;
    }
    std::cout << std::string(70, '-') << std::endl;
}

void Consola::procesarFind(std::stringstream& ss) {
    std::string nombre;
    if (!(ss >> nombre)) {
        throw ArgumentosInvalidosException("find", "find <nombre_exacto>");
    }

    std::cout << "\n--- Resultados de busqueda exacta para '" << nombre << "' ---\n";
    std::vector<Nodo*> resultados = arbol.buscarPorNombreExacto(nombre);

    if (resultados.empty()) {
        std::cout << "No se encontraron nodos con ese nombre exacto.\n";
    } else {
        std::cout << std::left << std::setw(8) << "ID"
                  << std::left << std::setw(15) << "TIPO"
                  << "RUTA" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        for (Nodo* nodo : resultados) {
            std::string tipoStr = (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO");
            std::cout << std::left << std::setw(8) << nodo->getId()
                      << std::left << std::setw(15) << tipoStr
                      << nodo->getRutaCompleta() << std::endl;
        }
        std::cout << std::string(70, '-') << std::endl;
    }
}

void Consola::procesarSearch(std::stringstream& ss) {
    std::string prefijo;
    if (!(ss >> prefijo)) {
        throw ArgumentosInvalidosException("search", "search <prefijo>");
    }

    std::cout << "\n--- Resultados de busqueda por prefijo '" << prefijo << "' ---\n";
    auto resultados = arbol.buscarPorPrefijo(prefijo);

    if (resultados.empty()) {
        std::cout << "No se encontraron nodos con ese prefijo.\n";
    } else {
        std::cout << std::left << std::setw(25) << "NOMBRE COMPLETO"
                  << "CANT. NODOS" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        for (const auto& par : resultados) {
            std::cout << std::left << std::setw(25) << par.first
                      << par.second.size() << std::endl;
        }
        std::cout << std::string(70, '-') << std::endl;
    }
}

void Consola::procesarSuggest(std::stringstream& ss) {
    std::string prefijo;
    if (!(ss >> prefijo)) {
        throw ArgumentosInvalidosException("suggest", "suggest <prefijo>");
    }

    std::cout << "\n--- Sugerencias de autocompletado para '" << prefijo << "' ---\n";
    std::vector<std::string> sugerencias = arbol.autocompletarNombres(prefijo);

    if (sugerencias.empty()) {
        std::cout << "No hay sugerencias.\n";
    } else {
        for (const std::string& sug : sugerencias) {
            std::cout << "- " << sug << "\n";
        }
    }
}

void Consola::procesarSave(std::stringstream& ss) {
    std::string archivo = "datos.json"; 
    std::string arg;
    if (ss >> arg) {
        archivo = arg;
    }

    GestorJSON::guardarArbol(arbol, archivo);
}

void Consola::procesarLoad(std::stringstream& ss) {
    std::string archivo = "datos.json"; 
    std::string arg;
    if (ss >> arg) {
        archivo = arg;
    }

    if (ErrorHandler::confirmarAccion("¿Está seguro de que desea cargar el arbol de '" + archivo + "'? (Perderá el estado actual)")) {
        GestorJSON::cargarArbol(arbol, archivo);
    } else {
        ErrorHandler::mostrarInfo("Operación de carga cancelada.");
    }
}

void Consola::procesarInfo() {
    std::cout << "\n--- Informacion del Sistema de Archivos ---\n";
    std::cout << std::left << std::setw(30) << "Tamaño del Arbol:" << arbol.getTamano() << " nodos\n";
    std::cout << std::left << std::setw(30) << "Tamaño de la Papelera:" << arbol.getTamanoPapelera() << " nodos\n";
    std::cout << std::left << std::setw(30) << "Proximo ID a usar:" << arbol.getProximoId() << "\n";
    std::cout << std::left << std::setw(30) << "Altura (Simple):" << arbol.getAltura() << "\n";
}

void Consola::procesarClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void Consola::procesarTrash() {
    std::cout << "\n--- Contenido de la Papelera (" << arbol.getTamanoPapelera() << " elementos) ---\n";
    std::vector<Nodo*> papelera = arbol.listarPapelera();
    
    std::cout << std::left << std::setw(8) << "ID"
              << std::left << std::setw(15) << "TIPO"
              << "NOMBRE ORIGINAL" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    if (papelera.empty()) {
        std::cout << "[Papelera vacia]" << std::endl;
    } else {
        for (Nodo* nodo : papelera) {
            std::string tipoStr = (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO");
            std::cout << std::left << std::setw(8) << nodo->getId()
                      << std::left << std::setw(15) << tipoStr
                      << nodo->getNombre() << std::endl;
        }
    }
    std::cout << std::string(70, '-') << std::endl;
}

void Consola::procesarRestore(std::stringstream& ss) {
    std::string arg;
    if (!(ss >> arg)) {
        throw ArgumentosInvalidosException("restore", "restore <id_papelera>");
    }

    int id = obtenerId(arg);
    arbol.restaurarNodo(id);
}

void Consola::procesarClearTrash() {
    arbol.vaciarPapelera();
}
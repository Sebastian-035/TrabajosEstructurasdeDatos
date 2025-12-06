#include "consola.h"
#include "gestor_json.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

Consola::Consola(Arbol& arbol) : arbol(arbol) {}

void Consola::mostrarMenu() const {
    std::cout << "\n=== SISTEMA DE ARCHIVOS JERARQUICO ===\n";
    std::cout << "Comandos disponibles:\n";
    std::cout << "  mkdir <nombre>      - Crear carpeta\n";
    std::cout << "  touch <nombre>      - Crear archivo\n";
    std::cout << "  ls [id]             - Listar contenido\n";
    std::cout << "  rm <id>             - Eliminar nodo\n";
    std::cout << "  mv <id> <destino>   - Mover nodo\n";
    std::cout << "  rename <id> <nombre>- Renombrar nodo\n";
    std::cout << "  path <id>           - Mostrar ruta completa\n";
    std::cout << "  preorden            - Mostrar recorrido preorden\n";
    std::cout << "  find <nombre>       - Buscar nodos por nombre exacto\n";
    std::cout << "  search <prefijo>    - Buscar por prefijo\n";
    std::cout << "  suggest <prefijo>   - Autocompletar nombres\n";
    std::cout << "  save <archivo>      - Guardar arbol\n";
    std::cout << "  load <archivo>      - Cargar arbol\n";
    std::cout << "  info                - Mostrar informacion del arbol\n";
    std::cout << "  help                - Mostrar ayuda\n";
    std::cout << "  exit                - Salir\n";
    std::cout << "===========================================\n";
}

void Consola::mostrarAyuda() const {
    std::cout << "\n=== AYUDA ===\n";
    std::cout << "Este sistema simula un sistema de archivos jerarquico.\n";
    std::cout << "Cada nodo tiene un ID unico que se muestra al listar contenido.\n";
    std::cout << "La raiz del sistema tiene ID 1.\n";
    std::cout << "Use 'ls' para ver los IDs de los nodos.\n\n";

    std::cout << "COMANDOS DE BUSQUEDA:\n";
    std::cout << "  find <nombre>       - Busca nodos con nombre exacto\n";
    std::cout << "  search <prefijo>    - Busca todos los nombres que empiezan con el prefijo\n";
    std::cout << "  suggest <prefijo>   - Muestra sugerencias de autocompletado\n\n";

    std::cout << "Ejemplos:\n";
    std::cout << "  mkdir Documentos     Crea una carpeta llamada Documentos\n";
    std::cout << "  touch archivo.txt    Crea un archivo llamado archivo.txt\n";
    std::cout << "  find proyecto        Busca nodos llamados 'proyecto'\n";
    std::cout << "  suggest doc          Sugiere nombres que empiezan con 'doc'\n";
    std::cout << "  rm 5                Elimina el nodo con ID 5\n";
    std::cout << "  mv 3 2              Mueve el nodo ID 3 al nodo ID 2\n";
}

void Consola::mostrarResultadosBusqueda(const std::vector<Nodo*>& resultados) const {
    if (resultados.empty()) {
        std::cout << "No se encontraron resultados.\n";
        return;
    }

    std::cout << "Resultados de busqueda (" << resultados.size() << " nodos):\n";
    std::cout << std::setw(5) << "ID"
        << std::setw(20) << "Nombre"
        << std::setw(10) << "Tipo"
        << std::setw(40) << "Ruta" << "\n";
    std::cout << std::string(75, '-') << "\n";

    for (Nodo* nodo : resultados) {
        std::cout << std::setw(5) << nodo->getId()
            << std::setw(20) << nodo->getNombre()
            << std::setw(10) << (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO")
            << std::setw(40) << nodo->getRutaCompleta() << "\n";
    }
}

void Consola::mostrarAutocompletado(const std::vector<std::string>& sugerencias) const {
    if (sugerencias.empty()) {
        std::cout << "No hay sugerencias de autocompletado.\n";
        return;
    }

    std::cout << "Sugerencias de autocompletado (" << sugerencias.size() << "):\n";
    for (size_t i = 0; i < sugerencias.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << sugerencias[i] << "\n";
    }
}

void Consola::procesarComando(const std::string& comando) {
    std::stringstream ss(comando);
    std::string cmd;
    ss >> cmd;

    // Convertir a minúsculas
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "mkdir") {
        std::string nombre;
        ss >> nombre;

        if (nombre.empty()) {
            std::cout << "Error: Debe especificar un nombre para la carpeta.\n";
            return;
        }

        Nodo* nuevo = arbol.crearNodo(nombre, CARPETA);
        if (nuevo) {
            std::cout << "Carpeta '" << nombre << "' creada con ID: " << nuevo->getId() << "\n";
        }
    }
    else if (cmd == "touch") {
        std::string nombre;
        ss >> nombre;

        if (nombre.empty()) {
            std::cout << "Error: Debe especificar un nombre para el archivo.\n";
            return;
        }

        Nodo* nuevo = arbol.crearNodo(nombre, ARCHIVO);
        if (nuevo) {
            std::cout << "Archivo '" << nombre << "' creado con ID: " << nuevo->getId() << "\n";
        }
    }
    else if (cmd == "ls") {
        std::string idStr;
        ss >> idStr;

        int id = 1; // Por defecto, listar la raíz

        if (!idStr.empty()) {
            try {
                id = std::stoi(idStr);
            }
            catch (...) {
                std::cout << "Error: ID invalido.\n";
                return;
            }
        }

        arbol.listarHijos(id);
    }
    else if (cmd == "rm") {
        std::string idStr;
        ss >> idStr;

        if (idStr.empty()) {
            std::cout << "Error: Debe especificar un ID.\n";
            return;
        }

        try {
            int id = std::stoi(idStr);
            if (arbol.eliminarNodo(id)) {
                std::cout << "Nodo eliminado correctamente.\n";
            }
        }
        catch (...) {
            std::cout << "Error: ID invalido.\n";
        }
    }
    else if (cmd == "mv") {
        std::string idOrigenStr, idDestinoStr;
        ss >> idOrigenStr >> idDestinoStr;

        if (idOrigenStr.empty() || idDestinoStr.empty()) {
            std::cout << "Error: Debe especificar ID de origen y destino.\n";
            return;
        }

        try {
            int idOrigen = std::stoi(idOrigenStr);
            int idDestino = std::stoi(idDestinoStr);

            if (arbol.moverNodo(idOrigen, idDestino)) {
                std::cout << "Nodo movido correctamente.\n";
            }
        }
        catch (...) {
            std::cout << "Error: ID(s) invalido(s).\n";
        }
    }
    else if (cmd == "rename") {
        std::string idStr, nuevoNombre;
        ss >> idStr >> nuevoNombre;

        if (idStr.empty() || nuevoNombre.empty()) {
            std::cout << "Error: Debe especificar ID y nuevo nombre.\n";
            return;
        }

        try {
            int id = std::stoi(idStr);
            if (arbol.renombrarNodo(id, nuevoNombre)) {
                std::cout << "Nodo renombrado correctamente.\n";
            }
        }
        catch (...) {
            std::cout << "Error: ID invalido.\n";
        }
    }
    else if (cmd == "path") {
        std::string idStr;
        ss >> idStr;

        if (idStr.empty()) {
            std::cout << "Error: Debe especificar un ID.\n";
            return;
        }

        try {
            int id = std::stoi(idStr);
            Nodo* nodo = arbol.buscarNodo(id);
            if (nodo) {
                std::cout << "Ruta completa: " << nodo->getRutaCompleta() << "\n";
            }
            else {
                std::cout << "Error: Nodo no encontrado.\n";
            }
        }
        catch (...) {
            std::cout << "Error: ID invalido.\n";
        }
    }
    else if (cmd == "preorden") {
        std::vector<Nodo*> recorrido = arbol.recorridoPreorden();
        std::cout << "Recorrido preorden (" << recorrido.size() << " nodos):\n";

        for (Nodo* nodo : recorrido) {
            std::cout << "  ID: " << nodo->getId()
                << " | Nombre: " << nodo->getNombre()
                << " | Tipo: " << (nodo->getTipo() == CARPETA ? "CARPETA" : "ARCHIVO")
                << " | Ruta: " << nodo->getRutaCompleta() << "\n";
        }
    }
    else if (cmd == "find") {
        std::string nombre;
        ss >> nombre;

        if (nombre.empty()) {
            std::cout << "Error: Debe especificar un nombre para buscar.\n";
            return;
        }

        std::vector<Nodo*> resultados = arbol.buscarPorNombreExacto(nombre);
        mostrarResultadosBusqueda(resultados);
    }
    else if (cmd == "search") {
        std::string prefijo;
        ss >> prefijo;

        if (prefijo.empty()) {
            std::cout << "Error: Debe especificar un prefijo para buscar.\n";
            return;
        }

        auto resultados = arbol.buscarPorPrefijo(prefijo);

        if (resultados.empty()) {
            std::cout << "No se encontraron nombres que empiecen con '" << prefijo << "'\n";
        }
        else {
            std::cout << "Resultados de busqueda por prefijo '" << prefijo << "':\n";
            for (const auto& resultado : resultados) {
                std::cout << "  Nombre: " << resultado.first << " (";
                std::cout << resultado.second.size() << " nodo(s) con este nombre)\n";

                for (int id : resultado.second) {
                    Nodo* nodo = arbol.buscarNodo(id);
                    if (nodo) {
                        std::cout << "    - ID: " << id
                            << ", Ruta: " << nodo->getRutaCompleta() << "\n";
                    }
                }
            }
        }
    }
    else if (cmd == "suggest") {
        std::string prefijo;
        ss >> prefijo;

        if (prefijo.empty()) {
            std::cout << "Error: Debe especificar un prefijo para autocompletar.\n";
            return;
        }

        std::vector<std::string> sugerencias = arbol.autocompletarNombres(prefijo);
        mostrarAutocompletado(sugerencias);
    }
    else if (cmd == "save") {
        std::string archivo;
        ss >> archivo;

        if (archivo.empty()) {
            archivo = "datos.json";
        }

        GestorJSON::guardarArbol(arbol, archivo);
    }
    else if (cmd == "load") {
        std::string archivo;
        ss >> archivo;

        if (archivo.empty()) {
            archivo = "datos.json";
        }

        GestorJSON::cargarArbol(arbol, archivo);
    }
    else if (cmd == "info") {
        std::cout << "\n=== INFORMACION DEL ARBOL ===\n";
        std::cout << "Total de nodos: " << arbol.getTamano() << "\n";
        std::cout << "Altura del arbol: " << arbol.getAltura() << "\n";
        std::cout << "Proximo ID disponible: " << arbol.getProximoId() << "\n";
        std::cout << "Nodo raiz: " << arbol.getRaiz()->getNombre()
            << " (ID: " << arbol.getRaiz()->getId() << ")\n";

        // Información adicional sobre índices
        std::cout << "Indices de busqueda: Trie y Mapa Hash activos\n";
    }
    else if (cmd == "help") {
        mostrarAyuda();
    }
    else if (cmd == "exit") {
        std::cout << "Saliendo del sistema...\n";
    }
    else {
        std::cout << "Comando no reconocido. Escriba 'help' para ver los comandos disponibles.\n";
    }
}

void Consola::ejecutar() {
    std::string comando;

    std::cout << "Bienvenido al Sistema de Archivos Jerarquico con Busqueda Inteligente\n";
    std::cout << "Implementacion: Dias 5-6 del cronograma (Trie y busqueda)\n";
    std::cout << "========================================================\n";

    while (true) {
        mostrarMenu();
        std::cout << "\n> ";

        std::getline(std::cin, comando);

        if (comando == "exit") {
            break;
        }

        procesarComando(comando);
    }
}
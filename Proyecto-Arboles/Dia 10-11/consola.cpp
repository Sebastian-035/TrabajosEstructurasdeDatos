#include "consola.h"

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
                std::transform(comando_low.begin(), comando_low.begin() + pos_space, 
                              comando_low.begin(), ::tolower);
            }
            else {
                std::transform(comando_low.begin(), comando_low.end(), 
                              comando_low.begin(), ::tolower);
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
        }
        else {
            std::cout << contenido << "\n";
        }
    }

    std::cout << std::string(50, '=') << "\n";
}

void Consola::mostrarAyuda() const {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "           AYUDA DEL SISTEMA DE ARCHIVOS JERARQUICO\n";
    std::cout << std::string(70, '=') << "\n";

    std::cout << std::left << std::setw(20) << "mkdir <nombre>" << "Crear una nueva carpeta. (Soporta espacios en el nombre)\n";  // MODIFICADO: Agregado nota sobre espacios
    std::cout << std::left << std::setw(20) << "touch <nombre>" << "Crear un nuevo archivo. (Soporta espacios en el nombre)\n";  // MODIFICADO: Agregado nota
    std::cout << std::left << std::setw(20) << "ls [id/ruta]" << "Listar contenido de una carpeta (o root por defecto). (Soporta espacios en ruta)\n";  // MODIFICADO
    std::cout << std::left << std::setw(20) << "cat <id/ruta>" << "Mostrar el contenido de un archivo. (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "rm <id/ruta>" << "Mover nodo a la papelera (borrado suave). (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "mv <id> <id_destino>" << "Mover nodo a otra carpeta.\n";
    std::cout << std::left << std::setw(20) << "rename <id> <nombre>" << "Renombrar un nodo. (Soporta espacios en el nuevo nombre)\n";
    std::cout << std::left << std::setw(20) << "path <id/ruta>" << "Mostrar ruta completa. (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "preorden" << "Listar todos los nodos en preorden.\n";
    std::cout << std::left << std::setw(20) << "find <nombre>" << "Buscar nodos por nombre exacto. (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "search <prefijo>" << "Buscar nodos por prefijo. (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "suggest <prefijo>" << "Sugerencias de autocompletado. (Soporta espacios)\n";
    std::cout << std::left << std::setw(20) << "save [archivo]" << "Guardar arbol en JSON (default: datos.json).\n";
    std::cout << std::left << std::setw(20) << "load [archivo]" << "Cargar arbol desde JSON (default: datos.json).\n";
    std::cout << std::left << std::setw(20) << "info" << "Mostrar informacion del sistema.\n";
    std::cout << std::left << std::setw(20) << "clear" << "Limpiar la pantalla.\n";
    std::cout << std::left << std::setw(20) << "trash" << "Listar contenido de la papelera.\n";
    std::cout << std::left << std::setw(20) << "restore <id>" << "Restaurar nodo de la papelera.\n";
    std::cout << std::left << std::setw(20) << "cleartrash" << "Vaciar papelera permanentemente.\n";
    std::cout << std::left << std::setw(20) << "help" << "Mostrar esta ayuda.\n";
    std::cout << std::left << std::setw(20) << "exit" << "Salir del sistema.\n";
    std::cout << std::string(70, '=') << "\n";
}

void Consola::procesarComando(const std::string& comando) {
    std::stringstream ss(comando);
    std::string cmd;
    ss >> cmd;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "mkdir") {
        procesarMkdir(ss);
    } else if (cmd == "touch") {
        procesarTouch(ss);
    } else if (cmd == "ls") {
        procesarLs(ss);
    } else if (cmd == "cat") {
        procesarCat(ss);
    } else if (cmd == "rm") {
        procesarRm(ss);
    } else if (cmd == "mv") {
        procesarMv(ss);
    } else if (cmd == "rename") {
        procesarRename(ss);
    } else if (cmd == "path") {
        procesarPath(ss);
    } else if (cmd == "preorden") {
        procesarPreorden();
    } else if (cmd == "find") {
        procesarFind(ss);
    } else if (cmd == "search") {
        procesarSearch(ss);
    } else if (cmd == "suggest") {
        procesarSuggest(ss);
    } else if (cmd == "save") {
        procesarSave(ss);
    } else if (cmd == "load") {
        procesarLoad(ss);
    } else if (cmd == "info") {
        procesarInfo();
    } else if (cmd == "clear") {
        procesarClear();
    } else if (cmd == "trash") {
        procesarTrash();
    } else if (cmd == "restore") {
        procesarRestore(ss);
    } else if (cmd == "cleartrash") {
        procesarClearTrash();
    } else if (cmd == "help") {
        mostrarAyuda();
    } else {
        throw ComandoNoReconocidoException(cmd);
    }
}

void Consola::procesarMkdir(std::stringstream& ss) {
    std::string nombre;
    std::getline(ss, nombre);  // MODIFICADO: Captura todo el resto como nombre (soporta espacios)
    nombre.erase(0, nombre.find_first_not_of(" \t"));  // Trim inicial
    nombre.erase(nombre.find_last_not_of(" \t") + 1);  // Trim final
    if (nombre.empty()) {
        throw ArgumentosInvalidosException("mkdir", "mkdir <nombre>");
    }
    arbol.crearNodo(nombre, CARPETA, "", arbol.getRaiz());
    ErrorHandler::mostrarInfo("Carpeta creada.");
}

void Consola::procesarTouch(std::stringstream& ss) {
    std::string nombre;
    std::getline(ss, nombre);  // MODIFICADO: Captura todo el resto como nombre (soporta espacios)
    nombre.erase(0, nombre.find_first_not_of(" \t"));
    nombre.erase(nombre.find_last_not_of(" \t") + 1);
    if (nombre.empty()) {
        throw ArgumentosInvalidosException("touch", "touch <nombre>");
    }
    arbol.crearNodo(nombre, ARCHIVO, "", arbol.getRaiz());
    ErrorHandler::mostrarInfo("Archivo creado.");
}

void Consola::procesarLs(std::stringstream& ss) {
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como arg (id o ruta, soporta espacios en ruta)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);

    Nodo* carpeta;
    if (arg.empty()) {
        carpeta = arbol.getRaiz();
    } else {
        try {
            int id = obtenerId(arg);
            carpeta = arbol.buscarNodo(id);
            if (!carpeta) {
                throw NodoNotFoundException(id);
            }
        } catch (const ArgumentosInvalidosException&) {
            carpeta = arbol.buscarNodoPorRuta(arg);
            if (!carpeta) {
                throw NodoNotFoundExceptionStr(arg);
            }
        }
    }

    if (carpeta->getTipo() != CARPETA) {
        throw OperacionInvalidaException("ls", "El nodo no es una carpeta");
    }

    std::cout << "\n--- Contenido de " << carpeta->getRutaCompleta() << " (" << carpeta->getCantidadHijos() << " elementos) ---\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::left << std::setw(15) << "TIPO"
              << "NOMBRE" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    const auto& hijos = carpeta->getHijos();
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

void Consola::procesarCat(std::stringstream& ss) {
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como arg (soporta espacios)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);
    if (arg.empty()) {
        throw ArgumentosInvalidosException("cat", "cat <id/ruta>");
    }

    Nodo* nodo;
    try {
        int id = obtenerId(arg);
        nodo = arbol.buscarNodo(id);
        if (!nodo) {
            throw NodoNotFoundException(id);
        }
    } catch (const ArgumentosInvalidosException&) {
        nodo = arbol.buscarNodoPorRuta(arg);
        if (!nodo) {
            throw NodoNotFoundExceptionStr(arg);
        }
    }

    if (nodo->getTipo() != ARCHIVO) {
        throw OperacionInvalidaException("cat", "El nodo no es un archivo");
    }

    std::cout << "\n--- Contenido de " << nodo->getNombre() << " ---\n";
    std::cout << nodo->getContenido() << std::endl;
    std::cout << std::string(70, '-') << std::endl;
}

void Consola::procesarRm(std::stringstream& ss) {
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como arg (soporta espacios)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);
    if (arg.empty()) {
        throw ArgumentosInvalidosException("rm", "rm <id/ruta>");
    }

    int id;
    try {
        id = obtenerId(arg);
    } catch (const ArgumentosInvalidosException&) {
        Nodo* nodo = arbol.buscarNodoPorRuta(arg);
        if (!nodo) {
            throw NodoNotFoundExceptionStr(arg);
        }
        id = nodo->getId();
    }

    if (ErrorHandler::confirmarAccion("¿Está seguro de mover el nodo ID " + std::to_string(id) + " a la papelera?")) {
        arbol.eliminarNodo(id);
        ErrorHandler::mostrarInfo("Nodo movido a la papelera.");
    } else {
        ErrorHandler::mostrarInfo("Operación cancelada.");
    }
}

void Consola::procesarMv(std::stringstream& ss) {
    std::string arg_origen, arg_destino;
    if (!(ss >> arg_origen >> arg_destino)) {
        throw ArgumentosInvalidosException("mv", "mv <id_origen> <id_destino>");
    }
    int id_origen = obtenerId(arg_origen);
    int id_destino = obtenerId(arg_destino);
    arbol.moverNodo(id_origen, id_destino);
    ErrorHandler::mostrarInfo("Nodo movido.");
}

void Consola::procesarRename(std::stringstream& ss) {
    std::string arg_id;
    if (!(ss >> arg_id)) {
        throw ArgumentosInvalidosException("rename", "rename <id> <nuevo_nombre>");
    }
    int id = obtenerId(arg_id);

    std::string nuevo_nombre;
    std::getline(ss, nuevo_nombre);  // MODIFICADO: Captura el resto como nuevo_nombre (soporta espacios)
    nuevo_nombre.erase(0, nuevo_nombre.find_first_not_of(" \t"));
    nuevo_nombre.erase(nuevo_nombre.find_last_not_of(" \t") + 1);
    if (nuevo_nombre.empty()) {
        throw ArgumentosInvalidosException("rename", "rename <id> <nuevo_nombre>");
    }

    arbol.renombrarNodo(id, nuevo_nombre);
    ErrorHandler::mostrarInfo("Nodo renombrado.");
}

void Consola::procesarPath(std::stringstream& ss) {
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como arg (soporta espacios)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);
    if (arg.empty()) {
        throw ArgumentosInvalidosException("path", "path <id/ruta>");
    }

    Nodo* nodo;
    try {
        int id = obtenerId(arg);
        nodo = arbol.buscarNodo(id);
        if (!nodo) {
            throw NodoNotFoundException(id);
        }
    } catch (const ArgumentosInvalidosException&) {
        nodo = arbol.buscarNodoPorRuta(arg);
        if (!nodo) {
            throw NodoNotFoundExceptionStr(arg);
        }
    }

    std::cout << "\nRuta completa: " << nodo->getRutaCompleta() << std::endl;
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
    std::getline(ss, nombre);  // MODIFICADO: Captura todo el resto como nombre (soporta espacios)
    nombre.erase(0, nombre.find_first_not_of(" \t"));
    nombre.erase(nombre.find_last_not_of(" \t") + 1);
    if (nombre.empty()) {
        throw ArgumentosInvalidosException("find", "find <nombre_exacto>");
    }

    std::cout << "\n--- Resultados de busqueda exacta para '" << nombre << "' ---\n";
    std::vector<Nodo*> resultados = arbol.buscarPorNombreExacto(nombre);

    if (resultados.empty()) {
        std::cout << "No se encontraron nodos con ese nombre exacto.\n";
    }
    else {
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
    std::getline(ss, prefijo);  // MODIFICADO: Captura todo el resto como prefijo (soporta espacios)
    prefijo.erase(0, prefijo.find_first_not_of(" \t"));
    prefijo.erase(prefijo.find_last_not_of(" \t") + 1);
    if (prefijo.empty()) {
        throw ArgumentosInvalidosException("search", "search <prefijo>");
    }

    std::cout << "\n--- Resultados de busqueda por prefijo '" << prefijo << "' ---\n";
    auto resultados = arbol.buscarPorPrefijo(prefijo);

    if (resultados.empty()) {
        std::cout << "No se encontraron nodos con ese prefijo.\n";
    }
    else {
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
    std::getline(ss, prefijo);  // MODIFICADO: Captura todo el resto como prefijo (soporta espacios)
    prefijo.erase(0, prefijo.find_first_not_of(" \t"));
    prefijo.erase(prefijo.find_last_not_of(" \t") + 1);
    if (prefijo.empty()) {
        throw ArgumentosInvalidosException("suggest", "suggest <prefijo>");
    }

    std::cout << "\n--- Sugerencias de autocompletado para '" << prefijo << "' ---\n";
    std::vector<std::string> sugerencias = arbol.autocompletarNombres(prefijo);

    if (sugerencias.empty()) {
        std::cout << "No hay sugerencias.\n";
    }
    else {
        for (const std::string& sug : sugerencias) {
            std::cout << "- " << sug << "\n";
        }
    }
}

void Consola::procesarSave(std::stringstream& ss) {
    std::string archivo = "datos.json";
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como archivo (opcional, soporta espacios)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);
    if (!arg.empty()) {
        archivo = arg;
    }

    GestorJSON::guardarArbol(arbol, archivo);
}

void Consola::procesarLoad(std::stringstream& ss) {
    std::string archivo = "datos.json";
    std::string arg;
    std::getline(ss, arg);  // MODIFICADO: Captura todo el resto como archivo (opcional, soporta espacios)
    arg.erase(0, arg.find_first_not_of(" \t"));
    arg.erase(arg.find_last_not_of(" \t") + 1);
    if (!arg.empty()) {
        archivo = arg;
    }

    if (ErrorHandler::confirmarAccion("¿Está seguro de que desea cargar el arbol de '" + archivo + "'? (Perderá el estado actual)")) {
        GestorJSON::cargarArbol(arbol, archivo);
    }
    else {
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
    }
    else {
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
    ErrorHandler::mostrarInfo("Nodo restaurado.");
}

void Consola::procesarClearTrash() {
    if (ErrorHandler::confirmarAccion("¿Está seguro de vaciar la papelera permanentemente?")) {
        arbol.vaciarPapelera();
    } else {
        ErrorHandler::mostrarInfo("Operación cancelada.");
    }
}
#include "arbol.h"
#include "consola.h"
#include "gestor_json.h"
#include "error_handler.h"
#include <iostream>
#include <cstdlib>

// Función para manejar excepciones no capturadas
void terminateHandler() {
    try {
        std::exception_ptr eptr = std::current_exception();
        if (eptr) {
            std::rethrow_exception(eptr);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\n*** ERROR CRITICO NO MANEJADO ***\n";
        std::cerr << "Descripcion: " << e.what() << "\n";
        std::cerr << "El programa se cerrara por este error.\n";
    }
    catch (...) {
        std::cerr << "\n*** ERROR DESCONOCIDO NO MANEJADO ***\n";
        std::cerr << "El programa se cerrara por este error.\n";
    }

    std::exit(EXIT_FAILURE);
}

int main() {
    std::set_terminate(terminateHandler);

    try {
        // Crear el árbol
        Arbol arbol;
        ErrorHandler::mostrarInfo("Sistema inicializado correctamente");


        ErrorHandler::mostrarInfo("Intentando cargar datos previos...");
        if (!GestorJSON::cargarArbol(arbol, "datos.json")) {
            ErrorHandler::mostrarInfo("Creado nuevo arbol vacio (o cargando la raiz)");
        }

        Consola consola(arbol);
        consola.ejecutar();

        ErrorHandler::mostrarInfo("Guardando estado del arbol...");
        GestorJSON::guardarArbol(arbol, "datos.json");

        ErrorHandler::mostrarInfo("Programa terminado correctamente");

    }
    catch (const std::exception& e) {
        ErrorHandler::mostrarError(FilesystemException(
            std::string("Error critico en main: ") + e.what()));
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error critico desconocido en main."));
    }

    return 0;
}
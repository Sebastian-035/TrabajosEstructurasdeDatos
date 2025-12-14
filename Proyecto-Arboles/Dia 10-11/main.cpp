#include "consola.h"
#include <cstdlib>
#include <exception>

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
        Arbol arbol;
        ErrorHandler::mostrarInfo("Sistema inicializado correctamente");

        ErrorHandler::mostrarInfo("Intentando cargar datos previos desde 'datos.json'...");
        if (!GestorJSON::cargarArbol(arbol, "datos.json")) {
            ErrorHandler::mostrarInfo("No se encontraron datos previos. Iniciando con arbol vacio.");
        }
        else {
            ErrorHandler::mostrarInfo("Datos cargados exitosamente.");
        }

        Consola consola(arbol);
        consola.ejecutar();

        ErrorHandler::mostrarInfo("Guardando estado del arbol en 'datos.json'...");
        GestorJSON::guardarArbol(arbol, "datos.json");

        ErrorHandler::mostrarInfo("Programa terminado correctamente");

    }
    catch (const std::exception& e) {
        ErrorHandler::mostrarError(FilesystemException(
            std::string("Error critico en main: ") + e.what()));
        return 1;
    }
    catch (...) {
        ErrorHandler::mostrarError(FilesystemException("Error critico desconocido en main."));
        return 1;
    }

    return 0;
}
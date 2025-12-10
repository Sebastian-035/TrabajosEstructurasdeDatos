#include "error_handler.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <limits>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

void ErrorHandler::logError(const std::string& tipo, const std::string& mensaje) {
    // Obtener fecha y hora actual
    std::time_t ahora = std::time(nullptr);
    std::tm* tiempoLocal = std::localtime(&ahora);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempoLocal);

    // Mostrar en consola con colores (si está disponible)
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (tipo == "ERROR") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    else if (tipo == "ADVERTENCIA") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else if (tipo == "INFO") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
#endif

    std::cout << "\n[" << buffer << "] [" << tipo << "] " << mensaje << std::endl;

#ifdef _WIN32
    // Restaurar colores de la consola a blanco/gris por defecto
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void ErrorHandler::mostrarError(const std::exception& e) {
    logError("ERROR", e.what());
}

void ErrorHandler::mostrarAdvertencia(const std::string& mensaje) {
    logError("ADVERTENCIA", mensaje);
}

void ErrorHandler::mostrarInfo(const std::string& mensaje) {
    logError("INFO", mensaje);
}

bool ErrorHandler::confirmarAccion(const std::string& mensaje) {
    std::string respuesta;
    
    // Configurar color para la advertencia de confirmación
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

    std::cout << "\n[CONFIRMACION] " << mensaje << " (s/n): ";

#ifdef _WIN32
    // Restaurar color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

    std::getline(std::cin, respuesta);

    // Limpiar entrada para evitar errores futuros
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::transform(respuesta.begin(), respuesta.end(), respuesta.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return respuesta == "s" || respuesta == "si";
}
#include "error_handler.h"

// ==================== EXCEPCIONES ====================
FilesystemException::FilesystemException(const std::string& message) : std::runtime_error(message) {}

NodoNotFoundException::NodoNotFoundException(int id)
    : FilesystemException("Nodo con ID " + std::to_string(id) + " no encontrado") {}

NodoNotFoundExceptionStr::NodoNotFoundExceptionStr(const std::string& nombre)
    : FilesystemException("Nodo '" + nombre + "' no encontrado") {}

NombreDuplicadoException::NombreDuplicadoException(const std::string& nombre, int idPadre)
    : FilesystemException("Ya existe un nodo con nombre '" + nombre +
        "' en el nodo padre ID " + std::to_string(idPadre)) {}

OperacionInvalidaException::OperacionInvalidaException(const std::string& operacion, const std::string& motivo)
    : FilesystemException("Operacion '" + operacion + "' invalida: " + motivo) {}

DestinoInvalidoException::DestinoInvalidoException(int idDestino)
    : FilesystemException("Destino ID " + std::to_string(idDestino) +
        " no es una carpeta") {}

ArgumentosInvalidosException::ArgumentosInvalidosException(const std::string& comando, const std::string& usoCorrecto)
    : FilesystemException("Argumentos invalidos para '" + comando + "'. Uso: " + usoCorrecto) {}

ArchivoIOException::ArchivoIOException(const std::string& archivo, const std::string& operacion)
    : FilesystemException("Error de E/S en archivo '" + archivo +
        "' durante operacion: " + operacion) {}

ArchivoVacioException::ArchivoVacioException(const std::string& archivo)
    : FilesystemException("Archivo '" + archivo + "' está vacío") {}

FormatoJSONException::FormatoJSONException(const std::string& mensaje)
    : FilesystemException("Error de formato JSON: " + mensaje) {}

ComandoNoReconocidoException::ComandoNoReconocidoException(const std::string& comando)
    : FilesystemException("Comando '" + comando + "' no reconocido. Escriba 'help' para ver los comandos.") {}

// ==================== ERROR HANDLER ====================
void ErrorHandler::logError(const std::string& tipo, const std::string& mensaje) {
    std::time_t ahora = std::time(nullptr);
    std::tm tiempoLocal;
#ifdef _WIN32
    localtime_s(&tiempoLocal, &ahora);
#else
    localtime_r(&ahora, &tiempoLocal);
#endif

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tiempoLocal);

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

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

    std::cout << "\n[CONFIRMACION] " << mensaje << " (s/n): ";

#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

    std::getline(std::cin, respuesta);

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::transform(respuesta.begin(), respuesta.end(), respuesta.begin(),
        [](unsigned char c) { return (char)std::tolower(c); });

    return respuesta == "s" || respuesta == "si";
}
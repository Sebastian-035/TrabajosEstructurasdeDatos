#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <cctype>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#define FOREGROUND_RED 0x0004
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_INTENSITY 0x0008
#define HANDLE void*
inline void SetConsoleTextAttribute(HANDLE, int) {}  // Added inline to fix multiple definitions
#endif

// ==================== EXCEPCIONES ====================
class FilesystemException : public std::runtime_error {
public:
    FilesystemException(const std::string& message);
};

class NodoNotFoundException : public FilesystemException {
public:
    NodoNotFoundException(int id);
};

class NodoNotFoundExceptionStr : public FilesystemException {
public:
    NodoNotFoundExceptionStr(const std::string& nombre);
};

class NombreDuplicadoException : public FilesystemException {
public:
    NombreDuplicadoException(const std::string& nombre, int idPadre);
};

class OperacionInvalidaException : public FilesystemException {
public:
    OperacionInvalidaException(const std::string& operacion, const std::string& motivo);
};

class DestinoInvalidoException : public FilesystemException {
public:
    DestinoInvalidoException(int idDestino);
};

class ArgumentosInvalidosException : public FilesystemException {
public:
    ArgumentosInvalidosException(const std::string& comando, const std::string& usoCorrecto);
};

class ArchivoIOException : public FilesystemException {
public:
    ArchivoIOException(const std::string& archivo, const std::string& operacion);
};

class ArchivoVacioException : public FilesystemException {
public:
    ArchivoVacioException(const std::string& archivo);
};

class FormatoJSONException : public FilesystemException {
public:
    FormatoJSONException(const std::string& mensaje);
};

class ComandoNoReconocidoException : public FilesystemException {
public:
    ComandoNoReconocidoException(const std::string& comando);
};

// ==================== ERROR HANDLER ====================
class ErrorHandler {
private:
    static void logError(const std::string& tipo, const std::string& mensaje);

public:
    static void mostrarError(const std::exception& e);
    static void mostrarAdvertencia(const std::string& mensaje);
    static void mostrarInfo(const std::string& mensaje);
    static bool confirmarAccion(const std::string& mensaje);
};

#endif // ERROR_HANDLER_H
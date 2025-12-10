#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <stdexcept>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// Clase base para excepciones del sistema de archivos
class FilesystemException : public std::runtime_error {
public:
    FilesystemException(const std::string& message) : std::runtime_error(message) {}
};

// Excepciones específicas
class NodoNotFoundException : public FilesystemException {
public:
    NodoNotFoundException(int id)
        : FilesystemException("Nodo con ID " + std::to_string(id) + " no encontrado") {
    }
};

class NodoNotFoundExceptionStr : public FilesystemException {
public:
    NodoNotFoundExceptionStr(const std::string& nombre)
        : FilesystemException("Nodo '" + nombre + "' no encontrado") {
    }
};

class NombreDuplicadoException : public FilesystemException {
public:
    NombreDuplicadoException(const std::string& nombre, int idPadre)
        : FilesystemException("Ya existe un nodo con nombre '" + nombre +
            "' en el nodo padre ID " + std::to_string(idPadre)) {
    }
};

class OperacionInvalidaException : public FilesystemException {
public:
    OperacionInvalidaException(const std::string& operacion, const std::string& motivo)
        : FilesystemException("Operacion '" + operacion + "' invalida: " + motivo) {
    }
};

class DestinoInvalidoException : public FilesystemException {
public:
    DestinoInvalidoException(int idDestino)
        : FilesystemException("Destino ID " + std::to_string(idDestino) +
            " no es una carpeta") {
    }
};

class ArgumentosInvalidosException : public FilesystemException {
public:
    ArgumentosInvalidosException(const std::string& comando, const std::string& usoCorrecto)
        : FilesystemException("Argumentos invalidos para '" + comando + "'. Uso: " + usoCorrecto) {
    }
};

class ArchivoIOException : public FilesystemException {
public:
    ArchivoIOException(const std::string& archivo, const std::string& operacion)
        : FilesystemException("Error de E/S en archivo '" + archivo +
            "' durante operacion: " + operacion) {
    }
};

class FormatoJSONException : public FilesystemException {
public:
    FormatoJSONException(const std::string& mensaje)
        : FilesystemException("Error de formato JSON: " + mensaje) {
    }
};

class ComandoNoReconocidoException : public FilesystemException {
public:
    ComandoNoReconocidoException(const std::string& comando)
        : FilesystemException("Comando '" + comando + "' no reconocido. Escriba 'help' para ver los comandos.") {
    }
};

// Clase para manejar y mostrar errores
class ErrorHandler {
private:
    static void logError(const std::string& tipo, const std::string& mensaje);

public:
    static void mostrarError(const std::exception& e);
    static void mostrarAdvertencia(const std::string& mensaje);
    static void mostrarInfo(const std::string& mensaje);
    static bool confirmarAccion(const std::string& mensaje);
};

#endif
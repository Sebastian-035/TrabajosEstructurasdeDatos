#include <iostream>
#include <string>
using namespace std;

class Persona {
private:
    string nombre, apellido, sexo;
    unsigned int  edad;

public:
    Persona();  // Constructor
    void pedirDatos();
    void mostrarDatos();
};

// Implementación del constructor
Persona::Persona() {
    nombre = "";
    apellido = "";
    sexo = "";
    edad = 0;
}

// Implementación de pedirDatos
void Persona::pedirDatos() {
    cout << "\n=== INGRESO DE DATOS ===" << endl;
    cout << "Nombre: ";
    getline(cin >> ws, nombre);  // ws para limpiar buffer

    cout << "Apellido: ";
    getline(cin, apellido);

    cout << "Sexo (M/F): ";
    getline(cin, sexo);

    cout << "Edad: ";
    cin >> edad;

}

// Implementación de mostrarDatos
void Persona::mostrarDatos() {
    cout << "\n=== DATOS DE LA PERSONA ===" << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Apellido: " << apellido << endl;
    cout << "Sexo: " << sexo << endl;
    cout << "Edad: " << edad << " años" << endl;

    cout << "============================" << endl;
}

int main() {
    Persona personas[2];

    cout << "SISTEMA DE REGISTRO DE PERSONAS" << endl;
    cout << "Ingrese los datos de 2 personas:" << endl;

    // Pedir y mostrar datos
    for(int i = 0; i < 2; i++) {
        cout << "\nPersona " << (i + 1) << ":" << endl;
        personas[i].pedirDatos();
        personas[i].mostrarDatos();
    }

    cout << "\nRegistro completado exitosamente!" << endl;

    return 0;
}
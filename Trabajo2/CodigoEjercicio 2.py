class Persona:
    def __init__(self):
        self.nombre = ""
        self.apellido = ""
        self.sexo = ""
        self.edad = 0

    def pedir_datos(self):
        print("\n=== INGRESO DE DATOS ===")
        
        self.nombre = input("Nombre: ")
        self.apellido = input("Apellido: ")
        self.sexo = input("Sexo (M/F): ")
        
        # Conversión segura de la edad
        edad_input = input("Edad: ")
        try:
            self.edad = int(edad_input)
            if self.edad < 0:
                self.edad = 0
        except ValueError:
            self.edad = 0

    def mostrar_datos(self):
        print("\n=== DATOS DE LA PERSONA ===")
        print(f"Nombre: {self.nombre}")
        print(f"Apellido: {self.apellido}")
        print(f"Sexo: {self.sexo}")
        print(f"Edad: {self.edad} años")
        print("============================")

def main():
    personas = [None] * 2

    print("SISTEMA DE REGISTRO DE PERSONAS")
    print("Ingrese los datos de 2 personas:")

    # Inicializar la lista de personas
    for i in range(len(personas)):
        personas[i] = Persona()

    # Pedir y mostrar datos
    for i in range(len(personas)):
        print(f"\nPersona {i + 1}:")
        personas[i].pedir_datos()
        personas[i].mostrar_datos()

    print("\nRegistro completado exitosamente!")

if __name__ == "__main__":
    main()
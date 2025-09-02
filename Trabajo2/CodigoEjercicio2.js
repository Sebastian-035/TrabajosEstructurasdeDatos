class Persona {
    constructor() {
        this.nombre = "";
        this.apellido = "";
        this.sexo = "";
        this.edad = 0;
    }
    
    async pedirDatos() {
        const readline = require('readline').createInterface({
            input: process.stdin,
            output: process.stdout
        });
        
        return new Promise((resolve) => {
            console.log("\n=== INGRESO DE DATOS ===");
            
            readline.question("Nombre: ", (nombre) => {
                this.nombre = nombre;
                
                readline.question("Apellido: ", (apellido) => {
                    this.apellido = apellido;
                    
                    readline.question("Sexo (M/F): ", (sexo) => {
                        this.sexo = sexo;
                        
                        readline.question("Edad: ", (edad) => {
                            this.edad = parseInt(edad);
                            readline.close();
                            resolve();
                        });
                    });
                });
            });
        });
    }
    
    mostrarDatos() {
        console.log("\n=== DATOS DE LA PERSONA ===");
        console.log("Nombre: " + this.nombre);
        console.log("Apellido: " + this.apellido);
        console.log("Sexo: " + this.sexo);
        console.log("Edad: " + this.edad + " aÃ±os");
        console.log("============================");
    }
}

async function main() {
    const personas = [];
    
    console.log("SISTEMA DE REGISTRO DE PERSONAS");
    console.log("Ingrese los datos de 2 personas:");
    
    // Pedir y mostrar datos
    for(let i = 0; i < 2; i++) {
        console.log("\nPersona " + (i + 1) + ":");
        personas[i] = new Persona();
        await personas[i].pedirDatos();
        personas[i].mostrarDatos();
    }
    
    console.log("\nRegistro completado exitosamente!");
    process.exit(0);
}

// Ejecutar el programa
main().catch(console.error);
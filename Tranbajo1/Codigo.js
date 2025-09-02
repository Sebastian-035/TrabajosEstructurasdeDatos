// Declaración
let numeros = new Array(5);

// Inicialización
numeros[0] = 10;
numeros[1] = 20;
numeros[2] = 30;
numeros[3] = 40;
numeros[4] = 50;

// Asignación
numeros[2] = 100;

// Prueba en pantalla
console.log("Contenido del arreglo:");
for (let i = 0; i < numeros.length; i++) {
    console.log(`numeros[${i}] = ${numeros[i]}`);
}

// Suma de dos elementos
let suma = numeros[0] + numeros[2];
console.log(`\nSuma de numeros[0] + numeros[2] = ${suma}`);


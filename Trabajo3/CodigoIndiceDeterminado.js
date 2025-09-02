// Inserción en array
let numeros = [10, 20, 30, 40, 50];
let valorAInsertar = 75;
let indice = 2; // Insertar en la posición 2 (entre 20 y 30)

if (indice >= 0 && indice <= numeros.length) {
    numeros.splice(indice, 0, valorAInsertar);
} else {
    console.log("Índice fuera de rango.");
}

// Mostrar el resultado
console.log("Array actualizado: " + numeros.join(" "));


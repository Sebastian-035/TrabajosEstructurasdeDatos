// Búsqueda lineal
let arreglo = [12, 23, 57, 45, 34, 67, 89, 34];
let valorBuscado = 34;

let indices = [];
arreglo.forEach((elemento, index) => {
    if (elemento === valorBuscado) {
        indices.push(index);
    }
});

if (indices.length > 0) {
    console.log("Valor encontrado en los índices: " + indices.join(", "));
} else {
    console.log("Valor no encontrado en el arreglo.");
}
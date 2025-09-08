const FILAS = 3;
const COLUMNAS = 4;

const matriz = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12]
];

// Recorrer por renglones (filas)
console.log("Recorrido por renglones:");
for (let fila = 0; fila < FILAS; fila++) {
    let output = `Renglón ${fila}: `;
    for (let col = 0; col < COLUMNAS; col++) {
        output += `${matriz[fila][col]} `;
    }
    console.log(output);
}
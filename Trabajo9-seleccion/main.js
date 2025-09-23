function ordenamientoPorSeleccion(arr) {
    const n = arr.length;
    
    for (let i = 0; i < n - 1; i++) {
        let indiceMinimo = i;
        
        for (let j = i + 1; j < n; j++) {
            if (arr[j] < arr[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        
        if (indiceMinimo !== i) {
            [arr[i], arr[indiceMinimo]] = [arr[indiceMinimo], arr[i]];
        }
    }
}

// Ejemplo de uso
let arreglo = [64, 25, 12, 22, 11];
console.log("Arreglo original: " + arreglo.join(" "));

ordenamientoPorSeleccion(arreglo);

console.log("Arreglo ordenado: " + arreglo.join(" "));
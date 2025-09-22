function ordenamientoInsercion(arr) {
    const n = arr.length;
    for (let i = 1; i < n; i++) {
        let clave = arr[i];
        let j = i - 1;
        
        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

// Ejemplo de uso
let arreglo = [12, 11, 13, 5, 6];
ordenamientoInsercion(arreglo);

console.log("Arreglo ordenado: " + arreglo.join(" "));
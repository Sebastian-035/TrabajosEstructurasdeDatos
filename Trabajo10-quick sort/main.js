// Función de utilidad para intercambiar dos elementos
function swap(arr, i, j) {
    [arr[i], arr[j]] = [arr[j], arr[i]];
}

function partition(arr, low, high) {
    // Se elige el último elemento como pivote
    let pivot = arr[high];

    // Índice del elemento más pequeño
    let i = (low - 1);

    for (let j = low; j <= high - 1; j++) {
        // Si el elemento actual es menor que el pivote
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
        }
    }
    // Coloca el pivote en su posición correcta
    swap(arr, i + 1, high);
    return (i + 1);
}

function quickSort(arr, low, high) {
    if (low < high) {
        // pi es el índice de partición
        let pi = partition(arr, low, high);

        // Ordena recursivamente los sub-arrays
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Ejemplo de uso
let arr = [10, 7, 8, 9, 1, 5];
let n = arr.length;
quickSort(arr, 0, n - 1);
console.log("Array ordenado:", arr);

// Nota: Una implementación más "JavaScript" a veces no modifica el array original (out-of-place), 
// pero esta versión es la clásica in-place.
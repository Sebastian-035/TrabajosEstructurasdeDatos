const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function bubbleSort(arr) {
    let n = arr.length;
    for (let i = 0; i < n - 1; i++) {
        for (let j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                let temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

rl.question('Ingrese la cantidad de números a ordenar: ', (n) => {
    let arr = [];
    let count = 0;
    
    console.log(`Ingrese ${n} números:`);
    
    rl.on('line', (input) => {
        arr.push(parseInt(input));
        count++;
        
        if (count == n) {
            let sortedArr = bubbleSort(arr);
            console.log("Arreglo ordenado: " + sortedArr.join(" "));
            rl.close();
        }
    });
});
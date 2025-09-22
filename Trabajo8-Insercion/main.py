def ordenamiento_insercion(arr):
    n = len(arr)
    for i in range(1, n):
        clave = arr[i]
        j = i - 1
        
        while j >= 0 and arr[j] > clave:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = clave

# Ejemplo de uso
arreglo = [12, 11, 13, 5, 6]
ordenamiento_insercion(arreglo)

print("Arreglo ordenado:", arreglo)
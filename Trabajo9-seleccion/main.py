def ordenamiento_por_seleccion(arr):
    n = len(arr)
    
    for i in range(n - 1):
        indice_minimo = i
        
        for j in range(i + 1, n):
            if arr[j] < arr[indice_minimo]:
                indice_minimo = j
        
        if indice_minimo != i:
            arr[i], arr[indice_minimo] = arr[indice_minimo], arr[i]

# Ejemplo de uso
arreglo = [64, 25, 12, 22, 11]
print("Arreglo original:", " ".join(str(x) for x in arreglo))

ordenamiento_por_seleccion(arreglo)

print("Arreglo ordenado:", " ".join(str(x) for x in arreglo))
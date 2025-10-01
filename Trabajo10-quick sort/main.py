# Función de utilidad para intercambiar dos elementos
def swap(arr, i, j):
    arr[i], arr[j] = arr[j], arr[i]

def partition(arr, low, high):
    # Se elige el último elemento como pivote
    pivot = arr[high]

    # Índice del elemento más pequeño
    i = (low - 1)

    for j in range(low, high):
        # Si el elemento actual es menor que el pivote
        if arr[j] < pivot:
            i += 1
            swap(arr, i, j)

    # Coloca el pivote en su posición correcta
    swap(arr, i + 1, high)
    return (i + 1)

def quick_sort(arr, low, high):
    if low < high:
        # pi es el índice de partición
        pi = partition(arr, low, high)

        # Ordena recursivamente los sub-arrays
        quick_sort(arr, low, pi - 1)
        quick_sort(arr, pi + 1, high)

# Ejemplo de uso
arr = [10, 7, 8, 9, 1, 5]
n = len(arr)
quick_sort(arr, 0, n - 1)
print("Array ordenado:", arr)

# Nota: En Python es muy común ver implementaciones más concisas que no son in-place,
# pero esta versión es la que sigue la estructura clásica recursiva.
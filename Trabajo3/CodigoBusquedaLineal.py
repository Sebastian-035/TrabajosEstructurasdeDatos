# Algoritmo de recorrido y búsqueda lineal
def recorrido_y_busqueda_lineal():
    # Arreglo inicial
    arreglo = [12, 23, 57, 45, 34, 67, 89, 34]
    
    # Recorrido e impresión de valores
    print("Arreglo completo:")
    for i, valor in enumerate(arreglo):
        print(f"Índice {i}: {valor}")
    
    # Búsqueda lineal
    valor_buscado = 34
    indices_encontrados = []
    
    print(f"\nBuscando valor {valor_buscado}...")
    
    for indice, elemento in enumerate(arreglo):
        if elemento == valor_buscado:
            indices_encontrados.append(indice)
    
    # Mostrar resultados de búsqueda
    if indices_encontrados:
        print(f"Valor encontrado en los índices: {', '.join(map(str, indices_encontrados))}")
        print(f"Total de ocurrencias: {len(indices_encontrados)}")
    else:
        print("Valor no encontrado en el arreglo.")

# Ejecutar la función
recorrido_y_busqueda_lineal()
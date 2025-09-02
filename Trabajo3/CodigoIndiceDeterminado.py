def recorrido_e_insercion():
    # Arreglo inicial
    numeros = [10, 20, 30, 40, 50]
    
    # Recorrido e impresión de valores
    print("Arreglo original:")
    for i, valor in enumerate(numeros):
        print(f"Índice {i}: {valor}")
    
    # Inserción de valor
    valor_a_insertar = 75
    indice = 2  # Insertar en la posición 2 (entre 20 y 30)
    
    print(f"\nInsertando valor {valor_a_insertar} en índice {indice}...")
    
    if 0 <= indice <= len(numeros):
        numeros.insert(indice, valor_a_insertar)
        print("Inserción exitosa!")
    else:
        print("Índice fuera de rango. No se pudo insertar.")
    
    # Mostrar resultado final
    print("\nArreglo actualizado:")
    for i, valor in enumerate(numeros):
        print(f"Índice {i}: {valor}")
    
    print(f"Array como lista: {numeros}")

# Ejecutar la función
recorrido_e_insercion()
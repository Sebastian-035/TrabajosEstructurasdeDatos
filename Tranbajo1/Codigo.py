def programa_principal():
    
    # Declaración e inicialización
    valores_numericos = [0] * 6
    
    # Asignación de valores iniciales
    valores_numericos[0] = 10
    valores_numericos[1] = 20
    valores_numericos[2] = 30
    valores_numericos[3] = 40
    valores_numericos[4] = 50
    valores_numericos[5] = 60
    
    # Modificación de un elemento
    valores_numericos[2] = 89
    
    # Visualización del contenido
    print("Contenido de la lista:")
    for indice in range(len(valores_numericos)):
        print(f"valores_numericos[{indice}] = {valores_numericos[indice]}")
    
    # Operación matemática
    resultado_suma = valores_numericos[0] + valores_numericos[2]
    print(f"\nResultado de valores numericos + valores numericos = {resultado_suma}")

# Ejecución del programa
    programa_principal()

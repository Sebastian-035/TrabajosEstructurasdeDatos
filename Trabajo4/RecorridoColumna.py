FILAS = 3
COLUMNAS = 4

matriz = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12]
]

# Recorrer por columnas
print("Recorrido por columnas:")
for col in range(COLUMNAS):
    print(f"Columna {col}: ", end="")
    for fila in range(FILAS):
        print(f"{matriz[fila][col]} ", end="")
    print()
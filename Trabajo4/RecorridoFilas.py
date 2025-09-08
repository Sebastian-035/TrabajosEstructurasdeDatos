FILAS = 3
COLUMNAS = 4

matriz = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12]
]

# Recorrer por renglones (filas)
print("Recorrido por renglones:")
for fila in range(FILAS):
    print(f"Renglón {fila}: ", end="")
    for col in range(COLUMNAS):
        print(f"{matriz[fila][col]} ", end="")
    print()
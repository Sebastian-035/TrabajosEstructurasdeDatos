def bubble_sort(arr):
    n = len(arr)
    for i in range(n - 1):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

def main():
    n = int(input("Ingrese la cantidad de números a ordenar: "))
    
    arr = []
    print(f"Ingrese {n} números:")
    for i in range(n):
        arr.append(int(input()))
    
    bubble_sort(arr)
    
    print("Arreglo ordenado:", " ".join(map(str, arr)))

if __name__ == "__main__":
    main()
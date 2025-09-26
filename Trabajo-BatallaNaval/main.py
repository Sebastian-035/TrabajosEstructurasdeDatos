class BatallaNaval:
    def __init__(self):
        self.filas = 10
        self.columnas = 10
        self.barcos = [
            {"nombre": "Portaaviones", "tamaño": 5},
            {"nombre": "Acorazado", "tamaño": 4},
            {"nombre": "Crucero", "tamaño": 3},
            {"nombre": "Submarino", "tamaño": 3},
            {"nombre": "Destructor", "tamaño": 2}
        ]
        self.tablero_jugador1 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]
        self.tablero_jugador2 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]

    def mostrar_tablero(self, tablero, revelar_barcos=False, jugador=1):
        print(f"\n📊 TABLERO DEL JUGADOR {jugador}")
        print("    " + "   ".join(str(i) for i in range(self.columnas)))
        for i in range(self.filas):
            fila = []
            for j in range(self.columnas):
                celda = tablero[i][j]
                if not revelar_barcos and celda == '🚢':
                    fila.append('🌊')
                else:
                    fila.append(celda)
            print(f"{chr(65+i)}  {'  '.join(fila)}")

    def posicion_valida(self, tablero, fila, col, tamaño, orientacion):
        if orientacion == 'H':
            if col + tamaño > self.columnas:
                return False
            return all(tablero[fila][col+i] == '🌊' for i in range(tamaño))
        else:
            if fila + tamaño > self.filas:
                return False
            return all(tablero[fila+i][col] == '🌊' for i in range(tamaño))

    def colocar_barco(self, tablero, fila, col, tamaño, orientacion):
        if orientacion == 'H':
            for i in range(tamaño):
                tablero[fila][col+i] = '🚢'
        else:
            for i in range(tamaño):
                tablero[fila+i][col] = '🚢'

    def entrada_coordenadas(self, mensaje):
        while True:
            try:
                entrada = input(mensaje).upper()
                if len(entrada) < 2:
                    raise ValueError
                fila = ord(entrada[0]) - 65
                col = int(entrada[1:])
                if 0 <= fila < self.filas and 0 <= col < self.columnas:
                    return fila, col
                print("❌ Coordenadas fuera de rango. Usa letras A-J y números 0-9")
            except ValueError:
                print("❌ Formato incorrecto. Usa LetraNúmero (ej: A3, J9)")

    def colocar_barcos_jugador(self, jugador_num):
        if jugador_num == 1:
            tablero = self.tablero_jugador1
        else:
            tablero = self.tablero_jugador2
            
        print(f"\n{'='*50}")
        print(f"🎯 JUGADOR {jugador_num} - FASE DE COLOCACIÓN DE BARCOS")
        print(f"{'='*50}")
        
        for barco in self.barcos:
            while True:
                self.mostrar_tablero(tablero, revelar_barcos=True, jugador=jugador_num)
                print(f"\nColocando {barco['nombre']} (tamaño: {barco['tamaño']})")
                
                fila, col = self.entrada_coordenadas("Posición inicial (ej: A3, J9): ")
                
                orientacion = input("Orientación ([H]orizontal / [V]ertical): ").upper()
                if orientacion not in ['H', 'V']:
                    print("❌ Por favor ingresa H o V")
                    continue

                if self.posicion_valida(tablero, fila, col, barco['tamaño'], orientacion):
                    self.colocar_barco(tablero, fila, col, barco['tamaño'], orientacion)
                    print(f"✅ {barco['nombre']} colocado exitosamente!")
                    break
                else:
                    print("❌ ¡Posición inválida! El barco se sale del tablero o se superpone con otro.")

    def iniciar_juego(self):
        print("🚢 BIENVENIDO A BATALLA NAVAL 🚢")
        print("Tablero 10x10 - Modo Manual para 2 Jugadores")
        print("="*55)
        
        # Jugador 1 coloca sus barcos
        input("\nPresiona Enter para que el JUGADOR 1 comience...")
        self.colocar_barcos_jugador(1)
        
        # Limpiar pantalla (simulado)
        print("\n" * 50)
        print("¡JUGADOR 1 ha terminado de colocar sus barcos!")
        
        # Jugador 2 coloca sus barcos
        input("\nPresiona Enter para que el JUGADOR 2 comience...")
        self.colocar_barcos_jugador(2)
        
        # Mostrar resumen final
        print("\n" * 50)
        print("🎉 ¡AMBOS JUGADORES HAN COLOCADO SUS BARCOS!")
        print("="*55)
        
        print("\nRESUMEN FINAL:")
        self.mostrar_tablero(self.tablero_jugador1, revelar_barcos=True, jugador=1)
        self.mostrar_tablero(self.tablero_jugador2, revelar_barcos=True, jugador=2)
        
        print("\n¡Listos para comenzar la batalla! ⚔️")

if __name__ == "__main__":
    juego = BatallaNaval()
    juego.iniciar_juego()
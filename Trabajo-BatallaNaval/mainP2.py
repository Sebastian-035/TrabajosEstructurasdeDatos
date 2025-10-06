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
        self.tablero_barcos_j1 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]
        self.tablero_barcos_j2 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]
        self.tablero_disparos_j1 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]
        self.tablero_disparos_j2 = [['🌊' for _ in range(self.columnas)] for _ in range(self.filas)]
        self.barcos_hundidos_j1 = 0
        self.barcos_hundidos_j2 = 0

    def mostrar_tablero_barcos(self, tablero, jugador=1):
        print(f"\n📊 TABLERO DE BARCOS - JUGADOR {jugador}")
        print("    " + "   ".join(str(i) for i in range(self.columnas)))
        for i in range(self.filas):
            fila = []
            for j in range(self.columnas):
                celda = tablero[i][j]
                fila.append(celda)
            print(f"{chr(65+i)}  {'  '.join(fila)}")

    def mostrar_tablero_disparos(self, tablero, jugador=1):
        print(f"\n🎯 TABLERO DE DISPAROS - JUGADOR {jugador}")
        print("    " + "   ".join(str(i) for i in range(self.columnas)))
        for i in range(self.filas):
            fila = []
            for j in range(self.columnas):
                celda = tablero[i][j]
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
            tablero = self.tablero_barcos_j1
        else:
            tablero = self.tablero_barcos_j2
            
        print(f"\n{'='*50}")
        print(f"🎯 JUGADOR {jugador_num} - FASE DE COLOCACIÓN DE BARCOS")
        print(f"{'='*50}")
        
        for barco in self.barcos:
            while True:
                self.mostrar_tablero_barcos(tablero, jugador_num)
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

    def realizar_disparo(self, jugador_atacante, fila, col):
        if jugador_atacante == 1:
            tablero_objetivo = self.tablero_barcos_j2
            tablero_disparos = self.tablero_disparos_j1
        else:
            tablero_objetivo = self.tablero_barcos_j1
            tablero_disparos = self.tablero_disparos_j2

        # Verificar si ya se disparó aquí
        if tablero_disparos[fila][col] != '🌊':
            return "repetido"

        # Realizar disparo
        if tablero_objetivo[fila][col] == '🚢':
            tablero_disparos[fila][col] = '💥'
            tablero_objetivo[fila][col] = '💥'
            
            # Verificar si el barco fue hundido completamente
            if self.verificar_barco_hundido(tablero_objetivo, fila, col):
                if jugador_atacante == 1:
                    self.barcos_hundidos_j1 += 1
                else:
                    self.barcos_hundidos_j2 += 1
                return "hundido"
            return "impacto"
        else:
            tablero_disparos[fila][col] = '❌'
            return "fallo"

    def verificar_barco_hundido(self, tablero, fila, col):
        # Verificar horizontalmente
        for i in range(col, -1, -1):
            if tablero[fila][i] == '🚢':
                return False
            if tablero[fila][i] == '🌊' or tablero[fila][i] == '❌':
                break
        
        for i in range(col + 1, self.columnas):
            if tablero[fila][i] == '🚢':
                return False
            if tablero[fila][i] == '🌊' or tablero[fila][i] == '❌':
                break
        
        # Verificar verticalmente
        for i in range(fila, -1, -1):
            if tablero[i][col] == '🚢':
                return False
            if tablero[i][col] == '🌊' or tablero[i][col] == '❌':
                break
        
        for i in range(fila + 1, self.filas):
            if tablero[i][col] == '🚢':
                return False
            if tablero[i][col] == '🌊' or tablero[i][col] == '❌':
                break
        
        return True

    def fase_de_batalla(self):
        print("\n" * 50)
        print("⚔️ ¡COMIENZA LA BATALLA NAVAL! ⚔️")
        print("="*55)
        
        turno = 1
        total_barcos = len(self.barcos)
        
        while self.barcos_hundidos_j1 < total_barcos and self.barcos_hundidos_j2 < total_barcos:
            print(f"\n{'='*50}")
            print(f"TURNO DEL JUGADOR {turno}")
            print(f"Barcos hundidos - J1: {self.barcos_hundidos_j1}/{total_barcos} | J2: {self.barcos_hundidos_j2}/{total_barcos}")
            print(f"{'='*50}")
            
            # Mostrar tableros del jugador actual
            if turno == 1:
                self.mostrar_tablero_disparos(self.tablero_disparos_j1, 1)
                self.mostrar_tablero_barcos(self.tablero_barcos_j1, 1)
            else:
                self.mostrar_tablero_disparos(self.tablero_disparos_j2, 2)
                self.mostrar_tablero_barcos(self.tablero_barcos_j2, 2)
            
            # Realizar disparo
            while True:
                print(f"\nJugador {turno}, es tu turno de disparar!")
                fila, col = self.entrada_coordenadas("Coordenadas para disparar (ej: B5): ")
                
                resultado = self.realizar_disparo(turno, fila, col)
                
                if resultado == "repetido":
                    print("❌ Ya disparaste en esta posición. Elige otra.")
                    continue
                elif resultado == "impacto":
                    print("🎯 ¡IMPACTO! Has golpeado un barco enemigo.")
                    break
                elif resultado == "hundido":
                    print("🔥 ¡BARCO HUNDIDO! Has destruido completamente un barco enemigo.")
                    break
                else:  # fallo
                    print("💦 ¡AGUA! El disparo cayó en el océano.")
                    break
            
            # Verificar si alguien ganó
            if self.barcos_hundidos_j1 == total_barcos:
                print("\n" + "🎉" * 20)
                print("¡EL JUGADOR 1 ES EL GANADOR!")
                print("¡FELICIDADES! Has hundido toda la flota enemiga.")
                print("🎉" * 20)
                break
            elif self.barcos_hundidos_j2 == total_barcos:
                print("\n" + "🎉" * 20)
                print("¡EL JUGADOR 2 ES EL GANADOR!")
                print("¡FELICIDADES! Has hundido toda la flota enemiga.")
                print("🎉" * 20)
                break
            
            # Cambiar turno
            input("\nPresiona Enter para pasar al siguiente turno...")
            print("\n" * 50)
            turno = 3 - turno  # Alterna entre 1 y 2

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
        self.mostrar_tablero_barcos(self.tablero_barcos_j1, 1)
        self.mostrar_tablero_barcos(self.tablero_barcos_j2, 2)
        
        input("\nPresiona Enter para comenzar la batalla...")
        
        # Iniciar fase de batalla
        self.fase_de_batalla()

if __name__ == "__main__":
    juego = BatallaNaval()
    juego.iniciar_juego()
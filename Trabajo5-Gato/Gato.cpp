using System;

namespace TicTacToe
{
    class Program
    {
        static char[] board = new char[9];
        static char currentPlayer = 'X';

        static void Main(string[] args)
        {
            bool playAgain = true;
            
            while (playAgain)
            {
                InitializeBoard();
                PlayGame();
                playAgain = AskToPlayAgain();
            }
        }

        static void InitializeBoard()
        {
            for (int i = 0; i < 9; i++)
            {
                board[i] = ' ';
            }
        }

        static void PlayGame()
        {
            bool gameEnded = false;
            int moveCount = 0;

            while (!gameEnded)
            {
                Console.Clear();
                PrintBoard();
                MakeMove();
                moveCount++;
                
                if (CheckWin())
                {
                    Console.Clear();
                    PrintBoard();
                    Console.WriteLine($"¡Jugador {currentPlayer} gana!");
                    gameEnded = true;
                }
                else if (moveCount == 9)
                {
                    Console.Clear();
                    PrintBoard();
                    Console.WriteLine("¡Empate!");
                    gameEnded = true;
                }
                else
                {
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }
            }
        }

        static void PrintBoard()
        {
            Console.WriteLine("  A B C");
            for (int i = 0; i < 3; i++)
            {
                Console.Write((i + 1) + " ");
                for (int j = 0; j < 3; j++)
                {
                    int index = i * 3 + j;
                    Console.Write(board[index]);
                    if (j < 2) Console.Write("|");
                }
                Console.WriteLine();
                if (i < 2) Console.WriteLine("  -+-+-");
            }
            Console.WriteLine();
        }

        static void MakeMove()
        {
            bool validMove = false;
            
            while (!validMove)
            {
                Console.WriteLine($"Jugador {currentPlayer}, ingresa una coordenada (ej: 1A, 2b, 3C):");
                string input = Console.ReadLine().Trim().ToUpper();
                
                if (input.Length != 2)
                {
                    Console.WriteLine("Entrada inválida. Usa formato: número (1-3) y letra (A-C)");
                    continue;
                }

                // Extraer fila y columna
                char rowChar = input[0];
                char colChar = input[1];
                
                // Verificar si el formato es letra primero
                if (char.IsLetter(rowChar) && char.IsDigit(colChar))
                {
                    // Intercambiar si la letra viene primero
                    char temp = rowChar;
                    rowChar = colChar;
                    colChar = temp;
                }

                if (rowChar >= '1' && rowChar <= '3' && colChar >= 'A' && colChar <= 'C')
                {
                    int row = rowChar - '1'; // Convertir a 0-2
                    int col = colChar - 'A'; // Convertir a 0-2
                    int index = row * 3 + col;
                    
                    if (board[index] == ' ')
                    {
                        board[index] = currentPlayer;
                        validMove = true;
                    }
                    else
                    {
                        Console.WriteLine("Casilla ocupada. Intenta otra.");
                    }
                }
                else
                {
                    Console.WriteLine("Posición fuera de rango. Usa números 1-3 y letras A-C.");
                }
            }
        }

        static bool CheckWin()
        {
            // Verificar filas
            for (int i = 0; i < 3; i++)
            {
                if (board[i * 3] == currentPlayer && 
                    board[i * 3 + 1] == currentPlayer && 
                    board[i * 3 + 2] == currentPlayer)
                    return true;
            }

            // Verificar columnas
            for (int i = 0; i < 3; i++)
            {
                if (board[i] == currentPlayer && 
                    board[i + 3] == currentPlayer && 
                    board[i + 6] == currentPlayer)
                    return true;
            }

            // Verificar diagonales
            if (board[0] == currentPlayer && board[4] == currentPlayer && board[8] == currentPlayer)
                return true;
            if (board[2] == currentPlayer && board[4] == currentPlayer && board[6] == currentPlayer)
                return true;

            return false;
        }

        static bool AskToPlayAgain()
        {
            Console.WriteLine("¿Quieren jugar de nuevo? (s/n)");
            string response = Console.ReadLine().ToLower();
            return response == "s" || response == "si";
        }
    }
}
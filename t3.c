/* t3 - Tic Tac Toe. Original version c. 2014 Joe Haddad, modifications c. 2018 T. O'Neil */

#include "blackdos.h"

/* function prototypes */
int checkSignWin(int [], int);
int gameOver(int []);
int computerMove(int []);
void printBoard(int []);

void main()
{
    int i, board[9], currentPlayer = 0, winner, sign, index, row, col, l;
    PRINTS("\r\n          Welcome to the ultimate test of man-versus-machine.\r\n\0");
    PRINTS("                            TIC - TAC - TOE\r\n\0");
    PRINTS("Make your move known by entering numbers for the row and column, 1, 2 or 3.\r\n\0");
    PRINTS("These numbers correspond to your desired board position, as illustrated:\r\n\r\n\0");
    PRINTS("                         row 1 | row 1 | row 1\r\n\0");
    PRINTS("                         col 1 | col 2 | col 3\r\n\0");
    PRINTS("                        -----------------------\r\n\0");
    PRINTS("                         row 2 | row 2 | row 2\r\n\0");
    PRINTS("                         col 1 | col 2 | col 3\r\n\0");
    PRINTS("                        -----------------------\r\n\0");
    PRINTS("                         row 3 | row 3 | row 3\r\n\0");
    PRINTS("                         col 1 | col 2 | col 3\r\n\r\n\0");
    PRINTS("Do you wish to start the first game (1) or defer to the second (0)? \0");
    SCANN(i);
    if (i == 1) currentPlayer = 1;
    PRINTS("Brace yourself, puny human. The battle is about to begin.\r\n\r\n\0");
    while (1) {
        for (i = 0; i < 9; i++) board[i] = 0;
        winner = 0;
        while (!gameOver(board)) {
            currentPlayer = currentPlayer + 1;
            if (currentPlayer == 2) currentPlayer = 0;
            sign = currentPlayer + 1;
            if (sign == 1) {
                PRINTS("Human, it is your turn.\r\n\0");
                while(1)
                {
                    PRINTS("Please select a row (between 1 and 3): \0");
                    SCANN(row);
                    PRINTS("Please select a column (between 1 and 3): \0");
                    SCANN(col);
                    while (row < 1 || row > 3 || col < 1 || col > 3) {
                        PRINTS("Bad human ... between 1 and 3.\r\n\0");
                        PRINTS("Please select a row (between 1 and 3): \0");
                        SCANN(row);
                        PRINTS("Please select a column (between 1 and 3): \0");
                        SCANN(col);
                    }
                    index = (row - 1) * 3 + (col - 1);

                    if (board[index] > 0) PRINTS("That spot is already chosen, sorry about that.\r\n\0");
                    else break;
                }
                PRINTS("\r\n\0");
            } else {
                index = computerMove(board);
            }
            if (index < 0 || index > 8) break;
            board[index] = sign;
            printBoard(board);
            PRINTS("\r\n\0");

            if (checkSignWin(board, sign)) {
                winner = sign;
                break;
            }
        }
        if (winner == 1) PRINTS("Congratulations human, you have won.\r\n\0");
        else if (winner == 2) PRINTS("I won, sorry about that human. You can\'t beat my artificial intelligence!\r\n\0");
        else PRINTS("The game was a draw, nice try.\r\n\0");

        PRINTS("\r\nWould you like to play again (1=y, 0=n)? \0");
        SCANN(l);
        if (l != 1) {
            PRINTS("Until we meet again, then.\r\n\r\n\0");
            break;
        }
        PRINTS("\r\n\r\n\0");
    }
    END;
}

int checkSignWin(int board[], int sign) {
    int i, start;
    for (i = 0; i < 3; i++) {
        start = i * 3;
        if (board[start] == sign && board[start + 1] == sign && board[start + 2] == sign) {
            return 1;
        } else if (board[i] == sign && board[i + 3] == sign && board[i + 6] == sign) {
            return 1;
        } else if (board[4] == sign && ((board[0] == sign && board[8] == sign) || (board[2] == sign && board[6] == sign))) return 1;
    }
    return 0;
}

int gameOver(int board[]) {
    int i;
    for (i = 0; i < 9; i++) if (board[i] == 0) return 0;
    return 1;
}

 /* Use by computerMove to evaluate prospective tic-tac-toe move. */
int goodMove(int board[], int i, int j, int k)
{
   if ((board[i] == board[j]) && (board[i] != 0) && (board[k] == 0))
      return 1;
   else return 0;
}

/* Logic by which computer player makes a tic-tac-toe move. */
int computerMove(int board[])
{
   PRINTS("My move:\r\n\r\n\0");
   /* Examine all rows. */
   if (goodMove(board,0,1,2))      return 2;
   else if (goodMove(board,0,2,1)) return 1;
   else if (goodMove(board,1,2,0)) return 0;
   else if (goodMove(board,3,4,5)) return 5;
   else if (goodMove(board,3,5,4)) return 4;
   else if (goodMove(board,4,5,3)) return 3;
   else if (goodMove(board,6,7,8)) return 8;
   else if (goodMove(board,6,8,7)) return 7;
   else if (goodMove(board,7,8,6)) return 6;

   /* Examine all columns. */
   else if (goodMove(board,0,3,6)) return 6;
   else if (goodMove(board,0,6,3)) return 3;
   else if (goodMove(board,3,6,0)) return 0;
   else if (goodMove(board,1,4,7)) return 7;
   else if (goodMove(board,1,7,4)) return 4;
   else if (goodMove(board,4,7,1)) return 1;
   else if (goodMove(board,2,5,8)) return 8;
   else if (goodMove(board,2,8,5)) return 5;
   else if (goodMove(board,5,8,2)) return 2;

   /* Examine diagonals. */
   else if (goodMove(board,0,4,8)) return 8;
   else if (goodMove(board,4,8,0)) return 0;
   else if (goodMove(board,0,8,4)) return 4;
   else if (goodMove(board,2,4,6)) return 6;
   else if (goodMove(board,4,6,2)) return 2;
   else if (goodMove(board,2,6,4)) return 4;

   /* Finally just pick a spot. */
   else if (board[4] == 0) return 4;
   else if (board[0] == 0) return 0;
   else if (board[2] == 0) return 2;
   else if (board[6] == 0) return 6;
   else if (board[8] == 0) return 8;
   else if (board[1] == 0) return 1;
   else if (board[3] == 0) return 3;
   else if (board[5] == 0) return 5;
   else if (board[7] == 0) return 7;

   /* otherwise no legal move */
   else return 9;
}

/* Print tic-tac-toe game board. */
 void printBoard(int board[])
 {
    int i;
    for (i = 0; i < 9; i++) {
	   if (board[i] == 1) PRINTS(" X \0");
	   else if (board[i] == 2) PRINTS(" O \0");
	   else PRINTS("   \0");
	   if (i < 8) {
	      if (i == 2 || i == 5) PRINTS("\r\n---|---|---\r\n\0");
	      else PRINTS("|\0");
	   }
	   else PRINTS("\r\n\r\n\0");
    }
 }

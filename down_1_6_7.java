import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class down_1_6_7
{
 private class White{
  private static final char PAWN = 'P';
  private static final char KNIGHT = 'N';
  private static final char BISHOP = 'B';
  private static final char ROOK = 'R';
  private static final char QUEEN = 'Q';
  private static final char KING = 'K';
 }

 private class Black{
  private static final char PAWN = 'p';
  private static final char KNIGHT = 'n';
  private static final char BISHOP = 'b';
  private static final char ROOK = 'r';
  private static final char QUEEN = 'q';
  private static final char KING = 'k';
 }

 static final int cols = 8;
 static final int rows = 8;
 static final char EMPTY = '.';
 static int[] whiteKing = new int[2];
 static int[] blackKing = new int[2];

 static char[][] board = new char[rows][cols];

    public static void main( String[] args ) throws IOException
    {
     final BufferedReader in = new BufferedReader(new InputStreamReader(
    System.in));

  String line;

  int row = 0;
  boolean terminate = true;
  int gameNum = 1;
  while ((line = in.readLine()) != null) {
   line = line.trim();
   if(!line.equals(""))
   {
    for(int c = 0; c < line.length(); c++){
     char piece = line.charAt(c);
     board[row][c] = piece;
     //store the location of black king
     if(piece == Black.KING){
      blackKing[0] = row;
      blackKing[1] = c;
     }
     else
      //store the location of white king
      if(piece == White.KING){
       whiteKing[0] = row;
       whiteKing[1] = c;
      }
     if(piece != EMPTY)
      terminate = false;
    }

    if(++row > rows - 1)
    {
     if(terminate) return;
     check(board, gameNum++);
     row = 0;
     terminate = true;
    }
   }
  }
    }

    private static void check(char[][] board, int gameNum){
     /*
      * check for check white king
      */
     if(
       isInCheckFromKnight(whiteKing, Black.KNIGHT) ||
       isInCheckFromPawn(whiteKing) ||
       isInCheckFromBishopOrQueen(whiteKing, Black.BISHOP, Black.QUEEN) ||
       isInCheckFromRookOrQueen(whiteKing, Black.ROOK, Black.QUEEN)
       )
     {
      System.out.print("Game #"+gameNum+": white king is in check.\n");
      return;
     }

     /*
      * check for check black king
      */
     if(
       isInCheckFromKnight(blackKing, White.KNIGHT) ||
       isInCheckFromPawn(blackKing) ||
       isInCheckFromBishopOrQueen(blackKing, White.BISHOP, White.QUEEN) ||
       isInCheckFromRookOrQueen(blackKing, White.ROOK, White.QUEEN)
       )
     {
      System.out.print("Game #"+gameNum+": black king is in check.\n");
      return;
     }

     System.out.print("Game #"+gameNum+": no king is in check.\n");
    }

    private static boolean isInCheckFromRookOrQueen(int[] kingCoord,
      char opposingRook, char opposingQueen){
     int row = kingCoord[0];
     int col = kingCoord[1];

     //up ||
     //   -K-
     //   |
     for(int row1 = row - 1; row1 >= 0; row1--){
      char piece = board[row1][col];

      if(piece != EMPTY){
       if(piece == opposingRook || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }
     }

     //down  |
     //   -K-
     //   ||
     for(int row2 = row + 1; row2 < rows; row2++){
      char piece = board[row2][col];

      if(piece != EMPTY){
       if(piece == opposingRook || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }
     }

     //left  |
     //     *K-
     //     |
     for(int col1 = col - 1; col1 >=0; col1--){
      char piece = board[row][col1];

      if(piece != EMPTY){
       if(piece == opposingRook || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }
     }

     //right  |
     //      -K*
     //      |
     for(int col2 = col + 1; col2 < cols; col2++){
      char piece = board[row][col2];

      if(piece != EMPTY){
       if(piece == opposingRook || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }
     }

     return false;
    }

    private static boolean isInCheckFromBishopOrQueen(int[] kingCoord,
      char opposingBishop, char opposingQueen){
     int row = kingCoord[0];
     int col = kingCoord[1];

     //north-east diagonal   \//
     //       K
     //      /\
     int col1 = col + 1, row1 = row - 1;
     while(col1 < cols && row1 >= 0){

      char piece = board[row1][col1];

      if(piece != EMPTY){
       if(piece == opposingBishop || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }

      col1++;
      row1--;
     }

     //south-west diagonal   \/
     //      K
     //        //\
     int col2 = col - 1, row2 = row + 1;
     while(col2 >= 0 && row2 < rows){

      char piece = board[row2][col2];

      if(piece != EMPTY){
       if(piece == opposingBishop || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }

      col2--;
      row2++;
     }

     //south-east diagonal   \/
     //       K
     //         /\\
     int col3 = col + 1, row3 = row + 1;
     while(col3 < cols && row3 < rows){

      char piece = board[row3][col3];

      if(piece != EMPTY){
       if(piece == opposingBishop || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }

      col3++;
      row3++;
     }

     //north-west diagonal   \\/
     //       K
     //          /\
     int col4 = col - 1, row4 = row - 1;
     while(col4 >= 0 && row4 >= 0){

      char piece = board[row4][col4];

      if(piece != EMPTY){
       if(piece == opposingBishop || piece == opposingQueen)
        return true;
       else //no need to check further,
         //any other piece effectively blocks the check
        break;
      }

      col4--;
      row4--;
     }

     return false;
    }

    private static boolean isInCheckFromPawn(int[] kingCoord){
     int row = kingCoord[0];
     int col = kingCoord[1];

     char opposingPawn = ' ';

     if(board[row][col] == White.KING)
     {
      row--;
      opposingPawn = Black.PAWN;
     }
     else if(board[row][col] == Black.KING)
     {
      row++;
      opposingPawn = White.PAWN;
     }

     int right = col + 1;
  int left = col - 1;
  if(row < rows && row >=0 &&
    ((right < cols && board[row][right] == opposingPawn) ||
    (left >=0 && board[row][left] == opposingPawn)))
   return true;

     return false;
    }

    private static boolean isInCheckFromKnight(int[] kingCoord, char knight){
     int row = kingCoord[0];
     int col = kingCoord[1];

     //8 possible coordinates need to be checked
     //   . * . * .  1
     //    * . . . *  2
     //   . . K . .
     //    * . . . *  3
     //   . * . * .  4
     //   1 2   3 4

     int row1 = row - 2;
     int row2 = row - 1;
     int row3 = row + 1;
     int row4 = row + 2;

     int col1 = col - 2;
     int col2 = col - 1;
     int col3 = col + 1;
     int col4 = col + 2;

     //process row 1
     if(row1 >= 0){
      if(col2 >= 0 && board[row1][col2] == knight)
       return true;
      if(col3 < cols && board[row1][col3] == knight)
       return true;
     }

     //process row 2
     if(row2 >= 0){
      if(col1 >= 0 && board[row2][col1] == knight)
       return true;
      if(col4 < cols && board[row2][col4] == knight)
       return true;
     }

     //process row 3
     if(row3 < rows){
      if(col1 >= 0 && board[row3][col1] == knight)
       return true;
      if(col4 < cols && board[row3][col4] == knight)
       return true;
     }

     //process row 4
     if(row4 < rows){
      if(col2 >= 0 && board[row4][col2] == knight)
       return true;
      if(col3 < cols && board[row4][col3] == knight)
       return true;
     }

     return false;
    }
}

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.StringTokenizer;

public class down_1_6_5
{
 static char[][] screen;
 final static String EXIT = "X";
 static int rows, cols;
 static Queue<  int[]  > q;

    public static void main( String[] args ) throws IOException
    {
     final BufferedReader in = new BufferedReader(new InputStreamReader(
    System.in));

  String line;

  while ((line = in.readLine()) != null) {
   line = line.trim();
   if(line.equals(EXIT))
    return;

   StringTokenizer tok = new StringTokenizer(line);
   List< String > params = new ArrayList< String >();

   while(tok.hasMoreTokens()){
    params.add(tok.nextToken());
   }

   processCommand(params);
  }
    }

    private static void processCommand(List<  String  > command){
     if(command.size()  > 0)
     {
      switch(command.get(0).charAt(0))
      {
       case 'I':
        I(command);
        break;
       case 'C':
        C();
        break;
       case 'L':
        L(command);
        break;
       case 'V':
        V(command);
        break;
       case 'H':
        H(command);
        break;
       case 'K':
        K(command);
        break;
       case 'F':
        F(command);
        break;
       case 'S':
        S(command);
        break;
       default:
        break;
      }
     }
    }

    private static boolean isXValid(int x){
     return x  >= 0 && x <  cols;
    }

    private static boolean isYValid(int y){
     return y  >= 0 && y <  rows;
    }

    private static void resetScreen(){
     for(int i = 0; i <  screen.length; i++)
   for(int j = 0; j <  screen[0].length; j++)
    screen[i][j] = 'O';
    }

    private static void I(List< String > command){
     cols = Integer.parseInt(command.get(1));
     rows = Integer.parseInt(command.get(2));

     screen = new char[rows][cols];

     resetScreen();
    }

    private static void C(){
     if(screen != null)
      resetScreen();
    }

    private static void L(List< String > command){
     if(screen != null){
      int x = Integer.parseInt(command.get(1)) - 1;
      int y = Integer.parseInt(command.get(2)) - 1;
      char color = command.get(3).charAt(0);

      if(isXValid(x) && isYValid(y))
       screen[y][x] = color;
     }
    }

    private static void V(List< String > command){
     if(screen != null){
      int x = Integer.parseInt(command.get(1)) - 1;
      int y1 = Integer.parseInt(command.get(2)) - 1;
      int y2 = Integer.parseInt(command.get(3)) - 1;
      char color = command.get(4).charAt(0);

      if(isXValid(x) && isYValid(y1) && isYValid(y2))
      {
       for(int row = Math.min(y1, y2); row <= Math.max(y1, y2); row++)
         screen[row][x] = color;
      }
     }
    }

    private static void H(List< String > command){
     if(screen != null){
      int x1 = Integer.parseInt(command.get(1)) - 1;
      int x2 = Integer.parseInt(command.get(2)) - 1;
      int y = Integer.parseInt(command.get(3)) - 1;
      char color = command.get(4).charAt(0);

      if(isXValid(x1) && isXValid(x2) && isYValid(y))
      {
       for(int col = Math.min(x1, x2); col <= Math.max(x1, x2); col++)
        screen[y][col] = color;
      }
     }
    }

    private static void K(List< String > command){
     if(screen != null){
      int x1 = Integer.parseInt(command.get(1)) - 1;
      int y1 = Integer.parseInt(command.get(2)) - 1;
      int x2 = Integer.parseInt(command.get(3)) - 1;
      int y2 = Integer.parseInt(command.get(4)) - 1;
      char color = command.get(5).charAt(0);

      if(isXValid(x1) && isXValid(x2) &&
            isYValid(y1) && isYValid(y2))
      {
       for(int row = Math.min(y1, y2); row <= Math.max(y1, y2); row++)
        for(int col = Math.min(x1, x2); col <= Math.max(x1, x2); col++)
         screen[row][col] = color;
      }
     }
    }

    private static void F(List< String > command){
     if(screen != null){
      int x = Integer.parseInt(command.get(1)) - 1;
      int y = Integer.parseInt(command.get(2)) - 1;
      char newColor = command.get(3).charAt(0);
      char targetColor = screen[y][x];

      if(newColor == targetColor)
       return;

      q = new LinkedList< int[] >();
      q.add(new int[] {x, y});

      while(!q.isEmpty()){
       int[] coord = q.remove();
       x = coord[0];
       y = coord[1];

       if(screen[y][x] == targetColor){
        screen[y][x] = newColor;
       }

       //west
       if(x - 1  >=0 && screen[y][x - 1] == targetColor){
        screen[y][x - 1] = newColor;
        q.add(new int[] {x - 1, y});
       }

       //east
       if(x + 1 <  cols  && screen[y][x + 1] == targetColor){
        screen[y][x + 1] = newColor;
        q.add(new int[] {x + 1, y});
       }

       //south
       if(y + 1 <  rows  && screen[y + 1][x] == targetColor){
        screen[y + 1][x] = newColor;
        q.add(new int[] {x, y + 1});
       }

       //north
       if(y - 1  >=0 && screen[y - 1][x] == targetColor){
        screen[y - 1][x] = newColor;
        q.add(new int[] {x, y - 1});
       }
      }
     }
    }

    private static void S(List<  String  > command){
     String fileName = command.get(1);
     StringBuilder builder = new StringBuilder(fileName);
     builder.append("\n");
     if(screen != null){
      for(int n = 0; n <  screen.length; n++){
       for(int m = 0; m <  screen[0].length; m++)
        builder.append(screen[n][m]);
       builder.append("\n");
      }
     }
     System.out.print(builder.toString());
    }
}

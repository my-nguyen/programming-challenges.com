// copied from helvidios.blogspot.com
import java.io.BufferedReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
 
 
public class down_1_6_2
{
    static String[][] fld = null;
    final static String BOMB = "*";
    final static String UNDEFINED = ".";
    static int rows = 0;
    static int cols = 0;
    public static void main( String[] args ) throws IOException
    {
     final BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        String line;    
         
        int currentRow = 0;
        int fieldNum = 0;
        while((line = in.readLine()) != null)
        {
         String REGEX_WHITESPACE = "\\s+";
          String cleanLine = line.trim().replaceAll(REGEX_WHITESPACE, " ");
          String[] tokens = cleanLine.split(" ");
   
   if (tokens.length == 2) 
   {
    currentRow = 0;
     
    try
    {
     rows = Integer.parseInt(tokens[0]);
     cols = Integer.parseInt(tokens[1]);
    }catch(NumberFormatException ex){
     break;
    }
     
    if(rows == cols && cols == 0) return;
    if(fieldNum > 0) System.out.print("\n");
     
    fld = new String[rows][cols];
    while(currentRow < rows){
     line = in.readLine();
     char[] chars = line.toCharArray();
     if(chars.length == cols)
     {
      for(int i = 0; i < chars.length; i++)
       fld[currentRow][i] = String.valueOf(chars[i]);
      currentRow++;
     }
    }
     
    for(int i = 0; i < fld.length; i++){
     for(int j = 0; j < fld[i].length; j++)
     {
      if(fld[i][j].equals(BOMB))
      {
        //north
        incrementValue(i-1, j);
         
        //north-east
        incrementValue(i-1, j+1);
         
        //east
        incrementValue(i, j+1);
         
        //south-east
        incrementValue(i+1, j+1);
         
        //south
        incrementValue(i+1, j);
         
        //south-west
        incrementValue(i+1, j-1);
         
        //west
        incrementValue(i, j-1);
         
        //north-west
        incrementValue(i-1, j-1);
        
      }else if(fld[i][j].equals(UNDEFINED))
       fld[i][j] = "0";
     }
    }
     
    //print out the field
    StringBuilder builder = new StringBuilder();
    fieldNum++;
    builder.append("Field #");
    builder.append(fieldNum);
    builder.append(":");
    // builder.append(System.getProperty("line.separator"));
    builder.append("\n");
    for(int i = 0; i < fld.length; i++){
     for(int j = 0; j < fld[i].length; j++)
      builder.append(fld[i][j]);
     // builder.append(System.getProperty("line.separator"));
      builder.append("\n");
    }
    System.out.print(builder.toString());
   }
        }
    }
     
    static void incrementValue(int i, int j){
     if(i >= 0 && i < rows && j >= 0 && j < cols)
     {
      if(!fld[i][j].equals(BOMB))
      {
       if(fld[i][j].equals(UNDEFINED))
        fld[i][j] = "0";
       int value = Integer.parseInt(fld[i][j]);
       value++;
       fld[i][j] = String.valueOf(value);
      }
     }
    }
}

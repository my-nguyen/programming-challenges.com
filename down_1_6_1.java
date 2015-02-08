// copied from helvidios.blogspot.com
import java.io.*;
 
public class down_1_6_1
{
   public static void main(String[] args) throws IOException
   {
      final BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      String line;
      while((line = in.readLine()) != null)
      {
         String REGEX_WHITESPACE = "\\s+";
         String cleanLine = line.trim().replaceAll(REGEX_WHITESPACE, " ");
         String[] tokens = cleanLine.split(REGEX_WHITESPACE);
        
         if (tokens.length == 2) {
           int a1 = Integer.parseInt(tokens[0]);
           int a2 = Integer.parseInt(tokens[1]);
           System.out.print(cleanLine + " " + solve(a1, a2) + "\n");
         }
      }
   }
 
   static int solve(int a1, int a2) throws IOException
   {
     int maxCycleLength = 0;
     final int from, to;
     if(a1 < a2){
       from = a1;
       to = a2;
     }else{
       from = a2;
       to = a1;
     }
    
     for(int i = from; i <= to; i++)
     {
       int currentCycleLength = calculateCycleLength(i);
       if(currentCycleLength > maxCycleLength)
         maxCycleLength = currentCycleLength;
     }
    
     return maxCycleLength;  
   }
    
   static int calculateCycleLength(int num)
   {
     int cycleLength = 1;
     
     while(num != 1)
     {
       cycleLength++;
       
       if(num % 2 == 0) //number is even
         num = num / 2;
       else //number is odd
         num = num * 3 + 1;
      }
     
      return cycleLength;
   }
}

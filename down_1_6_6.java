import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class down_1_6_6 {
 public static void main(String[] args) throws NumberFormatException, IOException{
  final BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
  //final BufferedReader in = new BufferedReader(new FileReader("in_test.txt"));

  int numTestCases = Integer.parseInt(in.readLine().trim());

  // read blank line
  in.readLine();

  for(int testCase = 1; testCase <= numTestCases; testCase++){
   int[] registers = new int[10];
   int[] ram = new int[1000];

   String line;
   int i = 0;
   while((line = in.readLine()) != null && !line.equals("")){
    ram[i++] = Integer.parseInt(line.trim());
   }

   System.out.println(solve(registers, ram));

   if(testCase < numTestCases)
    System.out.println();
  }
 }

 public static int solve(int[] registers, int[] ram){
  int total = 0;
  int location = 0;

  while(location < ram.length){
   total++;
   int instruction = ram[location];

   if(instruction == 100)
    return total;

   int dig1 = instruction / 100;
   int dig2 = (instruction / 10) % 10;
   int dig3 = instruction % 10;

   location++;

   if(dig1 == 2) registers[dig2] = dig3;
   else if(dig1 == 3) registers[dig2] = (registers[dig2] + dig3) % 1000;
   else if(dig1 == 4) registers[dig2] = (registers[dig2] * dig3) % 1000;
   else if(dig1 == 5) registers[dig2] = registers[dig3];
   else if(dig1 == 6) registers[dig2] = (registers[dig2] + registers[dig3]) % 1000;
   else if(dig1 == 7) registers[dig2] = (registers[dig2] * registers[dig3]) % 1000;
   else if(dig1 == 8) registers[dig2] = ram[registers[dig3]];
   else if(dig1 == 9) ram[registers[dig3]] = registers[dig2];
   else if(dig1 == 0 && registers[dig3] != 0) location = registers[dig2];
  }

  return total;
 }
}

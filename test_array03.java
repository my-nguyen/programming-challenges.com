import java.util.Arrays;

class tarray03 {
    public static void main(String[] args) {
        char[][] list = { "........".toCharArray(), "12345678".toCharArray() };
        boolean equal1 = list[0].equals("........");
        boolean equal2 = list[0].equals("........".toCharArray());
        boolean equal3 = Arrays.equals(list[0], "........".toCharArray());
        System.out.println("EQUAL1: " + equal1 + ", EQUAL2: " + equal2 + ", EQUAL3: " + equal3);
    }
}

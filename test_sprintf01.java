class tsprintf01 {
    public static void main(String[] args) {
        // PrintfFormat format = new PrintfFormat("%03d").sprintf("365");
        String number = String.format("%03d", 365);
        System.out.println("number: " + number);
    }
}

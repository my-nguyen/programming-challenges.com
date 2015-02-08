class tswap01 {
    private static void sort(int left, int right) {
        if (left > right) {
            int tmp = left;
            left = right;
            right = tmp;
        }
    }

    public static void main(String[] args) {
        int one = 5;
        int two = 8;
        int three = 2;

        System.out.println(one + " " + two + " " + three);
        sort(two, one);
        System.out.println(one + " " + two + " " + three);
        sort(one, three);
        System.out.println(one + " " + two + " " + three);
    }
}

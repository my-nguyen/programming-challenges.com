class thashcode01 {
    public static void main(String[] args) {
        int[] list = { 0, 8, 1, 7, 1, 9, 6, 7 };
        for (int i = 0; i < list.size(); i++)
            System.out.println("hash code: " + list[i].hashCode());
    }
}

class tarray02 {
    static class Location {
        int row;
        int column;

        Location(int rang, int colonne) {
            row = rang;
            column = colonne;
        }

        public String toString() {
            StringBuilder builder = new StringBuilder();
            builder.append(row + " " + column + "\n");
            return builder.toString();
        }
    }

    static Location[] kings = new Location[2];

    public static void main(String[] args) {
        kings[0] = new Location(1, 9);
        kings[1] = new Location(6, 7);
        System.out.println("KINGS:\n" + kings[0] + kings[1]);
        Location king = kings[1];
        king.row = -1;
        king.column = -1;
        System.out.println("KINGS:\n" + kings[0] + kings[1]);
    }
}

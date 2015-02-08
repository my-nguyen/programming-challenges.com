class tenum01 {
    enum Status { OPEN, BLOCKED, CHECKED }
    public static void main(String[] args) {
        Status status = Status.CHECKED;
        System.out.println("Status: " + status);
    }
}

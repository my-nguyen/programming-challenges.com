import java.util.ArrayList;

class tarraylist01 {
    static class Record {
        int _member;
        Record(int outsider) {
            _member = outsider;
        }
        public String toString() {
            return String.valueOf(_member);
        }
    }

    public static void main(String args[]) {
        ArrayList<Record> list = new ArrayList<Record>();
        list.add(new Record(1));
        list.add(new Record(9));
        list.add(new Record(6));
        list.add(new Record(7));
        System.out.println(list);

        Record last = list.get(list.size()-1);
        last._member = 5;
        System.out.println(list);
    }
}

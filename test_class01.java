class tclass01 {
    public static void main(String[] args) {
        A a = new A();
        a.member = 100;
        B b = new B();
        b.member = "Nguyen";
        System.out.println(a.member + " " + b.member);
    }
}
class A {
    int member;
}

class B {
    String member;
}

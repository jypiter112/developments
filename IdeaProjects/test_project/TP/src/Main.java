
package packX;

public class A {
    public void f1() {
        System.out.println("f1");
    }
    protected void f2() {
        System.out.println("f2");
    }
    private void f3() {
        System.out.println("f3");
    }
    void f4() {
        System.out.println("f4");
    }
    public void testaa() {
        A a = new A();
        a.f1();
        a.f2();
        a.f3();
        a.f4();
    }

}

package packX;

public class B {
    public void testaa() {
        A a = new A();
        a.f1();
        a.f2();
        a.f3();
        a.f4();
    }
}

package packX;

public class C extends A {
    public void testaa() {
        C c = new C();
        c.f1();
        c.f2();
        c.f3();
        c.f4();
    }
}

package packY;
import packX.A;

public class D {
    public void testaa() {
        A a = new A();
        a.f1();
        a.f2();
        a.f3();
        a.f4();
    }
}

package packY;
import packX.A;

public class E extends A {
    public void testaa() {
        E e = new E();
        e.f1();
        e.f2();
        e.f3();
        e.f4();
    }
}//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
void main() {

}

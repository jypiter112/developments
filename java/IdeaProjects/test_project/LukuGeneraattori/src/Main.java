import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class Main {
    static class Lukugeneraattori {
        Lukugeneraattori(int raja){

        }
        public int haeSeuraavaLuku(int luku){

        }
    }
    public static ArrayList<Integer> haeLuvut(int raja, Lukugeneraattori generaattori) {

    }
    public static void main(String[] args) {
        final Random r = new Random();
        for (int testi = 1; testi <= 3; testi++) {
            int raja = r.nextInt(50) + 10;
            System.out.println("Testataan rajalla " + raja);
            ArrayList<Integer> numerot = haeLuvut(raja, new Lukugeneraattori(r));
            System.out.println("Tulos:");
            System.out.println(numerot.toString());
            System.out.println("");
        }
    }
}
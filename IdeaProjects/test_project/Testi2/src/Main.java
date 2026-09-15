import java.util.Random;
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    public static int[][] lueDiagonaaliMatriisi(int koko) {
        if(koko <= 0) {
            // palauttaa tyhjän matriisin
            return new int[][]{{}};
        }

        int[][] matriisi = new int[koko][koko];

        Scanner user_input = new Scanner(System.in);
        for(int i = 0; i < koko; i++){
            for(int j = 0; j < koko; j++){
                // jos ei ole diagonaali täytä nollalla
                if(i != j){
                    matriisi[i][j] = 0;
                    continue;
                }

                // jos on diagonaali ohjelma jatkaa tästä
                System.out.print("Luku: ");
                String t = user_input.nextLine();

                try{
                    int num = Integer.parseInt(t);
                    matriisi[i][j] = num;
                } catch(NumberFormatException e){
                    throw new RuntimeException(e);
                }
            }
        }

        return matriisi;
    }

    public static void main(String[] args) {
        final Random r = new Random();
        int koko = 3;

        System.out.println("Metodikutsu: lueDiagonaaliMatriisi(" + koko + ")");
        int[][] tulos = lueDiagonaaliMatriisi(koko);
        System.out.println("Metodi palautti taulukon, jossa on " + tulos.length + " riviä ja " + tulos[0].length + " saraketta.");
        System.out.println("Kohteet:");
        for (int[] rivi : tulos) {
            System.out.println(Arrays.toString(rivi));
        }


    }
}
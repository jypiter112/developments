import java.text.NumberFormat;
import java.util.Random;
import java.util.Arrays;
import java.util.Scanner;


public class Testi{
    public static int[] lueArvot(){
        Scanner user_input = new Scanner(System.in);
        int[] arvot = {0,0};
        boolean invalid_input = true;

        while(invalid_input){
            System.out.print("Ensimmäinen luku: ");
            String raw_input = user_input.nextLine();
            try {
                // trim poistaa white spacet tai \n artifactit
                int t = Integer.parseInt(raw_input.trim());
                // bound checkki
                if(t >= 0){
                    arvot[0] = t;
                    invalid_input = false;
                }
            } catch(NumberFormatException e){
                throw new RuntimeException(e);
            }

        }
        invalid_input = true;

        while(invalid_input){
            System.out.print("Toinen luku: ");
            String raw_input = user_input.nextLine();
            try {
                // trim poistaa white spacet tai \n artifactit
                int t = Integer.parseInt(raw_input.trim());
                // bound checkki
                if(t != 0){
                    arvot[1] = t;
                    invalid_input = false;
                }
            } catch(NumberFormatException e){
                throw new RuntimeException(e);
            }

        }
        return arvot;
    }
    public static void main(String[] args) {
        final Random r = new Random();
        /*
        Kirjoita metodi nimeltä lueArvot,
        joka lukee käyttäjältä kaksi kokonaislukua käyttäen
        Scanner-luokkaa ja palauttaa arvot int[]-tyyppisenä taulukkona.
        Ensimmäisen luvun tulee olla positiivinen ja toinen ei saa olla nolla.
        Mikäli käyttäjä antaa ehdon vastaisen luvun, kyseistä lukua kysytään uudestaan, kunnes käyttäjä antaa kelvollisen luvun.
        nextLine-metodia
         */


        int[] tulos = lueArvot();
        System.out.println("Metodi palautti taulukon, jossa on " + tulos.length + " alkioita.");
        System.out.println("Alkiot ovat " + Arrays.toString(tulos));


    }
}
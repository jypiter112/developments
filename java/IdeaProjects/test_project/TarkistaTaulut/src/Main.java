
import java.util.Random;
import java.util.Arrays;


public class Main {
    public static int[][] tarkistaTaulukonTiedot(int[][] data){
        if(data.length == 0) {
            return new int[][]{{}};
        }
        // etsitään suurimman arrayn pituus, josta tulee padding pituus
        int padding_size = 0;
        for(int[] row : data){
            if(row.length > padding_size){
                padding_size = row.length;
            }
        }
        // tehdään palautettava taulukko
        int[][] checked_data = new int[data.length][padding_size];
        for(int i = 0; i < data.length; i++){
            for(int j = 0; j < padding_size; j++){
                if(j >= data[i].length) {
                    // jos ei löydy niin lisää "padding elementti"
                    checked_data[i][j] = 0;
                } else {
                    // jos löytyy elementti taulukosta lisää se
                    checked_data[i][j] = data[i][j];
                }
            }
        }

        return checked_data;
    }
    public static void main(String[] args) {
        final Random r = new Random();
        int[][] data = {{1, -1, 1}, {3,2}, {}};
        System.out.println("int[][] taulukko annettu argumenttina:");
        for (int[] rivi : data) {
            System.out.println(Arrays.toString(rivi));
        }

        int[][] tulos = tarkistaTaulukonTiedot(data);

        System.out.println("Metodi palautti taulukon, jonka koko on " + tulos.length + " riviä ja " + tulos[0].length + " saraketta.");

        System.out.println("Kohteet:");
        for (int[] rivi : tulos) {
            System.out.println(Arrays.toString(rivi));
        }


    }
}
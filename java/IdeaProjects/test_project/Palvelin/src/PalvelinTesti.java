import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ExecutionException;

public class PalvelinTesti {

    public static void main(String[] args) {
        final Random rnd = new Random();

        System.out.println("Palvelin palvelin = new Palvelin();");
        Palvelin palvelin = new Palvelin();
        palvelin.setIpOsoite("2001:db8::1:0");

        String[] srv = {"Palvelin", "Serveri", "Testi", "Servu", "Kone"};

        System.out.println("Testataan metodia setNimi eri arvoilla");
        String[] nimet = {srv[rnd.nextInt(srv.length)] + rnd.nextInt(999), null, "" + rnd.nextInt(999),
                "", "X", rnd.nextInt(999) + srv[rnd.nextInt(srv.length)]};
        for (String nimi : nimet) {
            if (nimi == null)
                System.out.println("Kutsutaan palvelin.setNimi(null);");
            else
                System.out.println("Kutsutaan palvelin.setNimi(\"" + nimi + "\");");
            palvelin.setNimi(nimi);
            System.out.println("palvelin.getNimi() palauttaa " + palvelin.getNimi());
        }
        System.out.println("");

        System.out.println("Testataan metodia setIpOsoite eri arvoilla");
        ArrayList<String> ipt = new ArrayList<>();

        for (int i=0; i<4; i++) {
            String ip = "" + Integer.toHexString(rnd.nextInt(65536));
            for (int n = 1; n < 7; n++) {
                ip += ":"+ Integer.toHexString(rnd.nextInt(65536));
            }
            ipt.add(ip);
        }
        ipt.add("::");
        String ip = Integer.toHexString(rnd.nextInt(65536)) + ":" + Integer.toHexString(rnd.nextInt(65536)) +
                "::" + Integer.toHexString(rnd.nextInt(65536))+":0:0:"+Integer.toHexString(rnd.nextInt(65536));
        ipt.add(ip);
        ipt.add(srv[rnd.nextInt(srv.length)] + rnd.nextInt(999));

        String[] letters = {"gggg","hhhh","iiii", "ghzy", "zzzz", "zxrh"};
        ipt.add(letters[rnd.nextInt(letters.length)] + ":db8:0000:1:1:1:1:1");
        //Collections.shuffle(ipt, rnd);

        for (String ipo : ipt) {
            System.out.println("Kutsutaan palvelin.setIpOsoite(\"" + ipo + "\");");
            palvelin.setIpOsoite(ipo);
            System.out.println("palvelin.getIpOosoite() palauttaa " + palvelin.getIpOsoite());
        }
        System.out.println("");

        System.out.println("Testataan metodia setKapasiteetti eri arvoilla");
        for (int i = 0; i < 10; i++) {
            int kapa = rnd.nextInt(10) + (i-5);
            System.out.println("Kutsutaan palvelin.setKapasiteetti(" + kapa + ");");
            palvelin.setKapasiteetti(kapa);
            System.out.println("palvelin.getKapasiteetti() palauttaa " + palvelin.getKapasiteetti());
        }
    }
}


class Palvelin {
    private String ipOsoite;

    private String nimi;

    private int kapasiteetti;

    public Palvelin() {
    }
    void setNimi(String nimi){
        if(nimi == null){
            return;
        }
        else if(!nimi.isEmpty() &&
                Character.isLetter(nimi.charAt(0))) {
            this.nimi = nimi;
        }
    }
    void setKapasiteetti(int kapasiteetti){
        if(kapasiteetti >= 0){
            this.kapasiteetti = kapasiteetti;
        }
    }
    void setIpOsoite(String ip_addr){
        // 1. sisältää vähintään 2 ja enintään 7 kaksoispistettä
        int dcolon_count = 0;
        for(var c : ip_addr.toCharArray()){
            if(c == ':') {
                dcolon_count++;
            }
        }
        if(dcolon_count < 2 || dcolon_count > 7){
            return;
        }

        // 2. Kaksoispisteiden väli
        String[] splits = ip_addr.split(":");
        /*
        for(var s : splits){
            if(s.isEmpty()){
                continue;
            }
            else if(!s.matches("\\b([a-fA-F0-9])\\b")){
                return;
            }
        } */
        for(var s : splits){
            if(s.isEmpty()){
                continue;
            }
            try {
                int hex = Integer.parseInt(s, 16);
            } catch(Exception e) {
                return;
            }
        }
        this.ipOsoite = ip_addr;
    }
    String getNimi(){
        return this.nimi;
    }
    String getIpOsoite(){
        return this.ipOsoite;
    }
    int getKapasiteetti(){
        return this.kapasiteetti;
    }
}
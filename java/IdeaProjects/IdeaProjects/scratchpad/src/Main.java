//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
void main() {
    //TIP Press <shortcut actionId="ShowIntentionActions"/> with your caret at the highlighted text
    // to see how IntelliJ IDEA suggests fixing it.
    IO.println(String.format("Hello and welcome!"));

    for (int i = 1; i <= 5; i++) {
        //TIP Press <shortcut actionId="Debug"/> to start debugging your code. We have set one <icon src="AllIcons.Debugger.Db_set_breakpoint"/> breakpoint
        // for you, but you can always add more by pressing <shortcut actionId="ToggleLineBreakpoint"/>.
        IO.println("i = " + i);
    }
}

/**
 * Yksittäistä tuotetta kuvaava luokka.
 *
 * @param name   Tuotteen nimi
 * @param weight Paino grammoina
 * @param height Korkeus millimetreinä
 * @param width  Leveys millimetreinä
 * @param length Pituus millimetreinä
 */
public record Product(
        String name, double weight,
        int height, int width, int length
) {
    public int getVolume() {
        return height * width * length;
    }

    @Override
    public String toString() {
        return name;
    }
}

/**
 * Varastotilannetta kuvaava luokka.
 */
public class Warehouse {
    private final List<InventoryItem> warehouse = new ArrayList<>();

    public void addNewProduct(Product product, int initialAmount) {
        warehouse.add(new InventoryItem(product, initialAmount));
    }

    public Collection<Product> getProducts() {
        return warehouse
                .stream()
                .map(InventoryItem::getProduct)
                .toList();
    }

    public Optional<InventoryItem> findItem(Product product) {
        return warehouse
                .stream()
                .filter(i -> i.getProduct().equals(product))
                .findFirst();
    }

    public void reserveProduct(Client client,Product product,int amount){
        Optional<InventoryItem> found = findItem(product);

        if (found.isPresent())
            found.get().reserve(client, amount);
    }

    public void adjustProduct(Product product, int amount) {
        Optional<InventoryItem> found = findItem(product);

        if (found.isPresent())
            found.get().setStock(amount);
    }

    @Override
    public String toString() {
        return warehouse.stream().map(Object::toString)
                .collect(Collectors.joining("\n\n"));
    }
}
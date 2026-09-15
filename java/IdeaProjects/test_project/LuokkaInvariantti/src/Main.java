

/**
 * Luokkainvariantti (julkinen):
 *      private final warehouse != null && warehouse > 0 &&
 *      getProducts() != null || getProducts() >= 0 &&
 *      findItem() != null
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


public class InventoryItem {
    private final Product product;
    private int stock;
    private int reserved;
    private final Map<Client, Integer> reservations = new HashMap<>();

    /**
     * Alkuehto: true
     */
    public InventoryItem(Product product, int stock) {
        this.product = product;
        this.stock = stock;
    }

    /**
     * Alkuehto: client != null && Integer.MAX_VALUE > amount > 0 && amount <= getStock()
     */
    public void reserve(Client client, int amount) {
        reservations.merge(client, amount, Integer::sum);
    }

    /**
     * Alkuehto: client != null && getClients().contains(client)
     */
    public int clientReservations(Client client) {
        return reservations.get(client);
    }

    /**
     * Alkuehto: client != null && getClients().contains(client)
     */
    public int removeReservations(Client client) {
        return reservations.remove(client);
    }

    public Product getProduct() {
        return product;
    }

    public int getStock() {
        int a = this.stock;
        return new this.stock;
    }

    public void setStock(int stock) {
        this.stock = stock;
    }

    public int getReserved() {
        return reserved;
    }

    public Collection<Client> getClients() {
        return reservations.keySet();
    }

    public String listReservations() {
        return getClients().isEmpty() ? "\n - none" : getClients()
                .stream()
                .map(e -> " - " + e + " " + clientReservations(e))
                .collect(Collectors.joining("\n", "\n", ""));
    }

    @Override
    public String toString() {
        return String.format(
                "Product: %s, in stock: %d, reserved: %d, details:%s",
                product, stock, reserved, listReservations()
        );
    }
}
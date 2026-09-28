package org.example;

import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main {

    public static String ReadFile(String filename) {
        StringBuilder file_contents = new StringBuilder();
        File fileobj = new File(filename);
        if (!fileobj.exists()) {
            throw new RuntimeException("file not found");
        }
        if (!fileobj.canRead()) {
            throw new RuntimeException("cant read file");
        }

        try (Scanner myReader = new Scanner(fileobj)) {
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                file_contents.append(data);
                System.out.println(data);
            }
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        return file_contents.toString();
    }

    public static void main(String[] args) {
        try {
            HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);
            server.createContext("/", new MyHandler());
            server.setExecutor(null);
            server.start();
            System.out.println("Server is running on port 8000");
        } catch (IOException e) {
            System.out.println("Error starting the server: " + e.getMessage());
        }
    }

    static class MyHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            byte[] responseBytes = ReadFile("index.html").getBytes();
            exchange.sendResponseHeaders(200, responseBytes.length);
            OutputStream os = exchange.getResponseBody();
            os.write(responseBytes);
            os.close();
        }
    }
}
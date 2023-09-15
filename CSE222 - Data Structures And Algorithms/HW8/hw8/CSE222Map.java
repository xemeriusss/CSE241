package hw8;

// başlangıç noktası 1 se exception
// graph oluştur
// dijkstra
// bfs
// path çiz
// pathi png ye yaz

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import javax.imageio.ImageIO;

/**
 * 
 * CSE222Map is a class that reads a map from a file.
 * @version 1.0 04.06.2023
 * @author  Berru Lafci
 */
public class CSE222Map {
    private Map<String, Integer> map; // key is coordinate, value is 1 or 0
    private String startPoint;
    private String endPoint;
    private int X_SIZE;
    private int Y_SIZE;
    
    /**
     * Constructor for CSE222Map class.
     * @param fileName name of the file that contains map information
     * @param X_SIZE width of the map
     * @param Y_SIZE height of the map
     * @throws IOException if file cannot be read
     */
    public CSE222Map(String fileName, int X_SIZE, int Y_SIZE) {
        this.map = new HashMap<>();
        this.X_SIZE = X_SIZE;
        this.Y_SIZE = Y_SIZE;

        try {
            BufferedReader reader = new BufferedReader(new FileReader(fileName)); // read file

            // Read start point 
            String[] startArr = reader.readLine().split(","); 
            int startY = Integer.parseInt(startArr[0]); // first coordinate is y
            int startX = Integer.parseInt(startArr[1]); // second coordinate is x
            startPoint = coordinateFormat(startX, startY); // format into x,y

            // Read end point
            String[] endArr = reader.readLine().split(",");
            int endX = Integer.parseInt(endArr[1]);
            int endY = Integer.parseInt(endArr[0]);
            endPoint = coordinateFormat(endX, endY);

            // Read map matrix
            String line;
            int lineCount = 0; 
            
            while ((line = reader.readLine()) != null && lineCount < Y_SIZE) { // read until end of file or until lineCount reaches Y_SIZE
                String[] coordinates = line.split(",");

                for (int i = 0; i < X_SIZE && i < coordinates.length; i++) {

                    // replace -1 with 1 and put into map
                    if (coordinates[i].equals("-1")) {
                        map.put(coordinateFormat(i, lineCount), 1);  
                    } 

                    // put coordinate into map
                    else { 
                        int value = Integer.parseInt(coordinates[i]);
                        map.put(coordinateFormat(i, lineCount), value);
                    }
                }
                lineCount++;
            }
            reader.close();
        } catch (IOException e) { // Catch the exception if the file cannot be read
            e.printStackTrace();
        }
    }

    
    /**
     * The method takes two integer parameters and returns a string in the format "x,y".
     * 
     * @param x An integer representing the x-coordinate of a point.
     * @param y The parameter "y" is an integer representing the y-coordinate of a point.
     * @return Returns a string in the format "x,y".
     */
    private String coordinateFormat(int x, int y) {
        return x + "," + y;
    }

    /**
     * Getter returns the map.
     * @return map
     */
    public Map<String, Integer> getMap() {
        return map;
    }

    /**
     * Getter returns the start point.
     * @return startPoint of the map
     */
    public String getStartPoint() {
        return startPoint;
    }

    /**
     * Getter returns the end point.
     * @return endPoint of the map
     */
    public String getEndPoint() {
        return endPoint;
    }

    /**
     * Getter returns the width of the map.
     * @return X_SIZE of the map
     */
    public int getX_SIZE() {
        return X_SIZE;
    }

    /**
     * Getter returns the height of the map.
     * @return Y_SIZE of the map
     */
    public int getY_SIZE() {
        return Y_SIZE;
    }

    public int[] getCoordinates(String current) {
        String[] arr = current.split(",");
        int[] coordinates = new int[2];
        coordinates[0] = Integer.parseInt(arr[0]); // x
        coordinates[1] = Integer.parseInt(arr[1]); // y
        return coordinates;
    }


    /**
     * The function converts a map represented by a hashmap into a PNG image and saves it to a file.
     * 
     * @param filename The name of the file to be saved as a PNG image.
     * @return A BufferedImage object is being returned.
     */
    public BufferedImage convertPNG(String filename) {
        BufferedImage image = new BufferedImage(Y_SIZE, X_SIZE, BufferedImage.TYPE_INT_RGB); 

        for (int i = 0; i < X_SIZE; i++) { // Loop through the map
            for (int j = 0; j < Y_SIZE; j++) {
                String coordinate = coordinateFormat(i, j);
                int value = map.get(coordinate); // Get the value of the coordinate

                // If the start point is 1, throw an exception
                if(coordinate.equals(startPoint)) {
                    if(value == 1) { 
                        throw new IllegalArgumentException("Start point cannot be 1!");
                    } 
                }  

                // If the end point is 1, throw an exception
                if(coordinate.equals(endPoint)) {
                    if(value == 1) {
                        throw new IllegalArgumentException("End point cannot be 1!");
                    }
                } 

                int rgb = 0;

                // Set the color of the coordinate according to its value
                if(value == 1){
                    rgb = Color.darkGray.getRGB(); // 1 is dark gray
                } else if(value == 0){
                    rgb = Color.pink.getRGB(); // 0 is pink
                } 

                image.setRGB(i, j, rgb); // Set the color of the coordinates
            }
        }

        try { 
            filename = "Mapof_" + filename + ".png";
            ImageIO.write(image, "png", new File(filename)); // Write the image to a file
        } catch (IOException e) { 
            e.printStackTrace();
        }
        
        return image;
    }
    
}


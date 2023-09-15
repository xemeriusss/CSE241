package hw8;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;

import javax.imageio.ImageIO;

/**
 * 
 * CSE222Dijkstra class is used to find the shortest path with Dijkstra algorithm.
 * @version 1.0 04.06.2023
 * @author  Berru Lafci
 */
public class CSE222Dijkstra {

    private CSE222Graph graph;
    private int length;
    List<String> path;

    /**
     * Constructor for CSE222Dijkstra class.
     * @param graph graph object that contains graph information
     */
    public CSE222Dijkstra(CSE222Graph graph) {
        this.graph = graph;
        path = findPath(); // find the shortest path when the object is created
    }

    /**
     * Getter method for length of the path.
     * @return length of the path
     */
    public int getLength() {
        return length;
    }

    /**
     * This function finds the shortest path between two points in a graph using Dijkstra's algorithm.
     * 
     * @return a List of Strings representing the shortest path from the start
     * coordinate to the end coordinate in a graph. If the start or end coordinate is not present in
     * the graph, the method returns null.
     */
    public List<String> findPath() {
        String startCoordinate = graph.getStartPoint();
        String endCoordinate = graph.getEndPoint();
    
        if (graph.containsCoordinate(startCoordinate) == false || graph.containsCoordinate(endCoordinate) == false) {
            return null; //  Start or end coordinate is not in the graph
        }
    
        Set<String> visited = new HashSet<>(); // // Set to track the visited coordinates
        Queue<String> queue = new LinkedList<>(); // Queue to track the coordinates to be visited

        Map<String, Integer> distance = new HashMap<>(); // Distance from start coordinate to a coordinate
                                                         // key is coordinate, value is distance

        Map<String, String> previous = new HashMap<>(); // Previous coordinate of a coordinate
                                                        // key is coordinate, value is previous coordinate
        
        // Initialize the distance and previous maps
        for (String coordinate : graph.getGraph().keySet()) { 
            distance.put(coordinate, 1000); // It can be maximum 1000
            previous.put(coordinate, null); // Initialize the previous coordinate as null
        }
    
        distance.put(startCoordinate, 0); // Distance from start to start is 0
        queue.add(startCoordinate); // Add the start coordinate to the queue
    
        // While the queue is not empty, visit the neighbors of the current coordinate
        while (!queue.isEmpty()) {
            String current = queue.poll(); // Get the first element of the queue
            visited.add(current); // Add the current coordinate to the visited set
    
            // Visit the neighbors of the current coordinate
            // with getting values from the graph
            for (String neighbor : graph.getGraph().get(current)) { 
                if (visited.contains(neighbor) == false) { 
                    // Calculate the distance from start to neighbor
                    int tempDist = distance.get(current) + graph.getDistance(current, neighbor);

                    // If the temp distance is less than the current distance, 
                    // update the distance and previous maps according to Dijsktra's algorithm
                    if (tempDist < distance.get(neighbor)) { 
                        distance.put(neighbor, tempDist); // Update the distance
                        previous.put(neighbor, current); // Update the previous coordinate
                        queue.add(neighbor); // Add the neighbor to the queue
                    }
                }
            }
        }
    
        List<String> path = new ArrayList<>();
        String current = endCoordinate;
        
        while (current != null) {
            path.add(current); // Add the current coordinate to the path
            current = previous.get(current); // Get the previous coordinate
        }

        return path;
    }

    public BufferedImage drawPath(BufferedImage image, String filename) {
        
        if(path == null || path.size() == 0) { // If there is no path, return the empty image
            System.out.println("No path found for BFS algorithm.");
            return image;
        }

        System.out.println("Dijkstra Path length: " + path.size() + " File name: " + filename);

        for (int i = 0; i < path.size(); ++i) {
            String[] current = path.get(i).split(","); // Split the coordinate string
            int x = Integer.parseInt(current[0]); // Get the x coordinate
            int y = Integer.parseInt(current[1]); // Get the y coordinate
            image.setRGB(x, y, Color.red.getRGB()); // Set the color of the path to red
        }
        try {
            filename = "Dijkstra: " + filename + ".png";
            ImageIO.write(image, "png", new File(filename)); // Write the image to a file
        } catch (IOException e) {
            e.printStackTrace();
        }

        return image;
    }  
    
}

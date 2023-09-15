package hw8;

import java.awt.Color;
import java.util.List;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Set;

import javax.imageio.ImageIO;

/**
 * 
 * CSE222BFS class is used to find the shortest path with BFS algorithm.
 * @version 1.0 04.06.2023
 * @author  Berru Lafci
 */
public class CSE222BFS {

    private CSE222Graph graph;
    private int length;
    List<String> path;

    /**
     * Constructor for CSE222BFS class.
     * @param graph graph object that contains graph information
     */
    public CSE222BFS(CSE222Graph graph) {
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
     * The method finds the shortest path between a start and end coordinate in a graph using BFS
     * traversal.
     * 
     * @return a List of Strings representing the shortest path from the
     * start coordinate to the end coordinate in a graph. If there is no path 
     * from the start to the end coordinate, it returns null.
     */
    public List<String> findPath() {
        String startCoordinate = graph.getStartPoint(); // Start coordinate as a string
        String endCoordinate = graph.getEndPoint(); // End coordinate as a string

        if (graph.containsCoordinate(startCoordinate) == false || graph.containsCoordinate(endCoordinate) == false) {
            return null; // Start or end coordinate is not in the graph
        }
    
        Queue<String> queue = new LinkedList<>(); // Queue for BFS 
        Map<String, String> parentMap = new HashMap<>(); // Map to track the parent of each coordinate
        Set<String> visited = new HashSet<>(); // Set to track the visited coordinates
    
        queue.offer(startCoordinate); // Add start coordinate to the queue
        visited.add(startCoordinate); // Mark start coordinate as visited
    
        // make a BFS traversal
        while (!queue.isEmpty()) {

            String currentCoordinate = queue.poll(); // Get the next coordinate from the queue
            
            // If the current coordinate is the end coordinate, we found the path
            if (currentCoordinate.equals(endCoordinate)) { 
                List<String> path = new ArrayList<>();

                // Backtrack from the end coordinate to the start coordinate, 
                // using the parent map to find the shortest path
                while (!currentCoordinate.equals(startCoordinate)) {
                    path.add(0, currentCoordinate); // Add the current coordinate to the path
                    currentCoordinate = parentMap.get(currentCoordinate); // Get the parent of the current coordinate
                }

                path.add(0, startCoordinate); // Add the start coordinate to the path and return the path
                return path; 
            }
    
            List<String> edges = graph.get(currentCoordinate); // Get the edges of the current coordinate
    
            // For each edge of the current coordinate
            for (String edge : edges) { 
                if (visited.contains(edge) == false) { 
                    queue.offer(edge); // Add the edge to the queue
                    visited.add(edge); // Mark the edge as visited
                    parentMap.put(edge, currentCoordinate); // Add the edge to the parent map
                }
            }
        }
    
        return null; // No path found from start to end
    }


    /**
     * The method draws the shortest path between a start and end coordinate in a graph on an image.
     * Then it saves the image to a file.
     * 
     * @param image a BufferedImage object representing the image on which the path needs to be drawn
     * @param filename The name of the file to which the image will be saved. 
     * @return a BufferedImage object representing the image on which the path has been drawn
     */
    public BufferedImage drawPath(BufferedImage image, String filename) {
        
        if(path == null || path.size() == 0) { // If there is no path, return the empty image
            System.out.println("No path found for BFS algorithm.");
            return image;
        }

        System.out.println("BFS Path length: " + path.size() + " File name: " + filename);

        for (int i = 0; i < path.size(); ++i) {
            String[] current = path.get(i).split(","); // Split the coordinate string
            int x = Integer.parseInt(current[0]); // Get the x coordinate
            int y = Integer.parseInt(current[1]); // Get the y coordinate
            image.setRGB(x, y, Color.red.getRGB()); // Set the color of the path to red
        }
        try {
            filename = "BFS: " + filename + ".png";
            ImageIO.write(image, "png", new File(filename)); // Write the image to a file
        } catch (IOException e) {
            e.printStackTrace();
        }

        return image;
    }  
    
}

package hw8;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 
 * CSE222Graph class is used to build a graph from a map. 
 * @version 1.0 04.06.2023
 * @author  Berru Lafci
 */
public class CSE222Graph {

    private CSE222Map map;
    private Map<String, List<String>> graph;    // It is a map where the keys are strings representing coordinates, 
                                                // and the values are lists of strings representing 
                                                // the neighboring coordinates.
    

    /**
     * Constructor for CSE222Graph class.
     * @param map map object that contains map information
     */
    public CSE222Graph(CSE222Map map) {
        this.map = map;
        this.graph = buildGraph();
    }

    /**
     * Getter method for graph.
     * @return graph
     */
    public Map<String, List<String>> getGraph() {
        return graph;
    }

    /**
     * Getter method for size of the graph.
     * @return size of the graph
     */
    public int size() {
        return graph.size();
    }

    /**
     * Getter method for map.
     * @return map
     */
    public CSE222Map getMap() {
        return map;
    }

    /**
     * Getter method for start point of the map.
     * @return start point
     */
    public String getStartPoint() {
        return map.getStartPoint();
    }

    /**
     * Getter method for end point of the map.
     * @return end point
     */
    public String getEndPoint() {
        return map.getEndPoint();
    }

    /**
     * This method checks if a given coordinate is in the graph.
     * @param coordinate coordinate to be checked
     * @return true if the coordinate is in the graph, false otherwise
     */
    public boolean containsCoordinate(String coordinate) {
        return graph.containsKey(coordinate);
    }

    /**
     * This method returns the list of edges of a given coordinate.
     * @param currentCoordinate coordinate to be checked
     * @return list of edges
     */
    public List<String> get(String currentCoordinate) {
        return graph.get(currentCoordinate);
    }

    /**
     * This method is used to format coordinates into x,y format.
     * @param x x coordinate
     * @param y y coordinate
     * @return formatted coordinate
     */
    private String coordinateFormat(int x, int y) {
        return x + "," + y;
    }

    /**
     * This method builds a graph by iterating over a map of coordinates and their values, adding
     * each coordinate and its valid neighboring coordinates as edges to the graph.
     * 
     * @return a `Map<String, List<String>>` object, which represents
     * a graph where the keys are coordinates in the format "x,y" and the values are lists of
     * neighboring coordinates.
     */
    private Map<String, List<String>> buildGraph() {
        Map<String, List<String>> graph = new HashMap<>(); // key: coordinate, value: list of edges
        Map<String, Integer> mapValues = map.getMap(); // key: coordinate, value: 1 or 0
        int X_SIZE = map.getX_SIZE();
        int Y_SIZE = map.getY_SIZE();
    
        for (int i = 0; i < X_SIZE; i++) {
            for (int j = 0; j < Y_SIZE; j++) {

                String currentCoordinate = coordinateFormat(i, j); // format into x,y
                int currentValue = mapValues.get(currentCoordinate); // get value from map
    
                // Skip non-zero coordinates
                if (currentValue != 0) {
                    continue;
                }
    
                // Add current coordinate and its edges to graph
                List<String> edges = new ArrayList<>();
                graph.put(currentCoordinate, edges);
    
                // Neighboring coordinates
                int[] neighborCoords = {
                        -1, 0,   // (y-1, x)
                        1, 0,    // (y+1, x)
                        0, -1,   // (y, x-1)
                        0, 1,    // (y, x+1)
                        -1, -1,  // (y-1, x-1)
                        -1, 1,   // (y-1, x+1)
                        1, 1,    // (y+1, x+1)
                        1, -1    // (y+1, x-1)
                };
    
                // Iterate over edge coordinates
                for (int k = 0; k < neighborCoords.length; k += 2) {

                    int offsetY = neighborCoords[k]; // first element
                    int offsetX = neighborCoords[k + 1]; // second element

                    int neighborY = i + offsetY; // current y + offset
                    int neighborX = j + offsetX; // current x + offset
                    
                    // Check if the neighbor coordinate is valid
                    if ((neighborY >= 0) && (neighborY < X_SIZE) && (neighborX >= 0) && (neighborX < Y_SIZE)) {
                        String neighborCoordinate = coordinateFormat(neighborY, neighborX); // format into x,y

                        int neighborValue = -1;

                        // Check if the neighbor coordinate is in the map
                        if(mapValues.containsKey(neighborCoordinate)){
                            neighborValue = mapValues.get(neighborCoordinate);  // get value from map
                        } else{
                            neighborValue = -1;  // -1 if it is not in the map
                        }
    
                        // Skip non-zero coordinates
                        if (neighborValue != 0) {
                            continue;
                        }
    
                        // Add neighbor coordinate to current coordinate's edges
                        edges.add(neighborCoordinate);
                    }
                }
            }
        }
    
        return graph;
    }

    public Integer getDistance(String current, String neighbor) {
        int[] currentCoordinates = map.getCoordinates(current);
        int[] neighborCoordinates = map.getCoordinates(neighbor);

        int currentX = currentCoordinates[0]; // x coordinate of current
        int currentY = currentCoordinates[1]; // y coordinate of current
        int neighborX = neighborCoordinates[0]; // x coordinate of neighbor
        int neighborY = neighborCoordinates[1]; // y coordinate of neighbor

        // Calculate distance between current and neighbor with Pythagorean theorem
        int distance = (int) Math.sqrt(Math.pow(currentX - neighborX, 2) + Math.pow(currentY - neighborY, 2));

        return distance;
    }

}

package hw7;
import java.util.ArrayList;
import java.util.LinkedHashMap;

/**
 * 
 * myMap class that stores a string and builds a map of the string 
 * with each character as a key and the number of times it appears.
 * @version 1.0 16.05.2023
 * @author  Berru Lafci
 */
public class myMap {
    
    private LinkedHashMap<String, info> map; // key: String(char), value: info(count, word)
    private int mapSize;
    private String str;
    private String preprocessedStr;

    /**
     * Constructor for myMap class.
     * @param str string to make the map
     */
    public myMap(String str) {

        // check if string is null
        try {
            if(str.isEmpty()) {
                throw new NullPointerException();
            }
        }
        catch(NullPointerException e) {
            System.out.println("Input is empty.");
            System.exit(1);
        }

        this.map = new LinkedHashMap<String, info>();
        this.mapSize = 0;
        this.str = str;
    }

    /**
     * Getter returns string to make the map.
     * @return string to make the map
     */
    public String getStr() {
        return str; 
    }

    /**
     * Getter returns preprocessed string.
     * @return preprocessed string
     */
    public String getPreprocessedStr() {
        return preprocessedStr;
    }

    /**
     * Getter returns map size.
     * @return map size
     */
    public int getMapSize() {
        return mapSize;
    }

    /**
     * Getter returns map.
     * @return map
     */
    public LinkedHashMap<String, info> getMap() {
        return map;
    }

    /**
     * The method returns an array of count values from a map of info objects.
     * @return An integer array containing the count values.
     */
    public int[] getCountValues() {
        int[] values = new int[mapSize];
        int i = 0;
        for (String key : map.keySet()) {
            info value = map.get(key);
            values[i] = value.getCount();
            i++;
        }
        return values;
    }

    /**
     * This method returns an ArrayList of all the keys in a map.
     * @return An ArrayList of all the keys in the map.
     */
    public ArrayList<String> getKeys() {
        ArrayList<String> keys = new ArrayList<String>();
        for (String key : map.keySet()) {
            keys.add(key);
        }
        return keys;
    }

    /**
     * The method removes all non-alphabetic characters from a string and converts it to lowercase.
     */
    public void processStr() {
        preprocessedStr = str.replaceAll("[^a-zA-Z ]", "");
        preprocessedStr = preprocessedStr.toLowerCase();
            
    }
 
    /**
     * The function builds a map where each key is a character in a preprocessed string and the value
     * is an "info" object containing the count of the character and the words in which it appears.
     */
    public void buildMap() {

        int wordCounter = 0;
        String[] words = preprocessedStr.split(" ");

        for (int i = 0; i < preprocessedStr.length(); i++) {

            String key = String.valueOf(preprocessedStr.charAt(i)); // char

            // if the character is a space
            if(key.equals(" ")) {
                wordCounter++;
                continue;
            }

            if (map.containsKey(key)) {     // if the key already exists
                info value = map.get(key);  // get the value of the key              
                value.setCount(value.getCount() + 1); // increment the count
                value.getWords().add(words[wordCounter]);
            } 
            else {                          // if the key does not exist
                info value = new info(1, words[wordCounter]);
                map.put(key, value); // add the key and value to the map
                mapSize++;
            }
        }
    }

    public void printMap() {
        for (String key : map.keySet()) {
            info value = map.get(key);
            System.out.println("Key: " + key + ", Value: " + value.getCount() + ", " + value.getWords());
        }
        
    }

    

}

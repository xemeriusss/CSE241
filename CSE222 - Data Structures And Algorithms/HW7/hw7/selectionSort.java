package hw7;
import java.util.ArrayList;

/**
 * 
 * selectionSort class that sorts a map by count of characters.
 * @version 1.0 20.05.2023
 * @author  Berru Lafci
 */
public class selectionSort {
    
    private myMap originalMap;
    private myMap sortedMap;
    private ArrayList<String> aux;

    /**
     * Constructor for selectionSort class.
     * @param map map to sort
     */
    public selectionSort(myMap map) {
        this.originalMap = map;
        this.sortedMap = new myMap(map.getStr());
        this.aux = originalMap.getKeys(); // get keys from original map
    }

    /**
     * Getter returns sorted map.
     * @return sorted map
     */
    public myMap getSortedMap() {
        return sortedMap;
    }

    /**
     * Getter returns aux arraylist which is the keys of the original map.
     * @return aux arraylist
     */
    public ArrayList<String> getAux() {
        return aux;
    }

    /**
     * The method sorts the values in a map using selection sort and stores them in a new map.
     */
    public void sortMap() {
        int[] values = originalMap.getCountValues(); // get count array from original map

        long startTime = System.nanoTime();

        selection_sort(values, aux, 0, values.length - 1); 

        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("Selection sort duration: " + duration + " nanoseconds");

        sortedMap = new myMap(originalMap.getStr()); // create new map to store sorted values

        // copy sorted values to sorted map
        for (int i = 0; i < values.length; i++) {
            String key = aux.get(i);
            info value = originalMap.getMap().get(key); // get value from original map
            sortedMap.getMap().put(key, value); // put key and value in sorted map
        }
    }
    
    /**
     * The selection_sort method sorts an array of integers and an ArrayList of strings using the
     * selection sort algorithm.
     * Tracks the minimum value during each iteration and at the end of each iteration it swaps the
     * minimum value with the value at the current index.
     * 
     * @param values an integer array that needs to be sorted using selection sort algorithm.
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the selection sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the subarray to be sorted.
     * @param right The index of the rightmost element in the subarray to be sorted.
     */
    public void selection_sort(int[] values, ArrayList<String> aux, int left, int right) {
        for (int i = left; i < right; i++) {
            int min = i;

            // find minimum value in right subarray
            for (int j = i + 1; j <= right; j++) {
                if (values[j] < values[min]) { // if value at j is less than value at min
                    min = j; // new minimum is j
                }
            }
            // swap values
            int temp = values[i];
            values[i] = values[min];
            values[min] = temp;

            // swap keys in aux 
            String temp2 = aux.get(i);
            aux.set(i, aux.get(min)); // set key at i to key at min
            aux.set(min, temp2);
        }
    }

    /**
     * This method prints the original map and the sorted map.
     */
    public void printMap() {
        System.out.println("\nSELECTON SORT:");
        System.out.println("Original (unsorted) Map:");
        for (String key : originalMap.getMap().keySet()) {
            info value = originalMap.getMap().get(key);
            System.out.println("Letter: " + key + " - Count:  " + value.getCount() + " - Words:" + value.getWords());
        }
        System.out.println("Sorted Map:");
        for (String key : sortedMap.getMap().keySet()) {
            info value = sortedMap.getMap().get(key);
            System.out.println("Letter: " + key + " - Count:  " + value.getCount() + " - Words:" + value.getWords());
        }
    }

}

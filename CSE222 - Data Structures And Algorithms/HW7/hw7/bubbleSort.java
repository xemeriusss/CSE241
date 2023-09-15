package hw7;
import java.util.ArrayList;

/**
 * 
 * bubbleSort class that sorts a map by count of characters.
 * @version 1.0 20.05.2023
 * @author  
 */
public class bubbleSort {

    private myMap originalMap;
    private myMap sortedMap;
    private ArrayList<String> aux;

    /**
     * Constructor for bubbleSort class.
     * @param map map to sort
     */
    public bubbleSort(myMap map) {
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
     * The method sorts the values in a map using bubble sort and stores them in a new map.
     */
    public void sortMap() {
        int[] values = originalMap.getCountValues(); // get count array from original map

        long startTime = System.nanoTime();

        bubble_sort(values, aux, 0, values.length - 1); 

        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("Bubble sort duration: " + duration + " nanoseconds");

        sortedMap = new myMap(originalMap.getStr()); // create new map to store sorted values

        // copy sorted values to sorted map
        for (int i = 0; i < values.length; i++) {
            String key = aux.get(i);
            info value = originalMap.getMap().get(key); // get value from original map
            sortedMap.getMap().put(key, value); // put key and value in sorted map
        }
    }

    /**
     * The function implements the bubble sort algorithm to sort an array of integers and an ArrayList
     * of strings.
     * 
     * @param values an integer array that needs to be sorted using bubble sort algorithm
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the bubble sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the array to be sorted.
     * @param right The index of the rightmost element in the array to be sorted.
     */
    public void bubble_sort(int[] values, ArrayList<String> aux, int left, int right) {
        // nested for loop to compare every pair of adjacent elements 
        // again and again until there are no more swaps
        boolean swapped;

        for (int i = left; i < right; i++) {
            swapped = false;
            for (int j = left; j < right - i; j++) {
                if (values[j] > values[j + 1]) { // check the pair
                    // swap values
                    int temp = values[j];
                    values[j] = values[j + 1];
                    values[j + 1] = temp;

                    // swap keys in aux 
                    String temp2 = aux.get(j);
                    aux.set(j, aux.get(j + 1)); 
                    aux.set(j + 1, temp2);

                    swapped = true;
                }
            }
            // if no swaps were made, then the array is already sorted
            if (swapped == false) {
                break;
            }
        }
    }

     /**
     * This method prints the original map and the sorted map.
     */
    public void printMap() {
        System.out.println("\nBUBBLE SORT:");
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

package hw7;
import java.util.ArrayList;

/**
 * 
 * insertionSort class that sorts a map by count of characters.
 * @version 1.0 20.05.2023
 * @author  
 */
public class insertionSort {
    
    private myMap originalMap;
    private myMap sortedMap;
    private ArrayList<String> aux;

    /**
     * Constructor for insertionSort class.
     * @param map map to sort
     */
    public insertionSort(myMap map) {
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
     * The method sorts the values in a map using insertion sort and stores them in a new map.
     */
    public void sortMap() {
        int[] values = originalMap.getCountValues(); // get count array from original map

        long startTime = System.nanoTime();

        insertion_sort(values, aux, 0, values.length - 1); 

        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("Insertion sort duration: " + duration + " nanoseconds");

        sortedMap = new myMap(originalMap.getStr()); // create new map to store sorted values

        // copy sorted values to sorted map
        for (int i = 0; i < values.length; i++) {
            String key = aux.get(i);
            info value = originalMap.getMap().get(key); // get value from original map
            sortedMap.getMap().put(key, value); // put key and value in sorted map
        }
    }

    
    /**
     * The function implements the insertion sort algorithm to sort an array of integers and an
     * ArrayList of strings.
     * 
     * @param values an integer array that needs to be sorted using insertion sort algorithm.
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the insertion sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the subarray being sorted.
     * @param right The index of the rightmost element in the subarray being sorted.
     */
    public void insertion_sort(int[] values, ArrayList<String> aux, int left, int right) {
        // start at index 1 in insertion sort
        for (int i = left + 1; i <= right; i++) {
            int count = values[i];
            String key = aux.get(i);
            int j = i - 1; // index of previous element

            // if previous element is greater than current element then shift elements to the right
            // until previous element is less than current element 
            while (left <= j) {
                // shift elements to the right 
                if(values[j] > count) { 
                    values[j + 1] = values[j];
                    aux.set(j + 1, aux.get(j));
                    j = j - 1;
                }
                else {
                    break;
                }
            }
            values[j + 1] = count;
            aux.set(j + 1, key);
        }
    }

    /**
     * This method prints the original map and the sorted map.
     */
    public void printMap() {
        System.out.println("\nINSERTION SORT:");
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

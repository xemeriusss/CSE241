package hw7;
import java.util.ArrayList;

/**
 * 
 * quickSort class that sorts a map by count of characters.
 * @version 1.0 20.05.2023
 * @author  Berru Lafci
 */
public class quickSort {
    
    private myMap originalMap;
    private myMap sortedMap;
    private ArrayList<String> aux;

    /**
     * Constructor for quickSort class.
     * @param map map to sort
     */
    public quickSort(myMap map) {
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
     * The method sorts the values in a map using quick sort algorithm and stores them in a new map.
     */
    public void sortMap() {
        int[] values = originalMap.getCountValues(); // get count array from original map

        long startTime = System.nanoTime();

        quick_sort(values, aux, 0, values.length - 1); 

        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("Quick sort duration: " + duration + " nanoseconds");
        
        sortedMap = new myMap(originalMap.getStr()); // create new map to store sorted values

        // copy sorted values to sorted map
        for (int i = 0; i < values.length; i++) {
            String key = aux.get(i);
            info value = originalMap.getMap().get(key); // get value from original map
            sortedMap.getMap().put(key, value); // put key and value in sorted map
        }
    }

    
    /**
     * This is a recursive implementation of the quicksort algorithm that sorts an array of integers
     * and an ArrayList of strings.
     * 
     * @param values an array of integers that needs to be sorted using the quick sort algorithm.
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the quick sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the subarray being sorted.
     * @param right The index of the rightmost element in the subarray being sorted.
     */
    public void quick_sort(int[] values, ArrayList<String> aux, int left, int right) {
        if (left < right) {
            int pivot = partition(values, aux, left, right);
            quick_sort(values, aux, left, pivot - 1);
            quick_sort(values, aux, pivot + 1, right);
        }
    }

    /**
     * The partition function takes an array of integers and an ArrayList of strings, and partitions
     * the array based on a pivot value, while also swapping the corresponding values in the ArrayList.
     * 
     * @param values an integer array that needs to be sorted using the quicksort algorithm
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the quick sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the array or subarray being partitioned.
     * @param right The index of the rightmost element in the subarray being partitioned.
     * @return The method is returning an integer value, which is the index of the pivot element after
     * partitioning the array.
     */
    public int partition(int[] values, ArrayList<String> aux, int left, int right) {
        int pivot = values[right];
        int i = left - 1;

        for (int j = left; j <= right; j++) {
            // if value at j is less than pivot, increment i and swap values at i and j
            if (values[j] < pivot) {
                i++;
                int temp = values[i];
                values[i] = values[j];
                values[j] = temp;

                String temp2 = aux.get(i);
                aux.set(i, aux.get(j)); 
                aux.set(j, temp2);
            }
        }
        i++;
        int temp = values[i];
        values[i] = values[right];
        values[right] = temp;

        String temp2 = aux.get(i);
        aux.set(i, aux.get(right)); // set key at i to key at j
        aux.set(right, temp2);

        return i;
    }

    /**
     * This method prints the original map and the sorted map.
     */
    public void printMap() {
        System.out.println("\nQUICK SORT:");
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

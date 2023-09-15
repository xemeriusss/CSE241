package hw7;
import java.util.ArrayList;

/**
 * 
 * mergeSort class that sorts a map by count of characters.
 * @version 1.0 16.05.2023
 * @author  Berru Lafci
 */
public class mergeSort {
    
    private myMap originalMap;
    private myMap sortedMap;
    private ArrayList<String> aux;

    /**
     * Constructor for mergeSort class.
     * @param map map to sort
     */
    public mergeSort(myMap map) {
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
     * This method uses merge sort to sort the values in an original map and stores them in a
     * new sorted map.
     */
    public void sortMap() {
        int[] values = originalMap.getCountValues(); // get count array from original map

        long startTime = System.nanoTime();

        merge_sort(values, aux, 0, values.length - 1); 

        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("Merge sort duration: " + duration + " nanoseconds");

        sortedMap = new myMap(originalMap.getStr()); // create new map to store sorted values

        // copy sorted values to sorted map
        for (int i = 0; i < values.length; i++) {
            String key = aux.get(i);
            info value = originalMap.getMap().get(key); // get value from original map
            sortedMap.getMap().put(key, value); // put key and value in sorted map
        }
    }

    /**
     * This is a recursive merge sort algorithm that sorts an array of integers.
     * 
     * @param values an array of integers that needs to be sorted using the merge sort algorithm.
     * @param aux The "aux" parameter is an ArrayList of Strings that is used as an auxiliary array
     * during the merge sort algorithm. It is used to temporarily store the sorted values during the
     * merging process.
     * @param left The index of the leftmost element in the array or subarray being sorted.
     * @param right The index of the rightmost element in the array or subarray being sorted.
     */
    public void merge_sort(int[] values, ArrayList<String> aux, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;
            merge_sort(values, aux, left, mid); // left half
            merge_sort(values, aux, mid + 1, right); // right half
            merge(values, aux, left, mid, right); // merge the two halves
        }
    }  


    /**
     * This method merges two subarrays of an array based on their values.
     * 
     * @param values an integer array that needs to be sorted using merge sort algorithm
     * @param aux The "aux" parameter is an ArrayList of Strings that contains auxiliary information
     * related to the "values" array being sorted. 
     * @param low The starting index of the subarray.
     * @param mid The index representing the end of the first subarray and the start of the second
     * subarray.
     * @param high The index of the last element in the subarray.
     */
    public void merge(int[] values, ArrayList<String> aux, int low, int mid, int high) {
        
        int[] tempArr = new int[high - low + 1]; // temporary array to store merged array
        ArrayList<String> tempKeys = new ArrayList<String>();
        int left = low; // index for left subarray
        int right = mid + 1; // index for right subarray
        int k = 0; // index for temp array

        // loop through left and right subarrays and copy values to temp array
        while (left <= mid && right <= high) {
            if (values[left] <= values[right]) {
                tempArr[k] = values[left]; // copy left subarray
                tempKeys.add(aux.get(left)); 
                left++; // move left index
            } else {
                tempArr[k] = values[right]; // copy right subarray
                tempKeys.add(aux.get(right));
                right++; // move right index
            }
            k++; // move temp index
        }

        // copy remaining elements from left subarray, if any
        if (left <= mid && right > high) {
            while (left <= mid) {
                tempArr[k] = values[left];
                tempKeys.add(aux.get(left));
                left++;
                k++;
            }
        }  
        // copy remaining elements from right subarray, if any
        else {
            while (right <= high) {
                tempArr[k] = values[right];
                tempKeys.add(aux.get(right));
                right++;
                k++;
            }
        }

        // copy temp array back to array
        for (k = 0; k < tempArr.length; k++) {
            values[low + k] = tempArr[k];
            aux.set(low + k, tempKeys.get(k));
        }
    }

    /**
     * This method prints the original map and the sorted map.
     */
    public void printMap() {
        System.out.println("\nMERGE SORT:");
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

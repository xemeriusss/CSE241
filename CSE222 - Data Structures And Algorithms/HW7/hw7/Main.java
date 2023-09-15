package hw7;

public class Main {
    
    public static void main(String[] args) {
        String str = "";
        myMap map = new myMap(str);
        System.out.println("Original string: " + map.getStr());
        map.processStr();
        System.out.println("Preprocessed string: " + map.getPreprocessedStr());

        map.buildMap();
        
        /*
        And I used these inputs for testing best, average and worst:
        abbccc dddd eeeee.!  (1,2,3,4,5) (sorted)
        aaa bbbbbb cc dddddddddd eee.!  (3,6,2,10,3) (randomly)
        aaaaaa bbbbb cccc dd e.!  (6,5,4,2,1) (descending)
        */
        
        mergeSort sort = new mergeSort(map);
        sort.sortMap();
        sort.printMap();
        
        
        /* 
        selectionSort sort = new selectionSort(map);
        sort.sortMap();
        sort.printMap();
        */

        /* 
        insertionSort sort = new insertionSort(map);
        sort.sortMap();
        sort.printMap();
        */
        
        /* 
        bubbleSort sort = new bubbleSort(map);
        sort.sortMap();
        sort.printMap();
        */

        /* 
        quickSort sort = new quickSort(map);
        sort.sortMap();
        sort.printMap();
        */
    }
}

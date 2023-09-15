package hw6;

public class Main {
    
    public static void main(String[] args) {
        String str = "Buzzing bees buzz.";
        myMap map = new myMap(str);
        System.out.println("Original string: " + map.getStr());
        map.processStr();
        System.out.println("Preprocessed string: " + map.getPreprocessedStr());

        map.buildMap();
        //map.printMap();
        mergeSort sort = new mergeSort(map);
        sort.sortMap();
        sort.printMap();
        
    }
}

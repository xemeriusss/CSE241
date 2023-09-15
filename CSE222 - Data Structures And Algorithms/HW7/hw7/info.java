package hw7;
import java.util.ArrayList;

/**
 * 
 * info class that stores the count and words of a character.
 * @version 1.0 16.05.2023
 * @author  Berru Lafci
 */
public class info {
    
    private int count;
    private ArrayList<String> words;

    /**
     * Constructor for info class.
     * @param count count of character
     * @param word words of character
     */
    public info(int count, String word) {
        this.count = count;
        this.words = new ArrayList<String>();
        this.words.add(word); // add word to words arraylist
    }

    /**
     * Getter returns count of character.
     * @return count of character
     */
    public int getCount() {
        return count;
    }

    /**
     * Getter returns words of character.
     * @return words of character
     */
    public ArrayList<String> getWords() {
        return words;
    }

    /**
     * Setter sets count of character.
     * @param count count of character
     */
    public void setCount(int count) {
        this.count = count;
    }

    /**
     * Setter sets words of character.
     * @param words words of character
     */
    public void setWords(ArrayList<String> words) {
        this.words = words;
    }
}

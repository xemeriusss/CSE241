package hw4;

/**
 * 
 * Password2 class is used to create an Password2 object and use in test classes.
 * @version 1.0 20.04.2023
 * @author  Berru Lafci
 */
public class Password2 {
    
    private int password2;
    private int[] denominations;

    /**
     * Constructor for Password2 class.
     * @param password2
     * @param denominations
     */
    public Password2(int password2, int[] denominations) {
        this.password2 = password2;
        this.denominations = denominations;
    }

    /**
     * Getter for password2.
     * @return password2
     */
    public int getPassword2() {
        return password2;
    }

    /**
     * Getter for denominations.
     * @return denominations
     */
    public int[] getDenominations() {
        return denominations;
    }

    /**
     * This function checks if a given password is valid based on certain conditions.
     * 
     * @param password2 an integer representing a password that needs to be validated.
     * @return The method is returning a boolean value, either true or false, depending on whether the
     * password2 is valid or not.
     */
    public boolean checkIfValidPassword2(int password2) {
        if (password2 < 10 || password2 > 10000) {
            System.out.println("The password2 is invalid. It should be between 10 and 10000.");
            return false;
        }
        if(!isExactDivision(password2, denominations, 0)) {
            System.out.println("The password2 is invalid. It is not compatible with the denominations.");
            return false;
        }
        return true;
    }

    /**
     * The function checks if a given integer can be exactly divided by a set of denominations using
     * recursion.
     * 
     * @param password2 The integer value that we want to check.
     * @param denominations an array of integers representing the denominations.
     * @param index The index parameter represents the current index of the denominations.
     * @return The method returns a boolean value indicating whether an exact division
     * of a given password2 integer can be achieved.
     */
    public static boolean isExactDivision(int password2, int[] denominations, int index) {
        // If password2 is 0 at the end, then the it can be exactly divided
        if (password2 == 0) {
            return true; 
        }
        // If password2 is negative or index is out of bounds, then the password2 
        // cannot be exactly divided
        if (password2 < 0 || index >= denominations.length) {
            return false; 
        }

        // Check if the password2 can be exactly divided by the current denomination
        boolean division = isExactDivision((password2 - denominations[index]), denominations, index);

        // If the password2 cannot be exactly divided by the current denomination,
        // then check if it can be exactly divided by the denominations after the current one
        if(!division) {
            division = isExactDivision(password2, denominations, index + 1);
        }
        
        return division;
    }
}

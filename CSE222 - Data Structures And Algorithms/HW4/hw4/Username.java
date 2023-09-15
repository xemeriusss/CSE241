package hw4;

/**
 * 
 * Username class is used to create an username object and use in test classes.
 * @version 1.0 20.04.2023
 * @author  Berru Lafci
 */
public class Username {
    
    private String username;

    /**
     * Constructor for Username class.
     * @param username
     */
    public Username(String username) {
        this.username = username;
    }
    /**
     * Getter for username.
     * @return username
     */
    public String getUsername() {
        return username;
    }

    /**
     * Setter for username.
     * @param username
     */
    public void setUsername(String username) {
        this.username = username;
    }

    /**
     * If given username is valid by calling the recursive function.
     * @return The method is returning a boolean value.
     */
    public boolean checkIfValidUsername() {
        return recursive(username);
    }

    /**
     * The function checks if a given username is valid by recursively checking if it contains only
     * letters and has at least one character. 
     * 
     * @param username The parameter "username"
     * @return The method is returning a boolean value.
     */
    public boolean recursive(String username) {
        if (username.length() == 0) {
            System.out.println("The username is invalid. It should have at least one character.");
            return false;
        } 
        else if (Character.isDigit(username.charAt(0))) {
            System.out.println("The username is invalid. It should not contain any digits.");
            return false;
        } 
        // If there is only one character left, the username is valid.
        else if (username.length() == 1) {
            return true;
        } 
        else {
            return recursive(username.substring(1));
        }
    }
    
    
}

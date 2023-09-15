package hw4;
import java.util.Stack;

/**
 * 
 * Password1 class is used to create an Password1 object and use in test classes.
 * @version 1.0 20.04.2023
 * @author  Berru Lafci
 */
public class Password1 {
    
    String username;
    String password1;

    /**
     * Constructor for Password1 class.
     * @param username
     * @param password1
     */
    public Password1(String username, String password1) {
        this.username = username;
        this.password1 = password1;
    }

    /**
     * Getter for username.
     * @return username
     */
    public String getUsername() {
        return username;
    }

    /**
     * Getter for password1.
     * @return password1
     */
    public String getPassword1() {
        return password1;
    }

    /**
     * The function checks if a given password meets certain criteria, including length, absence of
     * digits, presence of letters and brackets, and balance of brackets, and also checks if the
     * password contains the username.
     * 
     * @param username The username of the user trying to create a password.
     * @param password1 The password that needs to be checked for validity.
     * @return The method is returning a boolean value, either true or false, depending on whether the
     * password1 meets the specified criteria.
     */
    public boolean checkIfValidPassword1(String username, String password1) {

        boolean letter_flag = false;
        int bracket_count = 0;

        // Check if the password1 is longer than 8 characters.
        if(password1.length() < 8) {
            System.out.println("The password1 is invalid. It should have at least 8 characters.");
            return false;
        }
        
        for(int i = 0; i < password1.length(); i++) {
            // Check if the password1 contains any digits.
            if(Character.isDigit(password1.charAt(i))) {
                System.out.println("The password1 is invalid. It should not contain any digits.");
                return false;
            }
            // Check if the password1 contains at least one letter.
            if(Character.isLetter(password1.charAt(i))) {
                letter_flag = true;
            }
            // Check if the password1 contains at least two brackets.
            if(password1.charAt(i) == '(' || password1.charAt(i) == ')' || password1.charAt(i) == '[' || password1.charAt(i) == ']' || password1.charAt(i) == '{' || password1.charAt(i) == '}') {
                bracket_count++;
            }
        }

        if(!letter_flag) {
            System.out.println("The password1 is invalid. It should contain at least one letter.");
            return false;
        }
        if(bracket_count < 2) {
            System.out.println("The password1 is invalid. It should contain at least two brackets.");
            return false;
        }

        if(!containsUserNameSpirit(username, password1)) {
            return false;
        }
        if(!isBalancedPassword(password1)) {
            return false;
        }
        if(!isPalindromePossible(password1)) {
            return false;
        }
        return true;

    }

    /**
     * This function checks if a given password contains at least one letter from a given username.
     * 
     * @param username The username that needs to be checked for presence in the password.
     * @param password1 The password that needs to be checked for containing at least one letter from
     * the username.
     * @return The method is returning a boolean value. It returns true if the password1 contains at
     * least one letter of the username, and false otherwise.
     */
    public boolean containsUserNameSpirit(String username, String password1) {

        Stack<Character> stack = new Stack<Character>();
        for(int i = 0; i < username.length(); i++) {
            stack.push(username.charAt(i));
        }
        while(!stack.isEmpty()) {
            for(int i = 0; i < password1.length(); i++) {
                if(password1.charAt(i) == stack.peek()) {
                    return true;
                }
            }
            stack.pop();
        }
        if(username.length() > 0) {
            System.out.println("The password1 is invalid. It should contain at least one letter from the username.");
        }
        return false;
    }

    /**
     * The function checks if a given password has balanced parentheses, brackets, and curly braces.
     * 
     * @param password1 The input string representing the password to be checked for balanced
     * parentheses.
     * @return The method is returning a boolean value. It returns true if the input password1 is
     * balanced and false otherwise.
     */
    public boolean isBalancedPassword(String password1) {
            
        Stack<Character> stack = new Stack<Character>();

        for(int i = 0; i < password1.length(); i++) {
            // Push the opening brackets to the stack.
            if(password1.charAt(i) == '(' || password1.charAt(i) == '[' || password1.charAt(i) == '{') {
                stack.push(password1.charAt(i));
            }
            if(password1.charAt(i) == ')' || password1.charAt(i) == ']' || password1.charAt(i) == '}') {
                // If stack is empty, it means it is not balanced.
                if(stack.isEmpty()) {
                    System.out.println("The password1 is invalid. It should be balanced.");
                    return false;
                }
                // Check if the closing bracket matches the opening bracket. Then pop the opening bracket.
                if(password1.charAt(i) == ')' && stack.peek() == '(') {
                    stack.pop();
                }
                if(password1.charAt(i) == ']' && stack.peek() == '[') {
                    stack.pop();
                }
                if(password1.charAt(i) == '}' && stack.peek() == '{') {
                    stack.pop();
                }
            }
        }
        // If the stack is not empty after the loop, it means it is not balanced.
        if(!stack.isEmpty()) {
            System.out.println("The password1 is invalid. It should be balanced.");
            return false;
        }
        return true;
    }

    /**
     * The method checks if a given string can be rearranged to form a palindrome.
     * 
     * @param password1 The input string that needs to be checked for palindrome possibility.
     * @return The method isPalindromePossible is returning a boolean value, either true or false.
     */
    static boolean isPalindromePossible(String password1) {
        // Empty string for the helper function
        String temp = "";
        String without_brackets = "";

        // Remove the brackets from the string
        for(int i = 0; i < password1.length(); i++) {
            if(Character.isLetter(password1.charAt(i))) {
                without_brackets = without_brackets +  password1.charAt(i);
            }
        }

        if(!isPalindromePossibleHelper(without_brackets, 0, temp)) {
            System.out.println("The password1 is invalid. It should be a palindrome possible.");
            return false;
        } else {
            return true;
        }
    }

    /**
     * The method checks if a given string can be rearranged into a palindrome by recursively
     * checking if each character appears an even number of times, except for at most one character
     * which can appear an odd number of times.
     * 
     * @param password1 The input string that we are checking for palindrome possibility.
     * @param index The current index being processed in the recursive function. It starts at 0 and
     * increments by 1 in each recursive call until it reaches the length of the password string.
     * @param temp temp is a String variable that stores the characters that have been encountered so
     * far during the recursive function call.
     * @return The method isPalindromePossibleHelper is returning a boolean value.
     */
    static boolean isPalindromePossibleHelper(String password1, int index, String temp) {
        if (index == password1.length()) {
            // If the index is at the end of the string, check if the string is empty or has one character
            boolean even_case = (password1.length() % 2 == 0) && (temp.isEmpty());
            boolean odd_case = (password1.length() % 2 == 1) && (temp.length() == 1);
            return even_case || odd_case;
        }

        // Character at the current index
        String c = password1.charAt(index) + "";

        // Check if the current character is containes in String
        if (temp.contains(c)) {
            // Remove from the String
            temp = temp.replace(c, "");
        } else {
            // Add to the String
            temp += c;
        }

        // Recursive call
        return isPalindromePossibleHelper(password1, index + 1, temp);
    }

}

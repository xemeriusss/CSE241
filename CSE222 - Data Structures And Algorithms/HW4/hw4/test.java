package hw4;

public class test {
    
    public static void main(String[] args) {

        String username = "berry";
        String password1 = "{(abba)cac}";
        int password2 = 15;
        int[] denominations = {6, 4};

        boolean usernameCheck = false;
        boolean password1Check = false;
        boolean password2Check = false;

        Username u = new Username(username);
        usernameCheck = u.checkIfValidUsername();

        Password1 p1 = new Password1(username, password1);
        password1Check = p1.checkIfValidPassword1(username, password1);

        Password2 p2 = new Password2(password2, denominations);
        password2Check = p2.checkIfValidPassword2(password2);

        if (usernameCheck && password1Check && password2Check) {
            System.out.println("The username and passwords are valid. The door is opening.");
        } 
    }
}

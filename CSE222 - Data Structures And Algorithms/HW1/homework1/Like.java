package homework1;

/**
 * Like class is an inherited class of Interaction class.
 * @version 1.0 18.03.2023
 * @author  Berru Lafci
 */
public class Like extends Interaction {

    /**
     * Constructor for Like class.
     * @param interactionId interaction id of the like 
     * @param accountId account id of the like 
     * @param postId post id of the like 
     * @param username username of who liked the post
     */
    public Like(int interactionId, int accountId, int postId, String username) {
        super(interactionId, accountId, postId, username);
    }
       
}

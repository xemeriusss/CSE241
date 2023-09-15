package hw3_ll;

/**
 * Interaction class is used to create interactions.
 * Base class for Like and Comment classes.
 * @version 1.0 05.04.2023
 * @author  Berru Lafci
 */
public class Interaction {
    
    private int interactionId;
    private int accountId;
    private int postId;
    private String username;

    /**
     * Constructor for Interaction class.
     * @param interactionId interaction id of the interaction 
     * @param accountId account id of the interaction 
     * @param postId post id of the interaction 
     * @param username username of the interaction 
     */
    public Interaction(int interactionId, int accountId, int postId, String username) {
        this.interactionId = interactionId;
        this.accountId = accountId;
        this.postId = postId;
        this.username = username;
    }

    /**
     * Getter returns the interactionId of the interaction.
     * @return The interactionId of the interaction.
     */
    public int getInteractionId() {
        return interactionId;
    }

    /**
     * Getter returns the accountId of the interaction.
     * @return The accountId of the interaction.
     */
    public int getAccountId() {
        return accountId;
    }

    /**
     * Getter returns the postId of the interaction.
     * @return The postId of the interaction.
     */
    public int getPostId() {
        return postId;
    }

    /**
     * Getter returns the username of the interaction.
     * @return The username of the interaction.
     */
    public String getUsername() {
        return username;
    }

    /**
     * Setter to set the interaction ID
     * @param interactionId The interaction ID to be set.
     */
    public void setInteractionId(int interactionId) {
        this.interactionId = interactionId;
    }

    /**
     * Setter to set the account ID
     * @param accountId The account ID to be set.
     */
    public void setAccountId(int accountId) {
        this.accountId = accountId;
    }

    /**
     * Setter to set the post ID
     * @param postId The post ID to be set.
     */
    public void setPostId(int postId) {
        this.postId = postId;
    }

}

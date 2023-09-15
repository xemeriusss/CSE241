package hw3_ldll;

/**
 * Comment class is an inherited class of Interaction class.
 * @version 1.0 05.04.2023
 * @author  Berru Lafci
 */
public class Comment extends Interaction {
    
    private String content;
    
    /**
     * Constructor for Comment class.
     * @param interactionId interaction id of the comment 
     * @param accountId account id of the comment 
     * @param postId post id of the comment 
     * @param username username of who commented the post
     * @param content content of the comment
     */
    public Comment(int interactionId, int accountId, int postId, String username, String content) {
        super(interactionId, accountId, postId, username);
        this.content = content;
    }

    /**
     * Getter returns the content of the comment.
     * @return The content of the comment.
     */
    public String getContent() {
        return content;
    }
}

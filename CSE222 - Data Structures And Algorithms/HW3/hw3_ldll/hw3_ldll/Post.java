package hw3_ldll;

/**
 * Post class is used to create posts.
 * @version 1.0 18.03.2023
 * @author  Berru Lafci
 */
public class Post {
    
    private int postId;
    private int accountId;
    private LDLinkedList<Like> likes;
    private LDLinkedList<Comment> comments;
    private String content; 

    /**
     * Constructor for Post class.
     * @param postId post id of the post 
     * @param content content of the post 
     */
    public Post(int postId, String content) {
        this.postId = postId;
        this.content = content;
        this.likes = new LDLinkedList<Like>();
        this.comments = new LDLinkedList<Comment>();
    }

    /**
     * Getter returns the postId of the user.
     * @return The postId of the user.
     */
    public int getPostId() {
        return postId;
    }

    /**
     * Getter returns the accountId of the user.
     * @return The accountId of the user.
     */
    public int getAccountId() {
        return accountId;
    }

    /**
     * Getter returns the content of the post.
     * @return The content of the post.
     */
    public String getContent() {
        return content;
    }

    /**
     * Getter returns the likes array.
     * @return An array of Like objects.
     */       
    public LDLinkedList<Like> getLikes() {
        return likes;
    }

    /**
     * Getter returns the comments array.
     * @return An array of Comment objects.
     */
    public LDLinkedList<Comment> getComments() {
        return comments;
    }

    /**
     * Setter to set the post ID
     * @param postId The post ID to be set.
     */
    public void setPostId(int postId) {
        this.postId = postId;
    }

    /**
     * Setter to set the account ID
     * @param accountId The account ID to be set.
     */
    public void setAccountId(int accountId) {
        this.accountId = accountId;
    }

    /**
     * Setter to set the content of the post
     * @param content The content of the post to be set.
     */
    public void setContent(String content) {
        this.content = content;
    }

}

package homework1;

/**
 * Post class is used to create posts.
 * @version 1.0 18.03.2023
 * @author  Berru Lafci
 */
public class Post {
    
    private int postId;
    private int accountId;
    private Like[] likes;
    private Comment[] comments;
    private String content; 

    private int likeNum = 0;
    private int commentNum = 0;

    /**
     * Constructor for Post class.
     * @param postId post id of the post 
     * @param content content of the post 
     */
    public Post(int postId, String content) {
        this.postId = postId;
        this.content = content;
        this.likes = new Like[100];
        this.comments = new Comment[100];
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
     * Getter returns the number of likes of the post.
     * @return The number of likes of the post.
     */
    public int getLikeNum() {
        return likeNum;
    }

    /**
     * Getter returns the number of comments of the post.
     * @return The number of comments of the post.
     */
    public int getCommentNum() {
        return commentNum;
    }

    /**
     * Getter returns the likes array.
     * @return An array of Like objects.
     */       
    public Like[] getLikes() {
        return likes;
    }

    /**
     * Getter returns the comments array.
     * @return An array of Comment objects.
     */
    public Comment[] getComments() {
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

    /**
     * Increase the number of likes by 1.
     */
    public void increaseLikeNum() {
        this.likeNum++;
    }

    /**
     * Increase the number of comments by 1.
     */
    public void increaseCommentNum() {
        this.commentNum++;
    }

    /**
     * The toString() method returns a string representation of the object
     * @return The postId, accountId, and content of the post.
     */
    public String toString() {
        return "Post ID: " + postId + " Account ID: " + accountId + " Content: " + content;
    }

}


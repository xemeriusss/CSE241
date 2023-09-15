package homework1;

/**
 * 
 * Account class is used to create an account object and use in test classes.
 * @version 1.0 18.03.2023
 * @author  Berru Lafci
 */
public class Account {
    
    private int accountId;
    private String username;
    private String birthDate;
    private String location;
    private Post[] posts;
    private Account[] followers;
    private Account[] following;
    private Message[] inbox; // messages received
    private Message[] outbox; // messages sent

    private boolean isLogin = false;
    private boolean isProfile = false;
    private boolean isPosts = false;
    private int postNum = 0;
    private int  followingNum = 0; // null olayını düzelt
    private int followerNum = 0; // null olayını düzelt
    private int inboxNum = 0;
    private int outboxNum = 0;
    private String[] blockedAccounts = new String[10];

    
    /**
     * Constructor to create an account object.
     * @param accountId id of the account
     * @param username username of the account
     * @param birthdate birthdate of the account
     * @param location location of the account
     */
    public Account(int accountId, String username, String birthdate, String location) {
        this.accountId = accountId;
        this.username = username;
        this.birthDate = birthdate;
        this.location = location;
        this.posts = new Post[10];  // 10 posts per account
        this.followers = new Account[10];
        this.following = new Account[10];
        this.inbox = new Message[10];
        this.outbox = new Message[10];
        System.out.println("An account with username " + username + " has been created.\n");
    }


    /**
     * Getter returns the accountId of the user.
     * @return The accountId of the user.
     */
    public int getAccountId() {
        return this.accountId;
    }

    /**
     * Getter returns the username of the user.
     * @return The username of the user.
     */
    public String getUsername() {
        return this.username;
    }

    /**
     * Getter returns the birthdate of the user.
     * @return The birthdate of the user.
     */
    public String getBirthDate() {
        return this.birthDate;
    }

    /**
     * Getter returns the location of the user.
     * @return The location of the user.
     */
    public String getLocation() {
        return this.location;
    }

    /**
     * Getter returns the posts array.
     * @return An array of Post objects.
     */
    public Post[] getPosts() {
        return this.posts;
    }

    /**
     * Getter returns the followers array.
     * @return An array of Account objects.
     */
    public Account[] getFollowers() {
        return this.followers;
    }

    /**
     * Getter returns the following array.
     * @return An array of Account objects.
     */
    public Account[] getFollowing() {
        return this.following;
    }

    /**
     * Getter returns the inbox array.
     * @return An array of Message objects.
     */
    public Message[] getInbox() {
        return this.inbox;
    }

    /**
     * Getter returns the outbox array.
     * @return An array of Message objects.
     */
    public Message[] getOutbox() {
        return this.outbox;
    }

    /**
     * Getter returns the number of posts of the user.
     * @return The postNum of the user.
     */
    public int getPostNum() {
        return this.postNum;
    }

    /**
     * Getter returns the number of followers of the user.
     * @return The followerNum of the user.
     */
    public int getFollowerNum() {
        return this.followerNum;
    }

    /**
     * Getter returns the number of following of the user.
     * @return The followingNum of the user.
     */
    public int getFollowingNum() {
        return this.followingNum;
    }

    /**
     * Getter returns the number of inbox messages of the user.
     * @return The inboxNum of the user.
     */
    public int getInboxNum() {
        return this.inboxNum;
    }

    /**
     * Getter returns the number of outbox messages of the user.
     * @return The outboxNum of the user.
     */
    public int getOutboxNum() {
        return this.outboxNum;
    }

    /**
     * Getter returns the blocked accounts array.
     * @return An array of blocked account names.
     */
    public String[] getBlockedAccounts() {
        return this.blockedAccounts;
    }

    /**
     * This function returns true if the user is logged in, and false otherwise.
     * @return A boolean value.
     */
    public boolean isLogin() {
        return this.isLogin;
    }

    /**
     * Setter to set the account ID
     * @param accountId The account ID to be set.
     */
    public void setAccountId(int accountId) {
        this.accountId = accountId;
    }

    /**
     * Setter to set the username
     * @param username The username to be set.
     */
    public void setUsername(String username) {
        this.username = username;
    }

    /**
     * Setter to set the birthdate
     * @param birthDate The birthdate to be set.
     */
    public void setBirthDate(String birthDate) {
        this.birthDate = birthDate;
    }

    /**
     * Setter to set the location
     * @param location The location to be set.
     */
    public void setLocation(String location) {
        this.location = location;
    }

    /**
     * Increases the followerNum of the account by 1.
     * @param account the account to be updated
     */
    public void increaseFollowerNum(Account account) {
        account.followerNum++;
    }

    /**
     * Increases the followingNum of the account by 1.
     * @param account the account to be updated
     */
    public void increaseInboxNum(Account account) {
        account.inboxNum++;
    }

    /**
     * If the user is not logged in, then log them in
     */
    public void login() {
        if(this.isLogin == false){
            this.isLogin = true;
            System.out.println("-------------------------------");
            System.out.println("Logging into an account: " + this.username + "");
            System.out.println("-------------------------------\n");
        }
        else
            System.out.println("You are already logged in. (" + this.username + ")\n");
    }

    /**
     * If the user is logged in, log them out
     */
    public void logout() {
        if(this.isLogin == true){
            this.isLogin = false;
            System.out.println("Logging out from account: " + this.username + "\n");
        }
        else
            System.out.println("You are already logged out. (" + this.username + ")\n");
    }

    /**
     * If the user is logged in, prints the profile of the account that is given as a parameter
     * @param account The account that the user wants to view.
     */
    public void viewProfile(Account account) {

        if(isLogin == true){
            if(isBlocked(account.getBlockedAccounts()) == false){

                isProfile = true;
                System.out.println("Viewing " + account.getUsername() + "'s profile...");
                System.out.println("------------------------------------");
                System.out.println("UserId: " + account.getAccountId() + "\nUsername: " + account.getUsername() + "\nLocation: " + account.getLocation() + "\nBirthdate: " + account.getBirthDate());

                System.out.println(account.getUsername() + " is following " + account.getFollowingNum() + " accounts and has " + account.getFollowerNum() + " followers.");

                if(account.getFollowingNum() != 0) {
                    System.out.println(account.getUsername() + " is following: " + showFollowings(account));
                }

                if(account.getFollowerNum() != 0) {
                    System.out.println("The followers of " + account.getUsername() + " are: " + showFollowers(account));
                }
                
                System.out.println(account.getUsername() + " has " + account.getPostNum() + " posts." );
                System.out.println("------------------------------------\n");
            }
            else
                System.out.println("You cannot view this profile because you are blocked by " + account.getUsername() + ".\n");
        }
        else
            System.out.println("You can't view profile without logging in, " + username + ".\n");
    }

    /**
     * If the user is logged in, add the post to the array of posts
     * @param post the post to be added
     */
    public void addPost(Post post) {
        
        if(isLogin == true){
            for(int i = 0; i < this.posts.length; i++){
                if(this.posts[i] == null){
                    this.posts[i] = post;
                    post.setAccountId(this.accountId);
                    postNum++;
                    System.out.println(username + " added a new post.\n");
                    break;
                }
            }
        }
        else
            System.out.println("You can't add a post without logging in, " + username + ".\n");
    }


    /**
     * If the user is logged in, and not blocked by the account they want to view, and firstly viewed
     * the profile of the account, prints the posts of the account that is given as a parameter
     * @param account The account that the user wants to view.
     */
    public void viewPosts(Account account) { 

        Post[] posts = account.getPosts();
        int flag = 0; // to check zero post

        if(isLogin == true) {
            if(isBlocked(account.getBlockedAccounts()) == false){
                if(isProfile == true) {
                    isPosts = true;
                    System.out.println("Viewing " + account.getUsername() + "'s posts...");
                    System.out.println("--------------------------------------");
                    for(int i = 0; i < account.getPostNum() ; i++){
                        System.out.println("(PostID: " + posts[i].getPostId() + ") " + account.getUsername() + ": " + posts[i].getContent() + "");
                        flag = 1;
                    }
                    if(flag == 0){
                        System.out.println(account.getUsername() + " has no posts.");
                    }
                    isProfile = false;
                    System.out.println("--------------------------------------");
                }
                else
                    System.out.println("You can't view posts without viewing profile, " + username + ".\n");
            }
            else
                System.out.println("You cannot view this profile because you are blocked by " + account.getUsername() + ".\n");
        }
        else
            System.out.println("You can't view posts without logging in, " + username + ".\n");

    }

    /**
     * This function checks if the user is logged in, and not blocked by the account they want to follow,
     * and the account they want to follow is not themselves and not already in their following list.
     * If all of these conditions are true, then adds the account to following list,
     * and adds the user to the accounts followers list.
     * @param account The account that the user wants to follow.
     */
    public void followAccount(Account account) {  

        if(isLogin == true){
            if(account.getAccountId() == this.accountId) {
                System.out.println("You cannot follow yourself.\n");
            }
            else if(isBlocked(account.getBlockedAccounts()) == true) {
                System.out.println("You cannot follow this account because you are blocked by " + account.getUsername() + ".\n");
            }
            else{
                for(int i = 0; i < following.length; i++) {
                    if(following[i] == null) {
                        following[i] = account;
                        followingNum++;

                        for(int j = 0; j < account.getFollowers().length; j++) {
                            if(account.getFollowers()[j] == null) {
                                account.getFollowers()[j] = this;
                                account.increaseFollowerNum(account);
                                break;
                            }
                        }

                        System.out.println(this.username + " is now following " + account.getUsername() + ".\n");
                        return;
                    }
                    if(following[i].accountId == account.getAccountId()) {
                        System.out.println("You are already following " + account.getUsername() + ".\n");
                        return;
                    }
                }
            }
        }
        else
            System.out.println("You can't follow an account without logging in, " + username + ".\n");  
    }

    /**
     * Function to show the followings of the account.
     * @param account The account that you want to show the followings of.
     * @return A string of all the usernames of the accounts that the account is following.
     */
    public String showFollowings(Account account) {

        String followings = "";
        Account[] followingArray = account.getFollowing();

        for(int i = 0; i < account.getFollowingNum(); i++) {
            followings += followingArray[i].getUsername() + ", ";
        }
        return followings;
    }

    /**
     * Function to show the followers of the account.
     * @param account The account that you want to show the followers of.
     * @return A string of all the usernames of the accounts that are following the account.
     */
    public String showFollowers(Account account) {

        String followers = "";
        Account[] followerArray = account.getFollowers();

        for(int i = 0; i < account.getFollowerNum(); i++) {
            followers += followerArray[i].getUsername() + ", ";
        }
        return followers;
    }

    /**
     * The function checks if the user is logged in, if the user is not blocked by the account, and if
     * the user is following the account. If all of these conditions are met, the message is sent.
     * @param account The account that the message is being sent to.
     * @param message The message that will be sent.
     */
    public void sendMessage(Account account, Message message) { 

        if(isLogin == true) {
            if(isBlocked(account.getBlockedAccounts()) == false) {
                if(isFollowing(account.getAccountId()) == true) {

                    message.setSenderId(this.accountId);
                    message.setReceiverId(account.getAccountId());
                    message.setSenderUsername(this.username);
                    message.setReceiverUsername(account.getUsername());
    
                    System.out.println("Sending a message to " + account.getUsername() + "...\n");
    
                    this.outbox[this.outboxNum] = message;
                    outboxNum++;
    
                    for(int i = 0; i < account.getInbox().length; i++) {
                        if(account.getInbox()[i] == null) {
                            account.getInbox()[i] = message;
                            account.increaseInboxNum(account);
                            break;
                        }
                    }
               }
               else
                    System.out.println("You can't send a message without following " + account.getUsername() + ", " + this.username + ".\n");
            }
            else
                System.out.println("You cannot send a message to this account because you are blocked by " + account.getUsername() + ".\n");
        }
        else
            System.out.println("You can't send a message without logging in, " + this.username + ".\n");
    }

    /**
     * This function checks if the account is following another account
     * @param id the id of the account you want to check if you're following
     * @return A boolean value.
     */
    public boolean isFollowing(int id) {

        for(int i = 0; i < this.getFollowingNum(); i++) {
            if(getFollowing()[i].getAccountId() == id) {
                return true;
            }
        }
        return false;
    }

    /**
     * If the user is logged in, print out the number of messages in the inbox
     */
    public void checkInbox() {
        
        if(isLogin == true) {
            System.out.println("Checking inbox...");
            System.out.println("There is/are " + this.inboxNum + " message(s) in the inbox.\n");
        }
        else
            System.out.println("You can't check inbox without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in, print out the number of messages in the outbox
     */
    public void checkOutbox() {
        
        if(isLogin == true) {
            System.out.println("Checking outbox...");
            System.out.println("There is/are " + this.outboxNum + " message(s) in the outbox.\n");
        }
        else
            System.out.println("You can't check outbox without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in, print out the inbox contents
     */
    public void viewInbox() {
        
        if(isLogin == true) {
            System.out.println("Viewing inbox...");
            System.out.println("--------------------");
            for(int i = 0; i < this.inboxNum; i++) {
                System.out.println("MessageId: " + this.inbox[i].getMessageId());
                System.out.println("From: " + this.inbox[i].getSenderUsername());
                System.out.println("To: " + this.inbox[i].getReceiverUsername());
                System.out.println("Message: " + this.inbox[i].getContent() + "\n");
            }
        }
        else
            System.out.println("You can't view inbox without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in, print out the outbox contents
     */
    public void viewOutbox() {
        
        if(isLogin == true) {
            System.out.println("Viewing outbox...");
            System.out.println("--------------------");
            for(int i = 0; i < this.outboxNum; i++) {
                System.out.println("MessageId: " + this.outbox[i].getMessageId());
                System.out.println("From: " + this.outbox[i].getSenderUsername());
                System.out.println("To: " + this.outbox[i].getReceiverUsername());
                System.out.println("Message: " + this.outbox[i].getContent() + "\n");
            }
        }
        else
            System.out.println("You can't view outbox without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in, then create a new Like object and add it to the post's Like array
     * @param post the post that the user wants to like
     * @param interactionId the id of the interaction, which is the same as the id of the like.
     * @param account the account of the post.
     */
    public void likePost(Post post, int interactionId, Account account) {
            
        if(isLogin == true) {
            System.out.println("Liking a post of " + account.getUsername() + "...\n");
            Like like = new Like(interactionId, this.accountId, post.getPostId(), this.username);
            for(int i = 0; i < post.getLikes().length; i++) {
                if(post.getLikes()[i] == null) {
                    post.getLikes()[i] = like;
                    post.increaseLikeNum();
                    break;
                }
            }
        }
        else
            System.out.println("You can't like a post without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in, then create a new Comment object and add it to the post's Comment array
     * @param post the post that the user wants to comment on
     * @param interactionId the id of the interaction, which is the same as the id of the comment.
     * @param account the account of the post.
     * @param content the content of the comment
     */
    public void commentPost(Post post, int interactionId, Account account, String content) {
            
        if(isLogin == true) {
            System.out.println("Adding a comment on a post of " + account.getUsername() + "...\n");
            Comment comment = new Comment(interactionId, this.accountId, post.getPostId(), this.username, content);
            for(int i = 0; i < post.getComments().length; i++) {
                if(post.getComments()[i] == null) {
                    post.getComments()[i] = comment;
                    post.increaseCommentNum();
                    break;
                }
            }
        }
        else
            System.out.println("You can't comment a post without logging in, " + this.username + ".\n");
    }

    /**
     * If the user is logged in and viwed the posts of the account,
     * it prints out the interactions of the posts of the account that is passed as an argument.
     * @param account The account whose posts interactions you want to view.
     */
    public void viewInteractions(Account account) {
            
            if(isLogin == true) {
                if(isPosts == true) {
                    System.out.println("Viewing " + account.getUsername() + "'s posts' interactions...");
                    System.out.println("--------------------");
                    for(int i=0 ; i<account.getPostNum() ; i++) {
                        System.out.println("(PostId: " + account.getPosts()[i].getPostId() + "): " + account.getPosts()[i].getContent());
                        if(account.getPosts()[i].getLikeNum() != 0) {
                            System.out.print("The post was liked by the following account(s): ");
                            for(int j = 0 ; j<account.getPosts()[i].getLikeNum() ; j++){
                                System.out.println(account.getPosts()[i].getLikes()[j].getUsername() + ", ");
                            }
                        }
                        else
                            System.out.println("The post has no likes.");

                        if(account.getPosts()[i].getCommentNum() != 0) {
                            System.out.println("The post has " + account.getPosts()[i].getCommentNum() + " comment(s).");
                            for(int j = 0 ; j<account.getPosts()[i].getCommentNum() ; j++){
                                System.out.println("Comment " + account.getPosts()[i].getComments()[j].getInteractionId()+ ": '" + account.getPosts()[i].getComments()[j].getUsername() + "' said '" + account.getPosts()[i].getComments()[j].getContent() + "'.\n");                    
                            }
                        }
                        else
                            System.out.println("The post has no comments.");
                    }
                }
                else
                    System.out.println("You can't wiew interactions without viewing posts, " + this.username + ".");
            }
            else
                System.out.println("You can't show interactions without logging in, " + this.username + ".");

        System.out.println("--------------------\n");
    }

    /**
     * If the user is logged in, then add the account to the blockedAccounts array
     * @param account The account that the user wants to block.
     */
    public void block(Account account) {

        if(isLogin == true) {
            for(int i = 0; i < this.blockedAccounts.length; i++) {
                if(this.blockedAccounts[i] == null) {
                    this.blockedAccounts[i] = account.getUsername();
                    //this.blockedAccountsNum++;
                    break;
                }
            }
        }
        else
            System.out.println("You can't block an account without logging in, " + this.username + ".\n");
    }

    /**
     * If the username of the current user is in the list of blocked accounts, return true, otherwise
     * return false.
     * @param blockedAccounts An array of strings that contains the usernames of the blocked accounts.
     * @return A boolean value.
     */
    public boolean isBlocked(String[] blockedAccounts) {

        for(int i = 0; i < blockedAccounts.length; i++) {
            if(blockedAccounts[i] == this.username) {
                return true;
            }
        }
        return false;
    }

}

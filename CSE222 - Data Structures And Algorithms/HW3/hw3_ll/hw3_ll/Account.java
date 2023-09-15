package hw3_ll;
import java.util.Iterator;
import java.util.LinkedList;

/**
 * 
 * Account class is used to create an account object and use in test classes.
 * @version 1.0 05.04.2023
 * @author  Berru Lafci
 */
public class Account {
    
    private int accountId;
    private String username;
    private String birthDate;
    private String location;
    private LinkedList<Post> posts;
    private LinkedList<Account> followers;
    private LinkedList<Account> following;
    private LinkedList<Message> inbox; 
    private LinkedList<Message> outbox;

    private LinkedList<Account> blockedAccounts;
    private boolean isLogin = false;
    private boolean isProfile = false;
    private boolean isPosts = false; // will be used in view interactions
    private LinkedList<String> history;

    /**
     * Constructor to create an account object.
     * @param accountId id of the account
     * @param username username of the account
     * @param birthDate birthdate of the account
     * @param location location of the account
     */
    public Account(int accountId, String username, String birthDate, String location) {
        this.accountId = accountId;
        this.username = username;
        this.birthDate = birthDate;
        this.location = location;
        this.posts = new LinkedList<Post>();
        this.followers = new LinkedList<Account>();
        this.following = new LinkedList<Account>();
        this.inbox = new LinkedList<Message>();
        this.outbox = new LinkedList<Message>();
        this.blockedAccounts = new LinkedList<Account>();
        this.history = new LinkedList<String>();
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
     * Getter returns the posts LinkedList.
     * @return An LinkedList of Post objects.
     */
    public LinkedList<Post> getPosts() {
        return posts;
    }

    /**
     * Getter returns the followers LinkedList.
     * @return An LinkedList of Account objects.
     */
    public LinkedList<Account> getFollowers() {
        return followers;
    }

    /**
     * Getter returns the following LinkedList.
     * @return An LinkedList of Account objects.
     */
    public LinkedList<Account> getFollowing() {
        return following;
    }

    /**
     * Getter returns the inbox LinkedList.
     * @return An LinkedList of Message objects.
     */
    public LinkedList<Message> getInbox() {
        return inbox;
    }

    /**
     * Getter returns the outbox LinkedList.
     * @return An LinkedList of Message objects.
     */
    public LinkedList<Message> getOutbox() {
        return outbox;
    }

    /**
     * Getter returns the blockedAccounts LinkedList.
     * @return An LinkedList of Account objects.
     */
    public LinkedList<Account> getBlockedAccounts() {
        return blockedAccounts;
    }

    // SETTERS

    /**
     * If the user is not logged in, then log them in
     */
    public void login() {
        if(isLogin == true){
            System.out.println("You are already logged in. (" + this.username + ")\n");
            return;
        }
        this.isLogin = true;
        System.out.println("-------------------------------");
        System.out.println("Logging into an account: " + this.username + "");
        System.out.println("-------------------------------\n");
    }

    /**
     * If the user is logged in, log them out
     */
    public void logout() {
        if(isLogin == false){
            System.out.println("You are already logged out. (" + this.username + ")\n");
            return;  
        }
        this.isLogin = false;
        System.out.println("-------------------------------");
        System.out.println("Logging out of an account: " + this.username + "");
        System.out.println("-------------------------------\n");  
    }

    /**
     * If the user is logged in, prints the profile of the account that is given as a parameter
     * Blocked account can view profile but cannot view posts
     * @param account The account that the user wants to view.
     */
    public void viewProfile(Account account) {
        if (!isLogin) {
            System.out.println("You are not logged in. (" + username + ")\n");
            return;
        }
        System.out.println("Viewing " + account.getUsername() + "'s profile...");
        System.out.println("------------------------------------");
        System.out.println("UserId: " + account.getAccountId() + "\nUsername: " + account.getUsername() + "\nLocation: " + account.getLocation() + "\nBirthdate: " + account.getBirthDate());
        System.out.println(account.getUsername() + " is following " + account.getFollowing().size() + " accounts and has " + account.getFollowers().size() + " followers.");
        if(account.getFollowing().size() != 0) {
            System.out.println(account.getUsername() + " is following: " + showFollowings(account));
        }
        if(account.getFollowers().size() != 0) {
            System.out.println("The followers of " + account.getUsername() + " are: " + showFollowers(account));
        }
        if(account.getBlockedAccounts().size() != 0) {
            System.out.println(account.getUsername() + " has blocked: " + showBlockedAccounts(account));
        }
        System.out.println(account.getUsername() + " has " + account.getPosts().size() + " posts." );
        System.out.println("------------------------------------\n");
        this.isProfile = true;
    }


    /**
     * If the user is logged in, add the post to the LinkedList of posts
     * @param post the post to be added
     */
    public void addPost(Post post) {
        if (!isLogin) {
            System.out.println("You are not logged in. (" + username + ")\n");
            return;
        }
        posts.add(post);
        System.out.println("-------------------------------");
        System.out.println("Adding a post to " + username + "'s account.");
        System.out.println("-------------------------------\n");
        history.add("You added a post to your account.");
    }

    /**
     * If the user is logged in, and not blocked by the account they want to view, and firstly viewed
     * the profile of the account, prints the posts of the account that is given as a parameter
     * @param account The account that the user wants to view posts.
     */
    public void viewPosts(Account account) {
        if (!isLogin) {
            System.out.println("You are not logged in. (" + username + ")\n");
            return;
        }
        if (account.getPosts().size() == 0) {
            System.out.println("There are no posts to view. (" + username + ")\n");
            return;
        }
        if(!isProfile){
            System.out.println("You should view the profile first to view posts. (" + username + ")\n");
            return;
        }
        if(account.getBlockedAccounts().contains(this)){
            System.out.println("You are blocked by this account. You can't see her/his posts. (" + username + ")\n");
            return;
        }
        System.out.println("-------------------------------");
        System.out.println("Viewing posts of " + account.getUsername() + "'s account.");
        System.out.println("-------------------------------\n");
        /* 
        for (int i = 0; i < account.getPosts().size(); i++) {
            System.out.println("(PostID: " + account.getPosts().get(i).getPostId() + ") " + account.getUsername() + ": " + account.getPosts().get(i).getContent() + "");
        } */
        Iterator<Post> iterator = account.getPosts().iterator();
        while(iterator.hasNext()){
            Post post = iterator.next();
            System.out.println("(PostID: " + post.getPostId() + ") " + account.getUsername() + ": " + post.getContent() + "");
        }
        System.out.println("-------------------------------\n");
        this.isProfile = false;
        this.isPosts = true;
    }

    /**
     * This function checks if the user is logged in, and not blocked by the account they want to follow,
     * and the account they want to follow is not themselves and not already in their following list.
     * If all of these conditions are true, then adds the account to following list,
     * and adds the user to the accounts followers list.
     * @param account The account that the user wants to follow.
     */
    public void followAccount(Account account) {
        if (!isLogin) {
            System.out.println("You can't follow an account without logging in, " + username + ".\n");
            return;
        }
        if (account.getAccountId() == accountId) {
            System.out.println("You cannot follow yourself.\n");
            return;
        }
        if (account.getBlockedAccounts().contains(this)) {
            System.out.println("You cannot follow this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        if (following.contains(account)) {
            System.out.println("You are already following " + account.getUsername() + ".\n");
            return;
        }
        following.add(account);
        account.getFollowers().add(this);
    
        System.out.println(username + " is now following " + account.getUsername() + ".\n");
        history.add("You followed " + account.getUsername() + ".");
    }

    /**
     * This function checks if the user is logged in, and not blocked by the account they want to unfollow,
     * and the account they want to unfollow is not themselves and already in their following list.
     * If all of these conditions are true, then removes the account from following list,
     * and removes the user from the accounts followers list.
     * @param account The account that the user wants to unfollow.
     */
    public void unfollowAccount(Account account) {
        if (!isLogin) {
            System.out.println("You can't unfollow an account without logging in, " + username + ".\n");
            return;
        }
        if (account.getAccountId() == accountId) {
            System.out.println("You cannot unfollow yourself.\n");
            return;
        }
        if (!following.contains(account)) {
            System.out.println("You are not following " + account.getUsername() + ".\n");
            return;
        }
        following.remove(account); 
        account.getFollowers().remove(this);
    
        System.out.println(username + " is no longer following " + account.getUsername() + ".\n");
        history.add("You unfollowed " + account.getUsername() + ".");
    }

    /**
     * Function to show the followings of the account.
     * Iterator is used to iterate through the LinkedList of followings.
     * @param account The account that you want to show the followings of.
     * @return A string of all the usernames of the accounts that the account is following.
     */
    public String showFollowings(Account account) {
        String followings = "";
        Iterator<Account> iterator = account.getFollowing().iterator();
        while (iterator.hasNext()) {
            followings += iterator.next().getUsername() + ", ";
        }
        return followings;
    }
     

    /**
     * Function to show the followers of the account.
     * Iterator is used to iterate through the LinkedList of followers.
     * @param account The account that you want to show the followers of.
     * @return A string of all the usernames of the accounts that are following the account.
     */
    public String showFollowers(Account account) {
        String followers = "";
        Iterator<Account> iterator = account.getFollowers().iterator();
        while (iterator.hasNext()) {
            followers += iterator.next().getUsername() + ", ";
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
        if (!isLogin) {
            System.out.println("You can't send a message without logging in, " + this.username + ".\n");
            return;
        }
        if (account.getBlockedAccounts().contains(this)) {
            System.out.println("You cannot send a message to this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        
        if (!following.contains(account)) {
            System.out.println("You can't send a message without following " + account.getUsername() + ", " + this.username + ".\n");
            return;
        }
    
        message.setSenderId(this.accountId);
        message.setReceiverId(account.getAccountId());
        message.setSenderUsername(this.username);
        message.setReceiverUsername(account.getUsername());
    
        System.out.println("Sending a message to " + account.getUsername() + "...\n");
    
        outbox.add(message);
        account.getInbox().add(message);
        history.add("You sent a message to " + account.getUsername() + ".");
    }

    /**
     * If the user is logged in, print out the number of messages in the inbox
     */
    public void checkInbox() {
        if (isLogin) {
            System.out.println("Checking inbox...");
            System.out.println("There is/are " + inbox.size() + " message(s) in the inbox.\n");
        } else {
            System.out.println("You can't check inbox without logging in, " + username + ".\n");
        }
    }

    /**
     * If the user is logged in, print out the number of messages in the outbox
     */
    public void checkOutbox() {
        if (isLogin) {
            System.out.println("Checking outbox...");
            System.out.println("There is/are " + outbox.size() + " message(s) in the outbox.\n");
        } else {
            System.out.println("You can't check outbox without logging in, " + username + ".\n");
        }
    }

    /**
     * If the user is logged in, print out the inbox contents
     */
    public void viewInbox() {
        if (isLogin) {
            System.out.println("Viewing inbox...");
            System.out.println("--------------------");
            for (Message message : inbox) {
                System.out.println("MessageId: " + message.getMessageId());
                System.out.println("From: " + message.getSenderUsername());
                System.out.println("To: " + message.getReceiverUsername());
                System.out.println("Message: " + message.getContent() + "\n");
            }
        } else {
            System.out.println("You can't view inbox without logging in, " + username + ".\n");
        }
    }

    /**
     * If the user is logged in, print out the outbox contents
     */
    public void viewOutbox() {
        if (isLogin) {
            System.out.println("Viewing outbox...");
            System.out.println("--------------------");
            for (Message message : outbox) {
                System.out.println("MessageId: " + message.getMessageId());
                System.out.println("From: " + message.getSenderUsername());
                System.out.println("To: " + message.getReceiverUsername());
                System.out.println("Message: " + message.getContent() + "\n");
            }
        } else {
            System.out.println("You can't view outbox without logging in, " + username + ".\n");
        }
    }

    /**
     * If the user is logged in, then create a new Like object and add it to the post's Like LinkedList.
     * @param post the post that the user wants to like
     * @param interactionId the id of the interaction, which is the same as the id of the like.
     * @param account the account of the post.
     */
    public void likePost(Post post, int interactionId, Account account) {
        if (!isLogin) {
            System.out.println("You can't like a post without logging in, " + this.username + ".\n");
            return;
        }
        if (blockedAccounts.contains(account)) {
            System.out.println("You cannot like a post of this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        if(!isPosts) {
            System.out.println("You should view posts before like the post, " + this.username + ".\n");
            return;
        }
        Iterator<Like> iterator = post.getLikes().iterator();
        while (iterator.hasNext()) {
            Like like = iterator.next();
            if (like.getAccountId() == this.accountId) {
                System.out.println("You already liked this post, " + this.username + ".\n");
                return;
            }
        }
         
        System.out.println("Liking a post of " + account.getUsername() + "...\n");
        Like like = new Like(interactionId, this.accountId, post.getPostId(), this.username);
        
        post.getLikes().add(like);
        history.add("You liked a post of " + account.getUsername() + ", post id: " + post.getPostId() + ".");
    }

    /**
     * If the user is logged in, then remove the Like object from the post's Like LinkedList.
     * @param post the post that the user wants to unlike
     * @param interactionId the id of the interaction, which is the same as the id of the like.
     * @param account the account of the post.
     */
    public void unlikePost(Post post, int interactionId, Account account) {
        int flag=0; // to see if the user liked the post before

        if (!isLogin) {
            System.out.println("You can't unlike a post without logging in, " + this.username + ".\n");
            return;
        }
        if (blockedAccounts.contains(account)) {
            System.out.println("You cannot unlike a post of this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        if(!isPosts) {
            System.out.println("You should view posts before unlike the post, " + this.username + ".\n");
            return;
        }

        Iterator<Like> iterator = post.getLikes().iterator();
        while (iterator.hasNext()) {
            Like like = iterator.next();
            if (like.getAccountId() == this.accountId) {
                flag=1; // if flag=1, it means that the user liked the post before
                iterator.remove(); // remove the like from the post
                break;
            }
        }
        if(flag==0) {
            System.out.println("You didn't like this post, " + this.username + ".\n");
            return;
        }

        System.out.println("Unliking a post of " + account.getUsername() + "...\n");
        history.add("You unliked a post of " + account.getUsername() + ", post id: " + post.getPostId() + ".");
    }

    /**
     * If the user is logged in, then create a new Comment object and add it to the post's Comment LinkedList.
     * @param post the post that the user wants to comment on
     * @param interactionId the id of the interaction, which is the same as the id of the comment.
     * @param account the account of the post.
     * @param content the content of the comment
     */
    public void commentPost(Post post, int interactionId, Account account, String content) {
        if (!isLogin) {
            System.out.println("You can't comment a post without logging in, " + this.username + ".\n");
            return;
        }
        if (blockedAccounts.contains(account)) {
            System.out.println("You cannot comment a post of this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        if(!isPosts) {
            System.out.println("You should view posts before comment the post, " + this.username + ".\n");
            return;
        }
        System.out.println("Commenting a post of " + account.getUsername() + "...\n");
        Comment comment = new Comment(interactionId, this.accountId, post.getPostId(), this.username, content);
        
        post.getComments().add(comment);
        history.add("You commented a post of " + account.getUsername() + ", post id: " + post.getPostId() + ".");
    }

    /**
     * If the user is logged in, then remove the Comment object from the post's Comment LinkedList.
     * @param post the post that the user wants to delete the comment from
     * @param interactionId the id of the interaction, which is the same as the id of the comment.
     * @param account the account of the post.
     */
    public void uncommentPost(Post post, int interactionId, Account account) {
        int flag=0; // to see if the user commented the post before

        if (!isLogin) {
            System.out.println("You can't delete a comment without logging in, " + this.username + ".\n");
            return;
        }
        if (blockedAccounts.contains(account)) {
            System.out.println("You cannot delete a comment of this account because you are blocked by " + account.getUsername() + ".\n");
            return;
        }
        if(!isPosts) {
            System.out.println("You should view posts before delete the comment, " + this.username + ".\n");
            return;
        }
        Iterator<Comment> iterator = post.getComments().iterator();
        while (iterator.hasNext()) {
            Comment comment = iterator.next();
            if (comment.getAccountId() == this.accountId) {
                flag=1; // if flag=1, it means that the user commented the post before
                iterator.remove(); // remove the comment from the post
                break;
            }
        }
        
        if(flag==0) {
            System.out.println("You didn't comment this post, " + this.username + ".\n");
            return;
        }

        System.out.println("Deleting a comment of " + account.getUsername() + "...\n");
        history.add("You deleted a comment of " + account.getUsername() + ", post id: " + post.getPostId() + ".");
    }

    /**
     * If the user is logged in and viwed the posts of the account,
     * it prints out the interactions of the posts of the account that is passed as an argument.
     * @param account The account whose posts interactions you want to view.
     */
    public void viewInteractions(Account account) {
        if (!isLogin) {
            System.out.println("You can't show interactions without logging in, " + this.username + ".\n");
            return;
        }
        if (!isPosts) {
            System.out.println("You can't view interactions without viewing posts, " + this.username + ".\n");
            return;
        }
    
        System.out.println("Viewing " + account.getUsername() + "'s posts' interactions...");
        System.out.println("--------------------");

        LinkedList<Post> posts = account.getPosts(); 

        // enhanced for loop
        for(Post post : posts) {
            System.out.println("(PostId: " + post.getPostId() + "): " + post.getContent());
            if(post.getLikes().size() != 0) {
                System.out.print("The post was liked by the following account(s): ");
                for(int j = 0 ; j<post.getLikes().size() ; j++){
                    System.out.println(post.getLikes().get(j).getUsername() + ", ");
                }
            }
            else{
                System.out.println("The post has no likes.");
            }

            if(post.getComments().size() != 0) {
                System.out.println("The post has " + post.getComments().size() + " comment(s).");
                for(int j = 0 ; j<post.getComments().size() ; j++){
                    System.out.println("Comment " + post.getComments().get(j).getInteractionId()+ ": '" + post.getComments().get(j).getUsername() + "' said '" + post.getComments().get(j).getContent() + "'.");                    
                }
            }
            else{
                System.out.println("The post has no comments.");
            }
        }
    
        System.out.println("--------------------\n");
    }

     /**
     * If the user is logged in, then add the account to the blockedAccounts LinkedList.
     * @param account The account that the user wants to block.
     */
    public void block(Account account) {
        if (!isLogin) {
            System.out.println("You can't block an account without logging in, " + this.username + ".\n");
            return;
        }
        if (blockedAccounts.contains(account)) {
            System.out.println("You already blocked " + account.getUsername() + ".\n");
            return;
        }
        if (account.getBlockedAccounts().contains(this)) {
            System.out.println("You are already blocked by " + account.getUsername() + ".\n");
            return;
        }
        if (account.getAccountId() == this.accountId) {
            System.out.println("You can't block yourself, " + this.username + ".\n");
            return;
        }
        System.out.println("Blocking " + account.getUsername() + "...\n");
        blockedAccounts.add(account);
        account.getBlockedAccounts().add(this);
        history.add("You blocked " + account.getUsername() + ".");
    }

    /**
     * If the user is logged in, then remove the account from the blockedAccounts LinkedList.
     * @param account The account that the user wants to unblock.
     */
    public void unblock(Account account) {
        if (!isLogin) {
            System.out.println("You can't unblock an account without logging in, " + this.username + ".\n");
            return;
        }
        if (!blockedAccounts.contains(account)) {
            System.out.println("You didn't block " + account.getUsername() + ".\n");
            return;
        }
        System.out.println("Unblocking " + account.getUsername() + "...\n");
        blockedAccounts.remove(account);
        account.getBlockedAccounts().remove(this);
        history.add("You unblocked " + account.getUsername() + ".");
    }

    /**
     * Function to show the blocked accounts.
     * Iterator is used to iterate through the LinkedList of blocked accounts.
     * @param account The account that you want to show the blocked accounts.
     * @return A string of all the usernames of the accounts that are blocked.
     */
    public String showBlockedAccounts(Account account) {
        String blocked = "";
        Iterator<Account> iterator = account.getBlockedAccounts().iterator();
        while (iterator.hasNext()) {
            blocked += iterator.next().getUsername() + ", ";
        }
        return blocked;
    }

    public void showHistory() {
        if (!isLogin) {
            System.out.println("You can't show history without logging in, " + this.username + ".\n");
            return;
        }
        System.out.println("Showing " + this.username + "'s history...\n");
        System.out.println("--------------------");
        Iterator<String> iterator = history.iterator();
        while (iterator.hasNext()) {
            String data = iterator.next();
            System.out.println(data);
        }
        
        System.out.println("--------------------\n");
    }


    
    
    


    


    













    // blokladığın zaman unfollow da yapacak, bloklu profil görebilir ama postları göremez






}
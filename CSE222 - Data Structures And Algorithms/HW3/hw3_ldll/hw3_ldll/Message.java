package hw3_ldll;

/**
 * Message class is used to create messages.
 * @version 1.0 05.04.2023
 * @author  Berru Lafci
 */
public class Message {
    
    private int messageId;
    private int senderId;
    private int receiverId;
    private String content;
    private String senderUsername;
    private String receiverUsername;

    /**
     * Constructor for Message class.
     * @param messageId message id of the message 
     * @param content content of the message 
     */
    public Message(int messageId, String content) {
        this.messageId = messageId;
        this.content = content;
    }

    /**
     * Getter returns the messageId of the message.
     * @return The messageId of the message.
     */
    public int getMessageId() {
        return messageId;
    }

    /**
     * Getter returns the senderId of the message.
     * @return The senderId of the message.
     */
    public int getSenderId() {
        return senderId;
    }

    /**
     * Getter returns the receiverId of the message.
     * @return The receiverId of the message.
     */
    public int getReceiverId() {
        return receiverId;
    }

    /**
     * Getter returns the content of the message.
     * @return The content of the message.
     */
    public String getContent() {
        return content;
    }

    /**
     * Getter returns the username of sender of the message.
     * @return The content of the message.
     */
    public String getSenderUsername() {
        return senderUsername;
    }

    /**
     * Getter returns the username of receiver of the message.
     * @return The content of the message.
     */
    public String getReceiverUsername() {
        return receiverUsername;
    }

    /**
     * Setter to set the message ID
     * @param messageId The message ID to be set.
     */
    public void setMessageId(int messageId) {
        this.messageId = messageId;
    }

    /**
     * Setter to set the sender ID
     * @param senderId The sender ID to be set.
     */
    public void setSenderId(int senderId) {
        this.senderId = senderId;
    }

    /**
     * Setter to set the receiver ID
     * @param receiverId The receiver ID to be set.
     */
    public void setReceiverId(int receiverId) {
        this.receiverId = receiverId;
    }

    /**
     * Setter to set the content
     * @param content The content to be set.
     */
    public void setContent(String content) {
        this.content = content;
    }

    /**
     * Setter to set the sender username
     * @param senderUsername The sender username to be set.
     */
    public void setSenderUsername(String senderUsername) {
        this.senderUsername = senderUsername;
    }

    /**
     * Setter to set the receiver username
     * @param receiverUsername The receiver username to be set.
     */
    public void setReceiverUsername(String receiverUsername) {
        this.receiverUsername = receiverUsername;
    }


}

package hw3_ldll;

public class testclass1 {
    public static void main(String[] args) {
        long startTime = System.nanoTime();
        
        Account gizemsungu = new Account(1,"gizemsungu", "13.01.2000", "Istanbul");
        Account sibelgulmez = new Account(2,"sibelgulmez", "01.01.2000", "Tokyo");
        Account gokhankaya = new Account(3, "gokhankaya", "02.02.2000", "Seoul");


        sibelgulmez.login();

        Post post1 = new Post(1, "Tokyo is a great city!");
        Post post2 = new Post(2, "Tokyo is capital of Japan!");

        sibelgulmez.addPost(post1);
        sibelgulmez.addPost(post2);

        sibelgulmez.followAccount(gokhankaya);
        sibelgulmez.followAccount(gizemsungu);

        sibelgulmez.logout();


        gokhankaya.login();

        gokhankaya.viewProfile(sibelgulmez);
        gokhankaya.viewPosts(sibelgulmez);
        gokhankaya.likePost(post1, 1, sibelgulmez);
        gokhankaya.commentPost(post1, 1, sibelgulmez, "Yes, I love Tokyo too!");
        gokhankaya.followAccount(sibelgulmez);
        gokhankaya.followAccount(gizemsungu);

        Message message1 = new Message(1, "Hello, how are you?");

        gokhankaya.sendMessage(gizemsungu, message1);

        gokhankaya.logout();


        gizemsungu.login();

        gizemsungu.checkOutbox();
        gizemsungu.checkInbox();

        gizemsungu.viewInbox();
        
        gizemsungu.viewProfile(sibelgulmez);
        gizemsungu.viewPosts(sibelgulmez);
        gizemsungu.viewInteractions(sibelgulmez);
        gizemsungu.likePost(post2, 2, sibelgulmez);
        gizemsungu.viewInteractions(sibelgulmez);
/* 
        gokhankaya.login();
        gokhankaya.showHistory();
        gizemsungu.showHistory();  */
        
        long endTime = System.nanoTime();
        double nanoseconds = (endTime - startTime);
        double seconds = nanoseconds / 1000000000.0;
        System.out.println("Duration: " + seconds + " seconds");
  
    }
}
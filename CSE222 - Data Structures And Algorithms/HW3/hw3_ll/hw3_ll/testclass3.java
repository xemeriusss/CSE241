package hw3_ll;

public class testclass3 {
    
    public static void main(String[] args) {
        

        long startTime = System.nanoTime();

        Account gizemsungu = new Account(1,"gizemsungu", "13.01.2000", "Istanbul");
        Account sibelgulmez = new Account(2,"sibelgulmez", "01.01.2000", "Tokyo");
        Account gokhankaya = new Account(3, "gokhankaya", "02.02.2000", "Seoul");


        sibelgulmez.login();

        Post post1 = new Post(1, "Tokyo is a great city!");
        Post post2 = new Post(2, "Tokyo is capital of Japan!");
        Post post3 = new Post(3, "I went to Tokyo last year!");
        Post post4 = new Post(4, "This is my Korea trip!");

        sibelgulmez.addPost(post3);
        sibelgulmez.addPost(post4);

        sibelgulmez.followAccount(gokhankaya);
        sibelgulmez.followAccount(gizemsungu);

        sibelgulmez.logout();


        gokhankaya.login();

        gokhankaya.viewProfile(sibelgulmez);
        gokhankaya.viewPosts(sibelgulmez);
        gokhankaya.likePost(post3, 1, sibelgulmez);
        gokhankaya.commentPost(post3, 1, sibelgulmez, "Yes, I love Tokyo too!");
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
        gizemsungu.likePost(post4, 2, sibelgulmez);
        gizemsungu.viewInteractions(sibelgulmez);

        System.out.println("----------------------Continue to testclass2----------------------\n");

        gizemsungu.login();

        gizemsungu.addPost(post1);
        gizemsungu.addPost(post2);

        gizemsungu.logout();

        sibelgulmez.login();

        sibelgulmez.viewProfile(gizemsungu);
        sibelgulmez.likePost(post1, 3, gizemsungu);
        sibelgulmez.logout();

        gokhankaya.login();

        gokhankaya.viewProfile(gizemsungu);
        gokhankaya.commentPost(post2, 2, gizemsungu, "Nice!");

        Message message2 = new Message(2, "Hello!");

        gokhankaya.sendMessage(gizemsungu, message2);

        gokhankaya.logout();

        gizemsungu.login();
        gizemsungu.viewProfile(gizemsungu);

        gizemsungu.checkInbox();
        gizemsungu.viewInbox();

        System.out.println("----------------------Continue to testclass3----------------------\n");

        gizemsungu.login();

        gizemsungu.block(sibelgulmez);
        gizemsungu.logout();

        Message message3 = new Message(3, "Are you there?");

        sibelgulmez.login();
        sibelgulmez.viewProfile(gizemsungu);
        sibelgulmez.sendMessage(gizemsungu, message3);

        long endTime = System.nanoTime();
        double nanoseconds = (endTime - startTime);
        double seconds = nanoseconds / 1000000000.0;
        System.out.println("Duration: " + seconds + " seconds");

        // todo: testclass2, testclass3, yorumlar, javadoc, rapor, 
    }
}

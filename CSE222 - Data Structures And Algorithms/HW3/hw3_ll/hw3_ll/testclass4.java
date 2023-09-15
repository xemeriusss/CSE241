package hw3_ll;

public class testclass4 {
    public static void main(String[] args) {

        long startTime = System.nanoTime();
        
        Account gizemsungu = new Account(1,"gizemsungu", "13.01.2000", "Istanbul");
        Account sibelgulmez = new Account(2,"sibelgulmez", "01.01.2000", "Tokyo");
        Account gokhankaya = new Account(3, "gokhankaya", "02.02.2000", "Seoul");
        Account berrylafci = new Account(4, "berrylafci", "06.10.2001", "Istanbul");
        Account nanaosaki = new Account(5, "nanaosaki", "05.03.1981", "Osaka");
        Account shinokazaki = new Account(6, "shinokazaki", "25.01.1985", "Tokyo");
        Account ichigokurosaki = new Account(7, "ichigokurosaki", "15.07.2000", "Tokyo");
        Account orihimeinoue = new Account(8, "orihimeinoue", "03.09.2000", "Tokyo");
        Account lelouch = new Account(9, "lelouch", "05.12.1999", "Britannia");
        Account sasukeuchiha = new Account(10, "sasukeuchiha", "23.07.1950", "Konoha");


        sibelgulmez.login();

        Post post1 = new Post(1, "Tokyo is a great city!");
        Post post2 = new Post(2, "Tokyo is capital of Japan!");
        Post post3 = new Post(3, "I love Tokyo!");

        sibelgulmez.addPost(post1);
        sibelgulmez.addPost(post2);
        sibelgulmez.addPost(post3);
        sibelgulmez.viewProfile(sibelgulmez);
        sibelgulmez.viewPosts(sibelgulmez);
        sibelgulmez.followAccount(gokhankaya);
        sibelgulmez.followAccount(gizemsungu);
        sibelgulmez.followAccount(nanaosaki);
        sibelgulmez.followAccount(shinokazaki);
        sibelgulmez.viewProfile(sibelgulmez);
        sibelgulmez.unfollowAccount(gokhankaya);
        sibelgulmez.viewProfile(sibelgulmez);
        sibelgulmez.unfollowAccount(nanaosaki); 
        sibelgulmez.viewProfile(sibelgulmez);
        sibelgulmez.logout();

        gizemsungu.login();
        gizemsungu.viewProfile(sibelgulmez);
        gizemsungu.viewPosts(sibelgulmez);
        gizemsungu.likePost(post1, 1, sibelgulmez);
        gizemsungu.likePost(post1, 2, sibelgulmez); // give "You already liked this post!" message
        gizemsungu.logout();

        gokhankaya.login();
        gokhankaya.viewProfile(sibelgulmez);
        gokhankaya.viewPosts(sibelgulmez);
        gokhankaya.likePost(post1, 3, sibelgulmez);
        gokhankaya.viewInteractions(sibelgulmez);
        gokhankaya.unlikePost(post1, 4, sibelgulmez);
        gokhankaya.viewInteractions(sibelgulmez);
        gokhankaya.logout();

        gizemsungu.login();
        gizemsungu.viewProfile(sibelgulmez);
        gizemsungu.viewPosts(sibelgulmez);
        gizemsungu.unlikePost(post1, 5, sibelgulmez);
        gizemsungu.viewInteractions(sibelgulmez);
        gizemsungu.likePost(post1, 6, sibelgulmez);
        gizemsungu.viewInteractions(sibelgulmez);
        gizemsungu.showHistory();
        gizemsungu.logout();
        
        berrylafci.login();
        berrylafci.block(sasukeuchiha);
        berrylafci.block(nanaosaki);
        berrylafci.viewProfile(berrylafci);
        berrylafci.unblock(sasukeuchiha);
        berrylafci.viewProfile(berrylafci);
        berrylafci.unblock(nanaosaki);
        berrylafci.viewProfile(berrylafci);
        berrylafci.showHistory();
        berrylafci.logout();

        Post post4 = new Post(4, "Bleach new season is coming!");
        Post post5 = new Post(5, "I love Bleach!");

        ichigokurosaki.login();
        ichigokurosaki.addPost(post4);
        ichigokurosaki.addPost(post5);
        ichigokurosaki.logout();

        orihimeinoue.login();
        orihimeinoue.viewProfile(ichigokurosaki);
        orihimeinoue.viewPosts(ichigokurosaki);
        orihimeinoue.commentPost(post4, 7, ichigokurosaki, "I'm so excited!");
        orihimeinoue.commentPost(post5, 8, ichigokurosaki, "Me too!");
        orihimeinoue.viewInteractions(ichigokurosaki);
        orihimeinoue.logout();

        lelouch.login();
        lelouch.viewProfile(ichigokurosaki);
        lelouch.viewPosts(ichigokurosaki);
        lelouch.commentPost(post4, 9, ichigokurosaki, "Code Geass is better!");
        lelouch.viewInteractions(ichigokurosaki);
        lelouch.logout();

        sibelgulmez.login();
        sibelgulmez.viewProfile(ichigokurosaki);
        sibelgulmez.viewPosts(ichigokurosaki);
        sibelgulmez.commentPost(post4, 10, ichigokurosaki, "aaaaa");
        sibelgulmez.logout();

        berrylafci.login();
        berrylafci.viewProfile(ichigokurosaki);
        berrylafci.viewPosts(ichigokurosaki);
        berrylafci.commentPost(post4, 11, ichigokurosaki, "bbbbbb");
        berrylafci.logout();

        sasukeuchiha.login();
        sasukeuchiha.viewProfile(ichigokurosaki);
        sasukeuchiha.viewPosts(ichigokurosaki);
        sasukeuchiha.commentPost(post4, 10, ichigokurosaki, "Naruto is better!");
        sasukeuchiha.viewInteractions(ichigokurosaki);
        sasukeuchiha.uncommentPost(post4, 11, ichigokurosaki);
        sasukeuchiha.viewInteractions(ichigokurosaki);
        sasukeuchiha.logout();

        lelouch.login();
        lelouch.viewProfile(ichigokurosaki);
        lelouch.viewPosts(ichigokurosaki);
        lelouch.uncommentPost(post4, 12, ichigokurosaki);
        lelouch.viewInteractions(ichigokurosaki);
        lelouch.logout();




        long endTime = System.nanoTime();
        double nanoseconds = (endTime - startTime);
        double seconds = nanoseconds / 1000000000.0;
        System.out.println("Duration: " + seconds + " seconds");

    }
}
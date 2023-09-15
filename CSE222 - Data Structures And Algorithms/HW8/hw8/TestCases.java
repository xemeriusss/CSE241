package hw8;

import java.awt.image.BufferedImage;

public class TestCases implements Runnable {

    private  String FileName;
    private int X_SIZE;
    private int Y_SIZE;

    public TestCases(String FileName, int X_SIZE, int Y_SIZE) {
        this.FileName = FileName;
        this.X_SIZE = X_SIZE;
    	this.Y_SIZE = Y_SIZE;
    }

    
    public void test(){
    	
        System.out.println("\n\n*******************\nMap is " + this.FileName + " with X_SIZE " + this.X_SIZE + " and Y_SIZE " + this.Y_SIZE + "\n********************\n");
        CSE222Map Map = new CSE222Map(this.FileName, this.X_SIZE, this.Y_SIZE);
        
        BufferedImage image = Map.convertPNG(FileName);
        
	    CSE222Graph Graph = new CSE222Graph(Map);

        CSE222BFS BFS = new CSE222BFS (Graph);
        long startTimeBFS = System.nanoTime();
        BFS.drawPath(image, FileName);
        long endTimeBFS = System.nanoTime();
        long durationBFS = (endTimeBFS - startTimeBFS);
        System.out.println("BFS duration for file: " + FileName + " is " + durationBFS + " nanoseconds");

        CSE222Dijkstra Dijkstra = new CSE222Dijkstra (Graph);
        long startTimeDij = System.nanoTime();
        Dijkstra.drawPath(image, FileName);
        long endTimeDij = System.nanoTime();
        long durationDij = (endTimeDij - startTimeDij);
        System.out.println("Dijkstra duration for file: " + FileName + " is " + durationDij + " nanoseconds");
        

        /*
        CSE222Dijkstra Dijkstra = new CSE222Dijkstra (Graph)
        List DijkstraPath = Dijkstra.findPath()
        CSE222BFS BFS= new CSE222BFS (Graph)
        List BFSPath = BFS.findPath()
        Map.convertPNG()
        Map.drawLine(DijkstraPath)
        Map.drawLine(BFSPath)
        Map.writePath(BFSPath)
        Map.writePath(DijkstraPath)
        System.out.println(“Dijkstra Path: “+ Dijkstra.length)
        System.out.println(“BFS Path: “+ BFS.length)*/

    }

    @Override
    public void run() {
        test();
    }
}


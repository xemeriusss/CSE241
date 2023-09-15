import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;
import javax.swing.JFrame;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 * 
 * Jtree class is used to create a tree from a text file and its methods are used to search in the tree.
 * @version 1.0 04.05.2023
 * @author  Berru Lafci
 */
public class Jtree {

    private DefaultMutableTreeNode root;

    /**
     * Getter for root.
     * @return root
     */
    public DefaultMutableTreeNode getRoot() {
        return root;
    }

    /**
     * The main function creates a JTree object, 
     * creates a tree using the createTree method, and
     * searches with methods.
     */
    public static void main(String[] args) {

        String value;

        Jtree obj = new Jtree();
        obj.createTree();

        Scanner input = new Scanner(System.in);
        System.out.print("Enter a value to search: ");
        value = input.nextLine();

        obj.searchWithBFS(obj.getRoot(), value);
        input.close();
    }

    /**
     * The function performs a breadth-first search on a tree starting from the root node to find a
     * specific value.
     * 
     * @param root The root node of the tree from which the search will start.
     * @param value The value parameter is the value that we are searching for in the tree using BFS
     */
    public void searchWithBFS(DefaultMutableTreeNode root, String value) {
        int step=0;

        Queue<DefaultMutableTreeNode> queue = new LinkedList<DefaultMutableTreeNode>(); // create queue
        queue.add(root); // add root to queue

        while (!queue.isEmpty()) { // loop until queue is empty
            DefaultMutableTreeNode node = queue.remove(); // remove first element from queue
            System.out.println("Step " + step + "->" + node.getUserObject());
            step++;
            if (node.getUserObject().equals(value)) { // check if node's value is equal to value
                System.out.println("Found: " + node.getUserObject());
                return;
            }
            for (int i = 0; i < node.getChildCount(); i++) { // loop for node's children
                DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i); // get child node at index i
                queue.add(childNode); // add child node to queue
                //System.out.println("Added to queue: " + childNode.getUserObject());
            }
        }
        System.out.println("Not found");
    } 

    /**
     * The function creates a JTree based on the contents of a text file and displays it in a JFrame.
     */
    public void createTree() {

        ArrayList<String[]> input = new ArrayList<String[]>();
        File file = new File("tree.txt");
        
        try {
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] row = line.split(";"); // string array of elements
                input.add(row); // add string array to arraylist
            }
            scanner.close();
        } catch (Exception e) {
            e.printStackTrace(); // print error
        }

        root = new DefaultMutableTreeNode("Root"); // create root node

        
        // loop for each row in input arraylist
        for (int i = 0; i < input.size(); i++) {
            String year = "";
            String course = "";
            String lecture = "";
            String problem = "";

            boolean problemExists = false;
            
            if(input.get(i).length == 3) { 
                year = input.get(i)[0];
                course = input.get(i)[1];
                lecture = input.get(i)[2];
            }
            else if (input.get(i).length == 4) {
                year = input.get(i)[0];
                course = input.get(i)[1];
                lecture = input.get(i)[2];
                problem = input.get(i)[3];
                problemExists = true;
            }
            else {
                System.out.println("Invalid input");
                return;
            }
            
            // ==== year ====
            DefaultMutableTreeNode yearNode = null; // null as default value
            for (int j = 0; j < root.getChildCount(); j++) { // loop for root's children (year nodes)
                DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) root.getChildAt(j); // get child node at index i

                // check if the year at index i is equal to the year of the child node
                if (childNode.getUserObject().equals(year)) { 
                    // set year node to child node 
                    // then it will be used to add other nodes
                    yearNode = childNode; 
                    break;
                }
            }
            // check if year node exists
            if (yearNode == null) {
                yearNode = new DefaultMutableTreeNode(year); // create new year node
                root.add(yearNode); // add year node to root
            }

            // ==== course ====
            DefaultMutableTreeNode courseNode = null;
            for (int j = 0; j < yearNode.getChildCount(); j++) { // loop for year node's children (course nodes)
                DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) yearNode.getChildAt(j);
                if (childNode.getUserObject().equals(course)) {
                    courseNode = childNode;
                    break;
                }
            }
            if (courseNode == null) {
                courseNode = new DefaultMutableTreeNode(course);
                yearNode.add(courseNode);
            }

            // ==== lecture ====
            DefaultMutableTreeNode lectureNode = null; 
            for (int j = 0; j < courseNode.getChildCount(); j++) { // loop for course node's children (lecture nodes)
                DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) courseNode.getChildAt(j);
                if (childNode.getUserObject().equals(lecture)) {
                    lectureNode = childNode;
                    break;
                }
            }
            if (lectureNode == null) {
                lectureNode = new DefaultMutableTreeNode(lecture);
                courseNode.add(lectureNode);
            }

            // ==== problem ====
            if(problemExists) {
                DefaultMutableTreeNode problemNode = null; 
                for (int j = 0; j < lectureNode.getChildCount(); j++) { // loop for lecture node's children (problem nodes)
                    DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) lectureNode.getChildAt(j);
                    if (childNode.getUserObject().equals(problem)) {
                        problemNode = childNode;
                        break;
                    }
                }
                if (problemNode == null) {
                    problemNode = new DefaultMutableTreeNode(problem);
                    lectureNode.add(problemNode);
                }
            }
        }

        JTree jt = new JTree(root); // create tree
        JFrame jf = new JFrame("Tree"); // create frame 
        jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // set close operation
        jf.add(jt); // add tree to frame
        //jf.pack();
        jf.setSize(300,300);
        jf.setVisible(true); 
    }

    
}

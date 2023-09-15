package hw3_ldll;

import java.util.AbstractList;
//import java.util.Iterator;
import java.util.List;
//import java.util.NoSuchElementException;

/**
 * The LDLinkedList class is a linked list implementation that allows for marking and removing nodes.
 * @version 1.0 05.04.2023
 * @author  Berru Lafci
 */
public class LDLinkedList<E> extends AbstractList<E> implements List<E> {

    private Node<E> head; // head of list
    private int size;
    private int numDeleted=0;

    /*
     * Constructor for LDLinkedList
     */
    public LDLinkedList() {
        head = null;
        size = 0;
    }
    

    /**
     * This is a generic Node class with data, a boolean flag for deletion, 
     * and a reference to the next Node.
     */
    private static class Node<E> {
        private E data;
        private boolean isDeleted;
        private Node<E> next;
        // private Node<E> prev;  -> double linked list
  
        // Constructor for public Node
        public Node(E data) {
            this.data = data;
            this.isDeleted = false;
            this.next = null;
        }
    }

    // I couldn't use iterator for LDLinkedList , so I commented it out.
/* 
    @Override
    public Iterator<E> iterator() {
        return new Iterator<E>() {
            private Node<E> current = head;
    
            @Override
            public boolean hasNext() {
                while (current != null && current.isDeleted) {
                    current = current.next;
                }
                return current != null;
            }
    
            @Override
            public E next() {
                if (!hasNext()) {
                    throw new NoSuchElementException();
                }
                if(current.isDeleted) {
                    E data = current.data;
                    current = current.next.next;
                    return data;
                }
                E data = current.data;
                current = current.next;
                return data;
            }
        };
    }
   */ 

    /*
     * Returns the size of the list.
     */
    @Override
    public int size() {
        return size;
    }


    /**
     * This function retrieves the element at a specified index in LDLinkedList and throws an
     * exception if the index is out of bounds or the node is deleted.
     * 
     * @param index The index parameter is the position of the element in the LDLinkedList.
     * @return The method is returning the data stored in the node at the specified 
     * index in the LDLinkedList.
     */
    @Override
    public E get(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException();
        }
        Node<E> current = head;
        for (int i = 0; i < index; i++) {
            current = current.next;
            // if the node is deleted, throw an exception
            if (current == null) {
                throw new IndexOutOfBoundsException();
            }
        }
        return current.data;
    }

    /**
     * This function sets the element at a specified index in LDLinkedList and throws an
     * exception if the index is out of bounds or the node is deleted.
     * 
     * @param index The index parameter is the position of the element in the LDLinkedList.
     * @param data The data parameter is the data to be stored in the node at the specified 
     * index in the LDLinkedList.
     * @return The method is returning the data stored in the node at the specified 
     * index in the LDLinkedList.
     */
    @Override
    public E set(int index, E data) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException();
        }
        Node<E> current = head;
        for (int i = 0; i < index; i++) {
            current = current.next;
            // if the node is deleted, throw an exception
            if (current == null) {
                throw new IndexOutOfBoundsException();
            }
        }
        E oldData = current.data;
        current.data = data;
        return oldData;
    }

    /**
     * This function adds a new node to the end of the LDLinkedList.
     * 
     * @param data The data parameter is the data to be stored in the new node.
     * @return The method is returning true if the node is added successfully.
     */
    @Override
    public boolean add(E data) {
        // if list is empty, create a new node and make it head
        if (head == null) {
            head = new Node<E>(data);
            size++;
            return true;
        }

        // traverse till the last node
        Node<E> current = head;
        while (current.next != null) {
            current = current.next;
        }

        // create a new node and make it the next of the last node
        current.next = new Node<E>(data);
        size++;
        return true;
    }

    /**
     * This function removes a node from a linked list by marking it as deleted.
     * If there are more than 2 deleted nodes, the method calls the deleteNode() method.
     * 
     * @param data The object that needs to be removed from the linked list.
     * @return The method is returning a boolean value, which is true if the specified data is found
     * and marked as deleted, and false if the data is not found in the linked list.
     */
    @Override
    public boolean remove(Object data) {
        Node<E> current = head;
        //Node<E> previous = null;

        while (current != null) {
            // if the head is the node to be deleted, mark it as deleted
            if (current.data == data) {
                current.isDeleted = true;
                numDeleted++;
                // if there are more than 2 deleted nodes, remove them
                if (numDeleted >= 2) {
                    deleteNode();
                }
                return true;
            } 
            // if the head is not the node to be deleted, move to the next node
            else {
                //previous = current;
                current = current.next;
            }
        }
        // if the node is not found, return false
        return false;
    }

    /**
     * This function deletes all nodes marked as deleted in a LDLinkedList.
     */
    private void deleteNode() {
        Node<E> current = head;
        Node<E> previous = null;

        // traverse the list
        while (current != null) {
            // if the node is marked as deleted, remove it
            if (current.isDeleted) {
                // if isDeleted is node is the head, make the next node the head
                // (delete the head)
                if (previous == null) {
                    head = current.next;
                } 
                // if isDeleted node is not the head, make the next node the next of the previous node
                // (delete the node between the previous and the next node)
                else {
                    previous.next = current.next;
                }
                // move to the next node
                current = current.next;
                numDeleted--;
                size--;
                // if there are no more deleted nodes, break the loop
                if(numDeleted == 0) {
                    break;
                }
            }
            // if the node is not marked as deleted, move to the next node
            else {
                previous = current;
                current = current.next;
            }
        }
    }

    /**
     * The ldlist function returns a new linked list that contains all non-deleted elements from the
     * original linked list.
     * 
     * @return The method returns a new LDLinkedList object that contains all non-deleted
     * elements from the original linked list.
     */
    public LDLinkedList<E> ldlist(){
        LDLinkedList<E> list = new LDLinkedList<E>();
        
        Node<E> current = head;
        while (current != null) {
            if (!current.isDeleted) {
                list.add(current.data);
            }
            current = current.next;
        }
        return list;
    }

}
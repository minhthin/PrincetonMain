/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:  Implement a double-ended queue data type, which is a
 *  generalization of a stack and queue where the user can add or
 *  remove items from the front or end of the deque. Each operation
 *  takes constant time and for n items, use at most 48n + 192 bytes of
 *  memory.
 *
 *  To do so, we construct a data type like using an alternative version of
 *  a Linked List that has pointers both next and previous.  This is necessary
 *  to update the last element of LL in constant worst case time.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Iterator;
import java.util.NoSuchElementException;

// use Linked Lists for easy pop/push for arbitrary size and keeping
// track of only the first/last elements
// LL has a higher performance guaranteed and still satisfies the
// memory requirements of the problem

public class Deque<Item> implements Iterable<Item> {

    /* @citation Adapted from: https://algs4.cs.princeton.edu/13stacks/
    LinkedStack.java.html Accessed 09/7/2020. */

    private Node first; // the first node of LL
    private Node last; // the last node of LL
    private int dequeSize; // the size of the LL

    // Node class for LL
    private class Node {
        private Item item; // item of node
        private Node next; // next node
        private Node previous; // create another pointer backwards
    }

    // construct an empty deque
    public Deque() {
        first = null;
        last = null;
        dequeSize = 0;
    }

    // is the deque empty?
    public boolean isEmpty() {
        return dequeSize == 0;
    }

    // return the number of items on the deque
    public int size() {
        return dequeSize;
    }

    // add the item to the front
    public void addFirst(Item item) {

        // check null
        checkNull(item);

        // create new node to be inserted
        Node newFirst = new Node();
        newFirst.item = item;

        // if initially deque is empty
        if (isEmpty()) {
            first = newFirst;
            last = first;
        }
        // initial first node = first;
        // now, the new item points to the initial first node
        else {
            newFirst.next = first; // update pointers
            first.previous = newFirst; // update pointers

            // set the newFirst to be the first
            first = newFirst;
        }

        dequeSize++;
    }

    // add the item to the back
    public void addLast(Item item) {
        // check null
        checkNull(item);

        Node newLast = new Node();
        newLast.item = item;

        // case where deque initially empty
        if (isEmpty()) {
            first = newLast;
            last = first;
        }
        // have old last item refer/point to to new last item
        else {
            last.next = newLast; // update pointers
            newLast.previous = last;

            last = newLast;
        }

        // update size of deque
        dequeSize++;
    }

    // remove and return the item from the front
    public Item removeFirst() {
        checkEmpty();

        Item temp = first.item; // get item to return
        Node oldFirst = first;

        // if deque has one element
        if (dequeSize == 1) {
            first = null;
            last = null;
        }
        // update the new first node
        else {
            first = oldFirst.next;
            first.previous = null; // update pointers
        }

        // update the size of deque
        dequeSize--;

        return temp;
    }

    // remove and return the item from the back
    public Item removeLast() {
        checkEmpty();

        Item temp = last.item; // get the item of last node
        Node oldLast = last;

        // if only one element in deque
        if (dequeSize == 1) {
            first = null;
            last = null;
        }

        // update last node
        else {
            last = oldLast.previous;
            last.next = null; // update pointers

        }
        // update size of deque
        dequeSize--;

        return temp;
    }


    // return an iterator over items in order from front to back
    public Iterator<Item> iterator() {
        return new DequeIterator();
    }

    private class DequeIterator implements Iterator<Item> {
        private Node current; // the current node

        // iterates through the dequeue by returning from first to last
        public DequeIterator() {
            current = first;
        }

        public boolean hasNext() {
            return current != null;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }

        // return the next item; iterate
        public Item next() {

            if (!hasNext()) {
                throw new NoSuchElementException("No more elements to return.");
            }

            Item item = current.item;
            current = current.next;

            return item;
        }
    }

    // check if item is null
    private void checkNull(Item item) {
        if (item == null) throw new
                IllegalArgumentException("Item cannot be null.");
    }

    // check if empty
    private void checkEmpty() {
        if (isEmpty()) {
            throw new NoSuchElementException("The deque is empty.");
        }
    }

    // unit test
    public static void main(String[] args) {
        // create new Deque
        Deque<Integer> testDeque = new Deque<Integer>();

        // generate list of integers to test for reference
        StdOut.println("Reference list: ");
        int testLength = 10;
        int[] randInt = new int[testLength];
        for (int i = 0; i < randInt.length; i++) {
            randInt[i] = StdRandom.uniform(10);
            StdOut.print(randInt[i] + " ");
        }

        // add the first half items
        for (int i = testLength / 2 - 1; i >= 0; i--) {
            testDeque.addFirst(randInt[i]);
        }

        // add the last half items
        for (int i = testLength / 2; i < testLength; i++) {
            testDeque.addLast(randInt[i]);
        }


        // check deque : iterate over items with iterator()
        StdOut.println("\nDeque list: ");
        for (int i : testDeque) {
            StdOut.print(i + " ");
        }

        // return size
        StdOut.println("\nDeque size: " + testDeque.size());

        // remove the first element and return
        StdOut.println("Deque first element: " + testDeque.removeFirst());

        // remove the last element and return
        StdOut.println("Deque first element: " + testDeque.removeLast());

        // check deque : iterate over items with iterator()
        StdOut.println("Deque list: ");
        for (int i : testDeque) {
            StdOut.print(i + " ");
        }

        // check if Empty
        StdOut.println("\nIs this deque empty? " + testDeque.isEmpty());
    }
}

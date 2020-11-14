/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:  A generic data type that is similar to stack or queue
 *  but the item is chosen uniformly at random among the items in the data
 *  structure.
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class RandomizedQueue<Item> implements Iterable<Item> {
    /* @citation Adapted from: https://algs4.cs.princeton.edu/
    13stacks/ResizingArrayStack.java.html Accessed 09/7/2020. */

    // instance variables
    private Item[] rq; // array that holds the elements
    private int queueSize; // total number of items in stack


    // construct an empty randomized queue using array representation
    public RandomizedQueue() {
        rq = (Item[]) new Object[2]; // initial number of elements
        queueSize = 0;
    }

    // is the randomized queue empty?
    public boolean isEmpty() {
        return queueSize == 0;
    }

    // return the number of items on the randomized queue
    public int size() {
        return queueSize;
    }

    // resize
    // if array is full, create new array of twice the size and copy
    // if array is one-quarter full, halve the size.
    // invariant if array in between
    // resize the array to the new length
    private Item[] copyQueue(int newLength) {

        // must resize to a size that is larger than the number of items
        if (newLength < size()) {
            throw new IllegalArgumentException();
        }

        // initialize new array for copied items
        Item[] copyQueue = (Item[]) new Object[newLength];

        // copy array
        for (int i = 0; i < size(); i++) {
            // put everything at the front of copied array
            if (rq[i] != null) {
                copyQueue[i] = rq[i];
            }
        }

        return copyQueue;
    }

    // check if needed to double
    private boolean isFull() {
        // number of items is same as length of array
        return size() == rq.length;
    }

    // check if needed to halve
    private boolean quarterSize() {
        // number of items < 1/4 length of array
        return size() < rq.length / 4;
    }

    // resize
    private void resizeQueue() {
        if (isFull()) {
            rq = copyQueue(rq.length * 2);
        }
        if (quarterSize()) {
            rq = copyQueue(rq.length / 2);
        }
    }

    // check if item is null
    private void checkNull(Item item) {
        if (item == null) {
            throw new IllegalArgumentException();
        }
    }

    // if empty, output exception
    private void checkEmpty() {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
    }

    // add the item
    public void enqueue(Item item) {
        checkNull(item);

        // resize
        resizeQueue();

        // add item to the end
        rq[size()] = item;
        queueSize++;
    }


    // remove and return a random item
    public Item dequeue() {
        checkEmpty();

        // choose random item
        // size = total items
        // choose random index, replace put new item in that index
        // move original item to end
        int randomNum = StdRandom.uniform(size());
        Item toRemove = rq[randomNum];

        // move the last element to removed spot
        if (randomNum != size() - 1) {
            Item lastItem = rq[size() - 1];
            rq[randomNum] = lastItem;
        }

        rq[size() - 1] = null;

        // resize if necessary
        queueSize--;
        resizeQueue();


        return toRemove;
    }

    // return a random item (but do not remove it)
    public Item sample() {
        checkEmpty();

        // choose random item
        // size = total items
        int randomNum = StdRandom.uniform(size());

        return rq[randomNum];

    }


    // return an independent iterator over items in random order
    public Iterator<Item> iterator() {
        return new RandomQueueIterator();
    }

    private class RandomQueueIterator implements Iterator<Item> {
        private int i = -1; // index of current item in iterator
        private final Item[] q; // array of items in RQ to be iterated

        // constructor; generate randomized iterator over elements
        public RandomQueueIterator() {
            // shuffle the elements
            // size = number of elements
            q = copyQueue(size());
            StdRandom.shuffle(q);
        }

        public boolean hasNext() {
            return i < size() - 1;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }

        public Item next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            i++;
            return q[i];

        }
    }


    public static void main(String[] args) {
        // create new Deque
        RandomizedQueue<Integer> testRQ = new RandomizedQueue<Integer>();

        // generate list of integers to test for reference
        StdOut.println("Reference list: ");
        int testLength = 10;
        int[] randInt = new int[testLength];
        for (int i = 0; i < randInt.length; i++) {
            randInt[i] = StdRandom.uniform(100);
            StdOut.print(randInt[i] + " ");
        }

        // add all items
        for (int i = 0; i < testLength; i++) {
            testRQ.enqueue(randInt[i]);
        }

        // check RQ : iterate over items with iterator()
        StdOut.println("\nRQ list: ");
        for (int i : testRQ) StdOut.print(i + " ");

        // return sample
        StdOut.println("Return sample: " + testRQ.sample());

        // remove all items
        for (int i = 0; i < testLength; i++) {
            StdOut.print("\n" + testRQ.size());
            StdOut.print(" " + testRQ.dequeue());
        }

        // return isEmpty
        StdOut.println("\nIs Deque empty: " + testRQ.isEmpty());

    }
}

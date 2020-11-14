/* *****************************************************************************
 *  Name:    Minh-Thi
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:  A client program that takes an integer k as a command-
 *  line argument.  It prints a sequence of strings from standard input
 *  and prints exactly k uniformly at random.  Print each item at
 *  most once.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class Permutation {

    public static void main(String[] args) {

        // takes in integer k
        int k = Integer.parseInt(args[0]);

        // no items returned
        if (k == 0) {
            return;
        }

        if (k < 0) {
            throw new IllegalArgumentException();
        }

        // reads in String sequence
        // use Randomized Queue of at most size k
        RandomizedQueue<String> inputString = new RandomizedQueue<String>();

        // read from StdIn until reach max
        // input first k strings
        for (int i = 0; i < k; i++) {
            if (!StdIn.isEmpty()) {
                inputString.enqueue(StdIn.readString());
            }
            else {
                throw new IllegalArgumentException();
            }
        }


        // keep count of total words
        int count = k + 1;

        // after k words has been read, read in the rest of the strings from
        // StdIn and replace with a random word in inputString RandomQueue
        while (!StdIn.isEmpty()) {
            // read new string
            String newRead = StdIn.readString();

            // choose a random index to swap, or do not swap
            // (k + 1 total choices)
            int randNum = StdRandom.uniform(count);

            if (randNum < k) {
                inputString.dequeue();
                inputString.enqueue(newRead);
            }

            count++;
        }

        // print final
        for (int i = 0; i < k; i++) {
            StdOut.println(inputString.dequeue());
        }

    }
}

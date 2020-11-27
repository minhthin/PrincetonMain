/* *****************************************************************************
 *  Name:    Tomisin Fasawe
 *  NetID:   ofasawe
 *  Precept: P04
 *
 *  Partner Name: Minh-thi Nguyen
 *  Partner NetID: minhthin
 *  Partner Precept: P08
 *
 *  Description:  The Burrows–Wheeler transform rearranges the characters in
 *                the input so that there are lots of clusters with repeated
 *                characters, but in such a way that it is still possible to
 *                recover the original input. The inverse transform recovers
 *                the original input
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

public class BurrowsWheeler {
    // total number of characters
    private static final int R = 256;

    // apply Burrows-Wheeler transform,
    // reading from standard input and writing to standard output
    public static void transform() {

        String word = BinaryStdIn.readString();
        // circular suffix array to perform transforms

        CircularSuffixArray suffixes = new CircularSuffixArray(word);

        // write the lexicographic order of the original text with index 0
        for (int i = 0; i < word.length(); i++) {
            if (suffixes.index(i) == 0) {
                BinaryStdOut.write(i);
                // StdOut.println();
                break;
            }
        }

        // write each character to StdOut and flush binary output stream
        for (int i = 0; i < word.length(); i++) {
            int index = suffixes.index(i);
            if (index == 0) {
                BinaryStdOut.write(word.charAt(word.length() - 1));
            }
            else {
                BinaryStdOut.write(word.charAt(index - 1));
            }
        }

        BinaryStdOut.flush();
    }


    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {
        // read first 32 bits as int for first
        int first = BinaryStdIn.readInt();
        // read the rest of the bits into a char array i.e. String
        String ourString = BinaryStdIn.readString();
        // compute next array using decoding
        int n = ourString.length();
        int[] next = new int[ourString.length()];
        for (int i = 0; i < n; i++)
            next[i] = i;

        // LSD radix sort
        // modified from LSD Radix API:
        // https://algs4.cs.princeton.edu/51radix/LSD.java.html
        // compute frequency counts
        int[] count = new int[R + 1];
        for (int i = 0; i < n; i++) {
            count[ourString.charAt(i) + 1]++;
        }
        // compute cumulates
        for (int j = 0; j < R; j++)
            count[j + 1] += count[j];

        // move data
        // aux holds the next indices while sorted holds the final characters
        // in position
        int[] aux = new int[ourString.length()];
        char[] sorted = new char[ourString.length()];
        for (int i = 0; i < n; i++) {
            aux[count[ourString.charAt(i)]++] = next[i];
            sorted[count[ourString.charAt(i)] - 1] = ourString.charAt(i);
        }
        // copy data back
        for (int i = 0; i < n; i++)
            next[i] = aux[i];

        // write each character to StdOut and flush binary output stream
        int nextIndex = first;
        for (int i = 0; i < ourString.length(); i++) {
            BinaryStdOut.write(sorted[nextIndex]);
            nextIndex = next[nextIndex];
        }
        BinaryStdOut.flush();
    }


    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {

        if (args[0].equals("-"))
            transform();

        else if (args[0].equals("+"))
            inverseTransform();
    }

}

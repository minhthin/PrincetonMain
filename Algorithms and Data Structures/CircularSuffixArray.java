/* *****************************************************************************
 *  Name:    Tomisin Fasawe
 *  NetID:   ofasawe
 *  Precept: P04
 *
 *  Partner Name: Minh-thi Nguyen
 *  Partner NetID: minhthin
 *  Partner Precept: P08
 *
 *  Description:  A circular suffix array describes the abstraction of a
 *                sorted array of the n circular suffixes of a string of
 *                length n. This implementation provides access to the original
 *                index of sorted suffixes.
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class CircularSuffixArray {
    private final String suffix; // variable to store string for circular suffix array
    private final int[] offset;   // int array to store offsets and final positions

    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null)
            throw new IllegalArgumentException();
        suffix = s;
        int n = s.length();
        offset = new int[n];
        for (int i = 0; i < n; i++)
            offset[i] = i;
        sort(suffix, offset);
    }

    // @citation https://algs4.cs.princeton.edu/51radix/Quick3string.java.html
    // Accessed November 24th, 2020
    // sort the suffixes using 3 way quick sort
    private static void sort(String word, int[] offset) {
        StdRandom.shuffle(offset);
        sort(word, offset, 0, word.length() - 1, 0);
    }

    // return the dth character of s, assuming circular
    private static int charAt(String s, int d) {
        return s.charAt(d % s.length());
    }

    // private recursive sorting method for sorting sub arrays
    // in 3 way algorithm
    private static void sort(String suffix, int[] offset, int lo,
                             int hi, int d) {
        // cutoff to insertion sort for small subarrays

        if (hi <= lo) {
            return;
        }
        int lt = lo, gt = hi;

        int index = (d + offset[lo]);
        // check if index has reached the end if has looped around twice
        if (index >= 2 * (suffix.length() - 1)) return;

        int pivot = charAt(suffix, index);

        int i = (lo + 1);

        while (i <= gt) {
            int compIndex = (d + offset[i]);
            if (compIndex >= 2 * (suffix.length() - 1)) return;

            // check if index has reached the end if has looped around twice
            int c = charAt(suffix, compIndex);
            if (c < pivot) exch(offset, lt++, i++);
            else if (c > pivot) exch(offset, i, gt--);
            else i++;
        }

        sort(suffix, offset, lo, lt - 1, d);
        sort(suffix, offset, lt, gt, d + 1);
        sort(suffix, offset, gt + 1, hi, d);
    }

    // private method for exchanging terms in 3 way quicksort algorithm
    private static void exch(int[] offset, int i, int j) {
        int temp = offset[i];
        offset[i] = offset[j];
        offset[j] = temp;
    }


    // length of s
    public int length() {
        return suffix.length();
    }

    // returns index of ith sorted suffix
    public int index(int i) {
        if (i < 0 || i > length() - 1)
            throw new IllegalArgumentException();
        return offset[i];
    }

    // unit testing (required)
    public static void main(String[] args) {
        String magic = "AABAABAABAABAABAABAABAABAABAAB";
        CircularSuffixArray suffixes = new CircularSuffixArray(magic);
        StdOut.println("Length is" + suffixes.length());
        for (int i = 0; i < magic.length(); i++)
            StdOut.println(suffixes.index(i));
    }
}

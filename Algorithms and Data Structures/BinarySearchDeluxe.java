/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description: When a sorted array has multiple keys that are equal to
 *  the search key, return the first and last index of appearances
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Arrays;
import java.util.Comparator;

public class BinarySearchDeluxe {

    // Returns the index of the first key in the sorted array a[]
    // that is equal to the search key or -1 if there is no such key

    // throw IllegalArgumentException if any input is null
    public static <Key> int firstIndexOf(Key[] a, Key key,
                                         Comparator<Key> comparator) {
        if (a == null || key == null || comparator == null) {
            throw new IllegalArgumentException(
                    "Array and keys and comparator cannot be null.");
        }

        // adaptation of binary search
        int lo = 0; // left pointer
        int hi = a.length - 1; // right pointer
        int firstIndex = -1; // the index of first occurrence

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2; // index of middle index

            // compare the keys
            Key midTerm = a[mid];
            int c = comparator.compare(key, midTerm);

            if (c < 0) {
                hi = mid - 1;
            }

            else if (c > 0) {
                lo = mid + 1;
            }

            else {
                firstIndex = mid;
                hi = mid - 1;
            }

        }

        return firstIndex;
    }

    // Returns the index of the last key in the sorted array a[]
    // that is equal to the search key or -1 if there is no such key
    public static <Key> int lastIndexOf(Key[] a, Key key,
                                        Comparator<Key> comparator) {

        // throw exception with invalid arguments
        if (a == null || key == null || comparator == null) {
            throw new IllegalArgumentException(
                    "Array and keys and comparator cannot be null.");
        }

        // adaptation of binary search
        int lo = 0; // left pointer
        int hi = a.length - 1; // right pointer
        int lastIndex = -1; // the index of first occurrence

        while (lo <= hi) {
            int mid = lo + (hi - lo + 1) / 2; // index of middle index

            // compare the keys
            Key midTerm = a[mid];
            int c = comparator.compare(key, midTerm);

            if (c < 0) {
                hi = mid - 1;
            }

            else if (c > 0) {
                lo = mid + 1;
            }

            else {
                lastIndex = mid;
                lo = mid + 1;
            }

        }

        return lastIndex;
    }


    // unit testing
    public static void main(String[] args) {
        // given sorted array, return first and last index of
        // specific occurrence

        String st = "abcde";

        String[] a = new String[10];
        for (int i = 0; i < a.length; i++) {
            a[i] = Character.toString(
                    st.charAt(StdRandom.uniform(5)));
        }
        Arrays.sort(a);
        for (int i = 0; i < a.length; i++) {
            StdOut.print(a[i] + " ");
        }

        StdOut.println("\n Return First and Last Occurrence of: e");
        StdOut.println(BinarySearchDeluxe.firstIndexOf(
                a, "e", String.CASE_INSENSITIVE_ORDER));

        StdOut.println(BinarySearchDeluxe.lastIndexOf(
                a, "e", String.CASE_INSENSITIVE_ORDER));


    }
}

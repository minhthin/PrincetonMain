/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description: A data type that uses Term and BinarySearchDeluxe
 *  and sorts through terms in lexicographic order and uses
 *  binary search to find all query strings that start with a
 *  given prefix, sorting the matching terms in descending
 *  order by weight.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Arrays;

public class Autocomplete {

    private final Term[] termsArray; // array of terms

    // Initialize the data structure from the given array of terms
    // Throw IllegalArgumentException if terms array is null
    public Autocomplete(Term[] terms) {

        if (terms == null)
            throw new IllegalArgumentException("Terms array cannot be null.");

        // Defensive Copying
        termsArray = new Term[terms.length];

        // Check if entry is null and copy
        for (int i = 0; i < termsArray.length; i++) {

            termsArray[i] = terms[i];

            if (termsArray[i] == null)
                throw new IllegalArgumentException("Term cannot be null.");
        }

        // Sort in natural order
        Arrays.sort(termsArray);

    }

    // Returns all terms that start with given prefix, in descending
    // order of weight
    public Term[] allMatches(String prefix) {
        checkPrefix(prefix);

        Term pref = new Term(prefix, 0);

        // first occurrence
        int firstIndex = BinarySearchDeluxe.firstIndexOf(
                termsArray, pref, Term.byPrefixOrder(prefix.length()));

        // last occurrence
        int lastIndex = BinarySearchDeluxe.lastIndexOf(
                termsArray, pref, Term.byPrefixOrder(prefix.length()));


        // if it has no matches
        if (firstIndex == -1 && lastIndex == -1) {
            Term[] matchArray = new Term[0]; // array of
            // terms that start with given prefix
            return matchArray;
        }

        else {
            Term[] matchArray = new Term[lastIndex - firstIndex + 1];
            // array of terms that start with given prefix

            for (int i = 0; i < lastIndex - firstIndex + 1; i++) {
                matchArray[i] = termsArray[firstIndex + i];
            }

            // sort matched array in descending order of weight
            Arrays.sort(matchArray, Term.byReverseWeightOrder());

            return matchArray;
        }
    }

    // Returns the number of terms that start with given prefix
    public int numberOfMatches(String prefix) {
        checkPrefix(prefix);

        Term pref = new Term(prefix, 0);

        // first occurrence
        int firstIndex = BinarySearchDeluxe.firstIndexOf(
                termsArray, pref, Term.byPrefixOrder(prefix.length()));

        // last occurrence
        int lastIndex = BinarySearchDeluxe.lastIndexOf(
                termsArray, pref, Term.byPrefixOrder(prefix.length()));

        // return number of occurrences
        if (firstIndex == -1 && lastIndex == -1) return 0;
        else return lastIndex - firstIndex + 1;

    }

    // check if prefix is null
    private void checkPrefix(String prefix) {
        if (prefix == null)
            throw new IllegalArgumentException("Prefix cannot be null.");
    }

    // unit testing
    public static void main(String[] args) {

        // Copied from Assignment 3 testing example of autocomplete
        String filename = args[0];
        In in = new In(filename);

        int n = in.readInt(); // read in number of terms
        Term[] terms = new Term[n];
        // Assign terms in terms
        for (int i = 0; i < n; i++) {
            long weight = in.readLong(); // read the next weight
            in.readChar(); // skip over the tab
            String query = in.readLine(); // read the next query
            terms[i] = new Term(query, weight); // construct the term
        }

        // Read in the queries from standard input and print top k matching
        int k = Integer.parseInt(args[1]);
        Autocomplete autocomplete = new Autocomplete(terms);

        // Find and print matches
        while (StdIn.hasNextLine()) {
            String prefix = StdIn.readLine();
            Term[] results = autocomplete.allMatches(prefix);
            StdOut.printf("%d matches\n", autocomplete.numberOfMatches(prefix));

            for (int i = 0; i < Math.min(k, results.length); i++)
                StdOut.println(results[i]);
        }

    }
}

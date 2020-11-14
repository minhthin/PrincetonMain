/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:   An immutable data type that has a query string and associated
 *  weight, called Term.  The data type supports comparing in three different
 *  orders: lexicographic by query string, descending order by weight, and
 *  lexicographic order by query string using the first r characters
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;

import java.util.Arrays;
import java.util.Comparator;

public class Term implements Comparable<Term> {

    private final String query; // term string
    private final long weight; // term weight

    // Initializes a term with the given query string and weight
    // throws IllegalArgumentException if query is null or weight is negative
    public Term(String query, long weight) {
        checkTerm(query, weight);

        // initialize term
        this.query = query;
        this.weight = weight;
    }

    // Compares the two terms in descending order by weight
    // return 0 if the weights are equal
    // return positive number if the first term is less than the second term
    // return negative number if first term is larger than second term
    public static Comparator<Term> byReverseWeightOrder() {
        return new ByReverseWeightOrder();
    }

    // implements the compare by reverse weight order
    private static class ByReverseWeightOrder implements Comparator<Term> {

        // class that constructs comparison by reverse weight order
        private ByReverseWeightOrder() {
        }

        // compare weights
        public int compare(Term t1, Term t2) {
            if (t1.weight < t2.weight) return 1;
            else if (t1.weight > t2.weight) return -1;
            return 0;
        }
    }

    // Compares the two terms in lexicographic order,
    // but using only the first r characters of each query
    public static Comparator<Term> byPrefixOrder(int r) {
        checkPrefixInput(r);
        return new ByPrefixOrder(r);
    }

    // throw exception if r is negative
    private static void checkPrefixInput(int r) {
        if (r < 0) throw new IllegalArgumentException(
                "Input cannot be negative");
    }

    // implements the compare by prefix order
    private static class ByPrefixOrder implements Comparator<Term> {

        private final int r; // number of characters to search
        private char t1Char; // the character of first term
        private char t2Char; // the character of the second term


        // class that constructs comparison by prefix order
        private ByPrefixOrder(int r) {
            checkPrefixInput(r);
            this.r = r;
        }

        // compare the two substrings by character to meet
        // performance requirements
        // return 0 if equal,
        public int compare(Term t1, Term t2) {

            // if there is no prefix to be compared
            if (r == 0) return 0;

                // if words have the same length and can compare whole strings
            else if (r >= t1.query.length() &&
                    t1.query.length() == t2.query.length()) {
                return t1.query.compareTo(t2.query);
            }

            else {
                int i = 0; // tracker of character position

                while (i < r) {

                    // if r is larger than the length of either queries
                    if (i == t1.query.length()) return -1;
                    if (i == t2.query.length()) return 1;

                    // characters at positions
                    t1Char = t1.query.charAt(i);
                    t2Char = t2.query.charAt(i);

                    // compare characters
                    if (t1Char != t2Char)
                        return Character.compare(t1Char, t2Char);

                    i++;
                }
            }

            return 0;
        }
    }


    // Compares the two terms in lexicographic order by query
    public int compareTo(Term that) {
        // compare whole strings
        return this.query.compareTo(that.query);
    }


    // Returns a string representation of this term in the following format:
    // the weight, followed by a tab, followed by the query
    public String toString() {
        return String.format("%10d\t%s", weight, query);
    }

    // check if the query is null or the weight is zero
    private void checkTerm(String termQuery, long termWeight) {
        if (termQuery == null || termWeight < 0) {
            throw new IllegalArgumentException(
                    "The query cannot be null and the weight "
                            + "cannot be negative");
        }
    }

    // unit testing
    public static void main(String[] args) {

        // initialize five terms
        Term[] termList = new Term[5];
        termList[0] = new Term("The Smiths", 1000);
        termList[1] = new Term("The Velvet Underground", 15520);
        termList[2] = new Term("The Simpsons", 2234);
        termList[3] = new Term("Beyonce", 9434);
        termList[4] = new Term("The Shins", 47392);

        // test by Reverse Weight Order
        StdOut.println("Sort by Reverse Weight Order:");
        Arrays.sort(termList, Term.byReverseWeightOrder());
        for (int i = 0; i < termList.length; i++) {
            StdOut.println(termList[i].toString());
        }

        // test by Prefix Order with r = 5
        StdOut.println("\n Sort by Prefix Order (r = 5):");
        Arrays.sort(termList, Term.byPrefixOrder(5));
        for (int i = 0; i < termList.length; i++) {
            StdOut.println(termList[i].toString());
        }

        // compares two terms
        StdOut.println("\n Compare the Terms: ");
        StdOut.println(termList[4].toString());
        StdOut.println(termList[1].toString());
        StdOut.println(termList[4].compareTo(termList[1]));
    }
}

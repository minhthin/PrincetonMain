/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description: Given a list of Wordnet nouns, find the noun that is the
 *  least related to the others by computing the sum of the distances
 *  between each noun and every other one
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {

    private final WordNet wordnet;

    // constructor takes in wordnet object
    public Outcast(WordNet wordnet) {
        this.wordnet = wordnet;
    }

    // given an array of Wordnet nouns, return an outcast
    public String outcast(String[] nouns) {
        int maxDistance = 0;
        int stringIndex = -1;

        for (int i = 0; i < nouns.length; i++) {
            // compute the distance for each noun
            int dist = 0;
            for (int j = 0; j < nouns.length; j++) {
                dist = dist + wordnet.distance(nouns[i], nouns[j]);
            }

            if (dist >= maxDistance) {
                maxDistance = dist;
                stringIndex = i;
            }
        }

        return nouns[stringIndex];

    }

    // test client
    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        Outcast outcast = new Outcast(wordnet);
        for (int t = 2; t < args.length; t++) {
            In in = new In(args[t]);
            String[] nouns = in.readAllStrings();
            StdOut.println(args[t] + ": " + outcast.outcast(nouns));
        }
    }
}

/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:  Implement an immutable datatype that parses synsets file and
 *  hypernyms file to create a rooted digraph of synsets.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.Bag;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.LinearProbingHashST;
import edu.princeton.cs.algs4.StdOut;

public class WordNet {
    // instance variables
    private final Digraph G; // Wordnet digraph
    private final ShortestCommonAncestor sca; // shortest common ancestor
    private final LinearProbingHashST<Integer, String> synsetST; // symbol table of
    // synsets
    private final LinearProbingHashST<String, Bag<Integer>> nounST; // symbol table
    // of nouns, associated with a queue of synset ids

    // constructor takes the name of two input files
    // build Wordnet DAG
    public WordNet(String synsets, String hypernyms) {
        // throw exception if input is null
        if (synsets == null || hypernyms == null)
            throw new IllegalArgumentException("File names cannot be null.");

        // initialize symbol tables for synsets and nouns
        synsetST = new LinearProbingHashST<Integer, String>(); // synset ST
        nounST = new LinearProbingHashST<String, Bag<Integer>>(); // noun ST

        // read the synset file based on description in assignment page
        parseSynset(synsets);

        G = new Digraph(synsetST.size());

        // read the hypernyms file based on description in assignment page
        parseHypernyms(hypernyms);

        sca = new ShortestCommonAncestor(G);
    }

    // all WordNet nouns
    public Iterable<String> nouns() {
        return nounST.keys();
    }

    // is the word a Wordnet noun?
    public boolean isNoun(String word) {
        return nounST.contains(word);
    }

    // a synset that is a shortest common ancestor of noun1
    // and noun2
    public String sca(String noun1, String noun2) {
        // return the synset of the shortest common ancestor
        return synsetST.get(
                sca.ancestorSubset(nounST.get(noun1), nounST.get(noun2)));
    }


    // distance between noun1 and noun2
    public int distance(String noun1, String noun2) {
        // return the synset of the shortest common ancestor
        return sca.lengthSubset(nounST.get(noun1), nounST.get(noun2));

    }

    // read in synsets file
    private void parseSynset(String synsets) {
        In synsetsFile = new In(synsets);

        while (synsetsFile.hasNextLine()) {
            // split file by line
            String line = synsetsFile.readLine();
            // split synset file fields by commas
            String[] synLine = line.split(",");

            // read the synset ID
            int synsetID = Integer.parseInt(synLine[0]);
            // read the synset
            String synset = synLine[1];
            synsetST.put(synsetID, synset);

            String[] nounSynset = synset.split(" ");
            for (int j = 0; j < nounSynset.length; j++) {
                String noun = nounSynset[j];

                // add to the noun ST by adding the synset id to the noun
                if (nounST.contains(noun)) nounST.get(noun).add(synsetID);
                else {
                    Bag<Integer> idList = new Bag<Integer>();
                    idList.add(synsetID);
                    nounST.put(noun, idList);
                }

            }
        }
    }

    // read in hypernyms file
    private void parseHypernyms(String hypernyms) {
        In hypernymsFile = new In(hypernyms);

        while (hypernymsFile.hasNextLine()) {
            String lineHyper = hypernymsFile.readLine();
            String[] hFields = lineHyper.split(",");

            // read hypernyms
            int lengthHFields = hFields.length;
            // read synset id
            int id = Integer.parseInt(hFields[0]);

            // add all hypernyms
            // each directed edge v -> w represents that w is a
            // hypernym of v
            for (int i = 1; i < lengthHFields; i++) {
                G.addEdge(id, Integer.parseInt(hFields[i]));
            }
        }
    }


    // unit testing
    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);

        // test nouns()
        for (String s : wordnet.nouns()) {
            StdOut.println("Nouns: " + s);
        }

        // test isNoun?
        StdOut.println("Is cat a noun? " + wordnet.isNoun("cat"));

        // test sca
        String noun1 = "dog";
        String noun2 = "table";
        StdOut.println("Shortest Common Ancestor " + wordnet.sca(noun1, noun2));
        StdOut.println("Distance " + wordnet.distance(noun1, noun2));
    }
}

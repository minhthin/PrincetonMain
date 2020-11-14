/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Description:  Immutable datatype that takes in a rooted directed
 *  graph and returns the shortest common ancestor and length of
 *  shortest ancestral path.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.Bag;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.DirectedCycle;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;

public class ShortestCommonAncestor {

    // instance variables
    private final Digraph G; // digraph

    // constructor takes a rooted DAG as argument
    public ShortestCommonAncestor(Digraph G) {
        // throw exception if constructor is not a rooted DAG
        // rooted DAG is acyclic and has one vertex
        if (G == null || G.V() < 1) throw new
                IllegalArgumentException("Argument cannot be null.");

        // ensure that the digraph is acyclic iff it has toplogical order
        // check if there is a cyclic cycle
        DirectedCycle dc = new DirectedCycle(G);
        if (dc.hasCycle())
            throw new IllegalArgumentException("Digraph has a cycle.");

        // check if Root exists
        if (!hasRoot(G))
            throw new IllegalArgumentException("Digraph does not have root.");

        // initialize
        this.G = new Digraph(G);

    }

    // check to see if Digraph has a root
    private boolean hasRoot(Digraph g) {
        int count = 0;

        // count vertices that has outdegree of 0 (root)
        for (int i = 0; i < g.V(); i++) {
            if (g.outdegree(i) == 0) count++;
        }

        return (count == 1);

    }

    // length of shortest ancestral path between v and w
    public int length(int v, int w) {
        // throw exception when vertex argument is outside prescribed range
        validateVertex(v);
        validateVertex(w);

        if (v == w) return 0;

        Queue<Integer> q1 = new Queue<Integer>();
        q1.enqueue(v);
        Queue<Integer> q2 = new Queue<Integer>();
        q2.enqueue(w);

        ModifiedBreadthFirstSearch mbfs = new
                ModifiedBreadthFirstSearch(G, q1, q2);

        return mbfs.getLength();
    }

    // a shortest common ancestor of vertices v and w
    public int ancestor(int v, int w) {
        // throw exception when vertex argument is outside prescribed range
        validateVertex(v);
        validateVertex(w);

        if (v == w) return v;

        Queue<Integer> q1 = new Queue<Integer>();
        q1.enqueue(v);
        Queue<Integer> q2 = new Queue<Integer>();
        q2.enqueue(w);

        ModifiedBreadthFirstSearch mbfs = new
                ModifiedBreadthFirstSearch(G, q1, q2);

        return mbfs.getCommonAncestor();
    }

    // length of shortest ancestral path of vertex subsets A and B
    public int lengthSubset(Iterable<Integer> subsetA,
                            Iterable<Integer> subsetB) {
        validateSubset(subsetA);
        validateSubset(subsetB);

        ModifiedBreadthFirstSearch mbfs = new
                ModifiedBreadthFirstSearch(G, subsetA, subsetB);

        return mbfs.getLength();
    }


    // a shortest common ancestor of vertex subsets A and B
    public int ancestorSubset(Iterable<Integer> subsetA,
                              Iterable<Integer> subsetB) {

        validateSubset(subsetA);
        validateSubset(subsetB);

        ModifiedBreadthFirstSearch mbfs = new
                ModifiedBreadthFirstSearch(G, subsetA, subsetB);

        return mbfs.getCommonAncestor();
    }

    // check if vertex is within the described bounds
    private void validateVertex(int v) {
        if (v < 0 || v >= G.V()) {
            throw new IllegalArgumentException();
        }
    }

    // check if subset is within argument requirements
    private void validateSubset(Iterable<Integer> subset) {
        // null
        if (subset == null) {
            throw new IllegalArgumentException();
        }

        Iterator<Integer> iterateSubset = subset.iterator();

        if (!iterateSubset.hasNext()) throw new IllegalArgumentException();

        while (iterateSubset.hasNext()) {
            if (iterateSubset.next() == null)
                throw new IllegalArgumentException();
        }

    }

    // private class to implement extra credit: method takes time proportional
    // to number of vertices and edges reachable from argument vertices
    // re-implement breadth-first search
    private static class ModifiedBreadthFirstSearch {
        // declare instance variables
        private final Digraph G; // Digraph
        // create two arrays of marked for setA and setB
        private final boolean[] markedA; // setA
        private final boolean[] markedB; // setB

        // create two arrays of dist for setA and setB
        private final int[] distToA; // setA
        private final int[] distToB; // setB

        // source integers for setA and setB
        private final Iterable<Integer> subsetA; // setA
        private final Iterable<Integer> subsetB; // setB

        private static final int INFINITY = Integer.MAX_VALUE; // set infinity

        private final int shortestDistance; // total distance
        // to the shortest common ancestor
        private final int commonAncestor; // shortest common ancestor

        // constructor to initiate arrays for breadth search
        public ModifiedBreadthFirstSearch(Digraph G, Iterable<Integer> sA,
                                          Iterable<Integer> sB) {
            this.G = G;
            subsetA = sA;
            subsetB = sB;

            // for Breadth first search
            markedA = new boolean[G.V()];
            markedB = new boolean[G.V()]; // initialize array to
            // mark items
            distToA = new int[G.V()];
            distToB = new int[G.V()]; // initialize array to keep track
            // of distance of points from v and w

            // set temporary values for shortest distance and ancestor
            int tempShortestDistance = INFINITY;
            int tempCommonAncestor = -1;

            // initialize breadth first search, modified by tracking two
            // parallel bfs for each subset
            Queue<Integer> queueA = new Queue<>();
            Queue<Integer> queueB = new Queue<>();

            for (int i : subsetA) {
                validateVertex(i);
                markedA[i] = true;
                distToA[i] = 0;
                queueA.enqueue(i);
            }

            for (int j : subsetB) {
                validateVertex(j);
                if (markedA[j]) {
                    tempShortestDistance = 0;
                    tempCommonAncestor = j;
                }
                else {
                    markedB[j] = true;
                    distToA[j] = 0;
                    queueB.enqueue(j);
                }
            }


            // perform normal bfs on queue A until root reached
            while (!queueA.isEmpty()) {
                int v = queueA.dequeue();
                for (int i : G.adj(v)) {
                    if (!markedA[i] && !markedB[i]) {
                        bfsUpdate(i, v, markedA, distToA, queueA);
                    }

                    else if (!markedA[i] && markedB[i]) {
                        bfsUpdate(i, v, markedA, distToA, queueA);

                        // calculate distance to ancestor and compare with min
                        // update new shortest ancestor
                        int distance = distToA[v] + distToB[i] + 1;
                        if (distance <= tempShortestDistance) {
                            tempShortestDistance = distance;
                            tempCommonAncestor = i;
                        }
                    }
                }
            }

            // perform bfs on queue B to find shortest common ancestor
            while (!queueB.isEmpty()) {
                int w = queueB.dequeue();
                for (int i : G.adj(w)) {
                    if (!markedB[i] && !markedA[i]) {
                        bfsUpdate(i, w, markedB, distToB, queueB);
                    }
                    // reach a vertex already marked by A; common ancestor found
                    else if (!markedB[i] && markedA[i]) {
                        bfsUpdate(i, w, markedB, distToB, queueB);

                        // calculate distance to ancestor and compare with min
                        // update new shortest ancestor
                        int distance = distToA[i] + distToB[w] + 1;
                        if (distance <= tempShortestDistance) {
                            tempShortestDistance = distance;
                            tempCommonAncestor = i;
                        }
                    }
                }
            }

            shortestDistance = tempShortestDistance;
            commonAncestor = tempCommonAncestor;
        }

        // return length to shortest common ancestor
        public int getLength() {
            return shortestDistance;
        }

        // return shortest common ancestor
        public int getCommonAncestor() {
            return commonAncestor;
        }

        // add vertex to queue and mark as visited and update distance
        private void bfsUpdate(int i, int v, boolean[] marked, int[] dist,
                               Queue<Integer> q) {
            marked[i] = true;
            dist[i] = dist[v] + 1;
            q.enqueue(i);
        }

        // check if vertex is in range
        private void validateVertex(int v) {
            if (v < 0 || v >= G.V()) {
                throw new IllegalArgumentException();
            }
        }
    }

    // unit testing
    public static void main(String[] args) {
        In in = new In(args[0]);
        Digraph G = new Digraph(in);
        ShortestCommonAncestor sca = new ShortestCommonAncestor(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length = sca.length(v, w);
            int ancestor = sca.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }

        Bag<Integer> sA = new Bag<Integer>();
        sA.add(6);
        sA.add(7);
        sA.add(3);
        Bag<Integer> sB = new Bag<Integer>();
        sB.add(4);
        sB.add(null);
        int length = sca.lengthSubset(sA, sB);
        int ancestor = sca.ancestorSubset(sA, sB);
        StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);

    }
}

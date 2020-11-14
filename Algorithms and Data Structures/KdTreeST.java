/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Partner Name:    Tomisin Fasawe
 *  Partner NetID:   ofasawe
 *  Partner Precept: P04
 *
 *  Description:  A mutable data type that uses a 2D tree to implement
 *  the same API as PointST.  The data type is a BST with points
 *  in the nodes, using x and y coordinates as keys in strictly
 *  alternating sequence, starting with the x coordinate
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.Stopwatch;

public class KdTreeST<Value> {


    // instance variables
    private Node root; // root node
    // set the root rectangle to infinity (or if points between 0 and 1, then
    // set to be 0-1)
    private final RectHV rootRect = new RectHV(Double.NEGATIVE_INFINITY,
                                               Double.NEGATIVE_INFINITY,
                                               Double.POSITIVE_INFINITY,
                                               Double.POSITIVE_INFINITY);


    // create node class that has stores a point and value
    private class Node {
        private final Point2D p; // point associated with node (key)
        private Value val; // value associated with point
        private RectHV rect; // axis-aligned rectangle that corresponds to the
        // node
        private Node lb; // the left/bottom node
        private Node rt; // the right/top node
        private int size = 1; // size of subtree

        // implement Node constructor
        private Node(Point2D p, Value val) {
            this.p = p;
            this.val = val;

        }
    }

    // constructor
    public KdTreeST() {
    }


    // is the symbol table empty?
    public boolean isEmpty() {
        return root == null;
    }

    // number of points
    public int size() {
        return size(root);
    }

    // private class of size
    private int size(Node n) {
        if (n == null) return 0;

        else return n.size;
    }

    // associate the value val with point p
    public void put(Point2D p, Value val) {
        if (p == null || val == null) throw new
                IllegalArgumentException("Point and value cannot be null.");

        root = put(p, val, root, 0);

        // initiate the root rectangle
        root.rect = rootRect;
    }

    // private method to help identify if we are in odd/even levels
    private boolean compareX(int level) {
        return (level % 2 == 0);  // compare the x axis
        // else compare y axis
    }

    // implement root method; level indicates the level on the tree to
    // differentiate between even and odd levels
    private Node put(Point2D p, Value val, Node n, int level) {
        if (n == null) {
            return new Node(p, val);
        }

        // if point is already in the tree
        if (n.p.equals(p)) {
            n.val = val;
        }

        // check each level

        // check the x coordinate
        else if (compareX(level)) {
            if (p.x() < n.p.x()) {
                n.lb = put(p, val, n.lb, ++level);
                n.lb.rect = new RectHV(n.rect.xmin(),
                                       n.rect.ymin(), n.p.x(), n.rect.ymax());
            }
            else if (p.x() >= n.p.x()) {
                n.rt = put(p, val, n.rt, ++level);
                n.rt.rect = new RectHV(n.p.x(), n.rect.ymin(),
                                       n.rect.xmax(), n.rect.ymax());
            }
        }

        // check the y coordinate
        else {
            if (p.y() < n.p.y()) {
                n.lb = put(p, val, n.lb, ++level);
                n.lb.rect = new RectHV(n.rect.xmin(),
                                       n.rect.ymin(), n.rect.xmax(), n.p.y());
            }
            else if (p.y() >= n.p.y()) {
                n.rt = put(p, val, n.rt, ++level);
                n.rt.rect = new RectHV(n.rect.xmin(),
                                       n.p.y(), n.rect.xmax(), n.rect.ymax());
            }
        }

        // return size of tree
        n.size = size(n.lb) + size(n.rt) + 1;

        return n;
    }

    // value associated with point p
    public Value get(Point2D p) {
        return get(root, p, 0);
    }

    // private implementation of the get method
    private Value get(Node n, Point2D p, int level) {
        if (p == null) throw new IllegalArgumentException();
        if (n == null) return null;

        if (n.p.equals(p))
            return n.val;

        if (compareX(level)) {
            if (p.x() < n.p.x()) {
                return get(n.lb, p, ++level);
            }
            else if (p.x() >= n.p.x()) {
                return get(n.rt, p, ++level);
            }
        }
        else {
            if (p.y() < n.p.y()) {
                return get(n.lb, p, ++level);
            }
            else if (p.y() >= n.p.y()) {
                return get(n.rt, p, ++level);
            }
        }
        return null;
    }

    // does the symbol table contain point p?
    public boolean contains(Point2D p) {
        return get(p) != null;
    }

    // all points in the symbol table
    public Iterable<Point2D> points() {
        // citation from classnotes
        // https://www.cs.princeton.edu/courses/archive/fall20/cos226/
        // lectures/32BinarySearchTrees.pdf
        // use a queue as an iterable to return for this method

        Queue<Point2D> qOut = new Queue<Point2D>();
        for (Node n : levelOrder())
            qOut.enqueue(n.p);

        return qOut;
    }

    // implement sorting by level order
    private Iterable<Node> levelOrder() {
        Queue<Node> qIn = new Queue<Node>();
        Queue<Node> qOut = new Queue<Node>();

        qIn.enqueue(root);

        while (!qIn.isEmpty()) {
            Node x = qIn.dequeue();
            if (x != null) {
                qOut.enqueue(x);
                qIn.enqueue(x.lb);
                qIn.enqueue(x.rt);
            }
        }
        return qOut;
    }


    // all points that are inside the rectangle (or on the boundary)
    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) throw new
                IllegalArgumentException("Rectangle cannot be null.");

        Queue<Point2D> qOut = new Queue<>();
        range(rect, root, qOut);

        return qOut;
    }

    // implement range method
    private void range(RectHV rect, Node node, Queue<Point2D> qOut) {
        if (node == null) return;
        if (!node.rect.intersects(rect)) return;

        if (rect.contains(node.p)) {
            qOut.enqueue(node.p);
        }

        range(rect, node.lb, qOut);
        range(rect, node.rt, qOut);
    }

    // a nearest neighbor of point p; return null if the symbol table is empty
    public Point2D nearest(Point2D p) {
        if (p == null) throw new
                IllegalArgumentException("Point cannot be null.");

        if (isEmpty()) return null;

        Point2D nearestPoint = nearest(root, p, root.p, 0);

        return nearestPoint;
    }

    // check if bounding box of node n is closer than the
    // best point minPoint so far to point P
    private boolean checkBoundingBox(Node n, Point2D p, Point2D minPoint) {
        return p.distanceSquaredTo(minPoint) > n.rect.distanceSquaredTo(p);
    }

    // implement nearest neighbor method using pruining and recursion
    private Point2D nearest(Node n, Point2D p, Point2D minPoint, int level) {
        if (p == null) throw new
                IllegalArgumentException("Point cannot be null");

        if (n == null) {
            return minPoint;
        }

        // return if bounding box not closer than best so far
        if (!checkBoundingBox(n, p, minPoint)) {
            return minPoint;
        }

        // check if the point is closer than the best so far
        if (minPoint.distanceSquaredTo(p) > n.p.distanceSquaredTo(p)) {
            // update the new minPoint
            minPoint = n.p;
        }

        // if point left or below then go left then right
        // use the level to check if we're supposed to compare x or y
        if (compareX(level)) {
            if (p.x() < n.p.x()) {
                level++;
                // check to see if node should be checked
                if (n.lb != null && checkBoundingBox(n.lb, p, minPoint))
                    minPoint = nearest(n.lb, p, minPoint, level);
                if (n.rt != null && checkBoundingBox(n.rt, p, minPoint))
                    minPoint = nearest(n.rt, p, minPoint, level);
            }
            else {
                level++;
                if (n.rt != null && checkBoundingBox(n.rt, p, minPoint))
                    minPoint = nearest(n.rt, p, minPoint, level);
                if (n.lb != null && checkBoundingBox(n.lb, p, minPoint))
                    minPoint = nearest(n.lb, p, minPoint, level);
            }
        }
        else {
            if (p.y() < n.p.y()) {
                level++;
                if (n.lb != null && checkBoundingBox(n.lb, p, minPoint))
                    minPoint = nearest(n.lb, p, minPoint, level);
                if (n.rt != null && p.distanceSquaredTo(minPoint) > n.rt.rect
                        .distanceSquaredTo(p))
                    minPoint = nearest(n.rt, p, minPoint, level);
            }
            else {
                level++;
                if (n.rt != null && checkBoundingBox(n.rt, p, minPoint))
                    minPoint = nearest(n.rt, p, minPoint, level);
                if (n.lb != null && checkBoundingBox(n.lb, p, minPoint))
                    minPoint = nearest(n.lb, p, minPoint, level);
            }
        }
        return minPoint;
    }


    // unit testing
    public static void main(String[] args) {
        KdTreeST<String> testPoint = new KdTreeST<String>();

        // Point 1
        Point2D p1 = new Point2D(0.7, 0.2);
        Point2D p2 = new Point2D(0.5, 0.4);
        Point2D p3 = new Point2D(0.2, 0.3);
        Point2D p4 = new Point2D(0.4, 0.7);
        Point2D p5 = new Point2D(0.9, 0.6);

        String s1 = "A";
        String s2 = "B";
        String s3 = "C";
        String s4 = "D";
        String s5 = "E";

        // test put by inserting points
        testPoint.put(p1, s1);
        testPoint.put(p2, s2);
        testPoint.put(p3, s3);
        testPoint.put(p4, s4);
        testPoint.put(p5, s5);

        // test put and points
        StdOut.println(testPoint.points());

        // test nearest for point search
        Point2D search = new Point2D(0.88, 0.06);
        StdOut.println(testPoint.nearest(search));

        // test is empty
        StdOut.println("Empty? " + testPoint.isEmpty());

        // test get
        StdOut.println("Value of p1: " + testPoint.get(p2));

        // test contains
        StdOut.println("Contains p2? " + testPoint.contains(search));

        // test size
        for (Point2D p : testPoint.points())
            StdOut.println(p);
        StdOut.println("Size: " + testPoint.size());

        // test range
        RectHV rect = new RectHV(0, 0, 5, 5);
        for (Point2D p : testPoint.range((rect)))
            StdOut.println(p);

        String filename = args[0];
        In in = new In(filename);

        KdTreeST<Integer> kdtree = new KdTreeST<Integer>();
        // create kd-tree
        for (int i = 0; !in.isEmpty(); i++) {
            double x = in.readDouble();
            double y = in.readDouble();

            Point2D p = new Point2D(x, y);
            kdtree.put(p, i);
        }

        Stopwatch st = new Stopwatch();
        for (int i = 0; i < 100000; i++) {
            // test nearest for point search
            Point2D searchNew = new Point2D(StdRandom.uniform(),
                                            StdRandom.uniform());
            kdtree.nearest(searchNew);
        }

        StdOut.println(st.elapsedTime());
    }

}

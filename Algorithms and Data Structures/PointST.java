/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P02
 *
 *  Partner Name:    Tomisin Fasawe
 *  Partner NetID:   ofasawe
 *  Partner Precept: P04
 *
 *  Description:  A mutable data type that uses red-black
 *  BST to represent a symbol table whose keys are 2D points.  Throw illegal
 *  argument exception if any input is null
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.RedBlackBST;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.Stopwatch;

public class PointST<Value> {

    // instance variables
    private final RedBlackBST<Point2D, Value> points; // BST to represent
    // symbol table of points


    // construct an empty symbol table of points
    public PointST() {
        points = new RedBlackBST<Point2D, Value>();
    }

    // is the symbol table empty?
    public boolean isEmpty() {
        return points.isEmpty();
    }

    // number of points
    public int size() {
        return points.size();
    }

    // associate the value val with point p
    public void put(Point2D p, Value val) {
        if (p == null || val == null) throw new
                IllegalArgumentException(" Point and value cannot be null.");

        points.put(p, val);
    }

    // value associated with point p
    public Value get(Point2D p) {
        if (p == null) throw new
                IllegalArgumentException("Point cannot be null.");

        return points.get(p);

    }

    // does the symbol table contain point p?
    public boolean contains(Point2D p) {
        if (p == null) throw new
                IllegalArgumentException("Point cannot be null.");

        return points.contains(p);

    }

    // all points in the symbol table
    public Iterable<Point2D> points() {
        return points.keys();

    }

    // all points that are inside the rectangle (or on the boundary)
    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) throw new
                IllegalArgumentException("Rectangle cannot be null.");

        // start queue to store points inside rectangle
        Queue<Point2D> insideRectangle = new Queue<Point2D>();

        for (Point2D p : points.keys()) {
            if (rect.contains(p)) {
                insideRectangle.enqueue(p);
            }
        }

        return insideRectangle;
    }

    // a nearest neighbor of point p; null if the symbol table is empty
    public Point2D nearest(Point2D p) {
        if (p == null) throw new IllegalArgumentException();

        if (points == null) return null;

        // set minimum value for distance; initially set to infinity
        double min = Double.POSITIVE_INFINITY;

        // minimum point
        Point2D minPoint = null;

        for (Point2D p0 : points.keys()) {
            double dist = p0.distanceSquaredTo(p);

            if (dist <= min) {
                min = dist;
                minPoint = p0;
            }
        }

        return minPoint;
    }

    // unit testing (required)
    public static void main(String[] args) {

        // test symbol table
        PointST<Double> testPoint = new PointST<Double>();

        // create points
        Point2D p1 = new Point2D(1.0, 2.0);
        Point2D p2 = new Point2D(3.0, 1.0);
        Point2D p3 = new Point2D(-1.0, 22.0);

        // create associated values
        double val1 = 1.0;
        double val2 = 2.0;
        double val3 = 3.0;

        // check contains
        Point2D p4 = new Point2D(4.0, 1.0);
        StdOut.println("Contains p4? " + testPoint.contains(p4));

        // check put
        testPoint.put(p1, val1);
        testPoint.put(p2, val2);
        testPoint.put(p3, val3);
        testPoint.put(p4, val3);

        // check is empty
        StdOut.println(testPoint.isEmpty());

        // check contains after putting points in ST
        StdOut.println("Contains p4? " + testPoint.contains(p4));

        // check get
        StdOut.println("Value of p3: " + testPoint.get(p3));

        // check points
        for (Point2D i : testPoint.points()) {
            StdOut.println("Value of point" + i + ": " + testPoint.get(i));
        }

        // check range
        RectHV testRectangle = new RectHV(0.0, 0.0, 2.0, 2.0);
        for (Point2D j : testPoint.range(testRectangle)) {
            StdOut.println(j);
        }

        // check size
        StdOut.println("Size of ST: " + testPoint.size());

        // check nearest
        StdOut.println(testPoint.nearest(p1));


        String filename = args[0];
        In in = new In(filename);

        PointST<Integer> kdtree = new PointST<Integer>();
        // create kd-tree
        for (int i = 0; !in.isEmpty(); i++) {
            double x = in.readDouble();
            double y = in.readDouble();

            Point2D p = new Point2D(x, y);
            kdtree.put(p, i);
        }

        Stopwatch st = new Stopwatch();
        for (int i = 0; i < 50; i++) {
            // test nearest for point search
            Point2D searchNew = new Point2D(StdRandom.uniform(),
                                            StdRandom.uniform());
            kdtree.nearest(searchNew);
        }

        StdOut.println(st.elapsedTime());
    }
}

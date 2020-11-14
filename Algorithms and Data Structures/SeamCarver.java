/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P08
 *
 *  Partner Name:    Tomisin Fasawe
 *  Partner NetID:   ofasawe
 *  Partner Precept: P02
 *
 *  Description:  Implement Seam Carver, a content aware image resizing
 *  technique, where the image is reduced in size by one pixel of height
 *  or width at a time.  The object returns the energy of a pixel,
 *  the vertical seam, the horizontal seam, and removes each.
 **************************************************************************** */

import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.Picture;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stopwatch;

public class SeamCarver {

    // Picture global variable
    private Picture picture;

    // create a seam carver object based on the given picture
    public SeamCarver(Picture picture) {

        // check argument
        if (picture == null) throw new
                IllegalArgumentException("Picture cannot be null");

        // create defensive copy
        Picture pic = new Picture(picture.width(), picture.height());
        for (int c = 0; c < pic.width(); c++) {
            for (int r = 0; r < pic.height(); r++) {
                pic.setRGB(c, r, picture.getRGB(c, r));
            }
        }

        // create a defensive copy
        this.picture = pic;
    }

    // transpose the picture and return a new picture
    private Picture transposePicture(Picture p) {
        Picture newPicture = new Picture(p.height(), p.width());

        for (int c = 0; c < p.width(); c++) {
            for (int r = 0; r < p.height(); r++) {
                newPicture.setRGB(r, c, p.getRGB(c, r));
            }
        }

        return newPicture;

    }


    // get the gradient of two pixels (x1, y1) and (x2, y2)
    private double getGradient(int x1, int y1, int x2, int y2) {

        // get color of first pixel
        int rgb1 = picture.getRGB(x1, y1);
        int r1 = (rgb1 >> 16) & 0xFF;
        int g1 = (rgb1 >> 8) & 0xFF;
        int b1 = (rgb1) & 0xFF;

        // get color of second pixel
        int rgb2 = picture.getRGB(x2, y2);
        int r2 = (rgb2 >> 16) & 0xFF;
        int g2 = (rgb2 >> 8) & 0xFF;
        int b2 = (rgb2) & 0xFF;

        return Math.pow(r1 - r2, 2) +
                Math.pow(g1 - g2, 2) +
                Math.pow(b1 - b2, 2);
    }

    // get the gradient of two pixels given a color array
    private double getGradientColor(int x1, int y1, int x2, int y2,
                                    int[] colors) {
        int rgb1 = colors[to1D(x1, y1)];
        int r1 = (rgb1 >> 16) & 0xFF;
        int g1 = (rgb1 >> 8) & 0xFF;
        int b1 = (rgb1) & 0xFF;

        int rgb2 = colors[to1D(x2, y2)];
        int r2 = (rgb2 >> 16) & 0xFF;
        int g2 = (rgb2 >> 8) & 0xFF;
        int b2 = (rgb2) & 0xFF;

        return Math.pow(r1 - r2, 2) +
                Math.pow(g1 - g2, 2) +
                Math.pow(b1 - b2, 2);
    }

    // compute the energy of pixel (x, y) given a color array
    private double energyFromColor(int x, int y, int[] colors) {
        // initialize color array
        // get the x Gradient
        double gradX;
        // x is not on border left or border right
        if (width() == 1) {
            gradX = 0.0;
        }
        else if (x > 0 && x < width() - 1) {
            gradX = getGradientColor(x - 1, y, x + 1, y, colors);
        }
        // x is on border left
        else if (x == 0) {
            gradX = getGradientColor(width() - 1, y, x + 1, y, colors);
        }
        // x is on border right
        else {
            gradX = getGradientColor(x - 1, y, 0, y, colors);
        }

        // get the gradY component
        double gradY;
        // check corner cases
        if (height() == 1) {
            gradY = 0.0;
        }
        else if (y != 0 && y != height() - 1) {
            gradY = getGradientColor(x, y - 1, x, y + 1, colors);
        }
        else if (y == 0) {
            gradY = getGradientColor(x, height() - 1, x, y + 1, colors);
        }
        else {
            gradY = getGradientColor(x, y - 1, x, 0, colors);
        }

        return Math.sqrt(gradX + gradY);

    }


    // relax the edge in dijkstra algorithm
    private void relaxEdge(int neighbor, int v, IndexMinPQ<Double> pq,
                           double[] distTo, int[] edgeTo, double[] en) {
        double nEnergy; // energy of the neighbor pixel

        if (neighbor != width() * height()) {
            nEnergy = en[neighbor];
        }
        else nEnergy = 0.0;

        // distance to the neighbor
        double distToV = distTo[v] + nEnergy;

        // update if necessary
        if (distToV < distTo[neighbor]) {
            // update
            distTo[neighbor] = distToV;
            edgeTo[neighbor] = v;

            // update PQ
            if (pq.contains(neighbor))
                pq.decreaseKey(neighbor, distTo[neighbor]);
            else pq.insert(neighbor, distTo[neighbor]);
        }
    }

    // maintain storage in 1D arrays for efficiency
    // change 2D position of pixel to 1D representation in array
    private int to1D(int x, int y) {
        return (y) * width() + x;
    }

    // translate 1D rep to 2D position
    private int[] to2D(int n) {
        int[] point = new int[2];

        int row = n / width();
        int col = n % width();

        point[0] = col;
        point[1] = row;
        return point;
    }

    // validate the seam
    private void validateSeam(int[] seam, int w, int h) {
        if (h == 1) throw new IllegalArgumentException();
        if (seam == null) throw new IllegalArgumentException();

        if (seam.length != w) throw new IllegalArgumentException();

        if (seam.length == 1) {
            if (seam[0] < 0 || seam[0] >= h)
                throw new IllegalArgumentException();
        }

        for (int i = 0; i < seam.length - 1; i++) {
            if (seam[i] >= h || seam[i + 1] >= h ||
                    seam[i] < 0 || seam[i + 1] < 0) {
                throw new IllegalArgumentException();
            }
            if (Math.abs(seam[i + 1] - seam[i]) > 1)
                throw new IllegalArgumentException();
        }
    }


    // current picture
    public Picture picture() {
        return picture;
    }

    // width of current picture
    public int width() {
        return picture.width();
    }

    // height of current picture
    public int height() {
        return picture.height();
    }

    // energy of pixel at column x and y
    // use dual gradient function
    public double energy(int x, int y) {

        // get the x Gradient
        double gradX;

        // check corner cases
        if (width() == 1) {
            gradX = 0.0;
        }
        else if (x > 0 && x < width() - 1) {

            gradX = getGradient(x - 1, y, x + 1, y);
        }
        // x is on border left
        else if (x == 0) {
            gradX = getGradient(width() - 1, y, x + 1, y);
        }
        // x is on border right
        else {
            gradX = getGradient(x - 1, y, 0, y);
        }

        // get the gradY component
        double gradY;
        // check corner cases
        if (height() == 1) {
            gradY = 0.0;
        }
        else if (y > 0 && y < height() - 1) {

            gradY = getGradient(x, y - 1, x, y + 1);
        }
        else if (y == 0) {
            gradY = getGradient(x, height() - 1, x, y + 1);
        }
        else {
            gradY = getGradient(x, y - 1, x, 0);
        }

        return Math.sqrt(gradX + gradY);
    }


    // returns Horizontal seam of lowest energy
    public int[] findHorizontalSeam() {

        Picture originalPicture = picture;

        picture = transposePicture(picture);

        int[] finalSeam = findVerticalSeam();

        picture = originalPicture;

        return finalSeam;

    }


    // find the vertical seam of picture
    public int[] findVerticalSeam() {
        // implement digraph implicitly
        // use Dijkstra algorithm for shortest path with a virtual final node

        double[] distTo = new double[width() * height() + 1]; // distTo[v] =
        // distance of shortest path from the top to bottom of picture
        int[] edgeTo = new int[width() * height() + 1]; // edgeTo[v] =
        // last edge on shortest top to bottom path
        IndexMinPQ<Double> pq = new IndexMinPQ<Double>(width() * height() + 1);
        // priority queue of vertices

        // compute local color array
        int[] colors = new int[width() * height()];
        for (int r = 0; r < height(); r++) {
            for (int c = 0; c < width(); c++) {
                colors[to1D(c, r)] = picture.getRGB(c, r);
            }
        }

        // compute local energy array
        double[] en = new double[width() * height()];
        for (int r = 0; r < height(); r++) {
            for (int c = 0; c < width(); c++) {
                // use computed color array to get energies
                en[to1D(c, r)] = energyFromColor(c, r, colors);
            }
        }

        // set all distTo to infinity
        for (int i = 0; i < distTo.length; i++)
            distTo[i] = Double.POSITIVE_INFINITY;

        // add entire first row to distTo and PQ
        for (int i = 0; i < width(); i++) {
            distTo[to1D(i, 0)] = en[to1D(i, 0)];
            pq.insert(to1D(i, 0), distTo[to1D(i, 0)]);
        }

        // define virtual node connecting all bottom pixels
        int virtualNode = width() * height();

        // dijkstra
        while (!pq.isEmpty()) {

            // remove the shortest distance
            int v = pq.delMin();

            // obtain the 2D indices of pixel
            int col = to2D(v)[0];
            int row = to2D(v)[1];

            // bottom row
            if (row == height() - 1) {
                relaxEdge(virtualNode, v, pq, distTo, edgeTo, en);
            }
            // left edge
            else if (col == 0 && width() > 1) {
                relaxEdge(to1D(col, row + 1), v, pq, distTo, edgeTo, en);
                relaxEdge(to1D(col + 1, row + 1), v, pq, distTo, edgeTo, en);
            }
            // if the width is 1
            else if (col == 0) {
                relaxEdge(to1D(col, row + 1), v, pq, distTo, edgeTo, en);
            }
            // right edge
            else if (col == width() - 1 && width() > 1) {
                relaxEdge(to1D(col, row + 1), v, pq, distTo, edgeTo, en);
                relaxEdge(to1D(col - 1, row + 1), v, pq, distTo, edgeTo, en);
            }
            // middle
            else {
                relaxEdge(to1D(col, row + 1), v, pq, distTo, edgeTo, en);
                relaxEdge(to1D(col - 1, row + 1), v, pq, distTo, edgeTo, en);
                relaxEdge(to1D(col + 1, row + 1), v, pq, distTo, edgeTo, en);
            }

            // break dijkstra if reach the bottom
            if (row == height() - 1)
                break;

        }

        // find final seam by checking edgeTo
        int[] finalSeam = new int[height()]; // final seam of column values

        // trace back from virtual node to pixels leading to it
        int leadingEdge = edgeTo[height() * width()];

        // define the final Vertical Seam
        finalSeam[height() - 1] = to2D(leadingEdge)[0];

        // trace back using edgeTo
        for (int i = 1; i < height(); i++) {
            leadingEdge = edgeTo[leadingEdge];
            finalSeam[height() - i - 1] = to2D(leadingEdge)[0];
        }

        return finalSeam;
    }

    // remove the Vertical Seam
    public void removeVerticalSeam(int[] seam) {
        validateSeam(seam, height(), width());

        // create new Picture
        Picture newPicture = new Picture(width() - 1, height());

        // traverse each row to update
        for (int c = 0; c < width() - 1; c++) {
            for (int r = 0; r < height(); r++) {
                if (c < seam[r]) newPicture.setRGB(c, r, picture.getRGB(c, r));
                else newPicture.setRGB(c, r, picture.getRGB(c + 1, r));
            }
        }

        picture = newPicture;
    }

    // remove horizontal seam
    public void removeHorizontalSeam(int[] seam) {
        validateSeam(seam, width(), height());
        // create new Picture
        Picture newPicture = new Picture(width(), height() - 1);

        // traverse each row to update
        for (int c = 0; c < width(); c++) {
            for (int r = 0; r < height() - 1; r++) {
                if (r < seam[c]) newPicture.setRGB(c, r, picture.getRGB(c, r));
                else newPicture.setRGB(c, r, picture.getRGB(c, r + 1));
            }
        }
        picture = newPicture;
    }


    // unit testing
    public static void main(String[] args) {
        Picture picture = new Picture(args[0]);

        Stopwatch s = new Stopwatch();

        SeamCarver carver = new SeamCarver(picture);

        // test energy
        StdOut.println(carver.picture());
        StdOut.println(carver.height());
        StdOut.println(carver.width());
        StdOut.println(carver.energy(2, 6));

        // test find Vertical Seam
        int[] verticalSeam = carver.findVerticalSeam();
        // test find Horizontal Seam
        int[] horizontalSeam = carver.findHorizontalSeam();

        // remove both
        carver.removeVerticalSeam(verticalSeam);
        carver.removeHorizontalSeam(horizontalSeam);

        double time = s.elapsedTime();

        StdOut.println(time);

    }
}

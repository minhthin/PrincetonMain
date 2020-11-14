/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P09
 *
 *  Description:  Create a model of nxn grid of sites that can be open or
 *  blocked.  Determine if the system percolates. Output number of open sites,
 *  if the site if full/open.
 **************************************************************************** */

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;


public class Percolation {

    // declare instance variables
    private final int gridSize; // size of grid
    private boolean[][] grid; // grid representation of sites
    private int gridOpen; // number of open sites

    private final WeightedQuickUnionUF sites; // weighted quick union grid sites
    private final WeightedQuickUnionUF sitesNoBackwash; // extra credit noBackwash

    private final int virtualTop; // virtual top index
    private final int virtualBottom; // virtual bottom index


    // creates n-by-n grid, with all sites initially blocked
    public Percolation(int n) {
        // illegal argument
        if (n <= 0) {
            throw new IllegalArgumentException("Grid must have size at least "
                                                       + "one.");
        }

        // construct grid of size n; initialize to blocked
        gridSize = n;
        grid = new boolean[n][n];
        gridOpen = 0; // all sites initially blocked

        // let n = index of top virtual node
        // let n + 1 = index of bottom virtual node
        virtualTop = n * n;
        virtualBottom = n * n + 1;

        // include 2 virtual nodes
        sites = new WeightedQuickUnionUF(n * n + 2);

        // include another union to prevent backwash
        sitesNoBackwash = new WeightedQuickUnionUF(n * n + 2);

        // ensure that you start with all closed
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                grid[i][j] = false;
            }
        }

    }

    // translate indexes in nxn grid to an index in the site Weighted QU array
    private int gridToSite(int row, int col) {
        return row * gridSize + col;
    }

    // translate indexes on top of location to number,
    // assuming not in first row
    private void topSite(int row, int col) {
        if (row > 0) {
            if (grid[row - 1][col]) {
                sites.union(gridToSite(row, col), gridToSite(row - 1, col));
                sitesNoBackwash.union(gridToSite(row, col),
                                      gridToSite(row - 1, col));
            }
        }

    }

    // left of site, assuming not on left most edge
    private void leftSite(int row, int col) {
        if (col > 0) {
            if (grid[row][col - 1]) {
                sites.union(gridToSite(row, col), gridToSite(row, col - 1));
                sitesNoBackwash.union(gridToSite(row, col),
                                      gridToSite(row, col - 1));
            }
        }
    }

    // right of site, assuming not right most edge
    private void rightSite(int row, int col) {
        if (col < gridSize - 1) {
            if (grid[row][col + 1]) {
                // unite the sites for both Weighted QU arrays
                sites.union(gridToSite(row, col), gridToSite(row, col + 1));
                sitesNoBackwash.union(gridToSite(row, col),
                                      gridToSite(row, col + 1));
            }
        }
    }

    // bottom of site, assuming not the lowest row
    private void bottomSite(int row, int col) {
        if (row < gridSize - 1) {
            if (grid[row + 1][col]) {
                sites.union(gridToSite(row, col), gridToSite(row + 1, col));
                sitesNoBackwash.union(gridToSite(row, col),
                                      gridToSite(row + 1, col));
            }
        }
    }

    // check if rows and columns are in defined bounds of problem
    private void outOfBounds(int row, int col) {
        if (row < 0 || row > (gridSize - 1) || col < 0 || col > (gridSize - 1))
        {
            throw new IllegalArgumentException("The given row and columns are "
                                                       + "out of bounds.");
        }

    }

    // opens the site (row, col) if it is not open already
    public void open(int row, int col) {

        outOfBounds(row, col);

        // if the site is closed
        if (!grid[row][col]) {

            // open site
            grid[row][col] = true;

            // update number of open sites
            gridOpen++;

            // connect open sites

            // right
            rightSite(row, col);

            // left
            leftSite(row, col);

            // top
            topSite(row, col);

            // if the node is in the top row, union with virtual top
            if (row == 0) {
                sites.union(gridToSite(row, col), virtualTop);
                sitesNoBackwash.union(gridToSite(row, col), virtualTop);
            }

            // bottom
            bottomSite(row, col);

            // if the node is in the bottom row, connect with virtual bottom
            if (row == gridSize - 1) {
                sites.union(gridToSite(row, col), virtualBottom);

                // we do not connect the virtual bottom node to the NoBackwash
                // union array because that allows multiple bottom nodes
                // to be connected to the virtual bottom node, thus causing
                // backwash.

                // we keep the extra "sites" array because it allows us to check
                // if the system percolates.
            }
        }

    }


    // is the site (row, col) open?
    public boolean isOpen(int row, int col) {
        outOfBounds(row, col);

        return grid[row][col];

    }

    // is the site (row, col) full?
    public boolean isFull(int row, int col) {
        outOfBounds(row, col);

        // is the site open? if so, check to see if the node has the same root
        // as the virtual top node (definition of full)
        if (isOpen(row, col)) {

            // we have to use the backwash array because the node can be
            // connected to the bottom node but is not directly connected to
            // a node in the top row in reality
            return (sitesNoBackwash.find(gridToSite(row, col)) ==
                    sitesNoBackwash.find(gridSize * gridSize));
        }
        else {
            return false;
        }
    }


    // returns the number of open sites
    public int numberOfOpenSites() {
        return gridOpen;
    }

    // does the system percolate?
    public boolean percolates() {

        // even with backwash, the system still percolates with the
        // "sites" array
        return (sites.find(gridSize * gridSize + 1) ==
                sites.find(gridSize * gridSize));
    }

    // unit testing (required)
    public static void main(String[] args) {
        // testing

        // first read in the grid size integer
        int gridSize = StdIn.readInt();

        // create Percolation
        Percolation p = new Percolation(gridSize);

        // keep track of testing
        int rowCount = 1;

        while (!StdIn.isEmpty()) {
            // read in the two integers
            int num1 = StdIn.readInt();
            int num2 = StdIn.readInt();

            // is the site open?
            StdOut.println("Site Open: " + p.isOpen(num1, num2));

            // open the row, col
            p.open(num1, num2);

            // is the site full?
            StdOut.println("Site Full: " + p.isFull(num1, num2));

            // what is the number of open sites?
            StdOut.println("Open Sites: " + p.numberOfOpenSites());

            // does the system percolate?
            StdOut.println("System Percolates " + p.percolates());

            // update row count
            rowCount++;
            StdOut.println(rowCount);
        }


    }
}

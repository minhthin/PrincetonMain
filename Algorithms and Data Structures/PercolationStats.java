/* *****************************************************************************
 *  Name:    Minh-Thi Nguyen
 *  NetID:   minhthin
 *  Precept: P09
 *
 *  Description:  Perform a series of trials for a general nxn grid of sites
 *  and determine the percolation threshold by calculating the mean,
 *  standard deviation, low and high endpoints of confidence intervals.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.Stopwatch;

public class PercolationStats {

    private final int totalTrials; // total number of trials
    private final double[] results; // results from trials for percolation
    // threshold
    private final double confidenceConstant; // 1.96 used to compute the
    // confidence ends

    // perform independent trials on an n-by-n grid
    public PercolationStats(int n, int trials) {
        // exception
        if (n <= 0 || trials <= 0) {
            throw new IllegalArgumentException("n and trials are out "
                                                       + "of bounds.");
        }

        totalTrials = trials;
        confidenceConstant = 1.96;

        // create an array to store results
        results = new double[totalTrials];

        // for each trial, run Percolation
        for (int i = 0; i < trials; i++) {
            Percolation test = new Percolation(n);

            // choose random site while the grid does not percolate
            while (!test.percolates()) {

                // random row and column
                int randRow = StdRandom.uniform(n);
                int randCol = StdRandom.uniform(n);

                // open
                test.open(randRow, randCol);
            }

            // number of open sites
            int openSites = test.numberOfOpenSites();

            // percolation threshold
            results[i] = (double) openSites / (n * n); // ratio of open sites
        }
    }

    // sample mean of percolation threshold
    public double mean() {
        return StdStats.mean(results);
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return StdStats.stddev(results);
    }

    // low endpoint of 95% confidence interval
    public double confidenceLow() {
        return mean() - confidenceConstant * stddev() / Math.sqrt(totalTrials);
    }

    // high endpoint of 95% confidence interval
    public double confidenceHigh() {
        return mean() + confidenceConstant * stddev() / Math.sqrt(totalTrials);
    }

    // test client (see below)
    public static void main(String[] args) {
        // test client takes two command line arguments n and T
        // prints the relevant statistics for T computation experiments
        // on an nxn grid

        // read from input to get n and trials
        int n = Integer.parseInt(args[0]);
        int trials = Integer.parseInt(args[1]);

        // start StopWatch
        Stopwatch st = new Stopwatch();

        // set up PercolationStats
        PercolationStats test = new PercolationStats(n, trials);

        // compute mean and print
        double meanValue = test.mean();
        String meanString = "mean()";
        String meanOutput = String.format("%-16s" + "= " + "%07.6f",
                                          meanString, meanValue);
        StdOut.println(meanOutput);

        // compute stddev and print
        double stddevValue = test.stddev();
        String stddevString = "stddev()";
        String stddevOutput = String.format("%-16s" + "= " + "%07.6f",
                                            stddevString, stddevValue);
        StdOut.println(stddevOutput);

        // compute low Confidence and print
        double loC = test.confidenceLow();
        String loCString = "confidenceLow()";
        String loCOutput = String.format("%-16s" + "= " + "%07.6f",
                                         loCString, loC);
        StdOut.println(loCOutput);

        // compute high Confidence and print
        double hiC = test.confidenceHigh();
        String hiCString = "confidenceHigh()";
        String hiCOutput = String.format("%-16s" + "= " + "%07.6f",
                                         hiCString, hiC);
        StdOut.println(hiCOutput);

        // stop time and print
        double totalTime = st.elapsedTime();
        String timeString = "elapsed time";
        String timeOutput = String.format("%-16s" + "= " + "%4.3f",
                                          timeString, totalTime);
        StdOut.println(timeOutput);

    }
}


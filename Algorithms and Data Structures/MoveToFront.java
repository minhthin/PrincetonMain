/* *****************************************************************************
 *  Name:    Tomisin Fasawe
 *  NetID:   ofasawe
 *  Precept: P04
 *
 *  Partner Name: Minh-thi Nguyen
 *  Partner NetID: minhthin
 *  Partner Precept: P08
 *
 *  Description:  The main idea of move-to-front encoding is to maintain an
 *                ordered sequence of the characters in the alphabet by
 *                repeatedly reading a character from the input message;
 *                printing the position in the sequence in which that character
 *                appears; and moving that character to the front of the
 *                sequence.
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

public class MoveToFront {

    // total number of characters
    private static final int R = 256;

    // apply move-to-front encoding, reading from stdin and writing to stdout
    public static void encode() {

        char[] word = new char[R];
        for (char c = 0; c < R; c++) word[c] = c;

        // read while input stream is NOT empty
        while (!BinaryStdIn.isEmpty()) {
            char c = BinaryStdIn.readChar(8);
            // BinaryStdOut.write(c);
            // another loop to find the index of the char that we are looking for
            for (char i = 0; i < R; i++) {
                if (c == word[i]) {
                    BinaryStdOut.write(i);
                    // move characters to the right place
                    // char first = word[0];
                    for (int j = i; j >= 1; j--)
                        word[j] = word[j - 1];
                    word[0] = c;
                    break;
                }
            }
        }
        BinaryStdOut.flush();
    }


    // apply move-to-front decoding, reading from stdin and writing to stdout
    public static void decode() {

        char[] word = new char[R];
        // initialising ordered sequence
        for (char c = 0; c < R; c++)
            word[c] = c;

        // read while input stream is NOT empty
        while (!BinaryStdIn.isEmpty()) {
            int index = BinaryStdIn.readInt(8);
            char c = word[index];
            BinaryStdOut.write(c);
            // BinaryStdOut.write(c);
            // another loop to find the index of the char that we are looking for
            for (int i = index; i >= 1; i--) {
                word[i] = word[i - 1];
            }
            word[0] = c;
        }
        BinaryStdOut.flush();
    }


    // if args[0] is "-", apply move-to-front encoding
    // if args[0] is "+", apply move-to-front decoding
    public static void main(String[] args) {

        if (args[0].equals("-")) {
            encode();
        }
        if (args[0].equals("+"))
            decode();
    }

}

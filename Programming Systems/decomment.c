/*------------------------------------------------------------*/
/* decomment.c */
/* Author: Minh-Thi */
/*------------------------------------------------------------*/

/*
Read in characters from standard input stream 
Writes characters to standard output stream without comment,
replace comment with space. More info on COS217 assignment page.
Writes message to standard error stream if unterminated comment.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum Statetype {NO_UNTERMINATING_COMMENT, NEWLINE, NEWLINE_LITERAL, LITERAL, LITERAL_IGNORE, COMMENT_CHAR1, COMMENT_CHAR2, COMMENT_CHAR3, NEWLINE_COMMENT};
	
/*------------------------------------------------------------*/

/* Implements the NO_UNTERMINATING_COMMENT state of the DFA.
c is the current DFA character. Write c to stdout if c is not 
a slash, as specified by the DFA.  Return the next state. 
*/


enum Statetype handleNoUnterminatingCommentState(int c)
{
	enum Statetype state;
	
	if (c == 39 || c == 34)
	{
		putchar(c);
		state = LITERAL;
	}
	else if (c == 47) {
		state = COMMENT_CHAR1;
	}
	else if (c == 10)
	{
		putchar(c);
		state = NEWLINE;
	}
	else
	{
		putchar(c);
		state = NO_UNTERMINATING_COMMENT;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the NEWLINE state of the DFA. 
c is the current DFA character. 
Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleNewLineState(int c)
{
	enum Statetype state;
	if (c == 10)
	{
		putchar(c);
		state = NEWLINE;
	}
	else if (c == 47)
	{
		state = COMMENT_CHAR1;
	}
	else 
	{
		putchar(c);
		state = NO_UNTERMINATING_COMMENT;
	}
	return state;
}


/*------------------------------------------------------------*/

/* Implements the LITERAL state of the DFA. c is the current 
DFA character. Write c to stdout.  Return the next state. 
*/

enum Statetype handleLiteralState(int c)
{
	enum Statetype state;
	putchar(c);
	if (c == 39 || c == 34)
	{
		state = NO_UNTERMINATING_COMMENT;
	}
	else if (c == 10)
	{
		state = NEWLINE_LITERAL;
	}
	else if (c == 92)
	{
		state = LITERAL_IGNORE;
	}
	else 
	{
		state = LITERAL;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the NEWLINE_LITERAL state of the DFA. 
c is the current DFA character. 
Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleLiteralNewLineState(int c)
{
	enum Statetype state;
	putchar(c);
	if (c == 10)
	{
		state = NEWLINE_LITERAL;
	}
	else 
	{
		state = LITERAL;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the LITERAL_IGNORE state of the DFA. 
c is the current DFA character. 
Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleLiteralIgnoreState(int c)
{
	enum Statetype state = LITERAL;
	putchar(c);
	return state;
}

/*------------------------------------------------------------*/

/* Implements the COMMENT_CHAR1 (/) state of the DFA. c is the 
current DFA character. Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleCommentChar1State(int c)
{
	enum Statetype state;
	if (c == 42)
	{
		putchar(32); /* space */
		state = COMMENT_CHAR2;
	}
	else if (c == 47)
	{
		putchar(c);
		state = COMMENT_CHAR1;
	}
	else if (c == 39 || c == 34) 
	{
		putchar(47);
		putchar(c);
		state = LITERAL;
	}
	else 
	{
		putchar(47);
		putchar(c);
		state = NO_UNTERMINATING_COMMENT;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the COMMENT_CHAR2 state of the DFA. c is the 
current DFA character. Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleCommentChar2State(int c)
{
	enum Statetype state;
	if (c == 42)
	{
		state = COMMENT_CHAR3;
	}
	else if (c == 10)
	{
		putchar(c);
		state = NEWLINE_COMMENT;
	}
	else 
	{
		state = COMMENT_CHAR2;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the NEWLINE_COMMENT state of the DFA. 
c is the current DFA character. 
Write c to stdout depending on the 
conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleNewLineCommentState(int c)
{
	enum Statetype state;
	if (c == 10)
	{
		putchar(c);
		state = NEWLINE_COMMENT;
	}
	else if (c == 42)
	{
		state = COMMENT_CHAR3;
	}
	else 
	{
		state = COMMENT_CHAR2;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the COMMENT_CHAR3 state of the DFA. 
c is the current DFA character. Write c to stdout depending 
on the conditions outlined by DFA.  Return the next state. 
*/

enum Statetype handleCommentChar3State(int c)
{
	enum Statetype state;
	if (c == 47)
	{
		state = NO_UNTERMINATING_COMMENT;
	}
	else if (c == 42)
	{
		state = COMMENT_CHAR3;
	}
	else if (c == 10)
	{
		putchar(c);
		state = NEWLINE_COMMENT;
	}
	else 
	{
		state = COMMENT_CHAR2;
	}
	return state;
}

/*------------------------------------------------------------*/

/* Implements the exit state of the DFA. This occurs when 
there are no characters left for th unterminated comment.
nLines is the line of the comment. Write a message 
out to stderror as instructed.
*/

void handleExitState(int nLines)
{
	fprintf(stderr, "Error: line ");
	fprintf(stderr, "%d", nLines);
	fprintf(stderr, ": unterminated comment\n");
	
}

/*----------------------------------------------------------*/
/* Read text from stdin. Look for terminating comment. 
Replace comment with space.  
Keep track of unterminating comment and the line at which 
it appears. Return 0 or 1 based on existence of 
unterminating comment.
We keep track of n = current line, and m = current line of
unterminating comment. */

int main(void)
{
	int c; 
	/* Use a DFA approach.  state indicates the DFA state */
	enum Statetype state = NO_UNTERMINATING_COMMENT;
	
	int n = 1; /* current line */
	int m = 0; /* reference line of unterminating comment*/
	
	while ((c = getchar()) != EOF)
	{
		switch (state) 
		{
			case NO_UNTERMINATING_COMMENT:
				state = handleNoUnterminatingCommentState(c);
				
				if (m != 0)
				{
					m = 0;
				}
				
				break;
			case NEWLINE:
				state = handleNewLineState(c);
				n++;
				break;
			case NEWLINE_LITERAL:
				state = handleLiteralNewLineState(c);
				n++;
				break;
			case LITERAL:
				state = handleLiteralState(c);
				break;
			case LITERAL_IGNORE:
				state = handleLiteralIgnoreState(c);
				break;
			case COMMENT_CHAR1:
				state = handleCommentChar1State(c);
				break;
			case COMMENT_CHAR2:
				state = handleCommentChar2State(c);
				
				if (m == 0)
				{
					m = n;
				}
				
				break;	
			case NEWLINE_COMMENT:
				state = handleNewLineCommentState(c);
				n++;
				break;
			case COMMENT_CHAR3:
				state = handleCommentChar3State(c);
				break;
		}
	}
	
	if (state == COMMENT_CHAR1)
	{
		putchar(47);
	}
	
	else if (state == COMMENT_CHAR3 || state == NEWLINE_COMMENT || state == COMMENT_CHAR2)
	{
		handleExitState(m);
		return 1;
	}
	
	return 0;
}

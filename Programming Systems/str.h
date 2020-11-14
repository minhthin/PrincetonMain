/*--------------------------------------------------------------------*/
/* str.h                                            				  			  */
/* Author: Minh-Thi Nguyen                                            */
/*--------------------------------------------------------------------*/

size_t getLength(const char pcSrc[]);

char copy(const char pcSrc[]);

char concat(char dest[], const char pcSrc[]);

int compare(const char s1[], const char s2[]);

char search(const char pcSrc[], const char sub[]);

/*--------------------------------------------------------------------*/
/* ft.c                                                               */
/* Author: Minh-Thi Nguyen and Marti Vives                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "dynarray.h"
#include "ft.h"
#include "node.h"
#include "checker_ft.h"


/* A File Tree is an AO with 3 state variables: */
/* a flag for if it is in an initialized state (TRUE) or not (FALSE) */
static boolean isInitialized;
/* a pointer to the root Node in the hierarchy */
static Node root;
/* a counter of the number of Nodes in the hierarchy */
static size_t count;

/*
   Starting at the parameter curr, traverses as far down
   the hierarchy as possible while still matching the path
   parameter.
   Returns a pointer to the farthest matching Node down that path,
   or NULL if there is no node in curr's hierarchy that matches
   a prefix of the path
*/
static Node FT_traversePathFrom(char* path, Node curr) {
   Node found;
   size_t i;

   assert(path != NULL);

   if(curr == NULL)
      return NULL;

   else if(!strcmp(path,Node_getPath(curr)))
      return curr;

   else if(!strncmp(path, Node_getPath(curr), strlen(Node_getPath(curr)))) {
      for(i = 0; i < Node_getNumChildren(curr); i++) {
         found = FT_traversePathFrom(path,
                                Node_getChild(curr, i));
         if(found != NULL)
            return found;
      }
      return curr;
   }
   return NULL;
}

/*
   Given a prospective parent and child Node,
   adds child to parent's children list, if possible
   If not possible, destroys the hierarchy rooted at child
   and returns PARENT_CHILD_ERROR, otherwise, returns SUCCESS.
*/
static int FT_linkParentToChild(Node parent, Node child) {

   assert(parent != NULL);
   
   if (Node_isDir(parent) == FALSE)
   {
      return PARENT_CHILD_ERROR;
   }

   if(Node_linkChild(parent, child) != SUCCESS) {
      (void) Node_destroy(child);
      return PARENT_CHILD_ERROR;
   }
   return SUCCESS;
}

/*
   Returns the farthest Node reachable from the root following a given
   path, or NULL if there is no Node in the hierarchy that matches a
   prefix of the path.
*/
static Node FT_traversePath(char* path) {
   assert(path != NULL);
   return FT_traversePathFrom(path, root);
}

/*
   Destroys the entire hierarchy of Nodes rooted at curr,
   including curr itself.
*/
static void FT_removePathFrom(Node curr) {
   if(curr != NULL) {
      count -= Node_destroy(curr);
   }
}

/*
   Inserts a new dir path into the tree rooted at parent, or, if
   parent is NULL, as the root of the data structure.
   If a Node representing path already exists, returns ALREADY_IN_TREE
   If there is an allocation error in creating any of the new nodes or
   their fields, returns MEMORY_ERROR
   If there is an error linking any of the new nodes,
   returns PARENT_CHID_ERROR
   Otherwise, returns SUCCESS
*/
static int FT_insertRestOfPath_dir(char* path, Node parent) {
   Node curr = parent;
   Node firstNew = NULL;
   Node new;
   char* copyPath;
   char* restPath = path;
   char* dirToken;
   int result;
   size_t newCount = 0;

   assert(path != NULL);


   if(curr == NULL) {
      if(root != NULL) {
         return CONFLICTING_PATH;
      }
   }
   else {
      if(!strcmp(path, Node_getPath(curr)))
         return ALREADY_IN_TREE;

      restPath += (strlen(Node_getPath(curr)) + 1);
   }

/*
   if (Node_isDir(curr) == FALSE)
   {
      fprintf(stderr, "Inserting to a file parent.");
      return PARENT_CHILD_ERROR;
   }
*/
   copyPath = malloc(strlen(restPath)+1);
   if(copyPath == NULL)
      return MEMORY_ERROR;
   strcpy(copyPath, restPath);
   dirToken = strtok(copyPath, "/");

   /* dirToken will always be a dir to insert unless NULL */
   while(dirToken != NULL) {
      new = Node_dir_create(dirToken, curr);
      newCount++;

      if(firstNew == NULL)
         firstNew = new;
      else {
         result = FT_linkParentToChild(curr, new);
         if(result != SUCCESS) {
            (void) Node_destroy(new);
            (void) Node_destroy(firstNew);
            free(copyPath);
            return result;
         }
      }

      if(new == NULL) {
         (void) Node_destroy(firstNew);
         free(copyPath);
         return MEMORY_ERROR;
      }

      curr = new;
      dirToken = strtok(NULL, "/");
   }

   /* after inserting the nodes check for corner cases */

   free(copyPath);

   if(parent == NULL) {
      root = firstNew;
      count = newCount;
      return SUCCESS;
   }
   else {
      result = FT_linkParentToChild(parent, firstNew);
      if(result == SUCCESS)
         count += newCount;
      else
         (void) Node_destroy(firstNew);

      return result;
   }
}

/*
   Inserts a new file path into the tree rooted at parent, or, if parent 
   is NULL, as the root of the data structure, with contents and
   length_contents .If a Node representing path already exists, returns 
   ALREADY_IN_TREE.
   If there is an allocation error in creating any of the new nodes or
   their fields, returns MEMORY_ERROR
   If there is an error linking any of the new nodes,
   returns PARENT_CHID_ERROR
   Otherwise, returns SUCCESS
*/
static int FT_insertRestOfPath_file(char* path, Node parent, void *contents, 
   size_t length_contents) {

   Node curr = parent;
   Node firstNew = NULL;
   Node new;
   char* copyPath;
   char* restPath = path;
   char* dirToken;
   char* dirToken_next;
   int result;
   size_t newCount = 0;

   assert(path != NULL);

   if(curr == NULL) {
      if(root != NULL) {
         return CONFLICTING_PATH;
      }
   }
   else {
      if(!strcmp(path, Node_getPath(curr)))
         return ALREADY_IN_TREE;

      restPath += (strlen(Node_getPath(curr)) + 1);
   }

/*
   if (Node_isDir(curr) == FALSE)
   {
      fprintf(stderr, "Inserting to file parent.");
      return PARENT_CHILD_ERROR;
   }
*/

   copyPath = malloc(strlen(restPath)+1);
   if(copyPath == NULL)
      return MEMORY_ERROR;
   strcpy(copyPath, restPath);
   dirToken = strtok(copyPath, "/");
   dirToken_next = strtok(NULL, "/");

   /* we need to know the next token to make sure we insert a file
   only at the last insertion */
   while(dirToken != NULL) {

      if (dirToken_next == NULL)
         new = Node_file_create(dirToken,curr,contents,length_contents);
      else
         new = Node_dir_create(dirToken, curr);
      newCount++;

      if(firstNew == NULL)
         firstNew = new;
      else {
         result = FT_linkParentToChild(curr, new);
         if(result != SUCCESS) {
            (void) Node_destroy(new);
            (void) Node_destroy(firstNew);
            free(copyPath);
            return result;
         }
      }

      if(new == NULL) {
         (void) Node_destroy(firstNew);
         free(copyPath);
         return MEMORY_ERROR;
      }

      curr = new;
      dirToken = dirToken_next;
      dirToken_next = strtok(NULL, "/");
   }

   /* after inserting the nodes check for corner cases */


   free(copyPath);

   if(parent == NULL) {
      root = firstNew;
      count = newCount;
      return SUCCESS;
   }
   else {
      result = FT_linkParentToChild(parent, firstNew);
      if(result == SUCCESS)
         count += newCount;
      else
         (void) Node_destroy(firstNew);

      return result;
   }
}

/*
   Alternate version of strlen that uses pAcc as an in-out parameter
   to accumulate a string length, rather than returning the length of
   str, and also always adds one more in addition to str's length.
*/
static void FT_strlenAccumulate(char* str, size_t* pAcc) {
   assert(pAcc != NULL);

   if(str != NULL)
      *pAcc += (strlen(str) + 1);
}

/*
   Alternate version of strcat that inverts the typical argument
   order, appending str onto acc, and also always adds a newline at
   the end of the concatenated string.
*/
static void FT_strcatAccumulate(char* str, char* acc) {
   assert(acc != NULL);

   if(str != NULL)
      strcat(acc, str); strcat(acc, "\n");
}

/*
   Performs a pre-order traversal of the tree rooted at n,
   inserting each payload to DynArray_T d beginning at index i.
   Returns the next unused index in d after the insertion(s).
*/
static size_t FT_preOrderTraversal(Node n, DynArray_T d, size_t i) {
   size_t c;

   assert(d != NULL);

   /* Initialize two loops: one for directory children and one for file children */
   /* Print file children before directory children */
   if(n != NULL) {
      /* Loop file children */
      /* Node_getPath is assigned to be ith element of DynArray d */
      (void) DynArray_set(d, i, Node_getPath(n));
      /* increment i */
      i++;
      /* First print out the file children of n */
      for (c = 0; c < Node_getNumChildren(n); c++)
      {
         if (Node_isDir(Node_getChild(n, c)) == FALSE)
         {
            (void) DynArray_set(d, i, Node_getPath(Node_getChild(n, c)));
            i++;
         }
      }
      /* Then print out the directories */
      for (c = 0; c < Node_getNumChildren(n); c++)
      {
         if (Node_isDir(Node_getChild(n, c)) == TRUE)
         {
            i = FT_preOrderTraversal(Node_getChild(n, c), d, i);
         }
      }
   }
   return i;
}


/* see ft.h for specification */
char *FT_toString()
{
   DynArray_T nodes;
   size_t totalStrlen = 1;
   char* result = NULL;

   assert(Checker_FT_isValid(isInitialized,root,count)); 

   if(!isInitialized)
      return NULL;

   nodes = DynArray_new(count);
   (void) FT_preOrderTraversal(root, nodes, 0);

   DynArray_map(nodes, (void (*)(void *, void*)) FT_strlenAccumulate, (void*) &totalStrlen);

   result = malloc(totalStrlen);
   if(result == NULL) {
      DynArray_free(nodes);
      assert(Checker_FT_isValid(isInitialized,root,count));
      return NULL;
   }
   *result = '\0';

   DynArray_map(nodes, (void (*)(void *, void*)) FT_strcatAccumulate, (void *) result);

   DynArray_free(nodes);
   assert(Checker_FT_isValid(isInitialized,root,count));
   return result;
}

/* see ft.h for specification */
int FT_insertDir(char *path)
{
   Node curr;
   int result;

   assert(Checker_FT_isValid(isInitialized, root, count));
   assert(path != NULL);

   if (!isInitialized) 
      return INITIALIZATION_ERROR;

   curr = FT_traversePath(path);

   if (curr != NULL && strcmp(Node_getPath(curr), path) == 0) 
   {
      return ALREADY_IN_TREE;
   }
   
   if (curr != NULL && Node_isDir(curr) == FALSE)
   {
      return NOT_A_DIRECTORY;
   }

   result = FT_insertRestOfPath_dir(path, curr);
   assert(Checker_FT_isValid(isInitialized, root, count));

   return result;
}

/* see ft.h for specification */
boolean FT_containsDir(char *path)
{
   Node curr;
   boolean result;

   assert(Checker_FT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return FALSE;

   curr = FT_traversePath(path);

   if(curr == NULL)
      result = FALSE;
   else if (Node_isDir(curr) == FALSE) {
      result = FALSE;
   }
   else if(strcmp(path, Node_getPath(curr)))
      result = FALSE;
   else
      result = TRUE;

   assert(Checker_FT_isValid(isInitialized,root,count));
   return result;
}

/*
  Removes the directory hierarchy rooted at path starting from Node
  curr. If curr is the data structure's root, root becomes NULL.
  Returns NO_SUCH_PATH if curr is not the Node for path,
  and SUCCESS otherwise.
 */
static int FT_rmPathAt(char* path, Node curr) {

   Node parent;

   assert(path != NULL);
   assert(curr != NULL);

   parent = Node_getParent(curr);

   if(!strcmp(path,Node_getPath(curr))) {
      if(parent == NULL)
         root = NULL;
      else
         Node_unlinkChild(parent, curr);

      FT_removePathFrom(curr);

      return SUCCESS;
   }
   else
      return NO_SUCH_PATH;
}

/* see ft.h for specification */
int FT_rmDir(char *path)
{
   Node curr;
   int result;

   assert(path != NULL);
   curr = FT_traversePath(path);
   if(curr == NULL)
      result =  NO_SUCH_PATH;
   else if (Node_isDir(curr) == FALSE) 
      result = NOT_A_DIRECTORY;
   else
      result = FT_rmPathAt(path, curr);

   assert(Checker_FT_isValid(isInitialized,root,count));
   return result;   
}


/* see ft.h for specification */
int FT_insertFile(char *path, void *contents, size_t length)
{
   Node curr;
   int result;

   assert(Checker_FT_isValid(isInitialized, root, count));
   assert(path != NULL);


   if (!isInitialized) 
      return INITIALIZATION_ERROR;

   curr = FT_traversePath(path);

   if (curr == NULL && count != 0)
      return CONFLICTING_PATH;


   if (curr != NULL && strcmp(Node_getPath(curr),path) == 0)
      return ALREADY_IN_TREE;
   
   
   if (curr != NULL && Node_isDir(curr) == FALSE)
   {
      return NOT_A_DIRECTORY;
   }

   result = FT_insertRestOfPath_file(path, curr, contents, length);

   assert(Checker_FT_isValid(isInitialized, root, count));

   return result;
}

/* see ft.h for specification */
boolean FT_containsFile(char *path)
{
   Node curr;
   boolean result;

   assert(Checker_FT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return FALSE;

   curr = FT_traversePath(path);

   if(curr == NULL)
      result = FALSE;
   else if (Node_isDir(curr) == TRUE)
   {
      result = FALSE;
   }
   else if(strcmp(path, Node_getPath(curr))) 
      result = FALSE;
   else
      result = TRUE;

   assert(Checker_FT_isValid(isInitialized,root,count));
   return result;
}

/* see ft.h for specification */
int FT_rmFile(char *path)
{
   Node curr;
   int result;

   assert(Checker_FT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return INITIALIZATION_ERROR;

   curr = FT_traversePath(path);

   if(curr == NULL)
      result =  NO_SUCH_PATH;
   else if (Node_isDir(curr) == TRUE)
   {
      result = NOT_A_FILE;
   }
   else {
      /* this works for dir and files since node_destroy
      does accomodates both dir and files */
      result = FT_rmPathAt(path, curr);
   }
   assert(Checker_FT_isValid(isInitialized,root,count));
   return result;
}

/* see ft.h for specification */
void *FT_getFileContents(char *path)
{
   Node curr;

   assert(Checker_FT_isValid(isInitialized,root,count));
   assert(path != NULL);
   assert(isInitialized == TRUE);

   if (FT_containsFile(path) == TRUE)
   {
      curr = FT_traversePath(path);
      return Node_getContents(curr);
   }
   else return NULL;
}

/* see ft.h for specification */
void *FT_replaceFileContents(char *path, void *newContents, size_t newLength)
{
   Node curr;
   void *originalContent;
   
   assert(Checker_FT_isValid(isInitialized,root,count));
   assert(path != NULL);
   assert(isInitialized == TRUE);


   if (FT_containsFile(path) == TRUE)
   {
      curr = FT_traversePath(path);
      if (Node_isDir(curr) == FALSE) return NULL;
      originalContent = Node_getContents(curr);

      Node_replaceFileContents(curr, newContents, newLength);
      return originalContent;
   }
   else return NULL;
}

/* see ft.h for specification */
int FT_stat(char *path, boolean* type, size_t* length)
{
   Node curr;
   int result;

   assert(path != NULL);


   if(!isInitialized)
      return INITIALIZATION_ERROR;

   if (FT_containsDir(path) == FALSE && FT_containsFile(path) == FALSE)
   {
      result = 1;
      return NO_SUCH_PATH;
   }
   else if (FT_containsDir(path) == TRUE && FT_containsFile(path) == TRUE)
   {
      result = 1;
      return result;
   }
   else if (FT_containsDir(path) == TRUE)
   {
      result = TRUE;
      *type = FALSE;
   }
   else if (FT_containsFile(path) == TRUE)
   {
      curr = FT_traversePath(path);
      result = TRUE;
      *type = TRUE;
      *length = Node_getContentLength(curr);
   }
   return SUCCESS;
}

/* see ft.h for specification */
int FT_init(void)
{
   assert(Checker_FT_isValid(isInitialized,root,count));
   if(isInitialized)
      return INITIALIZATION_ERROR;
   isInitialized = TRUE;
   root = NULL;
   count = 0;
   assert(Checker_FT_isValid(isInitialized,root,count));
   return SUCCESS;
}

/* see ft.h for specification */
int FT_destroy(void)
{
   assert(Checker_FT_isValid(isInitialized,root,count));
   if(!isInitialized)
      return INITIALIZATION_ERROR;
   FT_removePathFrom(root);
   root = NULL;
   isInitialized = FALSE;
   assert(Checker_FT_isValid(isInitialized,root,count));
   return SUCCESS;
}
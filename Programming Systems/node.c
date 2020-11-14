/*--------------------------------------------------------------------*/
/* node.c                                                             */
/* Author: Minh-Thi Nguyen and Marti Vives                                          */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "dynarray.h"
#include "node.h"

/*
   A node structure represents a directory or file in the file tree
*/
struct node {
   /* the full path of this directory */
   char* path;

   /* the parent ft of this ft
      NULL for the root of the file tree */
   Node parent;


   /* a boolean isDir is TRUE if node
   is a directory, if it's a file then
   isDir is FALSE */
   boolean isDir;

   /* the subdirectories of this directory
      stored in sorted order by pathname */
   DynArray_T children;

   /* The contents of a file */
   void *contents;

   /* length of the contents */
   size_t contentLength;
};


/*
  returns a path with contents
  n->path/dir
  or NULL if there is an allocation error.
  Allocates memory for the returned string,
  which is then owned by the caller!
*/
static char* Node_buildPath(Node n, const char* dir) {
   char* path;

   assert(dir != NULL);

   /* If building path for a null node */
   if(n == NULL)
      path = malloc(strlen(dir)+1);
   else
      path = malloc(strlen(n->path) + 1 + strlen(dir) + 1);

   if(path == NULL)
      return NULL;
   *path = '\0';

   if(n != NULL) {
      strcpy(path, n->path);
      strcat(path, "/");
   }
   strcat(path, dir);

   return path;
}

/* see node.h for specification */
Node Node_dir_create(const char* dir, Node parent)
{
   Node new;

   assert(dir != NULL);

   if (parent != NULL)
      assert(parent->isDir == TRUE);

   new = malloc(sizeof(struct node));
   if(new == NULL)
      return NULL;

   new->path = Node_buildPath(parent, dir);
   if(new->path == NULL) {
      free(new);
      return NULL;
   }

   new->parent = parent;
   new->children = DynArray_new(0);
   if(new->children == NULL) {
      free(new->path);
      free(new);
      return NULL;
   }

   new->isDir = TRUE;
   new->contents = NULL;

   return new;
}

/* see node.h for specification */
Node Node_file_create(const char* file, Node parent,void *contents, size_t length_contents)
{
   Node new;
   void *c_copy;
   assert(file != NULL);
   if (parent != NULL) {
      assert(parent->isDir == TRUE);
   }

   new = malloc(sizeof(struct node));
   if(new == NULL)
      return NULL;

   new->path = Node_buildPath(parent, file);

   if(new->path == NULL) {
      free(new);
      return NULL;
   }

   new->isDir = FALSE;
   new->parent = parent;
   new->children = NULL;

   /* Create a copy of contents */
   if (contents == NULL) 
   {
      new->contents = NULL;
   }
   else {
      c_copy = malloc(length_contents*sizeof(void*));
      if(c_copy == NULL) {
         free(new->children);
         free(new->path);
         free(new);
         return NULL;
      }
      new->contents = c_copy;
   }
   new->contentLength=length_contents;
   return new;
}

/* see node.h for specification */
size_t Node_destroy(Node n) {
   size_t i;
   size_t count = 0;
   Node c;

   assert(n != NULL);

   if (n->isDir == TRUE)
   {
      for(i = 0; i < DynArray_getLength(n->children); i++)
      {
         c = DynArray_get(n->children, i);
         count += Node_destroy(c);
      }
      DynArray_free(n->children);
   }
   free(n->contents);
   free(n->path);
   free(n);
   count++;
   return count;
}

/* see node.h for specification */
const char* Node_getPath(Node n) {

   assert(n != NULL);
   return n->path;
}

/* see node.h for specification */
int Node_compare(Node node1, Node node2) {
   assert(node1 != NULL);
   assert(node2 != NULL);

   return strcmp(node1->path, node2->path);
}


/* see node.h for specification */
size_t Node_getNumChildren(Node n) {
   assert(n != NULL);
   assert(n->isDir == TRUE);

   return DynArray_getLength(n->children);
}

/* see node.h for specification */
int Node_hasChild(Node n, const char* path, size_t* childID) {
   size_t index;
   int result;
   Node checker;

   assert(n->isDir == TRUE);
   assert(n != NULL);
   assert(path != NULL);

   checker = Node_dir_create(path, NULL);
   if(checker == NULL)
      return -1;
   result = DynArray_bsearch(n->children, checker, &index,
                    (int (*)(const void*, const void*)) Node_compare);
   (void) Node_destroy(checker);

   if(childID != NULL)
      *childID = index;
   return result;
}

/* see node.h for specification */
Node Node_getChild(Node n, size_t childID) {
   assert(n != NULL);
   assert(n->isDir == TRUE);

   if(DynArray_getLength(n->children) > childID)
      return DynArray_get(n->children, childID);
   else
      return NULL;
}

/* see node.h for specification */
Node Node_getParent(Node n) {
   assert(n != NULL);

   return n->parent;
}

/* see node.h for specification */
int Node_linkChild(Node parent, Node child) {
   size_t i;
   char* rest;

   assert(parent->isDir == TRUE);
   assert(parent != NULL);
   assert(child != NULL);

   if(Node_hasChild(parent, child->path, NULL))
      return ALREADY_IN_TREE;
   i = strlen(parent->path);
   if(strncmp(child->path, parent->path, i))
      return PARENT_CHILD_ERROR;
   rest = child->path + i;
   if(strlen(child->path) >= i && rest[0] != '/')
      return PARENT_CHILD_ERROR;
   rest++;
   if(strstr(rest, "/") != NULL)
      return PARENT_CHILD_ERROR;

   child->parent = parent;

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 1)
      return ALREADY_IN_TREE;

   if(DynArray_addAt(parent->children, i, child) == TRUE)
      return SUCCESS;
   else
      return PARENT_CHILD_ERROR;
}

/* see node.h for specification */
int  Node_unlinkChild(Node parent, Node child) {
   size_t i;

   assert(parent->isDir == TRUE);
   assert(parent != NULL);
   assert(child != NULL);

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 0)
      return PARENT_CHILD_ERROR;

   (void) DynArray_removeAt(parent->children, i);
   return SUCCESS;
}


/* see node.h for specification */
int Node_addChild(Node parent, const char* dir) {
   Node new;
   int result;

   assert(parent->isDir == TRUE);
   assert(parent != NULL);
   assert(dir != NULL);   

   new = Node_dir_create(dir, parent);
   if(new == NULL)
      return PARENT_CHILD_ERROR;

   result = Node_linkChild(parent, new);
   if(result != SUCCESS)
      (void) Node_destroy(new);

   return result;
}


/* see node.h for specification */
char* Node_toString(Node n) {
   char* copyPath;

   assert(n != NULL);

   copyPath = malloc(strlen(n->path)+1);
   if(copyPath == NULL)
      return NULL;
   else
      return strcpy(copyPath, n->path);
}

/* see node.h for specification */
boolean Node_isDir(Node n) 
{
   assert(n != NULL);
   return n->isDir;
}

/* see node.h for specification */
void* Node_getContents(Node n)
{
   assert(n != NULL);
   assert(n->isDir == FALSE);
   return n->contents;
}

/* see node.h for specification */
size_t Node_getContentLength(Node n)
{

   assert(n != NULL);
   assert(n->isDir == FALSE);
   return n->contentLength;
}

/* see node.h for specification */
void* Node_replaceFileContents(Node n, void *newContents, size_t newLength)
{
   void* oldContents;

   assert(n != NULL); 
   if (n->isDir == TRUE)
      return NULL;

   oldContents = n->contents;
   n->contents = newContents;
   n->contentLength = newLength;
   return oldContents;
}
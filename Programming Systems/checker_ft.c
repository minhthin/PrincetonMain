/*--------------------------------------------------------------------*/
/* checker.c                                                          */
/* Author: Minh-Thi Nguyen and Marti Vives                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dynarray.h"
#include "checker_ft.h"


/* see checker.h for specification */
boolean Checker_Node_isValid(Node n) {
   Node parent;
   const char* npath;
   const char* ppath;
   const char* rest;
   size_t i;

   /* Sample check: a NULL pointer is not a valid Node */
   if(n == NULL) {
      fprintf(stderr, "Node is a NULL pointer\n");
      return FALSE;
   }

   parent = Node_getParent(n);
   if(parent != NULL) {
      npath = Node_getPath(n);

      /* Sample check that parent's path must be prefix of n's path */
      ppath = Node_getPath(parent);

      /* Sample check if the parent's path exists */
      if (ppath == NULL) 
      {
         fprintf(stderr, "P's path is NULL \n");
         return FALSE;
      }

      /* Check if the parent's path is the same as/conflicts with the children's path */
      if(strlen(ppath) == strlen(npath)) {
         fprintf(stderr, "P's path conflicts with C's path. \n");
         return FALSE;
      }

      /* Check if parent's path is not a prefix of children's path */
      i = strlen(ppath);
      if(strncmp(npath, ppath, i)) {
         fprintf(stderr, "P's path is not a prefix of C's path\n");
         return FALSE;
      }
      /* Sample check that n's path after parent's path + '/'
         must have no further '/' characters */
      rest = npath + i;
      rest++;
      if(strstr(rest, "/") != NULL) {
         fprintf(stderr, "C's path has grandchild of P's path\n");
         return FALSE;
      }
   }

   return TRUE;
}


/*
   Performs a pre-order traversal of the tree rooted at n.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.
   You may want to change this function's return type or
   parameter list to facilitate constructing your checks.
   If you do, you should update this function comment.
*/
static boolean Checker_treeCheck(Node n) {
   size_t c;

   if(n != NULL) {

      /* Sample check on each non-root Node: Node must be valid */
      /* If not, pass that failure back up immediately */
      if(!Checker_Node_isValid(n))
         return FALSE;

      /* Check if the node is a directory and recurse down the tree */
      if (Node_isDir(n) == TRUE) {
         for(c = 0; c < Node_getNumChildren(n); c++)
         {
            Node child = Node_getChild(n, c);

            if (c < Node_getNumChildren(n) - 1) 
            {
               Node nextChild = Node_getChild(n, c + 1);
               if (strcmp(Node_toString(nextChild), Node_toString(child)) < 0) 
               {
                  fprintf(stdout, "Children out of order \n");
                  return FALSE;
               }
            }

            /* if recurring down one subtree results in a failed check
               farther down, passes the failure back up immediately */
            if(!Checker_treeCheck(child))
               return FALSE;
         }
      }
   }
   return TRUE;
}

/* see checker.h for specification */
boolean Checker_FT_isValid(boolean isInit, Node root, size_t count) {

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!isInit)
      if(count != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

   
   /* Now checks invariants recursively at each Node from the root. */
   return Checker_treeCheck(root);
}
/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Minh-Thi Nguyen                                            */
/*--------------------------------------------------------------------*/

#include <stddef.h>
#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED

/* Defines the interface to symtablelist.c functions */

/*--------------------------------------------------------------------*/

/* SymTable_new: A SymTable_T object is a table of bindings with 
bindings, each represented by a key-value pair */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_new: Create and return a SymTable object that contains no 
bindings, or NULL if insufficient memory is available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_free: Free all memory occupied by oSymTable 
@ param oSymTable: SymTable object
*/

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_getLength: Return the number of bindings in oSymTable
@ param oSymTable: SymTable object
*/

size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_put: Add a new binding to oSymTable consisting 
of key pcKey and value pvValue and return 1 (TRUE)
@ param oSymTable: SymTable object
@ param pcKey: pointer to binding key 
@ param pvValue: pointer to binding value 
*/

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_replace: Replace the binding's value with pvValue and 
return the old value. Otherwise, rhe function leaves oSymTable 
unchanged and return NULL
@ param oSymTable: SymTable object
@ param pcKey: pointer to binding key 
@ param pvValue: pointer to binding value 
*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_replace: Replace the binding's value with pvValue and 
return the old value. Otherwise, rhe function leaves oSymTable 
unchanged and return NULL
@ param oSymTable: SymTable object
@ param pcKey: pointer to binding key 
@ param pvValue: pointer to binding value 
*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_contains: Return 1 (TRUE) if oSymTable contains a binding 
whose key is pcKey, and 0 (FALSE) otherwise
@ param oSymTable: SymTable object
@ param pcKey: pointer to binding key 
*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_get: Return the value of binding within oSymTable whose key
 is pcKey or NULL if no such binding exists 
 @ param oSymTable: SymTable object
 @ param pcKey: pointer to binding key 
*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_remove: Remove the binding that contains key pcKey and 
return binding's value.  Otherwise, do not change oSymTable and return 
NULL
 @ param oSymTable: SymTable object
 @ param pcKey: pointer to binding key 
*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/

/* SymTable_map: Apply function *pfApply to each binding in oSymbolTable
passing pvExtra as an extra parameter
 @ param oSymTable: SymTable object
 @ param pcKey: pointer to binding key 
 @ pvValue: pointer to binding value 
 @ pvExtra: pointer to extra parameter to use *pfApply
*/

void SymTable_map(SymTable_T oSymTable,
   void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
   const void *pvExtra);

/*--------------------------------------------------------------------*/

#endif

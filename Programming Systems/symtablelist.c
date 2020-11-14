/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Minh-Thi Nguyen 		                                      */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "symtable.h"

/*--------------------------------------------------------------------*/

/* Each binding is stored in a Node, linked to form a list */

struct Node
{
    /* The binding key */
    const char *key; 
    /* The binding value */
    const void *value; 
    /* The address of the next Node */
    struct Node *next; 
};

/*--------------------------------------------------------------------*/

/* SymTable points to the first Node */

struct SymTable
{
    /* The address of the first Node */
    struct Node *first;

    /* Keep track of number of bindings */
    /* Meet constant time requirements */
    size_t bindingCount;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void) 
{
    SymTable_T oSymTable;

    /* Allocate memory */
    oSymTable = (SymTable_T) malloc(sizeof(struct SymTable));
    /* Insufficient memory check */
    if (oSymTable == NULL) 
        return NULL;

    /* Point to first node */
    oSymTable->first = NULL; /* (*oSymTable).first = NULL; */

    /* Set count to 0 */
    oSymTable->bindingCount = 0;

    return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
    struct Node *current;
    struct Node *next;

    assert(oSymTable != NULL);

    for (current = oSymTable->first; current != NULL; 
    current = next)
    {
        /* Free the key */
        free((char*) current->key);

        /* Set the current node to the next node: current->next */
        next = current->next;

        /* Free the current node: free(current)*/
        free(current);

    }

    free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) 
{
    /* Keep track of count in SymTable and return count */
    return oSymTable->bindingCount;
}

/*--------------------------------------------------------------------*/

/* Insert new Node containing the key/value pair at the front 
of the list */

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue)
{
    struct Node *new;
    char *copyKey;
    /* instance char* key */

    /* Check for null arguments */
    assert(oSymTable != NULL);

    if (pcKey == NULL)
    {
        assert(oSymTable->bindingCount = 0);
        return 0;
    }


    /* check for duplicate key */
    if (SymTable_contains(oSymTable, pcKey) == 1) 
        return 0; 

    /* Allocate memory for new node */
    new = (struct Node*) malloc(sizeof(struct Node));
    if (new == NULL) return 0; /* Insufficient memory available */
    
    /* Insert node in front of linked list */
    /* Create defensive copy of key */
    copyKey = malloc(sizeof(char) * (strlen(pcKey) + 1));
    strcpy(copyKey, pcKey);

    new->key = (const char*)copyKey;
    new->value = pvValue;

    new->next = oSymTable->first;
    oSymTable->first = new;

    /* Increment the count */
    oSymTable->bindingCount++;

    return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue)
{
    struct Node *current;
    struct Node *next;
    void *oldValue;

    /* Check all arguments */
    assert(oSymTable != NULL);
    if (pcKey == NULL)
    {
        return NULL;
    }

    /* Traverse the list to find pcKey */
    for (current = oSymTable->first; current != NULL; current = next) 
    {
        if (strcmp(current->key, pcKey) == 0)
        {
            /* Replace the value of binding */
            oldValue = (void*) current->value;
            current->value = pvValue;

            return oldValue;
        }

        next = current->next;
    }

    return NULL;
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) 
{
    struct Node *current;
    struct Node *next;

    /* check all arguments */
    assert(oSymTable != NULL);
    if (pcKey == NULL)
    {
        return 0;
    }

    /* Traverse the list to find pcKey */
    for (current = oSymTable->first; current != NULL; current = next) 
    {
        if (strcmp(current->key, pcKey) == 0) 
        {
            return 1;
        }
        next = current->next;
    }

    return 0;

}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
    struct Node *current;
    struct Node *next;

    /* Check arguments */
    assert(oSymTable != NULL);
    if (pcKey == NULL)
    {
        return NULL;
    }

    /* Traverse the list to find pcKey */
    for (current = oSymTable->first; current != NULL; current = next) 
    {
        if (strcmp(current->key, pcKey) == 0) 
        {
            return (void*)current->value;
        }

        next = current->next;
    }

    return NULL;
}

/*--------------------------------------------------------------------*/

/* Page 431 of Kings */
/* Deletes the first node containing pcKey; does nothing if does not 
contain pcKey */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
    struct Node *current;
    struct Node *prev;
    const void *value = NULL;

    /* Check arguments */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Traverse the list to find pcKey */
    for (current = oSymTable->first, prev = NULL; 
    current != NULL && strcmp(current->key, pcKey) != 0; 
    prev = current, current = current->next);

    /* pcKey was not found */
    if (current == NULL)
        return NULL;

    /* pcKey is in the first Node */
    /* current points to the first node */
    else if (prev == NULL)
    {
        value = current->value;
        /* Assign new first node */
        oSymTable->first = current->next;
    }
    /* pcKey is in some other node */
    else 
    {
        /* Return binding value */
        value = current->value;
        prev->next = current->next;
    }

    /* Update count */
    oSymTable->bindingCount--;

    free((char*)current->key);
    free(current);

    return (void*)value;
    
}

/*--------------------------------------------------------------------*/

/* Apply *pfApply to each binding */
/* pass pvExtra as an extra parameter */
/* call (*pfApply) for each binding in oSymbolTable */
void SymTable_map(SymTable_T oSymTable, 
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), 
const void *pvExtra) 
{
    struct Node *current;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    /* Traverse the list to call (*pfApply) */
    for (current = oSymTable->first; current != NULL; current = current->next) 
    {
        /* call *pfApply */
        if((char*)current->key != NULL)
        {
            (*pfApply) ((char*)current->key, (void*)current->value, (void*)pvExtra);
        }
    }

}
   







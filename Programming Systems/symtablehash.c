/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Minh-Thi Nguyen 		                                      */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "symtable.h"

/* Symbol Table Bucket Count sizes described in assignment 
description */
enum {BUCKET_COUNT = 509, BUCKET_COUNT2 = 1021, BUCKET_COUNT3 = 2039,
BUCKET_COUNT4 = 4093, BUCKET_COUNT5 = 8191, BUCKET_COUNT6 = 16381,
BUCKET_COUNT7 = 32749, BUCKET_COUNT8 = 65521};

/* Create structure with binding key and value and reference 
to the next binding */
struct Binding
{
    /* Binding key */
    const char *key;
    /* Binding value */
    const void *value;
    /* Pointer to next binding */
    struct Binding *next;
};

/* SymTable points to the bucket array */
struct SymTable
{
    /* The number of bindings */
    size_t bindingCounts; 
    /* The number of buckets in bucket array */
    size_t bucketSize; 
    /* Array of buckets, points to first bucket in array */
    struct Binding **buckets; 
};

/*--------------------------------------------------------------------*/

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. Copied from Assignment Page. */
   
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];
      
   return uHash % uBucketCount;
}
/*--------------------------------------------------------------------*/

/* Returns the new capacity for the buckets array for expansion 
given an old BUCKET_COUNT size 
@param oldSize: the size of the old Symbol Table
Returns: newSize: the new size of the new Symbol Table for expansion
*/
static size_t SymTable_newLength(size_t oldSize)
{
    size_t newSize;

    /* initialize newSize */
    newSize = BUCKET_COUNT;

    if (oldSize == BUCKET_COUNT) newSize = BUCKET_COUNT2;
    else if (oldSize == BUCKET_COUNT2) newSize = BUCKET_COUNT3;
    else if (oldSize == BUCKET_COUNT3) newSize = BUCKET_COUNT4;
    else if (oldSize == BUCKET_COUNT4) newSize = BUCKET_COUNT5;
    else if (oldSize == BUCKET_COUNT5) newSize = BUCKET_COUNT6;
    else if (oldSize == BUCKET_COUNT6) newSize = BUCKET_COUNT7;
    else if (oldSize == BUCKET_COUNT7) newSize = BUCKET_COUNT8;
    else if (oldSize == BUCKET_COUNT8) newSize = BUCKET_COUNT8;

    return newSize;
}

/*--------------------------------------------------------------------*/

/* Resize the hashTable and implement expansion */
static void SymTable_resize(SymTable_T oSymTable) 
{
    /* Initialize new Symbol Table with new size */
    /* New SymTable */
    struct Binding **newBucket;
    size_t newSize;

    /* Old SymTable */
    size_t oldSize = (size_t) oSymTable->bindingCounts;

    /* Iterator variables */
    struct Binding *current;
    struct Binding *next;
    size_t i, j;
    size_t hash;

    /* Check arguments */
    assert(oSymTable != NULL);

    /* Check if resizing is necessary */
    if (oSymTable->bindingCounts < oSymTable->bucketSize) 
        return;

    /* Create a new HashTable */
    /* New Hashtable size */
    newSize = SymTable_newLength(oldSize);
    /* If no expansion necessary return the old SymTable*/

    /* Allocate memory for new bucket */
    newBucket = (struct Binding**)malloc(newSize * sizeof(struct Binding));
    assert(newBucket != NULL); /* Memory check */
    /* Set all buckets to null */
    for (j = 0; j < newSize; j++) newBucket[j] = NULL;

    
    /* Rehash */
    /* Loop through old Buckets*/
    for (i = 0; i < oSymTable->bucketSize; i++)
    {
        /* Loop through each bucket's linked lists and rehash to new bucket */
        for (current = oSymTable->buckets[i]; current != NULL; current = next)
        {
            next = current->next;

            /* Rehash binding */
            hash = SymTable_hash(current->key, newSize);

            /* Add new hash to New symtable */
            current->next = newBucket[hash];
            newBucket[hash] = current;
        }
    }
    /* Free old bucket */
    free(oSymTable->buckets);

    oSymTable->bucketSize = newSize;
    oSymTable->buckets = newBucket;
    oSymTable->bindingCounts = oSymTable->bindingCounts;

    return;
}

/*--------------------------------------------------------------------*/

/* Allocate Table structure by setting each bucket to NULL */
SymTable_T SymTable_new(void) 
{
    SymTable_T oSymTable;
    size_t i;

    /* Allocate memory for SymTable structure */
    oSymTable = (SymTable_T) malloc(sizeof(struct SymTable));

    /* Check insufficient memory */
    if (oSymTable == NULL) return NULL;

    /* Initialize structure*/
    /* No bindings */
    oSymTable->bindingCounts = 0;
    /* Set initial size to BUCKET_COUNT */
    oSymTable->bucketSize = (size_t) BUCKET_COUNT;
    /* Allocate memory for bucket array */
    oSymTable->buckets = (struct Binding**)
    malloc(sizeof(struct Binding) * (size_t) BUCKET_COUNT);
    assert(oSymTable->buckets != NULL);

    /* Set each bucket to NULL */
    for (i = 0; i < oSymTable->bucketSize; i++)
    {
        oSymTable->buckets[i] = NULL;
    }

    return oSymTable;

}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
    size_t i;
    struct Binding *current;
    struct Binding *next;

    /* Check arguments for null */
    assert(oSymTable != NULL);

    /* Traverse the Symbol Table  */
    /* Loop through the bucket count */
    for (i = 0; i < oSymTable->bucketSize; i++)
    {
        /* Loop through Linked List and free if not NULL */
        if (oSymTable->buckets[i] != NULL)
        {
            /* Free elements in Linked List */
            for (current = oSymTable->buckets[i]; current != NULL;
             current = next)
             {
                 /* Free the key */
                free((char*) current->key);

                /* Set the current node to the next node: current->next */
                next = current->next;

                /* Free the current node: free(current)*/
                free(current);

             }

        }
    }

    /* Free the bucket */
    free(oSymTable->buckets);
    free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) 
{
    return oSymTable->bindingCounts;
}

/*--------------------------------------------------------------------*/

/* Insert new Node containing the key/value pair at the front 
of the list */

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue)
{
    size_t hash;
    struct Binding *new;
    char *copyKey;

    /* Check arguments for null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);


    /* Expand the bucket if necessary */
    SymTable_resize(oSymTable);

    /* Hash the given key */
    hash = SymTable_hash(pcKey, oSymTable->bucketSize);

    /* Check for duplicate keys*/
    if (SymTable_contains(oSymTable, pcKey) == 1)
    {
        return 0;
    }

    /* Otherwise add the binding */
    /* Allocate memory for new node */
    new = (struct Binding*) malloc(sizeof(struct Binding));
    if (new == NULL) return 0; /* Insufficient memory available */

    /* Insert binding in front of linked list */
    /* Create defensive copy of key */
    copyKey = malloc(sizeof(char) * (strlen(pcKey) + 1));
    strcpy(copyKey, pcKey);
    new->key = (const char*)copyKey;
    new->value = pvValue; 

    new->next = oSymTable->buckets[hash];
    oSymTable->buckets[hash] = new;

    /* Increment the count */
    oSymTable->bindingCounts++;

    return 1;

}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue)
{
    struct Binding *current;
    struct Binding *next;
    void *oldValue;
    size_t hash;

    /* Check null arguments */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Traverse the Symbol Table */
    /* Hash the key */
    hash = SymTable_hash(pcKey, oSymTable->bucketSize);

    /* Traverse hashed bucket */
    for (current = oSymTable->buckets[hash]; 
    current != NULL; current = next)
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
    struct Binding *current;
    struct Binding *next;
    size_t hash;

    /* Check null arguments */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Traverse the Symbol Table */
    /* Hash the key */
    hash = SymTable_hash(pcKey, oSymTable->bucketSize);

    /* Traverse hashed bucket */
    for (current = oSymTable->buckets[hash]; 
    current != NULL; current = next)
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
    struct Binding *current;
    struct Binding *next;
    size_t hash;

    /* Check null arguments */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Traverse the Symbol Table */
    /* Hash the key */
    hash = SymTable_hash(pcKey, oSymTable->bucketSize);

    /* Traverse hashed bucket */
    for (current = oSymTable->buckets[hash]; 
    current != NULL; current = next)
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

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
    struct Binding *current;
    struct Binding *prev;
    const void *value = NULL;
    size_t hash;


    /* Check arguments */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    /* hash the key */
    hash = SymTable_hash(pcKey, oSymTable->bucketSize);

    /* Traverse the list to find pcKey */
    for (current = oSymTable->buckets[hash], prev = NULL; 
    current != NULL && strcmp(current->key, pcKey) != 0; 
    prev = current, current = current->next);

    /* pcKey was not found */
    if (current == NULL)
        return NULL;

    /* pcKey is in the first Node */
    /* current points to the first node */
    else if (prev == NULL)
    {
        value = (void*)current->value;
        /* Assign new first node */
        oSymTable->buckets[hash] = current->next;
    }
    /* pcKey is in some other node */
    else 
    {
        /* Return binding value */
        value = (void*)current->value;
        prev->next = current->next;
    }

    /* Update count */
    oSymTable->bindingCounts--;

    free((char*)current->key);
    free(current);

    return (void*)value;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable, 
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), 
const void *pvExtra) 
{
    size_t i;
    struct Binding *current;

    /* Traverse the entire symtable */
    for (i = 0; i < oSymTable->bucketSize; i++) 
    {
        /* if not NULL, count bindings */
        if (oSymTable->buckets[i] != NULL)
        {
            for (current = oSymTable->buckets[i]; current != NULL; 
            current = current->next)
                {
                    /* call *pfApply */
                    (*pfApply) ((void*)current->key, 
                    (void*)current->value, (void*)pvExtra);
                }
        }

    }

}








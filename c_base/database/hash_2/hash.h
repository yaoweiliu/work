#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHMAXSIZE	1000
#define LOADFACTOR	0.8

typedef	int KeyType;
typedef	int	ValueType;
typedef	size_t (*HashFunc)(KeyType key);

typedef	enum Stat
{
	Empty,
	Valid,
	Invalid
}Stat;

typedef	struct HashElem
{
	KeyType key;
	ValueType value;
	Stat stat;
}HashElem;

typedef	struct HashTable
{
	HashElem data[HASHMAXSIZE];
	size_t size;
	HashFunc hashFunc;
}HashTable;

void HashTableInit(HashTable *ht, HashFunc hashFunc);

int HashTableInsert(HashTable *ht, KeyType key, ValueType value);

int HashTableFind(HashTable *ht, KeyType key, ValueType *value, size_t *cur);

void HashTableRemove(HashTable *ht, KeyType key);

int HashTableEmpty(HashTable *ht);

size_t HashTableSize(HashTable *ht);

void HashTableDestroy(HashTable *ht);

void HashTablePrint(HashTable *ht);

#endif



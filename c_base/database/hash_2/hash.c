#include "hash.h"

size_t HashFuncDefault(KeyType key)
{
	return key%HASHMAXSIZE;
}

void HashTableInit(HashTable *ht, HashFunc hashFunc)
{
	assert(ht != NULL);

	ht->size = 0;
	ht->hashFunc = HashFuncDefault;

	for(size_t i = 0; i < HASHMAXSIZE; i++) {
		ht->data[i].key = 0;
		ht->data[i].stat = Empty;
		ht->data[i].value = 0;
	}
}

int HashTableInsert(HashTable *ht, KeyType key, ValueType value)
{
	assert(ht != NULL || ht->size < HASHMAXSIZE*LOADFACTOR);

	size_t cur = ht->hashFunc(key);
	while(1) {
		if(ht->data[cur].key == key)
			return 0;
		if(ht->data[cur].stat != Valid) {
			ht->data[cur].key = key;
			ht->data[cur].value = value;
			ht->data[cur].stat = Valid;
			ht->size++;
			return 1;
		}
		cur++;
	}
}

int HashTableFind(HashTable *ht, KeyType key, ValueType *value, size_t *cur)
{
	assert(ht != NULL);

	size_t offset = ht->hashFunc(key);
	if(ht->data[offset].key == key && ht->data[offset].stat == Valid) {
		*value = ht->data[offset].value;
		return 1;
	}
	else {
		while(ht->data[offset].stat != Empty) {
			if(ht->data[offset].key != key) {
				offset++;
				if(offset >= HASHMAXSIZE)
					offset = 0;
			}
			else {
				if(ht->data[offset].stat == Valid) {
					*value = ht->data[offset].value;
					return 1;
				}
				else {
					offset++;
				}
			}
		}
		return 0;
	}
}

int HashTableFindCur(HashTable *ht, KeyType key, size_t *cur)
{
	assert(ht != NULL);

	for(size_t i = 0; i < HASHMAXSIZE; i++) {
		if(ht->data[i].key == key && ht->data[i].stat == Valid) {
			*cur = i;
			return 1;
		}
	}
	return 0;	
}

void HashTableRemove(HashTable *ht, KeyType key)
{
	assert(ht != NULL);

	ValueType value = 0;
	size_t cur = 0;
	int ret = HashTableFindCur(ht, key, &cur);
	if(ret == 0)
		return ;
	else {
		ht->data[cur].stat = Invalid;
		ht->size--;
	}
}

int HashTableEmpty(HashTable *ht)
{
	if(ht == NULL)
		return 0;
	else
		return ht->size > 0 ? 1 : 0;
}

size_t HashTableSize(HashTable *ht)
{
	assert(ht != NULL);

	return ht->size;
}

void HashTableDestroy(HashTable *ht)
{
	assert(ht != NULL);

	ht->size = 0;
	ht->hashFunc = HashFuncDefault;

	for(size_t i = 0; i < HASHMAXSIZE; i++) {
		ht->data[i].key = 0;
		ht->data[i].stat = Empty;
		ht->data[i].value = 0;
	}

	free(ht);
}

void HashTablePrint(HashTable *ht)
{
	assert(ht != NULL || ht->size < 0);

	for(size_t i = 0; i < HASHMAXSIZE; i++)
		printf("value: %d\n", ht->data[i].value);
}


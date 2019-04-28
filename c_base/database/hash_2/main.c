#include "hash.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int ret;

	HashTable ht;

	HashTableInit(&ht, NULL);

	HashTableInsert(&ht, 1, 1);
	HashTableInsert(&ht, 2, 2);
	HashTableInsert(&ht, 3, 3);

	HashTablePrint(&ht);

	HashTableDestroy(&ht);

	HashTablePrint(&ht);

	return 0;
}
#pragma once

#include "Record.h"

#define MAXHASH 1001


template <class T> class HashTable
{
private: 
	int hashSize;
	int numberRecords;

	Record<T>** hashTable;
public:

	HashTable()
	{
		hashSize = 0;
		numberRecords = 0;
		hashTable = new Record<T>*[MAXHASH];
	}

	~HashTable()
	{
		delete[] hashTable;
	}

	bool hasValue(int hashKey) {
		Record<T>* record = hashTable[hashKey];
		return record != nullptr && record->isNormal() && !record->isTombstone();
	}

	bool insert(int key, T value, int& collisions) {
		unsigned int hashKey = hash(key);

		if (hasValue(hashKey)) {
			collisions++;
			unsigned int newKey = probe(key, collisions);
			return insert(key, value, collisions);
		}
		else {
			hashTable[hashKey] = new Record<T>(hashKey, value);
			return true;
		}
	}

	bool remove(int key) {
		unsigned int hashKey = hash(key);

		if (!hashTable[hashKey]->isNormal() || hashTable[hashKey]->isTombstone()) {
			return false;
		}
		else {
			// TODO: Determine if I need to delete the Record before nulling the table position.
			hashTable[hashKey].kill();
		}
	}

	bool find(int key, T& value) {
		unsigned int hashKey = hash(key);
		T* value = hashTable[hashKey];
		return value == nullptr;
	}

	float alpha() {
		return numberRecords / hashSize;
	}

	friend ostream& operator<<(ostream& os, const HashTable& table) {
		cout << "Hash Index | Hash Key | Hash Value"
		for (int hash = 0; hash < MAXHASH; hash++) {
			cout << ""
		}
	}

	/* This hash function was found via stackoverflow here: http://stackoverflow.com/a/12996028

		It bit shifts everything right in order to include all of the bits in the output answer.
		I modified the return value using modulo division to make sure the key fits in our range.

		Worst-case time complexity: T(1).
	*/
	unsigned int hash(unsigned int x) {
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		return x % MAXHASH;
	}

	/* This probe function simply just reuses our existing hash function on the key + the number of collisions.

		I'm not yet sure if this is a good idea

		Worst-case time complexity: T(1).
	*/
	unsigned int probe(unsigned int key, unsigned int collisions) {
		// Not sure if this is a good idea or not, needs verification.
		return hash(key + collisions);
	}
};

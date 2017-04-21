#pragma once

#include "Record.h"
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <algorithm>

// Increasing the maximum hash size to 1009 was vital in getting good performance out of my hash function/probe sequence
// 1009 is a prime number, and this greatly reduces the number of collisions encountered after about 40% capacity.
#define MAXHASH 1009


using namespace std;


/* Represent a Hash Table of fixed size, storing values of type T.
*/
template <class T> class HashTable
{
private: 
	int numberRecords;

	vector<int>* probeSequence;

	Record<T>** hashTable;

	/* Print a HashTable to an ostream.

		Only displays records that are not empty, including tombstones.
	*/
	friend ostream& operator<<(ostream& os, const HashTable& table) {
		for (int hashKey = 0; hashKey < MAXHASH; hashKey++) {
			Record<T>* currentRecord = table.hashTable[hashKey];

			if (!currentRecord->isEmpty()) {
				cout << *currentRecord << endl;
			}
		}

		return os;
	}

public:

	/* HashTable constructor. Initialize all arrays, and push indexes into our probe sequence and shuffle for pseudo-random probing.
	*/
	HashTable()
	{
		numberRecords = 0;

		hashTable = new Record<T>*[MAXHASH];
		probeSequence = new vector<int>[MAXHASH]();

		for (int i = 0; i < MAXHASH; i++) {
			hashTable[i] = new Record<T>();
			if (i == 0) {
				probeSequence->push_back(1);
			}
			else {
				probeSequence->push_back(i);
			}
			
		}

		random_shuffle(probeSequence->begin(), probeSequence->end());
	}

	/* HashTable destructor, clear up the hashTable and probeSequence array.
	*/
	~HashTable()
	{
		delete[] hashTable;
		delete[] probeSequence;
	}

	/* Determine if a given hashKey is a valid record with data.
	*/
	bool hasValue(int hashKey) {
		Record<T>* record = hashTable[hashKey];
		return record != nullptr && record->isNormal() && !record->isTombstone();
	}

	/* Insert a new record with given key and value into the table.

		The collisions parameter will be incremented based on the number of collisions for this insert (if any).
	*/
	bool insert(int key, T value, int& collisions) {
		if (numberRecords >= MAXHASH) {
			return false;
		}

		unsigned int hashKey = hash(key);

		while (hasValue(hashKey)) {
			collisions++;
			unsigned int newKey = probe(key, collisions);

			//cout << "Collision for hashKey: " << hashKey << "number of collisions: " << collisions << "next key: " << newKey << endl;

			if (!hasValue(newKey)) {
				hashKey = newKey;
			}
		}

		hashTable[hashKey] = new Record<T>(hashKey, value);
		numberRecords++;
		return true;
	}

	/* Remove a given key from the table.

		Marks records as tombstones. 
		If the record is not found in the table, this function returns false.
	*/
	bool remove(int key) {
		unsigned int hashKey = hash(key);

		if (hasValue(hashKey)) {
			hashTable[hashKey]->kill();
			return true;
		}
		else {
			// TODO: Determine if I need to delete the Record before nulling the table position.
			return false;
		}
	}

	/* Find a given key in the table, and update value to be a copy of the value stored there.
	*/
	bool find(int key, T& value) {
		unsigned int hashKey = hash(key);
		Record<T>* record = hashTable[hashKey];
		value = record->getValue();

		return hasValue(hashKey);
	}

	/* Return the current loading factor of our table.
	*/
	float alpha() {
		return numberRecords / MAXHASH;
	}

	/* This hash function was found via stackoverflow here: http://stackoverflow.com/a/665545
		
		This function seemed to get the most uniform distribution of keys for this hash size out of a few options that were tried, 
		which resulted in the fewest number of collisions.

		It uses multiplication with a relatively large number, so it may be considerably slower than other alternatives.
	*/
	unsigned int hash(unsigned int key) {
		return key * 2654435761 % (MAXHASH);
	}

	/* This hash function was found via stackoverflow here: http://stackoverflow.com/a/12996028

		It bit shifts everything right in order to include all of the bits in the output answer.
		I modified the return value using modulo division to make sure the key fits in our range.

		Worst-case time complexity: T(1).
	*/
	unsigned int probeHash(unsigned int key) {
		key = ((key >> 16) ^ key) * 0x45d9f3b;
		key = ((key >> 16) ^ key) * 0x45d9f3b;
		key = (key >> 16) ^ key;
		return key % MAXHASH;
	}

	/* This probe function simply just reuses our existing hash function on the key + the number of collisions.

		I'm not yet sure if this is a good idea

		Worst-case time complexity: T(1).
	*/
	unsigned int probe(unsigned int key, unsigned int collisions) {
		// Not sure if this is a good idea or not, needs verification.
		unsigned int offset = probeSequence->at(hash(key));
		return (hash(key) + offset * collisions) % MAXHASH;
		//return (hash(key) + collisions * probeHash(key)) % MAXHASH;
	}

	void clear() {
  		delete[] hashTable;

		hashTable = new Record<T>*[MAXHASH];

		for (int i = 0; i < MAXHASH; i++) {
			hashTable[i] = new Record<T>();
		}

		numberRecords = 0;
	}
};

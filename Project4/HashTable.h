#pragma once

#include "Record.h"
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <algorithm>

#define MAXHASH 1009


using namespace std;


template <class T> class HashTable
{
private: 
	int numberRecords;

	vector<int>* probeSequence;

	Record<T>** hashTable;

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

	~HashTable()
	{
		delete[] hashTable;
		delete[] probeSequence;
	}

	bool hasValue(int hashKey) {
		Record<T>* record = hashTable[hashKey];
		return record != nullptr && record->isNormal() && !record->isTombstone();
	}

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

	bool find(int key, T& value) {
		unsigned int hashKey = hash(key);
		Record<T>* record = hashTable[hashKey];
		value = record->getValue();

		return hasValue(hashKey);
	}

	float alpha() {
		return numberRecords / MAXHASH;
	}

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

	unsigned int oldHash(unsigned int key) {
		key = ((key << 16) ^ key) * 0x49fd3fb;
		key = ((key >> 16) ^ key) * 0x49fd3fb;
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

// Project4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <cmath>

#include "HashTable.h"


using namespace std;


int main()
{
	HashTable<string>* myHashTable = new HashTable<string>();
	int collisions = 0;
	int totalCollisions = 0;

	for (int i = 0; i < 50; i++) {
		myHashTable->insert(i, "test" + to_string(i), collisions);
		totalCollisions += collisions;
		collisions = 0;
	}

	for (int i = 0; i < 20; i++) {
		string value;
		myHashTable->find(i, value);
		cout << value << endl;
	}

	for (int i = 0; i < 35; i++) {
		myHashTable->remove(i);
	}

	myHashTable->remove(7000);

	cout << *myHashTable;

	myHashTable->clear();

	totalCollisions = 0;

	cout << "Sequential inserting" << endl;

	for (int numRecords = 0; numRecords < MAXHASH; numRecords++) {
		bool success = myHashTable->insert(numRecords, "test" + to_string(numRecords), collisions);
		totalCollisions += collisions;
		collisions = 0;

		//cout << "Inserted: " << "test" + to_string(numRecords) << " - " << success << endl;

		if (numRecords % 100 == 0) {
			cout << "Total number of collisions for " << numRecords << " inserts: " << totalCollisions << endl;
			totalCollisions = 0;
		}
	}

	myHashTable->clear();

	totalCollisions = 0;

	cout << "Randomized inserting" << endl;

	for (int numRecords = 0; numRecords < MAXHASH; numRecords++) {
		int currentIndex = fmod(pow(numRecords, 2), MAXHASH);

		bool success = myHashTable->insert(currentIndex, "test" + to_string(currentIndex), collisions);
		totalCollisions += collisions;
		collisions = 0;

		//cout << "Inserted: " << "test" + to_string(currentIndex) << " - " << success << endl;

		if (numRecords % 100 == 0) {
			cout << "Total number of collisions for " << numRecords << " inserts: " << totalCollisions << endl;
			totalCollisions = 0;
		}
	}

	system("pause");
    return 0;
}


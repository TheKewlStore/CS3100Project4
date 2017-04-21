// Project4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <cmath>

#include "HashTable.h"


using namespace std;


void collisionsTest() {
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
}


void cpuTest(int totalRecords) {
	HashTable<string>* myHashTable = new HashTable<string>();
	int collisions = 0;
	int totalCollisions = 0;

	for (int numRecords = 0; numRecords < totalRecords; numRecords++) {
		int currentIndex = fmod(pow(numRecords, 2), MAXHASH);

		bool success = myHashTable->insert(currentIndex, "test" + to_string(currentIndex), collisions);
		totalCollisions += collisions;
		collisions = 0;
	}

	cout << "Total number of collisions for " << totalRecords << " inserts: " << totalCollisions << endl;
}


void cpuTest100() {
	cpuTest(100);
}


void cpuTest200() {
	cpuTest(200);
}


void cpuTest300() {
	cpuTest(300);
}


void cpuTest400() {
	cpuTest(400);
}


void cpuTest500() {
	cpuTest(500);
}


void cpuTest600() {
	cpuTest(600);
}


void cpuTest700() {
	cpuTest(700);
}


void cpuTest800() {
	cpuTest(800);
}


void cpuTest900() {
	cpuTest(900);
}


void cpuTest1000() {
	cpuTest(1000);
}


int main()
{
	collisionsTest();
	/*cpuTest100();
	cpuTest200();
	cpuTest300();
	cpuTest400();
	cpuTest500();
	cpuTest600();
	cpuTest700();
	cpuTest800();
	cpuTest900();
	cpuTest1000();*/

	system("pause");
    return 0;
}


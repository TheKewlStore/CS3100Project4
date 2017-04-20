// Project4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>

#include "HashTable.h"


using namespace std;


int main()
{
	HashTable<string>* myHashTable = new HashTable<string>();
	int collisions;

	myHashTable->insert(0, "test", collisions);

    return 0;
}


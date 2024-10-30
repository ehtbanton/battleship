#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <vector>
using namespace std;

class Ship {
public:
	vector<int> start_coords;
	int orientation;
	int size;
	vector<int> hits;

	Ship(vector<int> start_coords, int orientation, int size);
};

#endif // SHIP_H
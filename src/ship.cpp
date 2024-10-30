#include "ship.h"
using namespace std;

vector<int> start_coords;
int orientation;
int size;
vector<int> hits;

Ship::Ship(vector<int> start_coords, int orientation, int size) {
	this->start_coords = start_coords;
	this->orientation = orientation;
	this->size = size;
	this->hits = vector<int>(size, 0);
}
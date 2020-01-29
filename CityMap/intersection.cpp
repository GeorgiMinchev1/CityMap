#include "intersection.h"
#include <iostream>

Intersection::Intersection() {
	name = std::string();
	_isClosed = false;
	neighbours = std::vector <std::pair<Intersection*, int>>();
}

Intersection::Intersection(std::string _name) {
	name = _name;
	_isClosed = false;
	neighbours = std::vector <std::pair<Intersection*, int>>();
}

Intersection::Intersection(Intersection const& inter) {
	name = inter.name;
	_isClosed = inter._isClosed;
	neighbours = inter.neighbours;
}

Intersection& Intersection::operator=(Intersection const& inter) {
	name = inter.name;
	_isClosed = inter._isClosed;
	neighbours = inter.neighbours;
	return *this;
}

void Intersection::addNeighbour(Intersection* neigh, int distance) {
	neighbours.push_back(std::make_pair(neigh, distance));
}

void Intersection::addIncomingNeighbour(Intersection* inter) {
	incomingNeighbours.push_back(inter);
}

bool Intersection::hasNeighbour(std::string name) {
	for (int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i].first->getName() == name)return true;
	}
	return false;
}

int Intersection::neighbourCount() {
	return neighbours.size();
}

int Intersection::incomingNeighbourCount() {
	return incomingNeighbours.size();
}

void Intersection::print(std::ostream& os) {
	os << name ;
	for (std::pair<Intersection*, int> neigh : neighbours) {
		//std::string neighName = neighbours;//;[i].first()->getName();
		os << " " << neigh.first->getName() << " "<< neigh.second;
	}
	os << std::endl;
}

void Intersection::printNeighbours(std::ostream& os) {
	for (std::pair<Intersection*, int> neigh : neighbours) {
		//std::string neighName = neighbours;//;[i].first()->getName();
		os << neigh.first->getName() << " " << neigh.second << " ";
	}
}

void Intersection::close() {
	_isClosed = true;
}

void Intersection::open() {
	_isClosed = false;
}

bool Intersection::isClosed() {
	return _isClosed;
}

std::pair<Intersection*, int>* Intersection::getNeighbour(std::string name) {
	for (std::pair<Intersection*, int> neigh : neighbours) {
		if(name == neigh.first->getName())
			return &neigh;
	}
	return nullptr;
}

std::vector<std::pair<Intersection*, int>> Intersection::getNeighbours() {
	return neighbours;
}

std::vector <Intersection*> Intersection::getIncomingNeighbours() {
	return incomingNeighbours;
}
/*
Intersection::~Intersection() {
	using iterator = std::vector<std::pair<Intersection*, int>>::iterator;
	iterator itNeigh = neighbours.begin();
	itNeigh = neighbours.erase(itNeigh);
	for (itNeigh; itNeigh != neighbours.end(); ++itNeigh) {
		itNeigh = neighbours.erase(itNeigh);
	}
	neighbours.clear();

	using iterator1 = std::vector<Intersection*>::iterator;
	iterator1 itInc = incomingNeighbours.begin();
	itInc = incomingNeighbours.erase(itInc);
	for (itInc; itInc != incomingNeighbours.end(); ++itInc) {
		itInc = incomingNeighbours.erase(itInc);
	}
	incomingNeighbours.clear();
}*/
#ifndef __CITYMAP_H
#define __CITYMAP_H
#include<vector>
#include <string>
#include "intersection.h"


using Path = std::pair<std::vector<Intersection>, int>;

//class City;

class Location {
	std::vector<Intersection*>* allIntersections;
	std::vector<std::string> closed;
	Intersection* current;
	Intersection* findIntersection(std::string name);

public:
	Location(std::string, std::vector<Intersection*>*);
	Location(Intersection*, std::vector<Intersection*>*);

	bool hasIntersection(std::vector<Intersection> intersections, std::string name);
	std::pair<std::vector<Intersection>, int> findShortestPath(std::vector<std::pair<std::vector<Intersection>, int>>);
	std::vector<std::pair<std::vector<Intersection>, int>> allPathsBetween(std::vector<Intersection> currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<std::pair<std::vector<Intersection>, int>>& result, bool considerClosed);
	std::pair<std::vector<Intersection>, int> getShortestPath(std::string target,bool considerClosed = true, std::ostream& os = std::cout);

	Location move(std::string, std::ostream& os);//използвам функцията ~showShorgestPaths
	Location change(std::string, std::ostream& os);

	void show(std::ostream& os);
	void showNeighbours(std::ostream& os);

	void close(std::string);
	void open(std::string);
	void showClosed(std::ostream& os);
};

class City {
	std::vector<Intersection*> map;
	bool hasPathBetweenHelper(std::vector<Intersection>& visited, Intersection inter1, Intersection inter2);
	bool hasIntersection(std::vector<Intersection> intersections, std::string name);
	
	std::vector<Intersection> findBigCycle(Intersection start, Intersection end, std::vector<Intersection> visited, std::vector<Intersection> result);
	void printBigCycle(Intersection start, Intersection end, std::ostream& os);
	std::vector<Path> allPathsBetween(std::vector<Intersection> currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<Path>& result, bool considerClosed);
	void printPaths(std::vector<Path> paths, std::ostream& out);
	Path findCycle(Intersection start, Intersection current, std::vector<Intersection> path, std::vector<Intersection>& visited, int length, bool considerClosed);
	std::vector<Path> expandPaths(std::vector<Path>& paths, bool considerClosd);
	std::vector<Path> reduceAndSortPaths(std::vector<Path> paths);

public:
	City() {
		
	}

	bool addIntersection(Intersection* inter);
	using iterator = std::vector<Intersection*>::iterator;

	iterator getIterator();
	
	iterator begin() {
		return map.begin();
	}
	
	iterator end() {
		return map.end();
	}
	/*iterator next(iterator it) {
		return it++;
	}*/

	void importIntersections(std::string path);
	void exportIntersections(std::string path);

	bool isUnique(std::string name);

	Location getLocation(std::string);

	Intersection* buildIntersection(std::string expression);//ако има такова кръстовище се вика функцията за адване на съседи
	Intersection* getIntersection(std::string name);

	void print();

	bool hasPathBetween(std::string name1, std::string name2);
	std::vector<std::pair<std::vector<Intersection>, int>> getShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os);
	void showTheShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os);
	bool hasSmallTour(std::string name1);
	bool hasTourToAllIntersections(std::ostream& os);
	bool canGoToAllIntersections(std::string name);
	void showDeadEnds(std::ostream& os);

	//~City();
};

#endif
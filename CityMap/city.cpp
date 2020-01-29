#include <iostream>
#include <sstream>
#include <queue>
#include <cmath>
#include <fstream>

#include "intersection.h"
#include "city.h"

Location::Location(std::string name, std::vector<Intersection*>* intersections) {
	closed = std::vector<std::string>();
	allIntersections = intersections;
	current = findIntersection(name);
}

Location::Location(Intersection* inter, std::vector<Intersection*>* intersections) {
	current = inter;
	closed = std::vector<std::string>();
	allIntersections = intersections;
}

bool Location::hasIntersection(std::vector<Intersection> intersections, std::string name) {
	using iterator = std::vector<Intersection>::iterator;
	for (int i = 0; i < intersections.size(); i++) {
		if (intersections[i].getName() == name) return true;
	}
	return false;
}

std::vector<std::pair<std::vector<Intersection>, int>> Location::allPathsBetween(std::vector<Intersection> currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<std::pair<std::vector<Intersection>, int>>& result, bool considerClosed) {
	if (hasIntersection(currPath, inter1.getName()))
		return result;
	if (considerClosed && inter1.isClosed())
		return result;

	currPath.push_back(inter1);

	for (std::pair<Intersection*, int> neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		allPathsBetween(currPath, currLength + neigh.second, next, inter2, result, considerClosed);
	}

	if (inter1.hasNeighbour(inter2.getName())) {
		currPath.push_back(inter2);
		result.push_back(std::make_pair(currPath, currLength + inter1.getNeighbour(inter2.getName())->second));
		return result;
	}

	return result;
}

std::pair<std::vector<Intersection>, int> Location::findShortestPath(std::vector<std::pair<std::vector<Intersection>, int>> paths) {
	int minVal = 645000;
	std::pair<std::vector<Intersection>, int> min;
	for (std::pair<std::vector<Intersection>, int> path : paths) {
		if (minVal > path.second) {
			min = path;
			minVal = path.second;
		}
	}
	return min;
}

std::pair<std::vector<Intersection>, int> Location::getShortestPath(std::string target, bool considerClosed, std::ostream& os) {
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;
	//std::pair<std::vector<Intersection>, int> result;
	
	Intersection inter2 = *(findIntersection(target));
	Paths temp;
	Paths paths = allPathsBetween(std::vector<Intersection>(), 0, *current, inter2, temp, considerClosed);
	if (paths.size() == 0) {
		os << "There is no path between these intersections, it might be beacuse of closed intersection." << std::endl;
		return std::make_pair(std::vector<Intersection>(), 0);
	}
	return findShortestPath(paths);
}


Location Location::move(std::string name, std::ostream& os) {
	std::pair<std::vector<Intersection>, int> shortestPath = getShortestPath(name, true, os);
	if (shortestPath.first.size() > 0) {
		for (Intersection inter : shortestPath.first) {
			os << inter.getName() << " ";
		}
		os << std::endl;
		return change(name, os);
	}
	return *this;
}

Location Location::change(std::string name, std::ostream& os) {
	Intersection* temp = findIntersection(name);
	if (!temp)
		os << "There is no intersection with this name, so we didn't change it." << std::endl;
	else
		current = temp;

	return *this;
}

Intersection* Location::findIntersection(std::string name) {
	for (Intersection* inter : *allIntersections) {
		if (inter->getName() == name) {
			return inter;
		}
	}
	return nullptr;
}

void Location::show(std::ostream& os) {
	os << current->getName() << std::endl;
}

void Location::showNeighbours(std::ostream& os) {
	current->printNeighbours(os);
}

void Location::close(std::string name) {
	//current->close();
	Intersection* temp = findIntersection(name);
	temp->close();
	closed.push_back(temp->getName());
}

void Location::open(std::string name) {
	Intersection* temp = findIntersection(name);
	temp->open();
	if (closed.size() > 0) {
		if (closed[closed.size() - 1] == name)closed.pop_back();
		else
			for (std::vector<std::string>::iterator it = closed.begin(); it < closed.end(); it++) {
				if ((*it) == name)it = closed.erase(it);
			}
	}
	else {
		std::cout << "No closed intersections!" << std::endl;
	}
}

void Location::showClosed(std::ostream& os) {
	if (closed.size() == 0) {
		os << "There are no closed intersections" << std::endl;
		return;
	}
	for (std::string name : closed) {
		os << name << " ";
	}
	os << std::endl;
}

bool City::isUnique(std::string name) {
	Intersection* inter = getIntersection(name);
	if (inter == nullptr)return true;
	return false;
}

bool City::addIntersection(Intersection* inter) {
	if (inter != nullptr && isUnique(inter->getName())) {
		map.push_back(inter);
		return true;
	}
	return false;
}

bool City::hasIntersection(std::vector<Intersection> intersections, std::string name) {
	using iterator = std::vector<Intersection>::iterator;
	for (int i = 0; i < intersections.size();i++) {
		if (intersections[i].getName() == name) return true;
	}
	return false;
}

bool isInVector(std::vector<std::string> list, std::string str, int index) {
	for (int i = 0; i < list.size(); i++)if (str.compare(list[i]) == 0 && i != index)return true;
	return false;
}

Intersection* City::getIntersection(std::string name) {
	using vectorIterator = std::vector<Intersection*>::iterator;
	for (iterator it = map.begin(); it != map.end(); ++it) {
		if (name.compare((*it)->getName()) == 0)return *it;
	}
	return nullptr;
}

std::vector<Intersection*> ::iterator City::getIterator() {
	return std::vector<Intersection*>::iterator();
}

Intersection* City::buildIntersection(std::string expression) {
	
	//unzipping expression 
	std::vector<std::string> strings;
	int index = 0;
	int nextIndex = expression.find_first_of(" ", index + 1);
	while (nextIndex != std::string::npos) {
		strings.push_back(expression.substr(index, nextIndex - index));
		index = nextIndex + 1;
		nextIndex = expression.find_first_of(" ", index + 1);
	}
	strings.push_back(expression.substr(index, expression.length()));
	//assigning intersection name
	Intersection *intersection = getIntersection(strings[0]);
	if (intersection == nullptr) {
		intersection = new Intersection(strings[0]);
		addIntersection(intersection);
	}
	if (intersection->neighbourCount() > 0 && strings.size() > 1)return nullptr;

	for (int i = 1; i < strings.size(); i += 2) {
		if (strings[i].compare(strings[0]) == 0) return nullptr;
		if (isInVector(strings, strings[i], i))return nullptr;
		//checking if the neighbour has the same name as the intersection
		Intersection* inters1 = getIntersection(strings[i]);
		std::stringstream stream(strings[i + 1]);
		int distance;
		stream >> distance;
		if (inters1 == nullptr) {
			//defining the neighbour
			inters1 = new Intersection(strings[i]);
			map.push_back(inters1);
			//this->addIntersection(inters1);
			//inters1 = strings[i];
			//using stringstream to make the string -> integer

			//adding neighbour
		}

		intersection->addNeighbour(inters1, distance);
		inters1->addIncomingNeighbour(intersection);
	}
	return intersection;
	
}

void City::importIntersections(std::string path) {
	std::ifstream is(path, std::ios::in);
	std::string line;
	getline(is, line);
	while (line != "\0") {
		buildIntersection(line);
		getline(is, line);
	}
}

void City::exportIntersections(std::string path) {
	std::ofstream os(path, std::ios::out);
	iterator it = map.begin();
	for (it; it < map.end(); it++)
		(*it)->print(os);
}

void City::print() {
	std::cout << std::endl << "All the intersections added in city are: " << std::endl;
	int i = 0;
	for (Intersection* inter : map) {
		std::cout <<++i<< ". intersection is" << std::endl;
		inter->print(std::cout);
		std::cout << "Incoming neighbour count = " << inter->incomingNeighbourCount() << std::endl;
	}
	std::cout << std::endl;
}

Location City::getLocation(std::string name) {
	return Location (name, &map);
}

void City::printPaths(std::vector<std::pair<std::vector<Intersection>, int>> paths, std::ostream& out) {
	int i = 0;
	for (std::pair<std::vector<Intersection>, int> path : paths) {
		out << "Path " << ++i <<". ";
		for (Intersection inter : path.first) {
			out << inter.getName() << " ";
		}
		out << "has length " << path.second << std::endl;
	}
}

bool City::hasPathBetweenHelper(std::vector<Intersection>& visited, Intersection inter1, Intersection inter2) {
	if (hasIntersection(visited, inter1.getName()))
		return false;

	if (inter1.hasNeighbour(inter2.getName()))
		return true;
		
	visited.push_back(inter1);
	for (std::pair<Intersection*, int> neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		if (hasPathBetweenHelper(visited, next, inter2))
			return true;
	}

	return false;
}

bool City::hasPathBetween(std::string name1, std::string name2) {
	Intersection inter1 = *(getIntersection(name1));
	Intersection inter2 = *(getIntersection(name2));
	std::vector<Intersection> temp;
	return hasPathBetweenHelper(temp, inter1, inter2);
}

std::vector<std::pair<std::vector<Intersection>, int>> City::allPathsBetween(std::vector<Intersection> currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<std::pair<std::vector<Intersection>, int>>& result, bool considerClosed) {
	if (hasIntersection(currPath, inter1.getName()))
		return result;
	if (considerClosed && inter1.isClosed())
		return result;

	currPath.push_back(inter1);

	for (std::pair<Intersection*, int> neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		allPathsBetween(currPath, currLength + neigh.second, next, inter2, result, considerClosed);
	}

	if (inter1.hasNeighbour(inter2.getName())) {
		currPath.push_back(inter2);
		result.push_back(std::make_pair(currPath, currLength + inter1.getNeighbour(inter2.getName())->second));
		return result;
	}

	return result;
}

std::pair<std::vector<Intersection>, int> City::findCycle(Intersection start, Intersection current, std::vector<Intersection> path, std::vector<Intersection>& visited, int length, bool considerClosed) {
	std::pair<std::vector<Intersection>, int> holder = std::make_pair(path, length);
	if (hasIntersection(visited, current.getName()))
		return holder;

	visited.push_back(current);
	path.push_back(current);

	if (current.hasNeighbour(start.getName())) {
		path.push_back(start);
		return std::make_pair(path, (length + current.getNeighbour(start.getName())->second));
	}

	bool hasPathInANeighbour = false;
	std::pair<std::vector<Intersection>, int>* result = nullptr;
	for (std::pair<Intersection*, int> neigh : current.getNeighbours()) {
		Intersection next = *(neigh.first);
		if (!considerClosed || !next.isClosed()) {
			std::pair<std::vector<Intersection>, int> cycle = findCycle(start, next, path, visited, length + neigh.second, considerClosed);
			if (cycle.second != 0 && (start.getName() == cycle.first[(cycle.first.size() - 1)].getName()))
				return cycle;
		}
		
	}
	return holder;
}

std::vector<Intersection> expandAPath(std::vector<Intersection> path, std::vector<Intersection> cycle) {
	std::vector<Intersection>::iterator it1 = path.begin();
	for (it1; it1 < path.end(); it1++) {
		if ((*it1).getName() == cycle[0].getName()) 
			break;
	}

	path.insert(it1 + 1, cycle.begin() + 1, cycle.end());
	return path;
}

std::vector<std::pair<std::vector<Intersection>, int>> City::expandPaths(std::vector<std::pair<std::vector<Intersection>, int>>& paths, bool considerClosd) {
	std::pair<std::vector<Intersection>, int> smallest = paths[0];
	for (Intersection inter : smallest.first) {
		std::vector<Intersection> temp;
		
		std::pair<std::vector<Intersection>, int> cycle = findCycle(inter, inter, std::vector<Intersection>(), temp, 0, considerClosd);
		std::vector<Intersection> expanded = expandAPath(paths[0].first, cycle.first);
		std::vector<std::pair<std::vector<Intersection>, int>> result;
		if (cycle.second != 0) {
			result = paths;

			if (paths.size() == 1) {
				result.push_back(std::make_pair(expanded, cycle.second + smallest.second));
				result.push_back(std::make_pair(expandAPath(expanded, cycle.first), 2*cycle.second + smallest.second));
			}
			else if (paths.size() > 1 && (cycle.second + smallest.second) < paths[1].second) {
				std::pair<std::vector<Intersection>, int> expanded1 = std::make_pair(expanded, (cycle.second + smallest.second));
				if (paths.size() == 2 && (2 * cycle.second + smallest.second) >= paths[1].second)
					result.push_back(paths[1]);
				else if (paths.size() == 2)
					result.push_back(std::make_pair(expandAPath(expanded1.first, cycle.first), 2 * cycle.second + smallest.second));
				else if ((2 * cycle.second + smallest.second) >= paths[1].second) 
					result[2] = paths[1];
				else 
					result[2] = std::make_pair(expandAPath(expanded1.first, cycle.first), 2 * cycle.second + smallest.second);
				result[1] = expanded1;
			}
			/*else if (paths.size() == 2 && (cycle.second + smallest.second) < paths[1].second) {
				result.push_back(paths[1]);
				result[1] = std::make_pair(expandAPath(paths[0].first, cycle.first), cycle.second + smallest.second);
			}*/
			else if (paths.size() == 2)
				result.push_back(std::make_pair(expandAPath(paths[0].first, cycle.first), cycle.second + smallest.second));
				
			else if (paths.size() > 2 && (cycle.second + smallest.second) < paths[2].second)
				result[2] = std::make_pair(expandAPath(paths[0].first, cycle.first), cycle.second + smallest.second);
				/*paths[2] = paths[1];
				paths[1] = std::make_pair(expandAPath(paths[0].first, cycle.first), cycle.second + smallest.second);*/
			return result;
		}
		std::cout << "ended" << std::endl;
	}
	return paths;
}

std::vector<std::pair<std::vector<Intersection>, int>> City::reduceAndSortPaths(std::vector<std::pair<std::vector<Intersection>, int>> paths) {
	int resultSize = (paths.size() < 3) ? paths.size() : 3;
	std::vector<std::pair<std::vector<Intersection>, int>> result;
	int min;
	for (int i = 0; i < resultSize; i++) {
		min = paths[i].second;
		for (int j = i + 1; j < paths.size(); j++)
			if (paths[j].second < min) {
				std::pair<std::vector<Intersection>, int> temp = paths[i];
				paths[i] = paths[j];
				paths[j] = temp;
				min = paths[i].second;
			}
	}
	for (int i = 0; i < resultSize; i++) {
		result.push_back(paths[i]);
	}
	return result;
}

std::vector<std::pair<std::vector<Intersection>, int>> City::getShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os) {
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;
	Paths result;
	if (!hasPathBetween(name1, name2))
		return result;

	Intersection inter1 = *(getIntersection(name1));
	Intersection inter2 = *(getIntersection(name2));

	Paths temp;
	Paths paths = allPathsBetween(std::vector<Intersection>(), 0, inter1, inter2, temp, considerClosed);
	if (paths.size() == 0) {
		os << "There is no path between these intersections, it might be beacuse of closed intersection." << std::endl;
		return result;
	}
	Paths reducedPaths = reduceAndSortPaths(paths);//sorted paths - maximum 3 
	result = expandPaths(reducedPaths, considerClosed);
	return result;
}

void City::showTheShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os) {
	//function find all paths between inter1 and inter 2 using BFS
	//expand the current paths by adding some loops
	//expand paths -> need function that returns <path, int> for the cycles to show
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;
	Paths result = getShortestPaths(name1, name2, considerClosed, os);
	if (result.size() > 0)
		printPaths(result, os);
	else
		os << "There is no path between these intersections" << std::endl;
}

bool City::hasSmallTour(std::string name1) {
	Intersection inter1 = *getIntersection(name1);
	std::vector<Intersection> temp;
	return hasPathBetweenHelper(temp, inter1, inter1);
}

std::vector<Intersection> City::findBigCycle(Intersection start, Intersection end, std::vector<Intersection> visited, std::vector<Intersection> result) {
	//check for every intersection its cycles(if it has) then visit them and continue with the neighbours
	/*if (hasIntersection(result, start.getName()))
		return result;
		
	result.push_back(start);

	if (start.hasNeighbour(end.getName())) {
		std::cout << result.size() << "    " <<  map.size() << std::endl;
		if (result.size() == map.size() - 1) {
			std::cout << "in here gothcha" << std::endl;
			result.push_back(end);
			return result;
		}
	}
	//operator == for Intersection 
	//another function that has vector<Inter.> for remembering the visited intersection it would use recursion and getNeighbours to move into the graph
	for (int i = 0; i < start.neighbourCount(); i++) {
		Intersection next = *(start.getNeighbours()[i].first);
		std::vector<Intersection> temp = findBigCycle(next, end, result);
		std::cout << temp.size() << std::endl;
		if (temp.size() == map.size()) {
			result = temp;
			break;
		}
	}
	*/
	return result;
}

void City::printBigCycle(Intersection start, Intersection end, std::ostream& os) {
	std::vector<Intersection> path = findBigCycle(start, end, std::vector<Intersection>(), std::vector<Intersection>());
	for (Intersection inter : path) {
		os << inter.getName();
	}
	os << std::endl;
}

bool City::hasTourToAllIntersections(std::ostream& os) {
	// ако НДУ за ойлеров път(функция за НДУ за ойлеров път/имплементиране на място след което взимаме 1 от 2-та върха с +-1 incoming nejghbours и викаме canGoToAllIntersections)
	int outcomingCount = 0;
	int incomingCount = 0;
	
	Intersection* start = nullptr;
	Intersection* end = nullptr;

	for (iterator it = map.begin(); it != map.end(); ++it) {
		if (outcomingCount > 1 || incomingCount > 1)
			return false;

		if ((*it)->neighbourCount() == (*it)->incomingNeighbourCount()) {
			if (start == nullptr) {
				start = (*it);
				end = (*it);
			}
		}
		else if((*it)->neighbourCount() - (*it)->incomingNeighbourCount() == -1){
			end = (*it);
			incomingCount++;
		}
		else if (((*it)->neighbourCount() - (*it)->incomingNeighbourCount()) == 1) {
			start = (*it);
			outcomingCount++;
		}
		else 
			return false;
	}

	if ((incomingCount - outcomingCount) != 0)return false;

	if (!canGoToAllIntersections(start->getName()))
		return false;

	printBigCycle(*start, *end, os);

	return true;
}

bool City::canGoToAllIntersections(std::string name) {
	// bfs with keeping visited till the end then checking if all the inters are visited
	Intersection inter = *getIntersection(name);
	std::vector<Intersection> visited;

	std::queue<Intersection> next;
	next.push(inter);

	while (!next.empty()) {
		visited.push_back(inter);
		if (visited.size() == map.size())
			return true;

		for (std::pair<Intersection*, int> neigh : inter.getNeighbours()) {
			Intersection temp = *(neigh.first);
			if(!hasIntersection(visited, temp.getName()))next.push(temp);
		}
		
		next.pop();
		if(!next.empty())inter = next.front();
	}
	return false;
}

void City::showDeadEnds(std::ostream& os) {
	//interating through the inttersections in city when intersection with no neighbours found then output all of its incoming neighbours->this intersection
	std::vector<Intersection*>::iterator it = map.begin();
	for (it; it < map.end(); it++) {
		if ((*it)->neighbourCount() == 0) {
			std::vector<Intersection*> neighbours = (*it)->getIncomingNeighbours();
			for (unsigned j = 0; j < (*it)->incomingNeighbourCount(); j++) {
				os << neighbours[j]->getName() << "->";
				os << (*it)->getName() << std::endl;
			}
		}
		
	}
}
/*
City::~City() {
	using iterator = std::vector<Intersection*>::iterator;
	iterator it = map.begin();
	it = map.erase(it);
	for (it; it != map.end(); ++it) {
		it = map.erase(it);
	}
	map.clear();
}*/
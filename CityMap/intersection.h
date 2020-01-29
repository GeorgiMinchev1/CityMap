#ifndef _1_INTERSECTION_H
#define _1_INTERSECTION_H
#include<string>
#include<vector>
#include<utility>

class Intersection {
    std::string name;
    bool _isClosed;
    std::vector <std::pair<Intersection*, int>> neighbours;
    std::vector <Intersection*> incomingNeighbours;

public:
    Intersection();

    Intersection(std::string nam);

    Intersection(Intersection const& inter);

    Intersection& operator= (Intersection const& inter);

    void print(std::ostream& os);
    void printNeighbours(std::ostream& os);

    std::string getName() {
        return this->name;
    }

    void close();
    void open();
    bool isClosed();

    void addNeighbour(Intersection* neigh, int distance);//setNeighbours?
    void addIncomingNeighbour(Intersection* neigh);

    bool hasNeighbour(std::string name);

    int neighbourCount();
    int incomingNeighbourCount();

    std::pair<Intersection*, int>* getNeighbour(std::string name);
    std::vector <std::pair<Intersection*, int>> getNeighbours();
    std::vector <Intersection*> getIncomingNeighbours();

    //~Intersection();
};

#endif

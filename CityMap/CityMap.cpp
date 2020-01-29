#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>

//#include "doctest.h";
#include "city.h"
#include "intersection.h"

/*
// CityMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
*/

void uniquenessTest(City map) {
    bool match = false;
    using iterator = City::iterator;
    iterator it = map.getIterator();
    for (it = map.begin(); it != map.end(); ++it) {
        iterator temp = it;
        iterator it1;
        if(temp != map.end())it1 = ++temp;

        while (it1 != map.end()) {
            if ((*it) == (*it1))match = true;
            ++it1;
        }
    }
    std::cout << !match << std::endl;
}

void function1Tests() {

}

void function2Tests() {

}

void function3Tests() {

}

void function4Tests() {

}

void function5Tests() {

}

void function6Tests() {

}

void function7Tests() {

}

std::vector<std::string> proccessInput(std::string input) {
    int pos = 0;
    //int previous = 0;
    std::vector<std::string> result;
    //previous = pos;
    pos = input.find_first_of(' ', pos);
    result.push_back(input.substr(0, pos));

    result.push_back(input.substr(pos + 1, input.length() - pos - 1));
    return result;
}

int main()
{
    std::cout << "For every test 1/true is passed, 0/false is failed, unless the result is not boolean" << std::endl;
    std::cout << "Constructs an Intersection with given name" << std::endl;
    City city;
    Intersection *ndk = city.buildIntersection("NDK");
    if (city.getIntersection("NDK"))std::cout << true;
    else std::cout << false;
    std::cout << std::endl;

    
    city.importIntersections("intersections.txt");

    /*std::cout << "Constructs an Intersection with given name and neighbours" << std::endl;
    Intersection *ndk1 = city.buildIntersection("NDK Popa 150 5Kiosheta 800");
    //ndk1->print();
    std::cout << std::endl;

    std::cout << "Intersection does not let neighbour with the same name be built" << std::endl;
    Intersection *ndk2 = city.buildIntersection("NDK NDK 150");
    if (ndk2 == nullptr)std::cout << 1 << std::endl << std::endl;
    else std::cout << 0 << std::endl << std::endl;

    std::cout << "Intersection does not let two neighbours with the same name be built" << std::endl;
    Intersection *popa1 = city.buildIntersection("Popa 5Kiosheta 800 NDK 400 5Kiosheta 450");
    if (popa1 == nullptr)std::cout << 1 << std::endl << std::endl;
    else std::cout << 0 << std::endl << std::endl;

    std::cout << "An Intersection can be added in Map\n" << std::endl;
    Intersection *popa2 = city.buildIntersection("Popa NDK 400 5Kiosheta 800");
    
    std::cout<< "Many Intersections can be added in Map\n" <<std::endl;
    Intersection *kiosheta = city.buildIntersection("5Kiosheta Popa 400 NDK 450 Dead 500");
    */
    //Intersection* dead = city.buildIntersection("Dead");
    city.exportIntersections("intersections2.txt");

    city.print();

    //std::cout<< "Cannot add one intersection more than once" <<std::endl;
    
    std::cout<< "Test of uniqueness in Map" <<std::endl;
    uniquenessTest(city);
    
    std::cout<< "Tests of funtion 1\n";
    std::cout << city.hasPathBetween("NDK", "5Kiosheta") << std::endl;

    std::cout<< "Tests of funtion 2\n";
    city.showTheShortestPaths("NDK", "Dead", false, std::cout);

    std::cout<< "Tests of funtion 3\n" ;
    city.getIntersection("5Kiosheta")->close();
    city.showTheShortestPaths("NDK", "Dead", true, std::cout);

    std::cout<< "Tests of funtion 4\n" ;
    std::cout << city.hasSmallTour("NDK") << std::endl;
    
    std::cout<< "Tests of funtion 5" ;
    std::cout << city.hasTourToAllIntersections(std::cout) << std::endl;
    
    std::cout<< "Tests of funtion 6\n" ;
    std::cout << city.canGoToAllIntersections("NDK") << std::endl;
    
    std::cout<< "Tests of funtion 7\n";
    city.showDeadEnds(std::cout);

    Location location = city.getLocation("NDK");

    do {
        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string> data = proccessInput(input);
        //location, change, neighbours, move, close, open, closed
        if (data[0] == "location") {
            location.show(std::cout);
        }
        else if (data[0] == "change") {
            if (data.size() > 1)
                location = location.change(data[1], std::cout);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "neighbours") {
            location.showNeighbours(std::cout);
        }
        else if (data[0] == "move") {
            if (data.size() > 1)
                location = location.move(data[1], std::cout);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "close") {
            if (data.size() > 1)
                location.close(data[1]);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "open") {
            if (data.size() > 1)
                location.open(data[1]);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "closed") {
            location.showClosed(std::cout);
        }
        else if (data[0] == "exit") {
            break;
        }
        else {
            std::cout << "Invalid command!" << std::endl;
        }
    } while (true);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

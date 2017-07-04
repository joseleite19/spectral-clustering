#ifndef POINT
#define POINT

#include <vector>
#include "helper.hpp"

class Point{
	std::vector<double> c;
public:
    Point();
    Point(const std::vector<double>);
    Point(int);
    double &operator[](int);
    double at(int) const;
    std::size_t size() const;

    void operator+=(const Point &);

    void operator/=(double);

    double dist(const Point &) const;
};

#endif
#include "point.hpp"

#include <cmath>
using namespace std;

Point::Point() {}
Point::Point(const vector<double> d): c(d) {}
Point::Point(int sz) : c(sz, 0) {}
double& Point::operator[](int i){ return c[i]; }
double Point::at(int i) const{ return c[i]; }
size_t Point::size() const{ return c.size(); }

void Point::operator+=(const Point &p){
    for(int i = 0; i < c.size(); i++)
        c[i] += p.at(i);
}

void Point::operator/=(double p){
    for(int i = 0; i < c.size(); i++)
        c[i] /= p;
}

double Point::dist(const Point &p) const{
    double ans = 0;
    for(int i = 0; i < max(c.size(), p.size()); i++)
        ans += sq(c[i] - p.at(i));
    
    return sqrt(ans);
}

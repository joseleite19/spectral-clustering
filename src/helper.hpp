#ifndef HELPER
#define HELPER

#include <vector>
#include <string>

const double eps = 1e-9;

double sq(double x);

std::vector<int> randompermutation(int size);

std::vector<std::string> tokenize(const std::string &s, char lim);

#endif
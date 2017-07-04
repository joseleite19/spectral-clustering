#ifndef MATRIX
#define MATRIX

#include <vector>
#include <utility>

class matrix{
    int n, m;
    std::vector<std::vector<double> > mat;
public:
    matrix(int n, int m);
    std::vector<double> &operator[](int i);
    double elem(int i, int j) const;
    int rows() const;
    int cols() const;
    matrix operator-(const matrix& other) const;
    matrix operator+(const matrix& other) const;
    matrix operator*(const matrix& other) const;

    std::pair<double, std::vector<double> > power(); // return o par <autovalor, autovetor>

    matrix autovetores(int k) const;

    std::vector<int> kmeans(int k);
};

#endif
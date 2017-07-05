#include "matrix.hpp"

#include <cassert>
#include <cmath>
#include <set>
#include "point.hpp"

#define ff first
#define ss second

using namespace std;

matrix::matrix(int n, int m) : n(n), m(m), mat(n, vector<double>(m, 0)){}
vector<double>& matrix::operator[](int i){ return mat[i]; }
double matrix::elem(int i, int j) const{ return mat[i][j]; }

matrix matrix::operator-(const matrix & other) const{
    assert(n == other.n && m == other.m);
    matrix ret(n, m);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            ret[i][j] = mat[i][j] - other.elem(i, j);
    return ret;
}

matrix matrix::operator+(const matrix & other) const{
    assert(n == other.n && m == other.m);
    matrix ret(n, m);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            ret[i][j] = mat[i][j] + other.elem(i, j);
    return ret;
}

matrix matrix::operator*(const matrix& other) const{
    assert(m == other.rows());

    matrix ans(n, other.cols());
    for(int i = 0; i < n; i++)
        for(int j = 0; j < other.cols(); j++)
            for(int k = 0; k < m; k++)
                ans[k][j] += mat[i][k] * other.elem(k, j);

    return ans;
}

int matrix::rows() const{ return n; };
int matrix::cols() const{ return m; };

pair<double, vector<double> > matrix::power(){ // return o par <autovalor, autovetor>
    vector<double> b(n);
    for(int i = 0; i < n; i++)
        b[i] = rand() % 100+1;
 
    double k = b[0], y;
    vector<double> c(n);

    int it = 1;
    do{
        y = k;
        for(int i = 0; i < n; i++){
            c[i] = 0;
            for(int j = 0; j < n; j++)
                c[i] += mat[i][j] * b[j];
        }

        k = fabs(c[0]);

        for(int i = 1; i < n; i++)
            k = max(fabs(c[i]), k);

        for(int i = 0; i < n; i++)
            b[i] = c[i] / k;
    }while(it++ < 1000 && fabs(k - y) > eps);

    return make_pair(k, b);
}

matrix matrix::autovetores(int k) const{
    matrix ans(n, k);
    matrix M = *this;
    for(int j = 0; j < k; j++){
        pair<double, vector<double> > ret = M.power();
        for(int i = 0; i < n; i++){
            ans[i][j] = ret.second[i];
            M[i][j] -= ret.first;
        }
    }
    return ans;
}

vector<int> matrix::kmeans(int k){
    vector<int> ret(n, -1);

    vector<pair<set<int>, Point>> cluster(k);
    vector<Point> P(n);
    
    for(int i = 0; i < n; i++)
        P[i] = Point(mat[i]);

    {
        int i = 0;
        for(int x : randompermutation(n)){
            if(i == k) break;
            cluster[i].ff.insert(x);
            cluster[i].ss = Point(mat[x]);
            i++;
        }
    }

    int it = 1;
    bool done = false;
    while(it++ < 10000 && !done){
        done = true;

        //calcula novo cluster para cada ponto
        for(int i = 0; i < n; i++){
            int nid = -1;
            double mn = 1e50;

            // calcula cluster mais perto
            for(int j = 0; j < k; j++){
                double d = P[i].dist(cluster[j].ss);
                if(d < mn) mn = d, nid = j;
            }

            if(ret[i] != nid){
                if(ret[i] != -1) cluster[ ret[i] ].ff.erase(i);
                ret[i] = nid;
                cluster[ ret[i] ].ff.insert(i);
                done = false;
            }
        }

        //calcular novo centro de cada cluster
        for(int i = 0; i < k; i++){
            if(!cluster[i].ff.size()) continue;
            Point p(k);
            for(int x : cluster[i].ff)
                p += P[x];
            if(cluster[i].ff.size()) p /= cluster[i].ff.size();
            cluster[i].ss = p;
        }
    }
    return ret;
}

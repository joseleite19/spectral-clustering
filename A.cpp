//#include <bits/stdc++.h>

#include <cstdio>
#include <vector>
#include <fstream>
#include <set>
#include <utility>
#include <list>
#include <cmath>
#include <cassert>

#define ff first
#define ss second

using namespace std;

inline double sq(double x){ return x * x; }

double sigma = 0.728;
int k = 3;
const double eps = 1e-9;

vector<int> randompermutation(int size){
  vector<int> ret(size, 0);
  list<int> tmp;
  list<int>::iterator it;

  for(int i = 0; i < size; i++)
      tmp.push_back(i);

  srand(time(NULL));
  for(int i = 0; i < size; i++){
      int id = rand() % int(tmp.size());

      it = tmp.begin();
      while(id--) it++;

      ret[i] = *it;

      tmp.erase(it);
  }

  return ret;
}

class Point{
public:
    vector<double> c;
    Point(): c(k, 0) {}
    Point(const vector<double> d): c(d) {}
    Point(int sz) : c(sz, 0) {}
    double &operator[](int i){ return c[i]; }
    double at(int i) const{ return c[i]; }
    inline size_t size() const{ return c.size(); }

    void operator+=(const Point &p){
        for(int i = 0; i < c.size(); i++)
            c[i] += p.at(i);
    }

    void operator/=(double p){
        for(int i = 0; i < c.size(); i++)
            c[i] /= p;
    }

    double dist(const Point &p) const{
        double ans = 0;
        for(int i = 0; i < max(c.size(), p.size()); i++)
            ans += sq(c[i] - p.at(i));
        
        return sqrt(ans);
    }
};

vector<string> tokenize(const string &s, char lim){
    vector<string> v;
    string el = "";
    for(int i = 0; i < s.size(); i++){
        if(s[i] == lim){
            if(el == "?") return vector<string>();
            v.push_back(el);
            el = "";
        }
        else el += s[i];
    }
    if(el.size()) v.push_back(el);
    return v;
}

class matrix{
public:
    int n, m;
    vector<vector<double> > mat;
    matrix(int n, int m) : n(n), m(m), mat(n, vector<double>(m, 0)){}
    vector<double> &operator[](int i){ return mat[i]; }
    double elem(int i, int j) const{ return mat[i][j]; }
    matrix operator-(const matrix & other){
        assert(n == other.n && m == other.m);
        matrix ret(n, m);
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                ret[i][j] = mat[i][j] - other.elem(i, j);
        return ret;
    }

    pair<double, vector<double> > power(){ // return o par <autovalor, autovetor>
        vector<double> b(n);
        for(int i = 0; i < n; i++)
            b[i] = rand() % 100;
        
        double k = b[0], y;
        vector<double> c(n);

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
        }while(fabs(k - y) > eps);
        
        return make_pair(k, b);
    }

    matrix autovetores() const{
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


    vector<int> kmeans(){
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
                Point p;
                for(int x : cluster[i].ff)
                    p += P[x];
                p /= cluster[i].ff.size();
                cluster[i].ss = p;
            }
        }
        return ret;
    }

};

void normalize(vector<Point> &v){
    Point mx = v[0], mn = v[0];

    for(int i = 0; i < v.size(); i++){
        for(int j = 1; j < v[i].size(); j++){
            mn[j] = min(mn[j], v[i][j]);
            mx[j] = max(mx[j], v[i][j]);
        }
    }

    for(int i = 0; i < v.size(); i++){
        for(int j = 1; j < v[i].size(); j++){
            double den = mx[j] - mn[j];
            if(den) v[i][j] = (v[i][j] - mn[j]) / den;
            else v[i][j] = 0;
        }
    }
}

void pretty_print(const Point &p){
    printf("Class: %lf\n", p.at(0));
    printf("Age: %.2lf\n", p.at(1));
    printf("Sex: %.2lf\n", p.at(2));
    printf("Steroid: %.2lf\n", p.at(3));
    printf("Antivirals: %.2lf\n", p.at(4));
    printf("Fatigue: %.2lf\n", p.at(5));
    printf("Malaise: %.2lf\n", p.at(6));
    printf("Anorexia: %.2lf\n", p.at(7));
    printf("Liver big: %.2lf\n", p.at(8));
    printf("Liver firm: %.2lf\n", p.at(9));
    printf("Spleen palpable: %.2lf\n", p.at(10));
    printf("Spiders: %.2lf\n", p.at(11));
    printf("Ascites: %.2lf\n", p.at(12));
    printf("Varices: %.2lf\n", p.at(13));
    printf("Bilirubim: %.2lf\n", p.at(14));
    printf("Alk Phosphate: %.2lf\n", p.at(15));
    printf("Sgot: %.2lf\n", p.at(16));
    printf("Albumin: %.2lf\n", p.at(17));
    printf("Protime: %.2lf\n", p.at(18));
    printf("Hitology: %.2lf\n", p.at(19));
}

int main(int argc, char **argv){

    if(argc == 3){
        k = atoi(argv[1]);
        sigma = atof(argv[2]);
    }

    vector<Point> v, original;

    ifstream in("hepatitis.data");

    if(!in.is_open()) return printf("Falha no arquivo"), 0;

    while(!in.eof()){
        string s;
        getline(in, s);

        vector<string> ret = tokenize(s, ',');
        if(ret.size() == 0) continue;
        if(ret.size() !=20) continue;
        vector<double> d;
        for(int i = 0; i < ret.size(); i++)
            d.push_back(stod(ret[i]));
        
        v.emplace_back(d);
    }
    in.close();

    original = v;

    normalize(v);

    matrix A(v.size(), v.size()), D(v.size(), v.size()), L(v.size(), v.size());

    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < v.size(); j++)
            A[i][j] = exp(-sq(v[i].dist(v[j]))/(2*sq(sigma)));

    for(int i = 0; i < A.n; i++){
        double sum = 0;
        for(int j = 0; j < A.m; j++)
            sum += A[i][j];
        D[i][i] = sqrt(sum);
    }

    L = D - A; // A é simétrico, D é diagonal => L é simetrica :D
    matrix X = L.autovetores();
    matrix Y(X.n, X.m);
    
    for(int i = 0; i < X.n; i++){
        double sum = Point(X[i]).dist(vector<double>{0,0});

        if(sum < eps) continue;
        for(int j = 0; j < X.m; j++)
            Y[i][j] = X[i][j] / sum;
    }

    vector<int> ret = Y.kmeans();

    for(int i = 0; i < ret.size(); i++)
        printf("%d %d\n", i, ret[i]);

    vector<vector<int>> cluster(k);
    for(int i = 0; i < ret.size(); i++){
        cluster[ ret[i] ].push_back(i);
    }

    for(int i = 0; i < k; i++){
        Point p(original.back().size());
        for(int x : cluster[i])
            p += original[x];
        if(cluster[i].size()) p /= cluster[i].size();
        printf("Cluster %d:\n", i);
        pretty_print(p);
        printf("\n");
    }
}

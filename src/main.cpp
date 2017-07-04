#include <cstdio>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "point.hpp"
#include "helper.hpp"
#include "matrix.hpp"

using namespace std;

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

vector<int> clustering(vector<Point> v, int k, double sigma){
    normalize(v);

    int n = v.size();
    matrix A(n, n), D(n, n), L(n, n);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            A[i][j] = exp(-sq(v[i].dist(v[j]))/(2*sq(sigma)));

    for(int i = 0; i < n; i++){
        double sum = 0;
        for(int j = 0; j < n; j++)
            sum += A[i][j];
        D[i][i] = 1/sqrt(sum);
    }

    // L = D - A; // A é simétrico, D é diagonal => L é simetrica :D
    L = D * A * D;

    matrix X = L.autovetores(k);
    matrix Y(n, k);
    
    for(int i = 0; i < n; i++){
        double sum = Point(X[i]).dist(vector<double>{0,0});

        if(sum < eps) continue;
        for(int j = 0; j < k; j++)
            Y[i][j] = X[i][j] / sum;
    }

    return Y.kmeans(k);
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

    double sigma = 0.728;
    int k = 3;

    if(argc == 3){
        k = atoi(argv[1]);
        sigma = atof(argv[2]);
    }

    vector<Point> v;

    ifstream in("data/hepatitis.data");

    if(!in.is_open()) return printf("Falha no arquivo."), 0;

    while(!in.eof()){
        string s;
        getline(in, s);

        vector<string> ret = tokenize(s, ',');
        if(ret.size() != 20) continue;
        
        vector<double> d;

        transform(ret.begin(), ret.end(), back_inserter(d), [](const string &s){ return stod(s); });
        
        v.emplace_back(d);
    }

    in.close();

    vector<int> ret = clustering(v, k, sigma);

    for(int i = 0; i < ret.size(); i++)
        printf("Intancia %d pertence ao cluster %d.\n", i, ret[i]);
    printf("\n");

    vector<vector<int>> cluster(k);
    for(int i = 0; i < ret.size(); i++)
        cluster[ ret[i] ].push_back(i);

    for(int i = 0; i < k; i++){
        Point p(v.back().size());
        for(int x : cluster[i]) p += v[x];
        if(cluster[i].size()) p /= cluster[i].size();
        
        printf("Caracteristicas do cluster %d:\n", i);
        pretty_print(p);
        printf("\n");
    }
}

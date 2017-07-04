#include <bits/stdc++.h>

#define ff first
#define ss second

using namespace std;

inline double sq(double x){ return x * x; }

const double sigma = 0.728;
const int k = 3;
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
    double &operator[](int i){ return c[i]; }

    /* data */
};

class paciente{
public:
    vector<double> att;
    paciente(vector<double> d): att(d){}
    paciente(): att(20, 0.0) {}
    
    double operator-(const paciente &o){
        double ans = 0;
        for(int i = 0; i < att.size(); i++)
            ans += sq(att[i] - o.att[i]);
        return ans;
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

    pair<double, vector<double> > power(){
        double k;
        vector<double> b(n);
        for(int i = 0; i < n; i++)
            b[i] = rand()%100;
        k = b[0];
        double y;
        matrix c(n, 1);

        do{
            y = k;
            for(int i = 0; i < n; i++){
                c[i][0] = 0;
                for(int j = 0;j < n;j++)
                    c[i][0] = c[i][0] + mat[i][j] * b[j];
            }

            k = fabs(c[0][0]);

            for(int i = 1; i < n; i++)
                k = fabs(c[i][0]) > k ? fabs(c[i][0]) : k;
            for(int i = 0; i < n; i++)
                b[i] = c[i][0]/k;
        }while(fabs(k-y) > eps);
        return make_pair(k, b);
    }

    matrix autovetores(){
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

        pair<set<int>, Point > cluster[k];

        {
            int i = 0;
            for(int x : randompermutation(k)){
                cluster[i].ff.insert(x);
                cluster[i].ss.c = mat[x];
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
                double mx = 1e20;

                // calcula cluster mais perto
                for(int j = 0; j < k; j++){
                    double sum = 0;
                    for(int q = 0; q < k; q++)
                        sum += sq(mat[i][q] - cluster[j].ss[q]);
                    sum = sqrt(sum);
                    if(sum < mx) mx = sum, nid = j;
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
                for(int x : cluster[i].ff){
                    for(int j = 0; j < k; j++){
                        p[j] += mat[i][j];
                    }
                }
                for(int j = 0; j < k; j++)
                    p[j] /= cluster[i].ff.size();
                cluster[i].ss = p;
            }
        }
        return ret;
    }

};

void normalize(vector<paciente> &v){
    paciente mx = v[0], mn = v[0];

    for(int i = 0; i < v.size(); i++){
        for(int j = 1; j < 20; j++){
            mn.att[j] = min(mn.att[j], v[i].att[j]);
            mx.att[j] = max(mx.att[j], v[i].att[j]);
        }
    }

    for(int i = 0; i < v.size(); i++){
        for(int j = 1; j < 20; j++){
            double den = mx.att[j] - mn.att[j];
            if(den) v[i].att[j] = (v[i].att[j] - mn.att[j]) / den;
            else v[i].att[j] = 0;
        }
    }
}


int main(){
    vector<paciente> v;

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

        v.push_back(paciente(d));
    }
    in.close();

    normalize(v);

    matrix A(v.size(), v.size()), D(v.size(), v.size()), L(v.size(), v.size());

    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < v.size(); j++)
            A[i][j] = exp(-(v[i]-v[j])/(2*sq(sigma)));

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
        double sum = 0;
        for(int j = 0; j < X.m; j++)
            sum += sq(X[i][j]);
        sum = sqrt(sum);

        if(sum < 1e-9) continue;
        for(int j = 0; j < X.m; j++)
            Y[i][j] = X[i][j] / sum;
    }

    vector<int> ret = Y.kmeans();

    for(int i = 0; i < ret.size(); i++)
        printf("%d %d\n", i, ret[i]);

}

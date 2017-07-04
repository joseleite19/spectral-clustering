#include "helper.hpp"

#include <list>
#include <ctime>
#include <cstdlib>

using namespace std;

double sq(double x){ return x * x; }

vector<int> randompermutation(int size){
  vector<int> ret(size, 0);
  list<int> tmp;
  list<int>::iterator it;

  for(int i = 0; i < size; i++)
      tmp.push_back(i);

  srand(time(0));
  for(int i = 0; i < size; i++){
      int id = rand() % int(tmp.size());

      it = tmp.begin();
      while(id--) it++;

      ret[i] = *it;

      tmp.erase(it);
  }

  return ret;
}

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
    if(el.size()){
        if(el == "?") return vector<string>();
    	v.push_back(el);
    }
    return v;
}
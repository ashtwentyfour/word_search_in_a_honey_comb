#include <iostream>


class loc {       // each object stores a position x , y 
 public:
  loc(int x = 0, int y = 0) : x(x) , y(y) {}
  int x , y;
};

bool operator==(const loc& lhs , const loc& rhs) {   // operator overloading to make comparisons
  return lhs.x == rhs.x && lhs.y == rhs.y;
}



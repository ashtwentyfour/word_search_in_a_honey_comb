#include <iostream>
#include <vector>



class Hex {           // hexagonal cell object class
 
 public:
  
 Hex(char x, int X , int Y) : alphabet(x) , pos_x(X), pos_y(Y) {}       // constructor
  
 void insert_adj(Hex* H) {            // update the list of cells adjacent 
   adjacency_list.push_back(H);
   return;
  }

 char getChar() const {    // get the character stored in the cell
   return alphabet;
  }

 unsigned int num_adj() const {     // return the number of adjacent cells
   return adjacency_list.size();
  }

  
 Hex* get_adjacent_alphabet(int i) const {  // return a pointer to the ith cell adjacent to the current one
    return adjacency_list[i];
  }

 int get_x_pos() const {    // get the x position of the cell
    return pos_x;
  }

 int get_y_pos() const {  // get the y position of the cell
    return pos_y;
  }

 private:
   
 // cell data
  
 char alphabet;          // character stored in the cell 
 std::vector<Hex*> adjacency_list;   // list of adjacent cells
 int pos_x;     // x positionof cell
 int pos_y;     // y position of cell

};

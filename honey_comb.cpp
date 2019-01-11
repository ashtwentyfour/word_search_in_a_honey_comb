
/* Honeycomb Word Search - Ashwin Menon */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <fstream>
#include "hex.h"
#include "loc.h"


typedef std::ifstream INPUT;
typedef std::vector< std::vector<Hex*> > HoneyComb;
typedef std::string String;
typedef std::vector<Hex*> layer;
typedef std::vector<loc> search_path;




// ===================================================================================================================




void make_layer(layer& layer , const String word, int x) {     // function that processes each layer from the input file
  for(unsigned int i = 0; i < word.size(); i++) {
    Hex* block = new Hex(word[i] , x , i);                     // create the corresponding hexagonal cell
    layer.push_back(block);                                    // add it to the layer
  }
  return;
}
  



// ===================================================================================================================




void insert(HoneyComb& H , const layer& layer) {               // adds a new layer to the honeycomb and upadtes the adjacencies between the cells                   
  if(H.size() == 0) {
    H.push_back(layer);                                        // if the honeycomb was empty add the first cell
    return;
  }
  
 
  H.push_back(layer);                                         // add the layer
  
  
  if(layer.size() > 1) {                                      // update adjacencies within the same layer
    for(unsigned int r = 0; r < H[H.size() - 1].size() - 1; r++) {
      H[H.size() - 1][r] -> insert_adj(H[H.size() - 1][r+1]);
      H[H.size() - 1][r+1] -> insert_adj(H[H.size() - 1][r]);
    }
    H[H.size() - 1][0] -> insert_adj(H[H.size() - 1][layer.size() - 1]);
    H[H.size() - 1][layer.size() - 1] -> insert_adj(H[H.size() - 1][0]);
 }


     
  int l = H[H.size() - 1].size() - 1;                       // size of the last/latest layer
  int m = H[H.size() - 2].size() - 1;                       // size of the second to last layer

  for(int i = m; i >= 0; i--) {

    while(H[H.size() - 2][i] -> num_adj() != 6) {          // every internal cell must have 6 adjacent cells 
      H[H.size() - 2][i] -> insert_adj(H[H.size() - 1][l]);     // record the adjacencies from the second to last layer to the last layer 
      H[H.size() - 1][l] -> insert_adj(H[H.size() - 2][i]);     // vice versa
      l--;
      if(l < 0 && H.size() != 2) l = H[H.size() - 1].size() - 1;
    }
    l++;
    
  }
  
    
  return;
}




// ==========================================================================================================




bool on_path(loc position , const search_path& path) {   // returns true if the hexagonal cell is already on the solution path
  for(int i = 0; i < path.size(); i++) {
    if(position == path[i]) return true;
  }

  return false;
}




// ==========================================================================================================




bool honey_comb_word_search(loc position, const HoneyComb& H , const String& search_word, search_path& path) {  // recursive function which does non-linear word searches
  path.push_back(position);                             // add the cell to the path
  if(path.size() == search_word.size()) return true;    // if the path size and word size match we are done
 
  for(int i = 0; i < H[position.x][position.y] -> num_adj(); i++) {  // loop over the adjacent cells
    int x = H[position.x][position.y] -> get_adjacent_alphabet(i) -> get_x_pos();
    int y = H[position.x][position.y] -> get_adjacent_alphabet(i) -> get_y_pos();
    if(on_path(loc(x , y) , path)) continue;                      // ignore the cell if it has already been processed
    if(search_word[path.size()] == H[x][y] -> getChar()) {
      if(honey_comb_word_search(loc(x , y) , H , search_word , path)) return true;    // else recurse from this point on
    }
  }

  path.pop_back();   // if the search failed from that point - pull out the cell from the queue
  return false;
}
   


// ============================================================================================================



void destroy_honey_comb(HoneyComb& H) {     // this function frees the dynamically allocated memory
  for(int i = 0; i < H.size(); i++) {
    for(int j = 0; j < H[i].size(); j++) {
      delete H[i][j];
      H[i][j] = NULL;                      // set pointer to NULL 
    }
  }
  return;
}



// ==============================================================================================================



int main(int argc , char* argv[]) {
  if(argc != 3) {                             // if the input is given incorrectly
    std::cerr<<"Incorrect nnumber of input arguements"<<std::endl;
    exit(0);
  }
  
  INPUT Honey_Comb(argv[1]);                 // input honeycomb file
  INPUT Dictionary(argv[2]);                 // word list
  
  if(!Honey_Comb || !Dictionary) {           // exit if the file does not exist/is corrupted
    std::cerr<<"Bad input"<<std::endl;
    exit(0);
  }
  
  int num_layers = 0;                
  Honey_Comb >> num_layers;                // read in the number of layers       
  if(num_layers == 0) {                    // exit if the number of layers = 0
    std::cerr<<"0 layers specified"<<std::endl;  
    exit(0);
  }
  
  HoneyComb H;                             // the honeycomb data structure (vector of vector of hexagonal cell objects)
  layer layer;                             
  String word;

  int count = 0;
  while(count <= num_layers) {            // read in each layer and insert it into the honeycomb
    std::getline(Honey_Comb , word);
    if(count == 0) {
      count++;
      continue;
    }
    make_layer(layer , word , H.size());
    insert(H , layer);                   // insert
    layer.clear();
    count++;
  }
  

  // begin the word searching now that the honeycomb has been constructed

  String search_word;
  bool found = false;                  // boolean value indicating whether the word was found
  search_path path;
  std::list<String> found_words;       // vector which stores the words found
   

  while(getline(Dictionary , search_word)) {
    for(int i = 0; i < H.size() && !found; i++) {
      for(int j = 0; j < H[i].size() && !found; j++) {
        if(H[i][j] -> getChar() == search_word[0])           // if the first letter of the word to be searched is found call the recursive function here 
	  found = honey_comb_word_search(loc(i , j), H , search_word, path);
      }
    }
    if(found) found_words.push_back(search_word);       // add the word to the list if it was found 
    found = false;
    path.clear();
  }

  found_words.sort();           // sort the output 
  
  // display output
  
  for(std::list<String>::iterator it = found_words.begin(); it != found_words.end(); it++) {
    std::cout<<*it<<std::endl;
  }
 

  destroy_honey_comb(H);    // free the dynamically allocated memory  

  
  return 0;

}
  
  

// ========================================================================================================



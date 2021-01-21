// g++ -Wall -std=c++11 
#include <iostream>
#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <fstream>

//std::vector<std::string>words = {"cat", "dog", "elephant"};
std::vector<std::string>words = {"cat", "dog", "elephant", "locust", "coniferous", "lion", "deciduous", "phone", "phylogenetic", "atom", "bit", "dna"};

std::map<std::string, long> words_map;

int main(int argv, char** argc) {
  
  int min = 0;
  int max = words.size() - 1;
  int range = max - min + 1;
  std::cout << "Range: " << range << std::endl;

  std::srand(std::time(nullptr));
  
  std::ofstream fd("file.txt", std::ios::out);
  if (!fd.is_open()) {
    std::cout << "Cannot open file!" << std::endl;
  }

  long size = 1 * (1024 * 1024 * 1024); 
  std::cout << "size bytes: " << size << std::endl;
  //for (int i = 0; i < 1000; i++) {
  while (size > 0) {
    int num = std::rand() % range + min;
    auto word = words[num];
    fd << word << " ";
    size -= word.size() + 1;
    words_map[word] += 1;
  }
  fd << "\n";
  fd.flush();
  fd.close();
  
  for (auto item : words_map) {
    std::cout << item.first << " = " << item.second << std::endl;
  } 
  return 0;
}

// g++ -Wall -std=c++11 -pthread main.cpp -o parser`
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <future>
#include <algorithm>

std::vector<std::string>key_words = {"cat", "dog", "elephant", "locust", "coniferous", "lion", "deciduous", "phone", "phylogenetic", "atom", "bit", "dna"};

int main(int argc, char** argv) {
  if (argc < 2) { std::cout << "Please provide file name as first argument!" << std::endl; }
  std::string file_name(argv[1]);
  
  std::ifstream fd(file_name, std::ios::binary);
  if (!fd.is_open()) { std::cout << "ERROR: Cannot open file: " << file_name << std::endl; }

  fd.seekg(0, std::ios::end);
  auto file_size = fd.tellg();
  fd.seekg(0, std::ios::beg);
  std::cout << file_name << " size: " << file_size << " bytes" << std::endl;

  const auto cores = std::thread::hardware_concurrency();

  std::cout << "CPU cores: " << cores << std::endl;

  std::thread worker([&]() {	  
    std::vector<std::string> words;
    unsigned long long bytes_read{0}; 
    std::vector<std::vector<std::string>> streams;
    std::map<std::string, int> words_count;

    while(!fd.eof()) {
      std::string word;
      char c = fd.get();
      while (c != 32 && !fd.eof()) {
        word += c;
	c = fd.get();
	bytes_read++;
      }

      words.emplace_back(word);

      if (bytes_read >= 2048 || fd.eof()) {
        streams.push_back(words);

	if (streams.size() == cores || fd.eof()) { 
	  std::vector<std::future<std::map<std::string, int>>>futures;
          for (auto item : streams) {
            futures.push_back(std::async(std::launch::async, [](std::vector<std::string> words_stream) {
		std::map<std::string, int>result;
	        for (auto& tmp_word : words_stream) {
		  if (std::find(key_words.begin(), key_words.end(), tmp_word) != key_words.end()) {
		    result[tmp_word] += 1;
		  }
		}   
	        return result;	
            }, item));
          }

          for (auto& fut : futures) {
            auto wm = fut.get();
	    for (auto& w : wm) {
	      words_count[w.first] += w.second;
	    }
          }
          streams.clear();	  
        }
	words.clear();
	bytes_read = 0;
      }
    }
    for (const auto& item : words_count) {
      std::cout << " - " << item.first << " : " << item.second << std::endl;
    }
  });
  
  worker.join();
  fd.close();

  return 0;
}

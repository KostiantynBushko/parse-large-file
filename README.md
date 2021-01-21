# Build file generator 
g++ -Wall -std=c++11 gen_text_file.cpp -o generator

# Build parser
g++ -Wall -std=c++11 main.cpp parser

# Run 
./generator
./parser file.txt

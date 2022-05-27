file = main.cpp
exe = main

all: $(file)
	g++ -std=c++11 $(file) -o $(exe)

run: $(exe)
	./$(exe)

clean: 
	rm $(exe)
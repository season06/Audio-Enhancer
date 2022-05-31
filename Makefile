source = main.cpp
test = ./tests/
exe = main
CC = g++

cflag = -O3 -Wall -shared -std=c++11 -fPIC
pyflag = $(shell python3-config --includes)
pyext = $(shell python3-config --extension-suffix)
pybind = -I/usr/include/pybind11 -lpython3.8

# pybind = $(shell python3 -m pybind11 --includes)

all: $(source)
	$(CC) $(cflag) $(pyflag) $(pybind) $(source) -o $(exe)$(pyext)

local: $(source)
	$(CC) -std=c++11 $(source) -o $(exe)

test: ${test}
	python3 -m pytest -s ${test}

clean: 
	rm -rf $(exe) $(exe)$(pyext) ${test}__pycache__ __pycache__ .pytest*

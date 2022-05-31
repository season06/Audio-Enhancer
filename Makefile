source = enhancer.cpp
test = ./tests/
exe = enhancer
CC = g++

cflag = -O3 -Wall -shared -std=c++11 -fPIC
pyflag = $(shell python3-config --includes)
pyext = $(shell python3-config --extension-suffix)
# pybind = -I/usr/include/pybind11 -lpython3.8

# local
source_local = main.cpp
exe_local = main
pybind = $(shell python3 -m pybind11 --includes)

all: $(source)
	$(CC) $(cflag) $(pyflag) $(pybind) $(source) -o $(exe)$(pyext)

local: $(source)
	$(CC) -std=c++11 $(source_local) -o $(exe_local)

test: ${test}
	python3 -m pytest -s ${test}

clean: 
	rm -rf $(exe) $(exe)$(pyext) ${test}__pycache__ __pycache__ .pytest* performance.txt
	rm -rf $(exe_local)
g++ -c test.cpp
g++ -o test.exe test.o compiled/bignum.o compiled/conversion.o
del test.o
start "" test.exe

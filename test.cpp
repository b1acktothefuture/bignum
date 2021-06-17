#include "bignum.h"

// g++ -c test.cpp
// g++ -o test.exe test.o compiled/bignum.o compiled/conversion.o
// ./test

int main()
{
    big a("1000000000000000418871887878878781881878717888888888188770000000000000001888888888888888888888888888888888777");
    big b = a * a;
    print(&b, 0);
    cout << "\nPress enter to exit";
    cin.get();
}
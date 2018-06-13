#include <iostream>
#include "Event.h"

void c(int* x) {
	std::cout << "Hello from 'c': " << (*x) << std::endl;
}

struct D {
	void operator()(int* x) {
		std::cout << "Hello from 'd': " << (*x) << std::endl;
	}
} d;

int main() {
	Event<int*> a;
	a += [](int* x) {
		std::cout << "Hello from lambda: " << (*x) << std::endl;
	};
	Event<int*> b;
	b += a;
	a += c;
	b += d;
	int x = 4;
	b(&x);
	getchar();
	return 0;
}

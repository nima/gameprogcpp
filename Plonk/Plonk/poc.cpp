#include <iostream>
#include <typeindex>
#include <vector>

class TargetClass {};
class DerivedClass : public TargetClass {};
class OtherClass {};

const std::type_info* TARGET = &typeid(TargetClass);
void ptrDump(const std::type_info* tid) { std::cout << (TARGET == tid) << std::endl; }
void addrDump(const std::type_info& tid) { std::cout << (*TARGET == tid) << std::endl; }

void modifyValue(int* ptr) {
    (*ptr) += 10;
}

int main() {
	const int x = 5;
	int *y = const_cast<int*>(&x);
	int *z = const_cast<int*>(std::addressof(x));
	modifyValue(y);
	modifyValue(z);
	std::cout << "x: " << x << std::endl;  // Output: x: 15
	std::cout << "y: " << *y << std::endl;  // Output: x: 15
	std::cout << "z: " << *z << std::endl;  // Output: x: 15

	return 0;

	const std::type_info* target = &typeid(TargetClass);
	std::vector<const std::type_info*> tids = {
		&typeid(DerivedClass),
		target,
		&typeid(OtherClass),
	};

	for (const std::type_info *tid : tids) {
		// Method 1
		ptrDump(tid);

		// Method 2
		addrDump(*tid);
	}

	// Method 3
	const std::type_info &tid = typeid(TargetClass);
	ptrDump(&tid);
	addrDump(tid);

	// Method 4 — no way of doing this (Method 3 + vector encap)
	/*
	std::vector<const std::type_info&> tids2 = {
		typeid(DerivedClass),
		typeid(OtherClass),
	};
	*/

	return 0;
}


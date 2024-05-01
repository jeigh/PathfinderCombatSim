#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

using std::make_shared;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;
using std::cout;

class bigClass
{
public:
	bigClass()
	{
		ints[0] = 4;
		ints[1] = 3;
		ints[2] = 2;
	}

	~bigClass()
	{
		delete[] ints;
	}

	int* ints = new int[500]();
};

auto makeBigArray(const int num)
{
	auto v = vector<shared_ptr<bigClass>>{};
	for (int i = 0; i < num; ++i)
	{
		auto original = make_shared<bigClass>();
		
		v.push_back(original);
	}
	return v;
}

int main()
{
	weak_ptr<bigClass> yourMomma;
	vector<shared_ptr<bigClass>> ghi;

	{
		auto theVector = makeBigArray(10);
		yourMomma = theVector.at(8);
		ghi = theVector;
	}

	weak_ptr<bigClass> abc = yourMomma;
	auto def = abc.lock();
	return 0;

	
}


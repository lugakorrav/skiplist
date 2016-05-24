#include <iostream>
#include "skiplist.h"
#include <set>
#include <ctime>
using namespace std;

int main()
{
	srand(time(NULL));
	set<int> s;
	skiplist<int> sl;
	int start_set = clock();
	for (int i = 0; i < 10000; i++)
	{
		s.insert(rand() % 10000);
	}
	int stop_set = clock();
	int start_sl = clock();
	for (set<int>::iterator it = s.begin(); it != s.end(); it++)
	{
		sl.insert(*it);
	}
	int stop_sl = clock();

	cout << stop_set - start_set << endl;
	cout << stop_sl - start_sl << endl;

	for (int i = 1; i < 1000; i++)
	{
		int r = rand() % 10000;
		s.erase(r);
		sl.erase(r);
	}

	cout << s.size() << endl;
	cout << sl.size() << endl;

	set<int>::iterator it1 = s.begin();
	skiplist<int>::iterator it2 = sl.begin();
	for (; it1 != s.end(); it1++, it2++)
	{
		if (*it1 != *it2)
		{
			cout << "something went wrong";
		}
	}

	return 0;
}

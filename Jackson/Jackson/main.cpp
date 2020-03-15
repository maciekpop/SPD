#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>

using namespace std;

int main() {

	fstream plik;
	plik.open("JACK1.DAT", ios::in);
	multimap<int, int> dane;
	int n;
	vector<int> cpi;
	cpi.push_back(0);

	plik >> n;

	if (plik.good())
	{
		int r{ 0 }, p{ 0 };
		while ((plik >> r) && (plik >> p))
		{
			dane.insert({ r,p });
		}
	}
	else
	{
		cout << "Blad otwierania pliku " << endl;
		exit(0);
	}

	int licznik{ 0 };
	for (auto& x : dane)
	{
		int noweC;
		noweC = max(cpi[licznik], x.first) + x.second;
		cpi.push_back(noweC);
		++licznik;
	}

	cout << cpi[n];

	return 0;
}
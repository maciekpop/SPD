#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>

using namespace std;

void Jackson(string fileName)
{

	fstream plik;
	plik.open(fileName, ios::in);

	if (plik.good())
	{
		multimap<int, int> dane;
		int n;
		int r{ 0 }, p{ 0 };

		plik >> n;
		while ((plik >> r) && (plik >> p))
			dane.insert({ r,p });

		int noweC{ 0 };
		for (auto& x : dane)
			noweC = max(noweC, x.first) + x.second;
		cout << noweC <<endl;
	}
	else
	{
		cout << "Blad otwierania pliku " << endl;
		exit(0);
	}
}

int main() 
{

	for (int i = 1; i <= 8; i++)
	{
		string name = "JACK";
		string num = to_string(i);
		string ext = ".DAT";
		Jackson(name + num + ext);
	}
	return 0;
}

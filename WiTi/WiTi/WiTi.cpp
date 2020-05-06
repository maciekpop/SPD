// żeby program działał poprawnie należy usunąć ostatnią pustą linię w pliku z danymi

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int n;

struct Task 
{
	int p{ 0 }, w{ 0 }, d{ 0 };
};

struct Set 
{
	vector<Task> tasksInSet;
	int P{ 0 };
};

const vector<Task> loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{
		plik >> n;
		vector<Task> T(n+1); 
		int i = 1; // pozycja 0 nie jest brana pod uwagę, żeby ułatwić identyfikację zadań
		while (!plik.eof())
		{
			plik >> T[i].p >> T[i].w >> T[i].d;
			i++;
		}
		plik.close();
		return T;
	}
	else
	{
		cerr << "File error\n";
		exit(0);
	}
}

// pozycje bitow ustawionych na 1 w liczbie dziesiętnej
vector<int> getBitsPositions(int decNumber)
{
	vector<int> positions; 
	int pos = 1;
	while (decNumber != 0)
	{
		if (decNumber & 1 != 0)
		{
			positions.push_back(pos);
		}
		pos++;
		decNumber = decNumber >> 1;
	}
	return positions; // zwracany wektor pozycji
}

// długość wykonywania wszystkich zadań z danego zbioru
void definePValueOfEverySet(vector<Set>& sets)
{
	for (auto&& set : sets)
		for (auto task : set.tasksInSet)
			set.P += task.p;
}

// generowanie (2^n - 1) zbiorów
vector<Set> defineSets(vector<Task> tasks)
{
	int q = pow(2, n);
	vector<int> bitsPos;
	vector<Set> sets(q); // wszystkie podzbiory

	for (int i = q - 1; i > 0; i--) 
	{
		bitsPos = getBitsPositions(i);

		while (!bitsPos.empty())
		{
			int pos = bitsPos.back();
			sets[i].tasksInSet.push_back(tasks[pos]);
			bitsPos.pop_back();
		}
	}
	definePValueOfEverySet(sets);
	return sets; // zwracane zbiory
}

void WiTi(vector<Task> tasks)
{



}
	
int main()
{
	vector<Set> test;
	test = defineSets(loadFromFile("dane3.txt"));

	vector<Task> zad = test[15].tasksInSet;
	for(auto x : zad)
		cout << x.p <<' ';
	cout << test[15].P;
}
// żeby program działał poprawnie należy usunąć ostatnią pustą linię w pliku z danymi

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int n;

// struktura zadań
struct Task 
{
	int id{ 0 }, // numer zadania wg kolejności wczytania z z pliku
		p{ 0 }, // czas wykonania
		w{ 0 }, // współczynnik kary
		d{ 0 }; // deadline
};

// struktura zbiorów
struct Set 
{
	vector<Task> tasksInSet; // zadania w zbiorze
	int P{ 0 }; // czas wykonania wszystkich zadań ze zbioru
	int OPT{ 0 }; // wartość optymalna funkcji celu
};

// wczytywanie z pliku z danymi
// in: nazwa pliku zawierającego dane zadań
// out: wektor z zadaniami
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
			T[i].id = i; // zadania ponumerowane od 1 do n
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

// pozycje bitów ustawionych na 1 w liczbie dziesiętnej
// in: 5 (bin: 0101)
// out: vector<int>: {1, 3}
vector<int> getBitsPositions(int decNumber)
{
	vector<int> positions; // wektor, w którym gromadzą się pozycje jedynek
	int pos = 1; // zaczynając od pozycji 1
	while (decNumber != 0) // dopóki liczba nie zostanie przesunięta w prawo aż do 0
	{
		if (decNumber & 1) // jeżeli na pozycji 1 jest bit ustawiony na 1
			positions.push_back(pos); // to do wktora jest dodawana obecna pozycja
		pos++; // przejście na następną pozycję
		decNumber >>= 1; // przesunięie bitowe w prawo o jeden
	}
	return positions; // zwracany wektor pozycji
}

// długość wykonywania wszystkich zadań w zbiorach
void definePValueOfEverySet(vector<Set>& sets)
{
	for (auto&& set : sets) 
		for (auto task : set.tasksInSet)
			set.P += task.p;
}

// generowanie (2^n - 1) zbiorów
// in: zadania pobrane z pliku txt
// out: (2^n - 1) zbiorów
vector<Set> defineSets(vector<Task> tasks)
{
	vector<int> bitsPositions; // vector pomocniczy do gromadzenia pozycji jedynek w liczbie binarnej
	vector<Set> sets(pow(2, n)); // wszystkie podzbiory typu Set (bez 0 --> 2^n - 1)

	for (int i = pow(2, n) - 1; i > 0; i--)
	{
		bitsPositions = getBitsPositions(i); // pobieranie pozycji jedynek w liczbie binarnej 

		while (!bitsPositions.empty())
		{
			int pos = bitsPositions.back(); // pobranie pierwszej pozycji
			sets[i].tasksInSet.push_back(tasks[pos]); // dodawanie zadania o danej pozycji do zbioru 
			bitsPositions.pop_back(); // usuwanie wykorzystanej pozycji
		}
	}
	definePValueOfEverySet(sets); // dodanie czasów wykonywania wszystkich zadań we wszystkich zbiorach
	return sets; // zwracane zbiory
}

// wyznaczanie zbioru po skasowaniu danego bitu (taskNumber) w zbiorze (setNumber)
// in: setNumber = 1100, taskNumber = 0100
// out: 1000
int setWithout_z(int setNumber, int taskNumber) { return setNumber & ~(1 << taskNumber - 1); }

// in: wszystkie zbiory (2^n - 1)
// out: wartość optymalna dla pełnego zbioru
int WiTi(vector<Set>& sets)
{
	int min;
	int ZSize;
	for (int i = 1; i < pow(2, n); i++)
	{
		Set& Z = sets[i]; // zbiór Z 
		ZSize = Z.tasksInSet.size();
		min = INT_MAX; 
		for (auto task : Z.tasksInSet) // dla każdego zadania w zbiorze Z
		{
			Task& z = task; // zadanie z, które będzie odejmowane od zbioru Z
			Set& Z_no_z = sets[setWithout_z(i, task.id)]; // zbiór Z\{z}
			Z.OPT = Z_no_z.OPT + z.w * max(0, Z.P - z.d); // wzór (3) z instrukcji
			if(Z.OPT < min)
				min = Z.OPT; 
		}
		Z.OPT = min; // przypisywanie zbiorowi wartości optymalnej funkcji celu
	}
	
	return sets[pow(2, n) - 1].OPT; // zwracanie OPT dla kompletnego zbioru
}
	


int main()
{
	string name = "dane";
	string ext = ".txt";
	for (int i = 1; i <= 9; i++)
	{
		string num = to_string(i);
		vector<Set> sets = defineSets(loadFromFile(name + num + ext));
		cout << WiTi(sets) << endl;
	}
}
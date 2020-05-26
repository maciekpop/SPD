#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

int n, // iloœæ pe³nych zadañ 
m; // iloœæ maszyn

// podzadanie z jednej maszyny
struct SubTask
{
	int p{ 0 }, // czas wykonywania podzadania
		C{ 0 }; // termin zakoñczenia wykonywania podzadania
};

// pe³ne zadanie na wszystkich maszynach
struct FullTask
{
	int id{ 0 }, // numer zadania wzglêdem kolejnoœci wczytywania z pliku
		sum_p{ 0 }; // suma wykonywania siê wszystkih podzadañ na wszystkich maszynach (czas wykonywania zadania na wszystkich maszynach)
	vector<SubTask> subTask; // zbiór podzadañ pe³nego zadania
};

// permutacje wraz z czasem wykonania
struct Permutation
{
	vector<FullTask> perm; // permutacja pe³nych zadañ
	int cMax{ 0 }; // termin zakoñczenia wykonywania zadañ
};

// najwiêksza wartoœæ sum_p pe³nego zadania na szczycie kolejki
struct HighestSum
{
	bool operator()(const FullTask& x, const FullTask& y)
	{
		return x.sum_p < y.sum_p;
	}
};

// najmniejsza wartoœæ cMax permutacji na szczycie kolejki
struct LowestC
{
	bool operator()(const Permutation& x, const Permutation& y)
	{
		return x.cMax > y.cMax;
	}
};

// wyœwietlanie permutacji 
void printVec(const vector<FullTask>& vec)
{
	for (auto x : vec) {
		cout << ' ' << x.id;
	}
}

// czyszczenie zawartoœci kolejki priorytetowej
template<typename T, class C>
void cleanQueue(priority_queue<T, vector<T>, C>& Q)
{
	while (!Q.empty())
		Q.pop();
}

// wczytywanie danych z pliku do kolejki
priority_queue<FullTask, vector<FullTask>, HighestSum> loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{

		plik >> n >> m;
		vector<FullTask> task(n); // vector n pe³ych zadañ
		priority_queue<FullTask, vector<FullTask>, HighestSum> taskList; // kolejka pe³ych zadañ (od najwiêkszego sum_p)

		for (int i = 0; i < n; i++)
		{
			task[i].id = i + 1; // nadawanie numeru zadaniu
			task[i].subTask.push_back(*new SubTask); // na pozycjê 0 jest dodwane puste podzadanie (na potrzeby dzia³ania wzoru (1))
			for (int j = 0; j < m; j++)
			{
				SubTask subTask; // zmienna pomocnicza podzadania
				plik >> subTask.p; // wczytywanie czasu wykonywania podzadania
				task[i].subTask.push_back(subTask); // dodawanie podzadania jako czêœæ pe³nego zadania
				task[i].sum_p += subTask.p; // zwiêkszenie czasu wykonywania pe³nego zadania
			}

			taskList.push(task[i]); // dodawanie pe³nego zadania do kolejki
		}

		plik.close();
		return taskList; // zwracanie listy pe³nych zadañ w kolejnoœci od najwiêkszego sum_p
	}
	else
	{
		cerr << "File error\n";
		exit(0);
	}
}

// wyliczanie cMax dla danej permutacji
// implementacja wzoru (1) z instrukcji
void countPermutation_cMax(Permutation& P)
{
	int permFullTaskNumber = P.perm.size(); // iloœæ pe³nych zadañ w permutacji

	// wzór (1) z instrukcji
	for (int j = 1; j < permFullTaskNumber; j++)
		for (int k = 1; k <= m; k++)
			P.perm[j].subTask[k].C = max(P.perm[j - 1].subTask[k].C, P.perm[j].subTask[k - 1].C) + P.perm[j].subTask[k].p;

	// cMax permutacji pe³ych zadañ to termin zakoñczenia ostaniego podzadania ostatniego pe³nego zadania na ostatniej maszynie
	P.cMax = P.perm[permFullTaskNumber - 1].subTask[m].C;
}

// wyznaczanie permutacji wraz z opymalnym czasem wykonania
void NEH(priority_queue<FullTask, vector<FullTask>, HighestSum>& taskList, Permutation& permutation)
{
	FullTask task; // zmienna pomocnicza pe³nego zadania
	priority_queue<Permutation, vector<Permutation>, LowestC> permutationQueue; // kolejka permutacji (od najmniejszego C)

	// na pozycjê 0 w permutacji jest dodwane puste pe³ne zadanie (na potrzeby dzia³ania wzoru (1))
	for (int i = 0; i <= m; i++)
		task.subTask.push_back(*new SubTask);
	permutation.perm.push_back(task);

	while (!taskList.empty())
	{
		task = taskList.top(); // z listy pobierane jest pe³ne zadanie
		taskList.pop(); // i usuwane

		// pe³ne zadanie wstawiane próbnie jest na wszystkie pozycje w tworzonej permutacji
		for (int i = 1; i < (int)permutation.perm.size() + 1; i++)
		{
			auto it = permutation.perm.begin(); // ustalenie iteratora na pocz¹tek permutacji
			permutation.perm.insert(it + i, task); // dodanie pe³nego zadania na pozycjê i
			countPermutation_cMax(permutation); // dla ka¿dej w ten sposób utworzonej permutacji wyznaczany jest cMax
			permutationQueue.push(permutation); // utworzona permutacja jest dodawana do kolejki w celu wyznaczenia najlepszego cMax
			it = permutation.perm.begin(); // ustalenie iteratora na pocz¹tek permutacji
			permutation.perm.erase(it + i); // usuniêcie pe³nego zadania z pozycji i
		}

		permutation = permutationQueue.top(); // ostatecznie zadanie wstawiane jest na pozycjê dla której cMax permutacji by³ najmniejszy
		cleanQueue(permutationQueue); // kolejka jest czyszczona
	}
}

int main()
{
	for (int i = 1; i <= 9; i++)
	{
		priority_queue<FullTask, vector<FullTask>, HighestSum> taskList;
		Permutation P;
		string name = "NEH";
		string num = to_string(i);
		string ext = ".DAT";
		cout << i << ". ";
		taskList = loadFromFile(name + num + ext);
		NEH(taskList, P);
		//cout << "permutacja: "; printVec(P.perm); cout << endl; 
		cout << "   cmax: " << P.cMax << endl;
	}
}
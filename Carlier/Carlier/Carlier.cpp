#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>


using namespace std;

int n;

struct Action
{
	int id{ 0 }, r{ 0 }, p{ 0 }, q{ 0 }, f{ 0 }; // f -- termin rozpoczęcia
};

struct SchrageData
{
	int U;
	vector<Action> PI;
};

struct ActionAscending
{
	bool operator()(const Action& x, const Action& y)
	{
		return x.r > y.r;
	}
};

struct ActionDescending
{
	bool operator()(const Action& x, const Action& y)
	{
		return x.q < y.q;
	}
};

Action* loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);

	if (plik.good())
	{
		plik >> n;
		Action* A = new Action[n];
		int i = 0;
		while (!plik.eof())
		{
			A[i].id = i + 1;
			plik >> A[i].r >> A[i].p >> A[i].q;
			i++;
		}
		
		return A;	
		delete[] A;
	}
	else
	{
		cout << "Blad otwierania\n";
		exit(0);
	}
	plik.close();

}

SchrageData Schrage(Action* actions, vector<Action> PI)
{
	priority_queue<Action, vector<Action>, ActionAscending> N;
	priority_queue<Action, vector<Action>, ActionDescending> G;

	for (int i = 0; i < n; i++)
		N.push(actions[i]);

	int t = 0, cMax = 0;
	Action e;

	while (!N.empty() || !G.empty())
	{
		while (!N.empty() && t >= N.top().r)
		{
			e = N.top();
			G.push(e);
			N.pop();
		}
		if (G.empty())
		{
			t = N.top().r;
			continue;
		}
		e = G.top();
		G.pop();
		t += e.p;
		e.f = t; // termin zakończenia zadania
		cMax = max(cMax, t + e.q);
		PI.push_back(e);
	}
	int U = cMax;

	cout << "U: " << U << ' ';

	int piSize = PI.size();
	cout << "PI: ";
	for (int i = 0; i < piSize; i++)
	{
		int piId = PI[i].id;
		cout << piId << ' ';
	}

	SchrageData SD;
	SD.PI = PI;
	SD.U = U;

	return SD;
}

int Carlier(Action* actions, vector<Action> PI)
{
	SchrageData schrageData = Schrage(actions, PI);
	int U = schrageData.U;

	Action a, b, c;

	// cos jeszcze
	
	for (int i = n - 1; i >= 0; i--) // pętla -- szukanie b, a, c
	{
		// b.id == 0 żeby nie szukać kolejnych
		if (schrageData.PI[i].f + schrageData.PI[i].q == U && b.id == 0)
			b = schrageData.PI[i];
		if (i > 0 && b.id != 0 && schrageData.PI[i].f - schrageData.PI[i].p == schrageData.PI[i - 1].f)
			a = schrageData.PI[i - 1];
		if (c.id == 0 && schrageData.PI[i].q < b.q)
			c = schrageData.PI[i];
	}

	cout << endl;
	cout << a.id << ' ' << c.id << ' ' << b.id;
	
	return 0;
}


int main()
{
	/*
	for (int i = 1; i <= 9; i++)
	{
		string name = "SCHRAGE";
		string num = to_string(i);
		string ext = ".DAT";
		Schrage(name + num + ext);
	}
	*/
	vector<Action> PI;
	Carlier(loadFromFile("SCHRAGE3.DAT"), PI);
	
}
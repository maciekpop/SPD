#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <climits>

using namespace std;

int n;


struct Action
{
	int id{ 0 }, r{ 0 }, p{ 0 }, q{ 0 }, f{ 0 }; // f -- termin rozpoczęcia
};

vector<Action> PI;

struct SchrageData
{
	int U;
	vector<Action> PI;
};

struct LowestR
{
	bool operator()(const Action& x, const Action& y)
	{
		return x.r > y.r;
	}
};

struct HighestQ
{
	bool operator()(const Action& x, const Action& y)
	{
		return x.q < y.q;
	}
};

struct LowestQ
{
	bool operator()(const Action& x, const Action& y)
	{
		return x.q > y.q;
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


int /*SchrageData*/ Schrage(Action* actions/*, vector<Action> PI*/)
{
	if (!PI.empty())
		PI.clear();

	priority_queue<Action, vector<Action>, LowestR> N;
	priority_queue<Action, vector<Action>, HighestQ> G;

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
	return U;
	/*
	SchrageData SD;
	SD.PI = PI;
	SD.U = U;
	return SD;
	*/
}

void Carlier(Action* actions, int UB)
{
	int U, LB;
	Action a, b, c;
	vector<Action> pi;
	int cR, cQ;
	int r1{ 0 }, p1{ 0 }, q1{ 0 }; // primy
	bool isC = false; // czy c jest potrzbne
	
	// 1.
	cout << "1. ";
	U = Schrage(actions);
	cout <<"U: " << U << endl;
	
	// 2.
	if (U < UB) { cout << "2. "; UB = U; pi = PI; }

	int s = pi.size();
	for (int i = 0; i < n; i++)
	{
		cout << pi[i].id << ' ';
	}
	cout << endl;

	// 3.
	cout << "3. \n";
	for (int i = n - 1; i >= 0; i--) // pętla -- szukanie b, a, c
	{
		// b.id == 0 żeby nie szukać kolejnych
		if (pi[i].f + pi[i].q == U && b.id == 0)
			b = pi[i];
		if (i > 0 && b.id != 0 && pi[i].f - pi[i].p == pi[i - 1].f)
			a = pi[i - 1];
		if (c.id == 0 && pi[i].q < b.q)
			c = pi[i];
	}

	if (b.f - b.p != a.f)
		isC = true; // c jest potrzebne


	// 4.
	if (!isC)
	{
		cout << "4.\n";
		return;
	}
		

	// 5.
	cout << "5.\n";
	int j = 0;
	priority_queue<Action, vector<Action>, LowestR> R; // kolejki do wyznaczania r' i q'
	priority_queue<Action, vector<Action>, LowestQ> Q;
		
	while (pi[j].id != c.id) // przewijanie do zadania c
		j++;

	while (pi[j].id != b.id) // wyznaczanie r', p', q'
	{
		j++;
			
		R.push(pi[j]); // wybieranie najmniejszego r z zakresu c+1 <= j <= b
		Q.push(pi[j]); // wybieranie najmniejszego q z zakresu c+1 <= j <= b
		p1 += pi[j].p;
	}
	r1 = R.top().r;
	q1 = Q.top().q;


	// 6.
	cout << "6. \n";
	cR = c.r; // przechowanie starego c.r
	c.r = max(c.r, r1 + p1); // modyfikacja c.r -- przesunięcie c za b
	actions[c.id - 1] = c;
	

	// 7. 
	cout << "7. ";
	LB = Schrage(actions); // wywołanie Schrage dla nowego ustawienia
	cout << "LB: " << LB << endl;

	// 8. 9.
	if (LB < UB)
	{
		cout << "8. 9.\n";
		Carlier(actions, UB);
	}

	// 10.
	cout << "10.\n";
	c.r = cR;
	actions[c.id - 1] = c;
	

	// 11.
	cout << "11.\n";
	cQ = c.q; // przechowanie starego c.q
	c.q = max(c.q, q1 + p1); // modyfikacja c.q -- przesunięcie c przed c + 1
	actions[c.id - 1] = c;

	// 12.
	cout << "12. ";
	LB = Schrage(actions);
	cout << "LB: " << LB << endl;

	// 13. 14.
	if (LB < UB)
	{
		cout << "13. 14.\n";
		Carlier(actions, UB);
	}

	// 15.
	cout << "15.";
	c.q = cQ;
	actions[c.id - 1] = c;

	cout << a.id << ' ' << c.id << ' ' << b.id <<endl;
	cout << U << endl;

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
	int UB = INT_MAX;
	Carlier(loadFromFile("SCHRAGE3.DAT"), UB);

}
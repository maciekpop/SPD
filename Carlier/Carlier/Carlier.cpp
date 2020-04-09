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
	int id{ 0 }, r{ 0 }, p{ 0 }, q{ 0 }, f{ 0 }; // f -- termin zakończenia
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
		cerr << "File error\n";
		exit(0);
	}
	plik.close();

}

int Schrage(Action* actions, vector<Action>& pi)
{
	if (!pi.empty())
		pi.clear();

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
		e.f = t; // f -- termin zakończenia zadania
		cMax = max(cMax, t + e.q);
		pi.push_back(e);
	}
	int U = cMax;
	return U;

}

int preSchrage(Action* actions)
{

	priority_queue<Action, vector<Action>, LowestR> N;
	priority_queue<Action, vector<Action>, HighestQ> G;

	for (int i = 0; i < n; i++)
		N.push(actions[i]);

	int t = 0, cMax = 0;
	Action e;
	Action l; l.q = INT_MAX;

	while (!N.empty() || !G.empty())
	{
		while (!N.empty() && t >= N.top().r)
		{
			e = N.top();
			G.push(e);
			N.pop();

			if (e.q > l.q)
			{
				l.p = t - e.r;
				t = e.r;
				if (l.p > 0)
					G.push(l);
			}
		}
		if (G.empty())
		{
			t = N.top().r;
			continue;
		}
		e = G.top();
		G.pop();
		l = e;
		t += e.p;
		e.f = t; // f -- termin zakończenia zadania
		cMax = max(cMax, t + e.q);
	}
	int U = cMax;
	return U;

}

void selectABC(vector<Action> pi, Action& a, Action& b, Action& c, int U, bool& isC) 
{
	bool firstBlock = false; // zmienna do pilnowania czy a wyznaczane jest w tym samym bloku co b
	
	for (int i = n - 1; i >= 0; i--) // pętla -- szukanie b, a, c, od ostatniego zadania 
	{
		// jeśli id = 0 to zadanie jeszcze nie istnieje
		// b.id == 0 żeby nie szukać kolejnych, wyznacza zadanie najbardziej na prawo
		if (b.id == 0 && pi[i].f + pi[i].q == U)
		{
			b = pi[i];
			firstBlock = true; // pierwszy blok istnieje
		}

		// wyznaczanie a tylko wtedy, gdy jest w jednym bloku z b
		if (firstBlock && i > 0 && pi[i].f - pi[i].p == pi[i - 1].f)
			a = pi[i - 1];
		else
			firstBlock = false; // zamknięcie dostępu do pierwszego bloku

		// jeśli c jeszcze nie istnieje, wyznacza zadanie najbardziej na prawo
		if (c.id == 0 && pi[i].q < b.q)
			c = pi[i];
	}

	if (b.f - b.p > a.f && c.id != 0)
		isC = true; // c jest potrzebne
}

void rqp(vector<Action> pi, int& r1, int& p1, int& q1, Action b, Action c)
{
	int j = 0;
	priority_queue<Action, vector<Action>, LowestR> R; // kolejki do wyznaczania r' i q'
	priority_queue<Action, vector<Action>, LowestQ> Q;

	while (pi[j].id != c.id) // przewijanie do zadania c
		j++;

	// od c + 1 do b
	while (pi[j].id != b.id) // wyznaczanie r', p', q'
	{
		j++;
		R.push(pi[j]); // wybieranie najmniejszego r z zakresu c+1 <= j <= b
		Q.push(pi[j]); // wybieranie najmniejszego q z zakresu c+1 <= j <= b
		p1 += pi[j].p;
	}
	r1 = R.top().r;
	q1 = Q.top().q;
}

void showPermutation(vector<Action> pi)
{
	int s = pi.size();
	cout << "PI: ";
	for (int i = 0; i < n; i++)
		cout << pi[i].id << ' ';
}

void showNodeDetails(int nd, int pnd, int U, int LB, int a, int b, int c, vector<Action> pi)
{
	cout << "nd: " << nd << " pnd: " << pnd << " U: " << U << " LB: " << LB
		 << " A: " << a << " B: " << b << " C: " << c << ' ';
	showPermutation(pi);
	cout << endl;
}

void showCutLeftDetails(int nd, int LB, int UB)
{
	cout << "cut left child of node: " << nd << " LB: " << LB << " UB: " << UB << endl;
}

void showCutRightDetails(int nd, int LB, int UB)
{
	cout << "cut right child of node: " << nd << " LB: " << LB << " UB: " << UB << endl;
}

void Carlier(Action* actions, vector<Action>& pi_opt, int& UB, int& overall_nodes, int current_node, int parent)
{
	// do liczenia węzłów
	parent = current_node; 
	overall_nodes++;
	current_node = overall_nodes;

	int U, LB;
	Action a, b, c;
	vector<Action> pi; // obecna permutacja
	int cR, cQ; // przechowanie c.r i c.q
	int r1{ 0 }, p1{ 0 }, q1{ 0 }; // primy
	bool isC = false; // czy c jest potrzbne

	// 1.
	U = Schrage(actions, pi); // uruchomienie Schrage 

	// 2.
	if (U < UB) // jeśli Schrage wyliczy lepsze U (cMax) niż globalnie najlepszy UB
	{
		UB = U; // nowy najlepszy cMax jest równy U, które wyliczył Scharge
		pi_opt = pi; // optymalna permutacja to obecne pi
	}

	// 3.
	selectABC(pi, a, b, c, U, isC); // wyznaczanie b, a, c
	
	// 4.
	if (!isC) // jeżeli c nie istnieje powrót
		return;

	// 5.
	rqp(pi, r1, p1, q1, b, c); // wyznaczanie r', p', q'

	// 6.
	cR = c.r; // przechowanie starego c.r przed wejściem w rekurencję
	c.r = max(c.r, r1 + p1); // modyfikacja c.r (kopii) -- przesunięcie c za b
	actions[c.id - 1] = c; // wprowadzenie zmienionego c.r do oryginalnego zadania
	
	// 7. 
	LB = preSchrage(actions); // sprawdzenie czy istnieje hipotetycznie lepsze rozwiązanie w lewych dzieciach

	//showNodeDetails(current_node, parent, U, LB, a.id, b.id, c.id, pi);

	// 8. 9.
	if (LB < UB)
		Carlier(actions, pi_opt, UB, overall_nodes, current_node, parent); // jeśli istnieje hipotetycznie lepsze rozwiązanie w lewym dziecku -- kolejny Carlier

	//showCutLeftDetails(current_node, LB, UB);

	// 10.
	c.r = cR; // odtworzenie c.r po powrocie z rekurencji
	actions[c.id - 1] = c; // przywrócenie starego c.r do oryginalnego zadania
	
	// 11.
	cQ = c.q; // przechowanie starego c.q przed wejściem w rekurencję
	c.q = max(c.q, q1 + p1); //  modyfikacja c.q (kopii)-- przesunięcie c przed c + 1
	actions[c.id - 1] = c; // wprowadzenie zmienionego c.q do oryginalnego zadania

	// 12.
	LB = preSchrage(actions); // sprawdzenie czy istnieje hipotetycznie lepsze rozwiązanie w prawych dzieciach

	//showNodeDetails(current_node, parent, U, LB, a.id, b.id, c.id, pi);

	// 13. 14.
	if (LB < UB)
		Carlier(actions, pi_opt, UB, overall_nodes, current_node, parent); // jeśli istnieje hipotetycznie lepsze rozwiązanie w prawym dziecku -- kolejny Carlier

	//showCutRightDetails(current_node, LB, UB);

	// 15.
	c.q = cQ; // odtworzenie c.q po powrocie z rekurencji
	actions[c.id - 1] = c; // przywrócenie starego c.q do oryginalnego zadania
}

int main()
{
	int ond = 0, nd = ond, pnd = -1; // do liczenia węzłów
	int UB = INT_MAX; // początkowe UB równe makymalnie dużo
	vector<Action> pi_opt; // optymalna permutacja
	
	for (int i = 1; i <= 10; i++)
	{
		string name = "SCHRAGE";
		string num = to_string(i);
		string ext = ".DAT";
		cout << i << ". ";
		Carlier(loadFromFile(name + num + ext), pi_opt, UB, ond, nd, pnd);
		cout << UB << endl;

		ond = 0, nd = 0, pnd = -1;
		UB = INT_MAX;
		pi_opt.clear();
	}
	// Carlier(loadFromFile("SCHRAGE3.DAT"), pi_opt, UB, ond, nd, pnd);
}
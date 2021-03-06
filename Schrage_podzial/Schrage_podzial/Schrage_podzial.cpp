﻿#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <climits>

using namespace std;

struct Action
{
public:
	int r{ 0 }, p{ 0 }, q{ 0 };
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

void Schrage(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{
		int n;
		plik >> n;
		Action* A = new Action[n];

		priority_queue<Action, vector<Action>, ActionAscending> N;
		priority_queue<Action, vector<Action>, ActionDescending> G;

		int i = 0;
		while (!plik.eof())
		{
			plik >> A[i].r >> A[i].p >> A[i].q;
			i++;
		}

		for (int i = 0; i < n; i++)
			N.push(A[i]);

		delete[] A;

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
				
				// wprowadzenie podzialu
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
			cMax = max(cMax, t + e.q);
		}
		cout << cMax << endl;
	}
	else
	{
		cout << "Blad otwierania\n";
		exit(0);
	}
	plik.close();
}

int main()
{
	for (int i = 1; i <= 9; i++)
	{
		string name = "SCHRAGE";
		string num = to_string(i);
		string ext = ".DAT";
		Schrage(name + num + ext);
	}
}
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

int n, m;

struct Task
{
	vector<int> p;
	int sum_p{ 0 },
		id{ 0 };
};

struct LowestSum
{
	bool operator()(const Task& x, const Task& y)
	{
		return x.sum_p > y.sum_p;
	}
};

const priority_queue<Task, vector<Task>, LowestSum> loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{
		plik >> n >> m;
		vector<Task> tasks(n);
		priority_queue<Task, vector<Task>, LowestSum> sortedTasks;
		for (int i = 0; i < n; i++)
		{
			tasks[i].id = i + 1;
			for (int j = 0; j < m; j++)
			{
				int temp;
				plik >> temp;
				tasks[i].p.push_back(temp);
				tasks[i].sum_p += temp;
			}
			sortedTasks.push(tasks[i]);
		}

		plik.close();
		return sortedTasks;
	}
	else
	{
		cerr << "File error\n";
		exit(0);
	}
}
int main()
{
	priority_queue<Task, vector<Task>, LowestSum> sortedTasks;

	sortedTasks = loadFromFile("NEH1.DAT");

}


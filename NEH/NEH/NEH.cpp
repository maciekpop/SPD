#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

int n,
	m;

struct SubTask
{
	int p{ 0 },
		C{ 0 };
};

struct FullTask
{
	int id{ 0 },
		sum_p{ 0 };
	vector<SubTask> subTask;
};

struct Permutation
{
	vector<FullTask> perm;
	int cMax{ 0 };
};

struct HighestSum
{
	bool operator()(const FullTask& x, const FullTask& y)
	{
		return x.sum_p < y.sum_p;
	}
};

struct LowestC
{
	bool operator()(const Permutation& x, const Permutation& y)
	{
		return x.cMax > y.cMax;
	}
};

void printVec(const vector<FullTask>& vec)
{
	for (auto x : vec) 
		cout << ' ' << x.id;
}

template<typename T, class C>
void cleanQueue(priority_queue<T, vector<T>, C>& Q)
{
	while (!Q.empty())
		Q.pop();
}

priority_queue<FullTask, vector<FullTask>, HighestSum> loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{
		plik >> n >> m;
		vector<FullTask> task(n);
		priority_queue<FullTask, vector<FullTask>, HighestSum> taskList;

		for (int i = 0; i < n; i++)
		{
			task[i].id = i + 1;
			task[i].subTask.push_back(*new SubTask);
			for (int j = 0; j < m; j++)
			{
				SubTask subTask;
				plik >> subTask.p;
				task[i].subTask.push_back(subTask);
				task[i].sum_p += subTask.p;
			}

			taskList.push(task[i]);
		}

		plik.close();
		return taskList;
	}
	else
	{
		cerr << "File error\n";
		exit(0);
	}
}

void countPermutation_cMax(Permutation& P)
{
	int permFullTaskNumber = P.perm.size();

	for (int j = 1; j < permFullTaskNumber; j++)
		for (int k = 1; k <= m; k++)
				P.perm[j].subTask[k].C = max(P.perm[j - 1].subTask[k].C, P.perm[j].subTask[k - 1].C) + P.perm[j].subTask[k].p;

	P.cMax = P.perm[permFullTaskNumber - 1].subTask[m].C;
}

void NEH(priority_queue<FullTask, vector<FullTask>, HighestSum>& taskList, Permutation& permutation)
{
	FullTask task;
	priority_queue<Permutation, vector<Permutation>, LowestC> permutationQueue;
	
	for (int i = 0; i <= m; i++)
		task.subTask.push_back(*new SubTask);
	permutation.perm.push_back(task);

	while(!taskList.empty())
	{
		task = taskList.top();
		taskList.pop();
		
		for (int i = 1; i < (int)permutation.perm.size() + 1; i++)
		{
			auto it = permutation.perm.begin();
			permutation.perm.insert(it + i, task);
			countPermutation_cMax(permutation);
			permutationQueue.push(permutation);
			it = permutation.perm.begin();
			permutation.perm.erase(it + i);
		}
 
		permutation = permutationQueue.top();
		cleanQueue(permutationQueue);
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
		cout << "   cmax: " << P.cMax <<endl;
	}
}
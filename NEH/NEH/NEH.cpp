#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

int n, m;

struct SubTask
{
	int p{ 0 },
		C{ 0 };
};

struct Task
{
	int id{ 0 }, sum_p{ 0 };
	vector<SubTask> subTask;
};

struct Permutation
{
	vector<Task> order;
	int C{ 0 };
};

struct HighestSum
{
	bool operator()(const Task& x, const Task& y)
	{
		return x.sum_p < y.sum_p;
	}
};

struct LowestC
{
	bool operator()(const Permutation& x, const Permutation& y)
	{
		return x.C > y.C;
	}
};

void print_vec(const vector<Task>& vec)
{
	for (auto x : vec) {
		cout << ' ' << x.id;
	}
}

template<typename T, class C>
void cleanQueue(priority_queue<T, vector<T>, C>& Q)
{
	while (!Q.empty())
		Q.pop();
}

priority_queue<Task, vector<Task>, HighestSum> loadFromFile(string fileName)
{
	fstream plik;
	plik.open(fileName, ios::in);
	if (plik.good())
	{
		plik >> n >> m;
		vector<Task> task(n);
		priority_queue<Task, vector<Task>, HighestSum> taskList;

		for (int i = 0; i < n; i++)
		{
			task[i].id = i + 1;
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

int countPermutationCMax(vector<Task> task)
{
	int C = 0;
	for (int j = 0; j < (int) task.size(); j++)
	{
		for (int k = 0; k < m; k++)
		{
			if (j == 0 && k == 0)
				task[j].subTask[k].C = task[j].subTask[k].p;
		
			if (j == 0 && k != 0)
				task[j].subTask[k].C = task[j].subTask[k - 1].C + task[j].subTask[k].p;
			
			if (j != 0 && k == 0)
				task[j].subTask[k].C = task[j - 1].subTask[k].C + task[j].subTask[k].p;

			if (j != 0 && k != 0)
				task[j].subTask[k].C = max(task[j - 1].subTask[k].C, task[j].subTask[k - 1].C) + task[j].subTask[k].p;

			C = max(C, task[j].subTask[k].C);
		}
	}
	return C;
}

Permutation NEH(priority_queue<Task, vector<Task>, HighestSum> taskList)
{
	Task task;
	vector<Task> taskOrder;
	priority_queue<Permutation, vector<Permutation>, LowestC> permutationQueue;
	Permutation permutation;
	int cMax = INT_MAX;

	while(!taskList.empty())
	{
		task = taskList.top();
		taskList.pop();

		for (int i = 0; i < (int)taskOrder.size() + 1; i++)
		{
			taskOrder.insert(taskOrder.begin() + i, task);
			permutation.order = taskOrder;
			permutation.C = countPermutationCMax(taskOrder);
			permutationQueue.push(permutation);
			taskOrder.erase(taskOrder.begin() + i);
		}

		taskOrder = permutationQueue.top().order;
		cMax = permutationQueue.top().C;
		cleanQueue(permutationQueue);
	}

	permutation.order = taskOrder;
	permutation.C = cMax;
	return permutation;
}

int main()
{
	Permutation P;
	priority_queue<Task, vector<Task>, HighestSum> taskList;
	for (int i = 1; i <= 9; i++)
	{
		string name = "NEH";
		string num = to_string(i);
		string ext = ".DAT";
		cout << i << ". ";
		taskList = loadFromFile(name + num + ext);
		P = NEH(taskList);
		//cout << "permutacja: "; print_vec(P.tasksOrder); cout << endl; 
		cout << "   cmax: " << P.C <<endl;
	}
}
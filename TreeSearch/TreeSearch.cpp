#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <Windows.h>
#include <ctime>
#include <Psapi.h>
#include <processthreadsapi.h>
#pragma comment(lib, "psapi.lib")
#define n 8

using namespace std;

short c[n]; // номер хода, на котором посещается вершина
short path[n]; // номера посещаемых вершин
short v0; // начальная вершина

short a[n][n] = //матрица смежности
{
0, 1, 1, 0, 0, 0, 0, 0,
1, 0, 0, 1, 0, 0, 0, 0,
1, 0, 0, 1, 0, 0, 0, 0,
0, 1, 1, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 1, 1, 0,
0, 0, 0, 1, 1, 0, 0, 1,
0, 0, 0, 0, 1, 0, 0, 1,
0, 0, 0, 0, 0, 1, 1, 0,
};

unsigned long GetAppMemory() {
	PROCESS_MEMORY_COUNTERS pmc;
	pmc.cb = sizeof(pmc);
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}


void PathPrint(void)
{
	short p;
	for (p = 0; p < n; p++)
	{
		cout << path[p] + 1;
		if (p != n - 1)
			cout << " -> ";
	}
	cout << endl;
}

//подпрограмма нахождения гамильтонова цикла
//k - номер прохода или количество найденных вершин пути, результат - массив path, возвращает путь
short Hamilton(int k)
{
	short v; //индекс вершины
	short q1 = 0; //признак нахождения пути, сначала - не найдено
	path[0] = v0; //начинаем путь с вершины v0
	c[v0] = 0; // и она пройдена
	for (v = 0; v < n && !q1; v++) // обход матрицы по всем вершинам и пока не найден путь
	{
		//есть ли ребро между текущей вершиной и вершиной, найденной при предыдущем вызове
		if (a[v][path[k - 1]] || a[path[k - 1]][v])
		{
			if (k == n) //если обошли все вершины,
				q1 = 1; // то путь найден
			else
				if (c[v] == -1) //формируем путь, если в вершине v еще не были
				{
					c[v] = k; //номер прохода
					path[k] = v; //занесение вершины в найденный путь (для printGam)
					q1 = Hamilton(k + 1); //ищем следующую вершину
					if (!q1) c[v] = -1; //если путь не найден, то помечаем текущую вершину, как непройденную
				}
				else
					continue; //если в вершине уже были, то на анализ следующей вершины
		}
	} return q1; //возвращаем нашли или нет
}

int main()
{

	setlocale(LC_ALL, "RUS");

	auto start = std::chrono::high_resolution_clock::now();
	cout << "Поиск Гамильтонова пути для графа размерностью " << n << "x" << n << ":\n";
	for (size_t i = 0; i < n; i++)
	{
		short j;
		for (j = 0; j < n; j++)
			c[j] = -1; //помечаем, что все вершины не пройдены

		v0 = i;
		cout << "Для вершины " << i + 1 << ": ";

		if (Hamilton(1)) { //ищем путь, начиная с прохода 1
			PathPrint();
			cout << "Поиск цепей для остальных вершин нецелесообразен, ибо Гамильтонов путь была найдена." << endl;
			break;

		}
		else {
			cout << "Цепь не найдена \n";
		}
	}
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

	cout << "\nВремя выполнения функции нахождения Гамильтонова пути \nдля заданного графа размерностью " << n << "x" <<
		n << " составило: \n" << microseconds << " микросекунд." << endl;

	std::cout << "Память: " << GetAppMemory() << " бит." << std::endl;

}

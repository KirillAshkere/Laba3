#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;
int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream file("info.txt");

	List<string> lines;

	while (file)
	{
		string str;
		getline(file, str);

		lines.push_back(str);
	}
	lines.pop_back();

	List<string*> normalized_info = convert(lines);

	List<string> namesList = get_unique_names(normalized_info);

	Graph G(namesList);

	G.create_Graph(normalized_info);

	string start, end;
	cout << "Город отправления: ";
	cin >> start;
	cout << "Город назначения: ";
	cin >> end;
	cout << endl << endl;
	cout << G.dijkstra(start, end);
	cout << endl << endl;

	for (size_t i = 0; i < normalized_info.get_size(); ++i)
	{
		delete[] normalized_info.at(i);
	}
	system("pause");
	return 0;
}
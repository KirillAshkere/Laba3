#pragma once
#include "List.h"
#include "Map.h"
#include "Priority_queue.h"
#include <string>
#include <stdexcept>

constexpr uint8_t City_From = 0;
constexpr uint8_t City_To = 1;
constexpr uint8_t Cost_Forward = 2;
constexpr uint8_t Cost_Backward = 3;
constexpr uint8_t Amount_input_words_in_one_input_string = 4;


template<typename T>
List<T> reverse(List<T> lst);


class Graph
{
private:
	//Список смежности в котором хранятся номера смежной вершины с весом ребра
	Map<uint16_t, uint64_t>* adjList; 
    //Список - который показывает связь между названием города и вершины
	List<string> List_with_name_citys; 
	//Список хранит номера вершин и их метки
	priority_queue<uint16_t> List_with_marks;
	size_t size;

	//Строит самый дешёвый путь с помощью маркированного графа
	List<string> buildPath(priority_queue<uint16_t>& visited_Mark, const uint16_t& start, const uint16_t& end_place)
	{
		List<string> path;

		//Проверка,на то, что самый прямой путь самый лучший!
		if (adjList[start].contains(end_place))
		{
			if (adjList[start].find(end_place) == visited_Mark.get_priority(end_place))
			{
				path.push_front(List_with_name_citys.at(end_place));
				path.push_front(List_with_name_citys.at(start));
				return path;
			}

		}

		//Проверка на то возможно ли попасть в конечную точку
		if (visited_Mark.get_priority(end_place) == UINT64_MAX)
		{
			path.push_front("There is no route between " + List_with_name_citys.at(start) + " and " + List_with_name_citys.at(end_place));
			return path;
		}

		path.push_front(List_with_name_citys.at(end_place));
		uint16_t current = end_place;
		auto cur_mark = visited_Mark.get_priority(end_place);

		//Делаем путь от конечного города попасть до стартового
		while (current != start)
		{
			string city;

			//присваивание всех текущих соседей
			auto neighbors_list = adjList[current].get_keys();
			uint16_t neighbor;

			//проверяет метку у каждого соседа
			for (size_t i = 0; i < neighbors_list.get_size(); ++i)
			{
				neighbor = neighbors_list.at(i);

				if (cur_mark - adjList[neighbor].find(current) == visited_Mark.get_priority(neighbor))
				{
					city = List_with_name_citys.at(neighbor);
					break;
				}
			}

			path.push_front(city);
			current = neighbor;
			cur_mark = visited_Mark.get_priority(neighbor);
		}

		return path;
	}

public:

	explicit Graph(List<string>& List_with_name_citys)
		: size(List_with_name_citys.get_size())
	{
		this->List_with_name_citys = List_with_name_citys;
		adjList = new Map<uint16_t, uint64_t>[size];

		for (size_t i = 0; i < size; i++)
			List_with_marks.insert(i, UINT64_MAX);
	}


	~Graph()
	{
		delete[] adjList;
	}

	//создание графа на основе текстового файла
	void create_Graph(List<string*>& info) const
	{
		for (size_t i = 0; i < info.get_size(); i++)
		{
			const auto line = info.at(i);

			uint16_t city_from_number = List_with_name_citys.find(line[City_From]);
			uint16_t city_to_number = List_with_name_citys.find(line[City_To]);
			uint64_t cost_forward = stoull(line[Cost_Forward]);
			uint64_t cost_backward = stoull(line[Cost_Backward]);

			//заполняем список смежности на основе входной информации
			for (auto j = 1; j <= 2; j++)
			{

				adjList[city_from_number].insert(city_to_number, cost_forward);

				swap(city_from_number, city_to_number);
				swap(cost_forward, cost_backward);
			}

		}
	}

	//Нахождение самого короткого пути по алгоритму Дейкстры
	string dijkstra(const string& start_point, const string& destination_point)
	{
		if (start_point == destination_point)
			return "Начальная и конечная точки одинаковые!!";

		if (!List_with_name_citys.contains(start_point) || !List_with_name_citys.contains(destination_point))
			return "Не существует начального или конечного города!";

		//Посещённые вершины с их метками
		priority_queue<uint16_t> visitedMarksList; 
		//начало пути
		const uint16_t start = List_with_name_citys.find(start_point);
		//конец пути
		const uint16_t destination = List_with_name_citys.find(destination_point); 

		List_with_marks.correction(start, 0);

		while (List_with_marks.getSize())
		{
			const auto current_mark = List_with_marks.get_priority();
			const auto current = List_with_marks.extract_min(); 			
			const auto neighbors_list = adjList[current].get_keys(); 

			for (size_t j = 0; j < neighbors_list.get_size(); j++)
			{
				const auto neighbor = neighbors_list.at(j); 
				const auto visited = !List_with_marks.contains(neighbor); 
				const auto edge_weight = adjList[current].find(neighbor);

				if (!visited && edge_weight != 0 && current_mark != UINT64_MAX)
				{
					const auto mark = current_mark + edge_weight;

					if (List_with_marks.get_priority(neighbor) > mark)
						List_with_marks.correction(neighbor, mark);
				}

			}

			visitedMarksList.insert(current, current_mark);
		}

		const auto way = buildPath(visitedMarksList, start, destination);
		string result;

		if (way.get_size() > 1)
		{
			result = "Самый дешёвый маршрут: ";

			for (size_t i = 0; i < way.get_size() - 1; ++i)
				result += way.at(i) + " -> ";

			result += way.at(way.get_size() - 1) + "\nИтоговая цена: " + to_string(visitedMarksList.get_priority(destination));
		}
		else
			result = way.at(0);

		return result;

	}
};

//Преобразования входной строки
inline List<string*> convert(List<string>& info)
{
	if (info.get_size() == 0)
		throw length_error("Входная строка пуста");

	List<string*> separated_info;

	for (size_t i = 0; i < info.get_size(); ++i)
	{
		const auto line = new string[Amount_input_words_in_one_input_string];
		auto word_number = 0;

		for (auto ch : info.at(i))
		{
			if (ch == ';')
				word_number++;
			else
				line[word_number] += ch;
		}

		if (word_number > Amount_input_words_in_one_input_string)
			throw length_error("Неправильный формат ввода");

		if (line[Cost_Forward] == "N/A")
			line[Cost_Forward] = "0";

		if (line[Cost_Backward] == "N/A")
			line[Cost_Backward] = "0";

		if (line[Cost_Forward] == "0" && line[Cost_Backward] == "0")
			throw logic_error("Эти города не связаны на прямую, вообще никак!");

		separated_info.push_back(line);
	}

	return separated_info;
}

//Возвращение списка со всеми именами
inline List<string> get_unique_names(List<string*>& info)
{
	List<string> List_with_name_citys;

	for (size_t i = 0; i < info.get_size(); ++i)
	{
		if (!List_with_name_citys.contains(info.at(i)[City_From]))
			List_with_name_citys.push_back(info.at(i)[City_From]);

		if (!List_with_name_citys.contains(info.at(i)[City_To]))
			List_with_name_citys.push_back(info.at(i)[City_To]);
	}

	return List_with_name_citys;
}

template<class T>
List<T> reverse(List<T> lst)
{
	List<T> reversed;

	for (size_t i = 1; i <= lst.getSize(); ++i)
		reversed.push_back(lst.at(lst.getSize() - i));

	return reversed;
}

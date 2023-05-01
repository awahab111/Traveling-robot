#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void readfile(int **&adj_matrix, int &vertex_count, int *&vertex_time, int &T_time, string *&vertex, string file){
	ifstream fin(file);
	string line, edge[128],w;
	int edge_count = 0, *weights;
	char v, e, delimiter;
    vertex = new string[128];
	//getting vertices
	getline(fin, line);
	stringstream iss(line);	
	while (iss >> v)
	{
		if (v == 'V' || v == ' ' || v == '=' || v == '{' ||v == '}'|| v == ','){continue;}
		else{
			vertex[vertex_count] = v;
			vertex_count++;
		}
	}

	//getting edges
	getline(fin, line);
	stringstream iss2(line);
	while (iss2 >> e)
	{
		if (e == 'E' || e == ' ' || e == '=' || e == '{' ||e == '}'|| e == ','|| e == '(' ||e == ')'){continue;}
		else{
			edge[edge_count] = e;
			edge_count++;
		}
	}

	// making Matrix
	adj_matrix = new int*[vertex_count + 1 ];
	for (int i = 0; i < vertex_count + 1; i++){
		adj_matrix[i] = new int[vertex_count + 1];
		for (int j = 0; j < vertex_count + 1; j++){
			adj_matrix[i][j] = 0;
		}
	}

	// Extract the weights from the file
	weights = new int[edge_count/2];
	getline(fin, line);
	stringstream iss3(line);
	while (iss3 >> w)
	{
		if (w == "Weights" || w == " " || w == "=" || w == "{" ||w == "}"|| w == ","){continue;}
		else{
			int i = 0;
			w = w.substr(9, w.size()-2);
			stringstream ss(w);
			int num;
			while (ss >> num) {
				weights[i] = num;
				ss >> delimiter; // Read and discard the comma delimiter
				i++;
			}
		}
	}

    // Update the adjacency matrix with the weights from the edges
    for (int i = 0; i < edge_count; i += 2) {
        string from_vertex = edge[i];
        string to_vertex = edge[i+1];
        int weight = weights[i/2];
        int from_index = -1, to_index = -1;

        // Find the indices of the vertices in the vertex array
        for (int j = 0; j < edge_count; j++) {
            if (vertex[j] == from_vertex) {
                from_index = j;
            }
            if (vertex[j] == to_vertex) {
                to_index = j;
            }
        }

        // Update the adjacency matrix with the weight of the edge
        if (from_index != -1 && to_index != -1) {
            adj_matrix[from_index + 1][to_index + 1] = weight;
            adj_matrix[to_index + 1][from_index + 1] = weight; 
        }
    }

	getline(fin, line);
	string number_string;
    int number, i = 0;
    bool number_found = false;
	vertex_time = new int[vertex_count - 1];
    for (char c : line) {
        if (isdigit(c)) {
            number_string += c;
            number_found = true;
        } 
		else if (number_found) {
            number = stoi(number_string);
            vertex_time[i] = number;
			i++;
            number_string.clear();
            number_found = false;
        }
    }

    if (number_found) {
        number = stoi(number_string);
		vertex_time[i] = number;
    }

	
	
	getline(fin, line);
	size_t equal_pos = line.find("=");
    string time = line.substr(equal_pos+1);
    T_time = stoi(time);
}

int hamiltonianCycle(int i, int mask, int **adj_matrix, int T_time, int **memoization_table, int vertex_count, int number, vector<int> *&path)
{
    if (mask == ((1 << i) | 3)){
        path[number].push_back(1);
        path[number].push_back(i);
        return adj_matrix[1][i];
    }
    if (memoization_table[i][mask] != 0){
        return memoization_table[i][mask];
    }
    int res = T_time;
    for (int j = 1; j <= vertex_count; j++)
    {
        if ((mask & (1 << j)) && j != i && j != 1)
        {
            int subproblem = hamiltonianCycle(j, mask & (~(1 << i)), adj_matrix, T_time, memoization_table, vertex_count, number , path) + adj_matrix[j][i];
            if (subproblem < res){
                res = subproblem;
                path[number].push_back(i);  // Add the current vertex to the path vector.
                return res;
            }
        }
    }
    memoization_table[i][mask] = res;
    return res;
}

// Calling Funtion
void FindPath(string file){
    int **adj_matrix , vertex_count = 0, *vertex_time, T_time, delivery_time = 0;
    string *vertex;
    readfile(adj_matrix, vertex_count, vertex_time, T_time,vertex, file);
    vector<int>* path = new vector<int> [vertex_count + 1]; 

    //Intialize memoization table 
    int **memoization_table = new int* [vertex_count + 1];
    for (int i = 0; i < vertex_count + 1; i++){
        memoization_table[i] = new int[(1 << (vertex_count + 1))];
        for (int j = 0; j < (1 << (vertex_count + 1)); j++){memoization_table[i][j] = 0;}
    }
    
    // Find the traveling cost
    int burh = 0;
    int cost = T_time;
    for (int i = 1; i <= vertex_count; i++)
    {
        int subproblem = hamiltonianCycle(i, (1 << (vertex_count + 1)) - 1, adj_matrix, T_time, memoization_table , vertex_count, i, path) + adj_matrix[i][1];
        if (subproblem < cost)
            cost = subproblem;
            burh = i;
    }
    // Find the delivery time
    for(int i = 0; i < vertex_count - 1; i++){delivery_time += vertex_time[i];}

    if ((delivery_time + cost)> T_time)
    {
        cout << "NO FEASIBLE CIRCUIT" << endl;
        return;
    }

    cout << "Expected output: (";
    cout << "h" << ",";
    for (int i = 1; i <= path[burh].size() - 1; i++)
    {
        cout << vertex[path[burh][i] - 1];
        cout << ",";
    }
    cout  << "h)";

    return;
}

int main()
{
    string file_names[2] = {"test_2.txt", "test_1.txt"};

    for (int i = 0; i < 2; i++)
    {
        cout << "Test " << i + 1 << endl;
        FindPath(file_names[i]);
        cout << endl;
    }
}

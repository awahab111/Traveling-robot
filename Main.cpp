#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// const int n = 4;
// const int MAX = 38;
// int dist[n + 1][n + 1] = {
// {0 ,0 , 0 , 0 , 0 },
// {0 ,0 , 2 , 2 , 3 },
// {0 ,2 , 0 , 4 , 5 },
// {0 ,2 , 4 , 0 , 8 },
// {0 ,3 , 5 , 8 , 0 }};
// int memo[n + 1][1 << (n + 1)];
int fun(int i, int mask, int **dist, int T_time, int **memo , int vertex_count);

void readfile(int **&adj_matrix, int &vertex_count, int *&vertex_time, int &T_time){
	ifstream fin("test_2.txt");
	string vertex[128], line, edge[128],w;
	int edge_count = 0, *weights;
	char v, e, delimiter;

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


// Helper function to print path
void printPath(int i, int mask, vector<int> &path, int ** adj_matrix, int T_time, int **memo, int vertex_count)
{
    // Base case
    if (mask == ((1 << i) | 3))
    {
        path.push_back(i);
        return;
    }

    // Recursion
    for (int j = 1; j <= vertex_count; j++)
    {
        if ((mask & (1 << j)) && j != i && j != 1)
        {
            int subproblem = fun(j, mask & (~(1 << i)), adj_matrix, T_time, memo, vertex_count) + adj_matrix[j][i];
            if (memo[i][mask] == subproblem)
            {
                printPath(j, mask & (~(1 << i)), path, adj_matrix, T_time, memo, vertex_count);
                path.push_back(i);
                return;
            }
        }
    }
}

int fun(int i, int mask, int **dist, int T_time, int **memo, int vertex_count)
{
    if (mask == ((1 << i) | 3))
        return dist[1][i];
    if (memo[i][mask] != 0)
        return memo[i][mask];

    int res = T_time;
    for (int j = 1; j <= vertex_count; j++)
    {
        if ((mask & (1 << j)) && j != i && j != 1)
        {
            int subproblem = fun(j, mask & (~(1 << i)), dist, T_time, memo, vertex_count) + dist[j][i];
            res = min(res, subproblem);
        }
    }

    memo[i][mask] = res;
    return res;
}

int main()
{
    int **adj_matrix , vertex_count = 0, *vertex_time, T_time, delivery_time = 0;
    readfile(adj_matrix, vertex_count, vertex_time, T_time);
    


    //Intialize memoization table 
    int **memo = new int* [vertex_count + 1];
    for (int i = 0; i < vertex_count + 1; i++){
        memo[i] = new int[(1 << (vertex_count + 1))];
        for (int j = 0; j < (1 << (vertex_count + 1)); j++){memo[i][j] = 0;}
    }
    
    int cost = T_time;
    for (int i = 1; i <= vertex_count; i++)
    {
        int subproblem = fun(i, (1 << (vertex_count + 1)) - 1, adj_matrix, T_time, memo , vertex_count) + adj_matrix[i][1];
        if (subproblem < cost)
            cost = subproblem;
    }

    for (int i = 0; i < vertex_count - 1; i++){delivery_time += vertex_time[i];}

    if ((delivery_time + cost)> T_time)
    {
        cout << "NO FEASIBLE CIRCUIT" << endl;
        return 0;
    }

    

    vector<int> path;
    for (int i = 1; i <= vertex_count; i++)
    {
        int subproblem = fun(i, (1 << (vertex_count + 1)) - 1, adj_matrix, T_time ,memo , vertex_count) + adj_matrix[i][1];
        if (subproblem == cost)
        {
            printPath(i, (1 << (vertex_count + 1)) - 1, path, adj_matrix, T_time, memo , vertex_count);
            break;
        }
    }

    cout << "The cost of most efficient tour = " << cost << endl;
    cout << "Path: ";
    cout << "1" << " -> ";
    for (int i = path.size() - 1; i >= 0; i--)
    {
        cout << path[i];
        if (i != 0)
            cout << " -> ";
    }
    cout  << " -> "<< "1";

    return 0;
}

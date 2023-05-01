# Traveling-robot
## Introduction 
From reading and researching about the given question it can be easily said the problem is quite like the travelling salesman problem thus same algorithms can be used to solve this problem.

The travelling salesman problem (TSP) is a well-known combinatorial optimisation problem. The goal is to choose the fastest route that visits each city just once and returns to the starting point.

The TSP is NP-hard, which means that no polynomial-time algorithm exists that can solve it exactly. There are, however, some approximation techniques that can discover a solution that is near to the ideal answer.

## Approach 
The basic idea is to use dynamic programming to solve subproblems and store the results in a memoization table. We will start by selecting a starting vertex and then recursively visit all unvisited vertices until all vertices have been visited exactly once. We will also keep track of the minimum cost for each subproblem and store it in our memoization table.

## Pseudocode 
```
def hamiltonianCycle(i, mask, adj_matrix, T_time , memoization_table,vertex_count ,number, path):
    if visited all vertexes then:
        append 1 and i to the path for the number-th cycle
        return the cost of the edge from node 1 to node i in the adjacency matrix
    end if
    if memoization_table[i][mask] is not equal to 0 then:
        return memoization_table[i][mask]
    end if
    Set cost to T_time
    for j from 1 to vertex_count do:
        if j-th bit in vertex_visited is 1 AND j is not equal to i AND j is not equal to 1 then:
            subproblem = hamiltonianCycle(j, (mark as visited in Visited_vertex), adj_matrix, T_time, memoization_table, vertex_count, number, path) + cost of edge from node j to node i in the adjacency matrix
            if subproblem is less than cost then:
                set cost to subproblem
                append i to the path for the number-th cycle
            end if
        end if
    end for
    memoization_table[i][mask] = cost
    return cost
End Function
```

## Arguements 
i: The index of the current vertex.

visited_vertex: A bitmask that indicates which vertices have already been visited.

adj_matrix: The adjacency matrix of the graph.

T_time: The current time.

memoization_table: A table that stores the results of previously solved subproblems.

vertex_count: The number of vertices in the graph.

number: The number of the current path.

path: A vector that stores the vertices in the current path.


## Asymptotic Time Complexity
The total number of subproblems to be solved is 2^n, where n is the number of vertices in the graph. Each subproblem takes O(n) time to solve. Therefore, the time complexity of the algorithm is O(n^2*2^n).

## Conclusion 
The Hamiltonian Cycle Problem is an important problem in graph theory, and it has many real-world applications, such as in transportation and logistics. Although the problem is NP-hard, we can use dynamic programming to find an optimal solution in polynomial time.

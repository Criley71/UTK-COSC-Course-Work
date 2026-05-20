/*
* Challenge 5: graphs
* Connor Riley
* 2/26/23
* Dr. Emrich COSC302
*/

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

/*This is the depth first search function. It uses recursion and will return the
* truth value if there is a path between 2 nodes on a graph. It takes in the adjacenency matrix,
* the start index and end index, as well as a vector to store the visited nodes. The visit vector will be
* initialized in main as a vector the size of the amount of nodes in the graph all set to false. Each call
* it will set the index of src as true in the visit vector. The function starts by checking if the src and dst 
* are connected directly via the adjacency matrix. If not then it will check if it is indirectly connected. Running
* a for loop on the inner vector of the adjacency matrix and check if its connected(true) as well as its not been visited
* dfs will be called again but with the src being index i.
*/
bool dfs(vector<vector<bool>> graph, int src, int dst, vector<bool>& visit) {
	if (graph[src][dst] == true) {
		return true;
	}
	visit[src] = true;
	for (int i = 0; i < graph.size(); i++) {
		if (graph[src][i] == true && (visit[i] == false)) {
			if (dfs(graph, i, dst, visit)) {
				return true;
			}
		}
	}
	return false;
}
int main(int argc, char* argv[]) {
	int nEdges, nPaths, graphNum = 1, newSize;
	int src1Index, dst1Index;
	char src1, dst1;
	vector<vector<bool>> adjMatrix;
	vector<char> added;
	vector<char>::iterator srcItr;
	vector<char>::iterator dstItr;
	vector<bool> visited;
	
	/*The program starts by taking in the amount of connections in the graph and filling in the adjacency matrix
	* with the connections made. We check if either the src or dst have been already added into the matrix, if not 
	* they need to be resized. Other wise the truth value for the connection is made true. The added vector
	* keeps note of what has been added and can be used to find the index of an element.
	*/
	while (cin >> nEdges) {
		for (int edgeLoop = 0; edgeLoop < nEdges; edgeLoop++) {
			cin >> src1;
			cin >> dst1;
			newSize = 0;

			srcItr = find(added.begin(), added.end(), src1);
			if (srcItr == added.end()) {
				newSize += 1;
				added.push_back(src1);
			}
			dstItr = find(added.begin(), added.end(), dst1);
			if (dstItr == added.end()) {
				newSize += 1;
				added.push_back(dst1);
			}
			if (newSize != 0) {
				adjMatrix.resize(adjMatrix.size() + newSize);
				for (int i = 0; i < adjMatrix.size(); i++) {
					adjMatrix[i].resize(adjMatrix.size(), false);
				}
			}
			for (int src1Loop = 0; src1Loop < added.size(); src1Loop++) {
				if (added[src1Loop] == src1) {
					src1Index = src1Loop;
					break;
				}
			}
			for (int dst1Loop = 0; dst1Loop < added.size(); dst1Loop++) {
				if (added[dst1Loop] == dst1) {
					dst1Index = dst1Loop;
					break;
				}	
			}
				adjMatrix[src1Index][dst1Index] = true;
		}

		/*Next the user inputs the amount of paths they want to be checked if a path is there.
		* We first find the index of the source and destination. We make the visited vector 
		* false for each node in the graph. We then call dfs on the matrix, source index, destination index
		* and the visited vector. We then print if a path bewteen the nodes exists or not.
		*/
		cin >> nPaths;
		for (int pathLoop = 0; pathLoop < nPaths; pathLoop++) {
			cin >> src1;
			cin >> dst1;
			if (graphNum != 1 && pathLoop == 0) {
				cout << "\n";
			}
			for (int src1Loop = 0; src1Loop < added.size(); src1Loop++) {
				if (added[src1Loop] == src1) {
					src1Index = src1Loop;
					break;
				}
				else {
					src1Index == -1;
				}
			}
			for (int dst1Loop = 0; dst1Loop < added.size(); dst1Loop++) {
				if (added[dst1Loop] == dst1) {
					dst1Index = dst1Loop;
					break;
				}
				else {
					dst1Index = -1;
				}
			}
			visited.resize(adjMatrix.size(), false);
			if (dfs(adjMatrix, src1Index, dst1Index, visited)) {
				cout << "In Graph " << graphNum << " there is a path from " << src1 << " to " << dst1 << "\n";
			}
			else {
				cout << "In Graph " << graphNum << " there is no path from " << src1 << " to " << dst1 << "\n";
			}
			visited.clear();
		}

		added.clear();
		adjMatrix.clear();
		graphNum += 1;

	}

	return (0);
}

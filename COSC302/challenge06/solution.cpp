//Challenge 6: Minimum Spanning Tree
//Connor Riley 
//3/7/23 COSC302 Dr Emrich
//this program takes in a graph and uses prims algorithm to make a mininum spanning tree
#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <map>
#include <iterator>
using namespace std;

//The minvertice function takes in a vector of edges weights, a vector of bools denoting if a vertice has been 
//visited and the adjacency matrix graph. The purpose is to find which edge has the samllest weight and return the vertice.
//It loops through the edges and visit vectors checking if it hasnt been visited and if its the smallest weight.
int minVertice(vector<int> edges, vector<bool> visit, vector<vector<int>> graph){
  int minWeight, minIndex;
  minWeight = INT_MAX;
  for(int i = 0; i < graph.size(); i++){
    if (visit[i] == false && edges[i] < minWeight){
      minWeight = edges[i];
      minIndex = i;
    } 
  }
  return minIndex;
}

/*The print function takes in the the minspantree vector and the graph adjacenecy matrix
*and prints out the connected vertices and the total weight of the MST
*/
void printPath(vector<int> finalTree, vector<vector<int>> graph){
  map<string, int> edgeWeights;
  map<string, int>::iterator itr;
  string concatCoordinate = "";
  char left, right;
  int weight = 0;
  for(int i = 1; i < graph.size(); i++){

    if((i+65) < (finalTree[i] + 65)){
      concatCoordinate += (char)(i + 65);
      concatCoordinate += (char)(finalTree[i] + 65);
    }
    else if((i+65) > (finalTree[i] + 65)){
      concatCoordinate += (char)(finalTree[i] + 65);
      concatCoordinate += (char)(i + 65);
    }
    edgeWeights.insert(make_pair(concatCoordinate, graph[i][finalTree[i]]));
    concatCoordinate = "";
}
for(itr = edgeWeights.begin(); itr != edgeWeights.end(); itr++){
  weight += itr->second;
}
cout << weight << "\n";
for(itr = edgeWeights.begin(); itr != edgeWeights.end(); itr++){
  cout << itr->first << "\n";
}

}

/**
 * prims takes in the adjacency matrix. It creates the mininum spanning tree. 
 * it first makes 3 vectors, one to store the mst, one to store the weight of the edges in the mst, and the
 * last to store the nodes we have visited. we loop through the indices calling the minVertice function to find the smallest
 * edge weight to the next vertice, we then add the vertice to the MST vector. We then print the MST and its total weight
 */
void prims(vector<vector<int>> graphMatrix){
  vector<int> minSpanTree;
  vector<int> edgeWeight;
  vector<bool> visitedNodes;
  
  minSpanTree.resize(graphMatrix.size());
  edgeWeight.resize(graphMatrix.size());
  visitedNodes.resize(graphMatrix.size());

  for(int i = 0; i < graphMatrix.size(); i++){
    edgeWeight[i] = INT_MAX;
    visitedNodes[i] = false;
  } 
    edgeWeight[0] = 0; 
    
    for(int j = 0; j < graphMatrix.size() - 1; j++){//got to size - 1 as you dont need to check last vert
      int index = minVertice(edgeWeight, visitedNodes, graphMatrix);
      visitedNodes[index] = true;
        for(int k = 0; k < graphMatrix.size(); k++){
          if(graphMatrix[index][k] != -1 && visitedNodes[k] == false && graphMatrix[index][k] < edgeWeight[k]){
            minSpanTree[k] = index;
            edgeWeight[k] = graphMatrix[index][k]; 
          }
        }
    }
  
  printPath(minSpanTree, graphMatrix);
}


//this allows for user input of the amount of vertices in the graph
//and the weight between each of the vertices
int main(int argc, char *argv[]){
  int x = 0;
  int vertIn, weight;
  int loopCount = 0;
  vector<vector<int>> graph;
  while (cin >> vertIn){
    if(loopCount != 0){
      cout << "\n";
    }
    graph.resize(vertIn);
    for (int i = 0; i < graph.size(); i++){
      graph[i].resize(vertIn);
      for (int j = 0; j < graph.size(); j++){
        cin >> weight;
        graph[i][j] = weight;
        x++;
      }
    }
    prims(graph);
    graph.clear();
    loopCount++;
  }
  return (0);
}

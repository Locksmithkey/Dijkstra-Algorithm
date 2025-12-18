#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <algorithm>
#include "queue.h"
#include "graph.h"
#include <vector>

using namespace std;


struct Vertices
{
	string vertex = "";
	int distance = INT_MAX;
	bool visited = false;
	bool cycled = false;
	string previous = "N/A";
};

string FindMin(vector<Vertices>&);
void UpdateVertex(Graph<string>, vector<Vertices>&, string);
bool IsVertex(vector<Vertices>, string);
int Index(vector<Vertices>, string);
void Dijkstra(Graph<string>&, fstream&);
void FindPaths(Graph<string>, vector<Vertices>&);
void PrintTable(vector<Vertices>);

int main (int argc, char* argv[])
{
	Graph<string> myGraph(50);

	string file = argv[argc - 1];
	
	fstream dataFile(file);
	
	if(dataFile.is_open())
	{
		Dijkstra(myGraph, dataFile);
	}
	else
		cout << "Invalid File: " << file << endl;
	
}

//Update vertex inforamtio
void UpdateVertex(Graph<string> myGraph, vector<Vertices>& V, string startCity)
{
	Queue<string> myQ;
	
	myGraph.GetToVertices(startCity, myQ);

	int currentIndex = Index(V, startCity);

			while(!myQ.isEmpty())
	{
		string adjV = myQ.dequeue();
		int adjIndex = Index(V, adjV);
		int weight = myGraph.WeightIs(startCity, adjV);

		if(!V[adjIndex].visited)
		{
			int newDistance = V[currentIndex].distance + weight;
	
			if(newDistance < V[adjIndex].distance)
			{
				V[adjIndex].distance = newDistance;
				V[adjIndex].previous = startCity;
			}	
		}		
	}
}

//Find the small distance of current vertex
//returns smallest vertix name
string FindMin(vector<Vertices>& V)
{
	int minDistance = INT_MAX;
	int minIndex = -1;

	for(int i = 0; i < V.size(); i++)
	{
		if(!V[i].visited && V[i].distance < minDistance)
		{	
			minDistance = V[i].distance;
			minIndex = i;
		}
	}

	if(minIndex == -1)
		return "";

	return V[minIndex].vertex;
}

//Returns if vertex exist in graph
bool IsVertex(vector<Vertices> V, string name)
{
	bool isV = false; 

	if(V.empty())
		return isV;

	for(int i = 0; i < V.size(); i++)
	{
		if(V[i].vertex == name)
			isV = true;
	}
	
	return isV;
}

//Take input file data and add vertex and edges
//also performs Dijkstra Algorithm
void Dijkstra(Graph<string>& myGraph, fstream& file)
{
	int numVertex = 0;
	string lineDat, origin, dest, dist;
	vector<Vertices> vertices(50);
	
	//Intialez vector from file data
	while(getline(file, lineDat))
	{
		origin = lineDat.substr(0, lineDat.find(';'));
		dest = lineDat.substr(origin.length() + 1, lineDat.rfind(';') - origin.length() - 1);
		dist = lineDat.substr(lineDat.rfind(';') + 1, lineDat.find('\n'));
	
		if(!IsVertex(vertices, origin))
		{	
			myGraph.AddVertex(origin);
			vertices[numVertex].vertex = origin;
			numVertex++;
		}	
	
		if(!IsVertex(vertices, dest))
		{
			myGraph.AddVertex(dest);
			vertices[numVertex].vertex = dest;
			numVertex++;
		}
		
		myGraph.AddEdge(origin, dest, stoi(dist));
	}

	//resizes and sorts vector after initialization
	vertices.resize(numVertex);

	sort(vertices.begin(), vertices.end(), [](const Vertices& a, const Vertices& b) {return a.vertex < b.vertex;});
 
	//Prints from orgin cities
	cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\tDIJKSTRA'S ALGORITHM\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
	cout << "A Weighted Graph Has Been Built For These " << numVertex << " Cities\n" << endl;
	
	for(int i = 1; i <= numVertex; i++)
	{
		cout << setw(30) << vertices[i - 1].vertex;
		
		if(i % 3 == 0)
			cout << "\n";
	}
	
	cout << endl;

	FindPaths(myGraph, vertices);
	PrintTable(vertices);
}

//returns index of string vertex
int Index(vector<Vertices> V, string city)
{
	int index = -1;

	for(int i = 0; i < V.size(); i++)
	{
		if(V[i].vertex == city)
			index = i;	
	}

	return index;
}

//Use Dijkstra steps to find shortest path for each city
//and updates values in vector
void FindPaths(Graph<string> myGraph, vector<Vertices>& V)
{
	string startingCity;

	cout << "\nPlease input your starting vertex: ";
	cin >> startingCity;
	cout << endl;

	int startingIndex = Index(V, startingCity);

	if(startingIndex == -1)
	{
		cout << "Invalid City Name, Try Again.\n";
		FindPaths(myGraph, V);
	}
		
	V[startingIndex].distance = 0;
	V[startingIndex].previous = "N/A";

	while(true)
	{
		string current = FindMin(V);
		
		if(current == "")
			break;

		int currentIndex = Index(V, current);
			
		V[currentIndex].visited = true;
		UpdateVertex(myGraph, V, current);
	}

}

//Print shortest paths from starting input city
void PrintTable(vector<Vertices> V)
{
	cout << "---------------------------------------------";
	cout << "---------------------------------------------\n";

	cout << setw(30) << "Vertex"
		     << setw(30) << "Distance"
		     << setw(30) << "Previous\n" << endl;

	sort(V.begin(), V.end(), [](const Vertices& a, const Vertices& b) {return a.distance < b.distance;});

	for(int i = 0; i < V.size(); i++)
	{
		cout << setw(30) << V[i].vertex;

		if(V[i].distance == INT_MAX)
			cout << setw(30) << "--";
 		else
		 	cout << setw(30) <<  V[i].distance;
		     
		cout << setw(30) << V[i].previous << endl;
	}
	
	cout << endl;
}

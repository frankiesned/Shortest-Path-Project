/****************************
Templated created by Kazumi Slott
CS311

Your names: Jose Urrutia, Tyler Tran, Frankie Snedecor
*****************************/
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <list>
#include <stack>
#include "minHeap-graph.h"

using namespace std;

class edge
{
  friend class graph;//You want to access the private members of edge class from graph class
  int neighbor; //adjacent vertex
  int wt; //weight of the edge
 public:
  edge() { neighbor = -999, wt = -999;};
  edge(int u, int w) {neighbor = u; wt = w;};
};

class graph
{
  int num_ver; //the total number of vertices in the graph
  list<edge*>* ver_ar; //pointer to the dynamic array that holds linked lists. The element of a node in a linked list is a pointer to an edge object 
  int nextUnvisitedNodes(int* num, int start, int s);

 public:
  graph(int V);
  ~graph();
  void addEdge(int v, int u, int w=1);  //If the weight is not given, 1 is used for wt.
  void DijkstraShortestPath(int start, int dest, stack<int>& s); 
  void showPath(int* curDist, int* predecessor, int start, int dest, stack<int>& s);
};

//constructor: num is the number of vertices
graph::graph(int num)
{
  //make a dynamic array with num slots where each slot holds a list object. 
  //The element of a node in the linked list is a pointer to an edge object 
  this->num_ver = num;
  ver_ar = new list<edge*>[num_ver];  
}

graph::~graph()
{
  for(int i = 0; i<num_ver; i++)
    {
      for (list<edge*>::iterator u = ver_ar[i].begin(); u != ver_ar[i].end(); u++)//traverse the list and delete edge objects *u
	{
	  delete *u;//gives me the element pointed to by u, which is a pointer to an edge object
	}
    }
  delete[] ver_ar;//delete the vertices array, the list class deletes itself
}


//add an edge from v to u with weight w into the graph
void graph::addEdge(int v, int u, int w)
{  
  ver_ar[v].push_back(new edge(u,w));//push new edge object into the list at current array slot
}


//num is the array of vertices                                                                                       //start is the start vertex                                                                      
//s is the vertext right after the vertex we started last time                       
int graph::nextUnvisitedNodes(int* num, int start, int s)
{
 
  for(int i = s%num_ver; i != start; i = (i+1)%num_ver)//using s%num_ver will simply give us the next vertex after start, until we reach the end. Then (i+1)%num_ver will loop back to check the zero slot in the array because it will evetually reach num_ver%num_ver which equals zero.
    {
      if(num[i] == 0)
	return i;
    }
  
  return -1;
}

//adds all the indexes of the path to the stack so we can print in the main.
//Parameter 1: int* curDist is a dynamic array that holds indexes for the current distances
//Parameter 2: int* predecessor is a dynamic array that holds the previously visted indexes
//Parameter 3 & 4: Start is where the graph starts and dest is where it is taking its path to
//Parameter 5: a stack of indexs used to print in main
void graph::showPath(int* curDist, int* predecessor, int start, int dest, stack<int>& s)
{
  if(predecessor[dest] == -1)//if edges dont connect, exit funct
    {
      return;
    }

  while(dest != start)//traverse thru predecessor arr and store it into stack
    {
      //insert path into stack
      s.push(dest);
      dest = predecessor[dest];//traverse backwards from dest to right before start
    }
 
  s.push(start);//push start into stack, didnt hit it in the while loop
}


//Dijkstra's shortest path. This function will generate a table that contains the shortest distance from start to every other vertex and the predecessor of each vertex.
void graph::DijkstraShortestPath(int start, int dest, stack<int>& s)
{
  minHeap<int> toBeChecked(num_ver); //the top of this heap has the vertex with the shortest distance
  int *curDist = new int[num_ver];//contains the current shortest distance from start to every other vertex
  int *predecessor = new int[num_ver];//contains the predecessor of each vertex
  int *locator = new int[num_ver];//tells where each vertex exists within the heap. e.g. heap [v3, v1, v2, v4,v0] locator [4, 1, 2, 0, 3] <== vertext 0 can be found at index 4 in heap, vertex 3 can be found at index 0 in heap


  //initialize each array
  //curDist should be 999 for all vertices to start with
  for(int i=0; i<num_ver; i++)
    {
      curDist[i] = 999;
    }

  //populate toBeChecked heap
  //insert all vetices into toBeChecked heap: [0, 1, 2, 3, 4, ...] the numbers are vertex numbers
  for(int i=0;i<num_ver; i++)
    {
      toBeChecked.insert(i, curDist, locator);
    }

  //to start with, locator [0, 1, 2, 3, 4, ...]
  for(int i=0; i<num_ver; i++)
    {
      locator[i] = i;
    }
  //Initialize predecessor for each vertex to -1
  for(int i=0; i<num_ver; i++)
    {
      predecessor[i] = -1;
    }
  
  curDist[start] = 0;//dist from first to first is zero
  int v = start;
  
  while(toBeChecked.getNum() > 0)//while tbc is not empty
    {

      toBeChecked.fixHeap(locator[v], curDist, locator);//fix heap to start with
      v = toBeChecked.getMin(curDist, locator);//v = a vertex to be checked w min curDist, getMin removes the top
      
      
      for (list<edge*>::iterator u = ver_ar[v].begin(); u != ver_ar[v].end();u++)//for all vertices u adjacent to v
	{
	  
	  
	  if(locator[(*u)->neighbor] < toBeChecked.getNum())//check if u is in tbc(heap)
	    {

	      if(curDist[(*u)->neighbor] > ((*u)->wt+curDist[v]))//check if old distance of u is greater than the new distance
		{

		  curDist[(*u)->neighbor] = ((*u)->wt+curDist[v]);//replace the dist of u with new distance
		  predecessor[(*u)->neighbor] = v;//add v to be the predecessor of u
		  toBeChecked.fixHeap(locator[(*u)->neighbor], curDist, locator);//fix the heap once again
		} 
	    }
	}
    }

    
  
  //Now currDist and predecessor have the info about the shortest distance from start to every other vertex and thepredecessor of each vertex
  
  
  
  showPath(curDist, predecessor, start, dest, s);//create stack, put path into it
  s.push(curDist[dest]);
  
  
  delete[] curDist;
  delete[] locator;
  delete[] predecessor;
  
}




#endif

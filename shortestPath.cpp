/********************************
Program made by Jose Urrutia, Tyler Tran, Frankie Snedecor 
Date: 5/11/23
Course: CS311 Data Structures and Algorithms

Description of program:
Displays the shortest route from city to city along with information of each city. Using graphs and dynamic arrays we calculated the shortest distance between the cities.


 *******************************/
#include <iostream>
#include "graph.h"
#include <fstream>
#include <stack>

using namespace std;


class city
{
  
  int num, pop, elev;
  string abrv;
  string name;
  city* next;

public:
  city() { num = pop = elev = 0; abrv = name = ""; next = NULL; }
  city(int num, const string& abrv, const string& name, int pop, int elev) {this-> num = num, this->abrv = abrv, this->name = name, this->pop = pop, this->elev = elev, next = NULL; }
  string getName() const { return name; }
  string getAbrv() const { return abrv; }
  int getNum() const { return num; }
  int getPop() const { return pop; }
  int getElev() const { return elev; }
  city* getNext() const { return next; }
  int find(city ar[], string abrv);
};

int find(city* ar[], string abrv);
void showBestPath(stack<int> s, city* ar[]);
int counter = 0;

int main (int num_args, char* arg[])
{
  //Print out project info
  cout<< "Program made by Jose Urrutia, Tyler Tran, Frankie Snedecor" << endl;
  cout<< "Date: 5/11/23\n";
  cout<< "Course: CS311 Data Structures and Algorithms\n";

  cout<< "Description of program: Displays the shortest route from city to city along with information of each city. Using graphs and dynamic arrays we calculated the shortest distance between the cities." << endl;
  cout << "--------------------------------------------------------------------------------------------------"<< endl;


  //variables to store file data into, will create city objects with these variables
  int num, pop, elev;
  string abrv, name;

  //check if user input the correct amount of args in the input
  if(num_args != 3)
    {
      cout << "Run as follows using two letters for city( ex: ./a.out AN BK) " << endl;
      cout << endl;
      return 1;
    }

  string start(arg[1]);//change char into string
  string dest(arg[2]);
  city* ar[20];//array of cities
  //int counter=0;

  ifstream fin;
  fin.open("city.txt");//open city file
  fin >> num >> abrv >> name >> pop >> elev;//store the first line of text into variables
  while(fin)//populate array with new city objects
    {
      ar[counter] = new city(num, abrv, name, pop, elev);//insert variables as new city object in the array
      counter++;
      fin >> num >> abrv >> name >> pop >> elev;//store next line of text into variables
    }
  fin.close();//close file
  
  //populate graph with road file
  fin.open("road.txt");
  graph g(counter);//create graph with the same amount of vertices as the amount of cities
  int from, to, dist;
  
  fin >> from >> to >> dist;//fill variables to store into graph
  while(fin)//fill graph
    {
      g.addEdge(from, to, dist);
      fin >> from >> to >> dist;
    }
  fin.close();
  
  //get index of starting and ending city
  from = find(ar,start);
  to = find(ar, dest);
  
  //if find function returns -1, city code is invalid
  if(from == -1)
    {    
      cout << "Invalid city code: " << start  <<  " "  << endl;
      return 1;
    }
  if(to == -1)
    {
      cout << "Invalid city code: " << dest  <<  " "  << endl;
      return 1;
    }
  
  stack<int> s;

  //print city info
  cout << "From City: " <<(*ar[from]).getName()<< ", population " << (*ar[from]).getPop() << ", elevation " << (*ar[from]).getElev() <<endl;
  cout << "To City: " <<(*ar[to]).getName()<< ", population " << (*ar[to]).getPop() << ", elevation " << (*ar[to]).getElev() <<endl;

  g.DijkstraShortestPath(from, to, s);//find shortest path, place it
  
  //check for input errors and instances of no available route FROM city TO city
  if(s.empty())
    {
      cout << "No route from " << (*ar[from]).getName() << " to " << (*ar[to]).getName();
    }
  else if(s.top() == 999)
    {
      cout << "No route from " << (*ar[from]).getName() << " to " << (*ar[to]).getName();
      cout << endl;
    }

  else 
    {
      cout << "The shortest distance from " << (*ar[from]).getName() << " to " << (*ar[to]).getName() << " is " << s.top() << endl;
      showBestPath(s, ar);//will find shortest path and print distance,
    }
  
}


//will take in string abreviation of city and return the index where it is located in the array
//returns index if found, -1 if not found
//Parameter 1: array of city object
//Parameter 2: city abbreviation in city.txt file
int find(city* ar[], string abrv)
{
  for(int i = 0; i < counter; i++)
    {
      if((*ar[i]).getAbrv() == abrv)
	return i;
    }
  return -1;//returns -1 if city is not found
}

//will print the path FROM city TO city, doesn't return anything
//Parameter 1: stack of indexes
//Parameter 2: array with city objects
void showBestPath(stack<int> s, city* ar[])
{
  string print;
  s.pop(); //takes out the shortest path number
  cout << "Through the route: ";
  while(!s.empty())
    {
      
      print = (*ar[s.top()]).getName();
      s.pop();
      if(!s.empty())
	cout << print << "---> ";
      else
	cout << print << endl;
    }
  cout << endl;
}


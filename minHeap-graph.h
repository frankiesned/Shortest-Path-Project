/*****************************************************
Template prepared by Kazumi Slott
CS311
min heap class

Your name: Jose Urrutia, Tyler Tran, Frankie Snedecor
*******************************************************/
#ifndef MINHEAP_H
#define MINHEAP_H
#include <iostream> //for operator<<()
using namespace std;
#include "swap.h" 

template <class T>
class minHeap;

template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h);

template <class T>
class minHeap
{
  friend ostream& operator<< <T>(ostream& o, const minHeap<T>& h);
private:
  T* ar; //dynamic array
  int capacity; //the size of ar
  int num; //the number of elements I have in ar
public:
  minHeap(){ar = NULL; capacity = 0; num = 0;}
  minHeap(int c);
  ~minHeap(){if(ar!=NULL)delete [] ar;}
  void min_heapify(int i, int* curDist, int* locator);
  void bubbleUp(int i, int* curDist, int* locator);
  void insert(const T& el, int* curDist, int* locator);
  int find(const T& key) const;
  void remove(int i, int* curDist, int* locator);
  T getMin(int* curDist, int* locator);
  void fixHeap(int i, int* curDist, int* locator);
  int getNum() const;

  class Overflow{};
  class BadIndex{};
  class NotFound{};
};

template<class T> 
minHeap<T> :: minHeap(int c)
{
  ar = new T[c];
  capacity = c;
  num = 0;
}


template<class T> 
void minHeap<T>  :: min_heapify(int i, int* curDist, int* locator)
{
  int l = (2*i)+1; //the index of the left child of i
  int r = (2*i)+2; //the index of the right child of i
  int smallest = i; //the index of the smallest value amoung the values at i, l and r
  
  if(l<num && curDist[ar[l]] < curDist[ar[smallest]])//if left child is lesser, smallest gets that index
    smallest = l;

  if(r<num && curDist[ar[r]] < curDist[ar[smallest]])//otherwise, smallest gets right index
    smallest = r;
  

  //The largest is either the right or left child. We need to exchange the parent with it.
  if(smallest != i)//if child > than parent, exchange largest child value with parent value
    {
      //manually swap values at smallest and i indexes
      locator[ar[i]] = smallest;
      locator[ar[smallest]] = i;
      //exchange the 2 values
      mySwap(ar[smallest], ar[i]);
      //fix violations
      min_heapify(smallest, curDist, locator);
    }
  
}


template<class T> 
void minHeap<T>::insert(const T& el, int* curDist, int* locator){
  
  if(num >= capacity)//
    throw Overflow(); //"The array is full"; 
  else
    {
      ar[num++] = el;//insert new element into the next available index
      bubbleUp(num-1, curDist, locator);//bubble up to correct place in heap
    }
}

template <class T>
int minHeap<T>::find(const T& key) const
{
  for(int i = 0; i < num; i++) 
    if(ar[i] == key)
      return i;

  //The element doesn't exist 
  throw NotFound();// "The element doesn't exist";                                                                                             
}

template<class T> 
void minHeap<T> :: remove(int i, int* curDist, int* locator)
{
  if(i > capacity || i < 0)
    throw BadIndex(); //"The element doesn't exist";
  
  else
    {
      locator[ar[i]] = num-1;
      locator[ar[num-1]] = i;

      mySwap(ar[i], ar[num-1]);//move element to be removed to the end
      num--;//decrease size of array
      fixHeap(i, curDist, locator);//will bubble up or minHeapify
      
    }
}



template<class T> 
T minHeap<T> :: getMin(int* curDist, int* locator)
{
  //This function removes the top element and returns it.
  T min;
  min = ar[0];//store lowest value
  remove(0, curDist, locator);//remove lowest value, remove func will fix heap
  
  return min;
}


template<class T> 
void minHeap<T> :: bubbleUp(int i, int* curDist, int* locator)
{
  int parent = (i-1)/2;
  int child = i;//called it child just for clarity
  while(curDist[ar[parent]] > curDist[ar[child]] && child > 0)//while parent is greater than the new element
    {
      locator[ar[child]] = parent;
      locator[ar[parent]] = child;

      mySwap(ar[parent], ar[child]);//exchange new element with parent
      child = parent;//i is new parent
      parent = (parent-1)/2;//grandparent
    }
  
}

template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h)
{
  o << "The queue has: ";

  for(int i=0; i< h.num; i++)
    {
      o << h.ar[i] << " ";
    }

  o << endl;
}

template<class T>
void minHeap<T> :: fixHeap(int i, int* curDist, int* locator)
{
  int parent = (i-1)/2;
  int child = (2*i)+1;
  if(i<num && i>=0)
    {
      if(curDist[ar[i]] < curDist[ar[parent]])//if swapped element is lesser than it's parent, bubbleUp
	bubbleUp(i, curDist, locator);
      
      else if(curDist[ar[i]] > curDist[ar[child]] || curDist[ar[i]] > curDist[ar[child+1]])//otherwise, min heapify
	min_heapify(i, curDist, locator);
      
    }
}
template<class T>
int minHeap<T> :: getNum() const
{
  return num;
}

#endif

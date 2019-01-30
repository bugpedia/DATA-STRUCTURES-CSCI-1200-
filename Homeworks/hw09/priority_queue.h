#ifndef priority_queue_h_
#define priority_queue_h_

#include <cassert>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// The DistancePixel_PriorityQueue is a customized, non-templated
// priority queue that stores DistancePixel pointers in a heap.  The 
// elements in the heap can be looked up in a map, to quickly find out
// the current index of the element within the heap.

// ASSIGNMENT: The class implementation is incomplete.  Finish the
//   implementation of this class, and add any functions you need.

// =========================================================================

class DistancePixel_PriorityQueue {

 public:
  
  // --------------------------
  // CONSTRUCTORS
  // default constructor
  DistancePixel_PriorityQueue() {}
  // construct a heap from a vector of data
  DistancePixel_PriorityQueue(const std::vector<DistancePixel*> &values) 
  {
    for (int i = 0; i < values.size(); i++)
    {
      this->push(values[i]);
    }
  }

  // ------------------------
  // ACCESSORS
  int size() { return m_heap.size(); }
  bool empty() { return m_heap.empty(); }
  int last_non_leaf() { return (size()-1) / 2; }
  int get_parent(int i) { assert (i > 0 && i < size()); return (i-1) / 2; }
  bool has_left_child(int i) { return (2*i)+1 < size(); }
  bool has_right_child(int i) { return (2*i)+2 < size(); }
  int get_left_child(int i) { assert (i >= 0 && has_left_child(i)); return 2*i + 1; }
  int get_right_child(int i) { assert (i >= 0 && has_right_child(i)); return 2*i + 2; }

  // read the top element
  const DistancePixel* top() const  {
    assert(!m_heap.empty());
    return m_heap[0]; 
  }

  // is this element in the heap?
  bool in_heap(DistancePixel* element) const {
    std::map<DistancePixel*,int>::const_iterator itr = backpointers.find(element);
    return (itr != backpointers.end());
  }

  // add an element to the heap
  void push(DistancePixel* element) {
    std::map<DistancePixel*,int>::iterator itr = backpointers.find(element);
    assert (itr == backpointers.end());
    m_heap.push_back(element);
    backpointers[element] = m_heap.size()-1;
    this->percolate_up(int(m_heap.size()-1));
  }

  // the value of this element has been edited, move the element up or down
  void update_position(DistancePixel* element) {
    std::map<DistancePixel*,int>::iterator itr = backpointers.find(element);
    assert (itr != backpointers.end());
    this->percolate_up(itr->second);
    this->percolate_down(itr->second);
  }
  
  // remove the top (minimum) element
  void pop() {
    assert(!m_heap.empty());
    int success = backpointers.erase(m_heap[0]);
    assert (success == 1);
    m_heap[0] = m_heap.back();
    m_heap.pop_back();
    this->percolate_down(0);
  }

  void print_heap() {
    for (int i = 0; i < m_heap.size(); i++)
    {
      cout << "(" << m_heap[i]->getY() << "," << m_heap[i]->getX() << ")  " << m_heap[i]->getValue() << endl;
    }
    cout << m_heap.size() << endl;
  }
  
 private:

  // REPRESENTATION
  // the heap is stored in a vector representation (the binary tree
  // structure "unrolled" one row at a time)
  std::vector<DistancePixel*> m_heap;
  // the map stores a correpondence between elements & indices in the heap
  std::map<DistancePixel*,int> backpointers;

  // private helper functions
  void percolate_up(int i) {
    // set an iterator to the pixel we want to find
    map<DistancePixel*,int>::iterator itr = backpointers.find(m_heap[i]);

    // loop through as long as we are not at the "root" so to speak
    while (i != 0 && i < m_heap.size())
    {
      // get the parent
      int par = get_parent(i);
      // if the value of the pixel is less than that of the parent, swap
      // the parent with the pixel
      if (m_heap[i]->getValue() < m_heap[par]->getValue())
      {
        DistancePixel* temp = m_heap[par];
        m_heap[par] = m_heap[i];
        m_heap[i] = temp;
        i = par;
      }
      else break;
    }

    // change the position of the pixel if it has changed
    itr->second = i;
  }
  
  void percolate_down(int i) {
 
    map<DistancePixel*,int>::iterator itr = backpointers.find(m_heap[i]);

    // as long as there exists a left child of the root we are currently 
    // inspecting...
    while (has_left_child(i))
    {
      int j;

      // if it has a right child, and the value of the right child is less than
      // the left child's value, set an integer j to the right child, otherwise
      // they're the left child
      if (has_right_child(i) && m_heap[get_right_child(i)]->getValue() < 
        m_heap[get_left_child(i)]->getValue())
      {
        j = get_right_child(i);
      }
      else
        j = get_left_child(i);

      // if the value of j is less than the value of i, perform a swap
      if (m_heap[j]->getValue() < m_heap[i]->getValue())
      {
        DistancePixel* temp = m_heap[j];
        m_heap[j] = m_heap[i];
        m_heap[i] = temp;
        i = j;
      }
      else break;
    }

    itr->second = i;
  }
};

#endif

// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.  
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// DepthIterator, and BreadthIterator.
//
// ===================================================================

#ifndef quad_tree_h_
#define quad_tree_h_

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <string>


using namespace std;

// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).

template <class number_type>
class Point {
public:
  Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
  // REPRESENTATION
  number_type x;
  number_type y;

  // OPERATORS
  bool operator== (const Point& pt2) {return x == pt2.x && y == pt2.y;}
  bool operator!= (const Point& pt2) {return x != pt2.x && y != pt2.y;}
};

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
  ostr << "(" << pt.x << "," << pt.y << ")";
  return ostr;
}

template<class number_type, class label_type>
void BalanceTree(vector<pair<Point<number_type>,label_type> >)
{

}

// ==============================================================
// ==============================================================



// --------------------------------------------------------------
// QUAD TREE NODE CLASS
template <class number_type, class label_type>
class Node {
public:
  // CONSTRUCTOR
  Node(const Point<number_type>& pt_init, 
    const label_type& lab_init) : pt(pt_init), label(lab_init), parent(NULL) 
  {
    // Set every child to NULL
    for (int i = 0; i < 4; i++) children[i] = NULL;
  }
  
  // MEMBER VARIABLES
  Point<number_type> pt;
  label_type label;
  Node<number_type, label_type>* children[4];
  Node<number_type, label_type>* parent;
};

template <class number_type, class label_type> class QuadTree;

// --------------------------------------------------------------
// DEPTH ITERATOR CLASS
template <class number_type, class label_type>
class DepthIterator {
public:
  // constructors
  DepthIterator() : ptr_(NULL) {}
  DepthIterator(Node<number_type,label_type>* p) : ptr_(p) {}

  // dereference operator (*)
  const Point<number_type>& operator*() const {return ptr_->pt;}

  // comparison operators
  bool operator== (const DepthIterator& rgt) {return (ptr_ == rgt.ptr_);}
  bool operator!= (const DepthIterator& rgt) {return (ptr_ != rgt.ptr_);}

  // HELPER FUNCTIONS
  // Goes through all possible places for children and sets the Node to
  // the first possible one
  bool AdvanceLevel(Node<number_type,label_type>* &p)
  {
    for (int i = 0; i < 4; i++)
    {
      // If the child exists, go to it
      if (p->children[i])
      {
        p = p->children[i];
        return true;
      }
    }
    return false;
  }

  // Checks to see if we need to use the NextNode helper function to advance
  // to a different branch (return true) or if we can continue in this 
  // branch (return false)
  bool AdvanceNode(Node<number_type,label_type>* &p, 
    Node<number_type,label_type>* t)
  {
    for (int i = 0; i < 3; i++)
    {
      if (t == p->children[i] && !isLast(t))
      {
        p = NextNode(t);
        return false;
      }
    }
    return true;
  }

  // Checks to see if the child is the last of the four, meaning if it is
  // the third child, true will be returned, and if it is either 1, 2, or 3
  // then true will be returned only if the greater children are NULL
  bool isLast(Node<number_type,label_type>* p)
  {
    // Only true when child 1, 2, and 3 are NULL
    if (p == p->parent->children[0])
      return (!p->parent->children[1] && !p->parent->children[2] 
        && !p->parent->children[3]);
    // Only true when child 2 and 3 are NULL
    else if (p == p->parent->children[1])
      return (!p->parent->children[2] && !p->parent->children[3]);
    // Only true when child 3 is NULL
    else if (p == p->parent->children[2])
      return (!p->parent->children[3]);
    // 3rd child, always true
    else
      return true;
  }

  // Function returns the next node in the sequence, for example, if given
  // a Node at child 0, it will advance it to the next non-NULL child,
  // either 1, 2, or 3 in that case
  Node<number_type,label_type>* NextNode(Node<number_type,label_type>* p)
  {
    // If the Node is child 0
    if (p == p->parent->children[0])
    {
      // In case 1 is NULL, move to 2, and if 2 is NULL, move to 3
      if (p->parent->children[1])
        return p->parent->children[1];
      else if (p->parent->children[2])
        return p->parent->children[2];
      else if (p->parent->children[3])
        return p->parent->children[3];
    }
    // If the Node is child 1
    else if (p == p->parent->children[1])
    {
      if (p->parent->children[2])
        return p->parent->children[2];
      else if (p->parent->children[3])
        return p->parent->children[3];
    }
    // If the Node is child 2
    else if (p == p->parent->children[2])
    {
      if (p->parent->children[3])
        return p->parent->children[3];
    }
    else
      return NULL;
  }

  // Function checks to see if a call to operator++ should return the end
  // iterator, which is NULL
  bool isEnd(Node<number_type,label_type>* p, Node<number_type, label_type>* t)
  {return (!p->parent && isLast(t));}

  // increment & decrement operators

  // pre-incrememnt... ++itr
  DepthIterator<number_type, label_type> & operator++()
  {
    // Go to the first child
    bool check = AdvanceLevel(ptr_);
    
    // If all the children are null (at a leaf node)...
    if (!check)
    {
      Node<number_type, label_type>* temp = ptr_;

      // Loop will set temp to be the leaf node, and ptr_ will be the parent
      // as long as ptr_ and temp are the last child of the four
      while(ptr_->parent && isLast(ptr_) && isLast(temp))
      {
        temp = ptr_;
        ptr_ = ptr_->parent;
      } 

      // condition to check if we are at the end, and if performing a ++ 
      // should return NULL
      if (isEnd(ptr_,temp))
      {
        ptr_ = NULL;
        return *this;
      }

      // Advances a Node, and depending on whether it is the last child or not,
      // it will either move to the next child, or have to move to the next 
      // branch/subtree
      bool last = AdvanceNode(ptr_,temp);
      if (last) ptr_ = NextNode(ptr_);
    }

    return *this;
  } 

  // post-increment... itr++
  DepthIterator<number_type, label_type> operator++(int)
  {
    DepthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

  // accessors
  const label_type getLabel() const {return ptr_->label;}
  int getDepth() const 
  {    
    if (!ptr_) return 0;

    int c = 0;
    Node<number_type, label_type>* temp = ptr_;

    // Starting at a given node, trail up and increment a counter
    // until we reach the root node
    while (temp->parent != NULL)
    {
      c++;
      temp = temp->parent;
    }

    return c;
  }

private:
  Node<number_type, label_type>* ptr_;
};


// --------------------------------------------------------------
// BREADTH ITERATOR CLASS
template <class number_type, class label_type>
class BreadthIterator {
public:
  BreadthIterator() : ptr_(NULL), tree_(NULL) {}
  BreadthIterator(Node<number_type,label_type>* p, 
    const QuadTree<number_type, label_type>* t) : ptr_(p), tree_(t) 
  {
    vector<Node<number_type,label_type>*> current;
    vector<Node<number_type,label_type>*> next;
    // case if tree is empty
    if (p != NULL) {
      current.push_back(p);
    }

    // This while loop will run until there are no more levels
    // of the tree
    while (current.size() > 0) 
    { 
      // Loop through all elements in a certain level of the tree
      for (int i = 0; i < current.size(); i++) 
      {
        // Push back the point into the vector "bf_ordered", which
        // will be how operator++ will be implemented
        bf_ordered.push_back(current[i]); 

        // Push back any children that a specific Node might have if
        // they exist to the vector "next" 
        if (current[i]->children[0])
          next.push_back(current[i]->children[0]);
        if (current[i]->children[1])
          next.push_back(current[i]->children[1]);
        if (current[i]->children[2])
          next.push_back(current[i]->children[2]);
        if (current[i]->children[3])
          next.push_back(current[i]->children[3]);
      }
      // Set "current" to "next" so that in the next iteration,
      // we will be able to work with the next level down
      current = next;
      next.clear();
    }
  }

  // dereference operator (*)
  const Point<number_type>& operator*() const {return ptr_->pt;}

  // comparison operators
  bool operator== (const BreadthIterator& rgt) {return (ptr_ == rgt.ptr_ && tree_ == rgt.tree_);}
  bool operator!= (const BreadthIterator& rgt) {return (ptr_ != rgt.ptr_ || tree_ != rgt.tree_);}

 // increment & decrement operators

  // pre-incrememnt... ++itr
  BreadthIterator<number_type, label_type> & operator++()
  {
    // Loop through all contents of bf_ordered
    for (int i = 0; i < bf_ordered.size(); i++)
    {
      // If the node we are incrementing appears, then either
      // return the next pointer in the vector or, if is the
      // last element in the vector, set the pointer to be NULL
      if (ptr_ == bf_ordered[i])
      {
        if (i == bf_ordered.size() - 1)
        {
          ptr_ = NULL;
          return *this;
        }
        else
        {
          ptr_ = bf_ordered[i+1];
          return *this;
        }
      }
    }
  }

  // post-increment... itr++
  BreadthIterator<number_type, label_type> operator++(int)
  {
    BreadthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

  // accessors
  const label_type getLabel() const {return ptr_->label;}

  int getDepth() const 
  {    
    if (!ptr_) return 0;

    int c = 0;
    Node<number_type, label_type>* temp = ptr_;

    // Go up to the root from the current Node, incrementing counter
    // "c" everytime we do, so that it will then be equal to the
    // depth
    while (temp->parent != NULL)
    {
      c++;
      temp = temp->parent;
    }

    return c;
  }

private:
  Node<number_type, label_type>* ptr_;
  const QuadTree<number_type, label_type>* tree_;
  vector<Node<number_type, label_type>* > bf_ordered;
};

// --------------------------------------------------------------
// QUAD TREE CLASS
template <class number_type, class label_type>
class QuadTree {
public:
  // CONSTRUCTORS (DEFAULT, COPY)
  QuadTree() : root_(NULL), size_(0) {}

  QuadTree(const QuadTree<number_type,label_type>& old) : size_(old.size_)
  {
    root_ = this->copy_tree(old.root_, NULL);
  }

  // ASSIGNMENT OPERATOR, DESTRUCTOR
  QuadTree& operator=(const QuadTree<number_type, label_type>& old)
  {
    if (&old != this)
    {
      this->destroy_tree(root_);
      root_ = this->copy_tree(old.root_, NULL);
      size_ = old.size_;
    }
    return *this;
  }

  ~QuadTree() 
  {
    this->destroy_tree(root_);
    root_ = NULL;
  }

  // ITERATOR TYPEDEFS
  typedef DepthIterator<number_type, label_type> iterator;
  friend class DepthIterator<number_type, label_type>;

  typedef BreadthIterator<number_type, label_type> bf_iterator;
  friend class BreadthIterator<number_type, label_type>;

  // ACCESSORS
  unsigned int size() const {return size_;}
  bool operator==(const QuadTree<number_type,label_type>& old) const
  {
    return (old.root_ == this->root_);
  }
  bool operator!=(const QuadTree<number_type,label_type>& old) const
  {
    return (old.root_ != this->root_);
  }

  // FIND, INSERT, & ERASE
  iterator find(const number_type& x, const number_type& y)
  {
    return find(x, y, root_);
  }

  pair<iterator, bool> insert(const Point<number_type> &point,
                              const label_type &label)
  {
    return insert(point, label, root_, NULL);
  }

  // ACCESSOR
  int height()
  {
    int h = 0;
    return height(root_);
  }

  // BEGIN AND END FOR BOTH ITERATORS
  iterator begin() const
  {
    if (!root_) return iterator(NULL);
    Node<number_type,label_type>* temp = root_;

    return iterator(temp);
  }

  iterator end() const
  {
    return iterator(NULL);
  }

  bf_iterator bf_begin() const
  {
    if (!root_) return bf_iterator(NULL,this);
    Node<number_type, label_type>* temp = root_;

    return bf_iterator(temp,this);
  }

  bf_iterator bf_end() const
  {
    return bf_iterator(NULL, this);
  }

  // OUTPUT AND PRINTING 
  void plot(int max_x, int max_y, bool draw_lines=true) const {
    // allocate blank space for the center of the board
    std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
    // edit the board to add the point labels and draw vertical and
    // horizontal subdivisions
    plot(root_,board,0,max_x,0,max_y,draw_lines);
    // print the top border of the plot
    std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
    for (int i = 0; i <= max_y; i++) {
      // print each row of the board between vertical border bars
      std::cout << "|" << board[i] << "|" << std::endl;
    }
    // print the top border of the plot
    std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
  }


  // prints all of the tree data with a pre-order (node first, then
  // children) traversal of the tree structure
  // driver function
  void print_sideways() const { print_sideways(root_,""); }


private:
  // VARIABLES
  Node<number_type, label_type>* root_;
  unsigned int size_;

  // PRIVATE HELPER FUNCTIONS

  // Called in the copy and assignment constructor
  Node<number_type, label_type>* copy_tree(Node<number_type, label_type>*
    old_root, Node<number_type, label_type>* the_parent)
  {
    // Case for an empty tree 
    if (old_root == NULL)
    {
      return NULL;
    }

    // Dynamically create a new Node to be the root of the copied tree
    Node<number_type,label_type>* temp = new Node<number_type,label_type>(
      old_root->pt, old_root->label);

    // Recursively copy the children nodes, passing the current Node
    // as the parent to be able to assign a parent to each node
    temp->children[0] = copy_tree(old_root->children[0],temp);
    temp->children[1] = copy_tree(old_root->children[1],temp);
    temp->children[2] = copy_tree(old_root->children[2],temp);
    temp->children[3] = copy_tree(old_root->children[3],temp);

    // Sets the parent of each node
    temp->parent = the_parent;

    return temp;
  }

  // Called in the assignment operator and destructor
  void destroy_tree(Node<number_type, label_type> *p)
  {
    // base case
    if (!p)
    {
      return;
    }
    else
    {
      // recurse through all four children until the bottom of each
      // branch is reached, then start traversing back through the
      // call stack and delete each node
      destroy_tree(p->children[0]);
      destroy_tree(p->children[1]);
      destroy_tree(p->children[2]);
      destroy_tree(p->children[3]);
      // After we've worked our way down, we traverse back, deleting each
      // node on the way
      delete p;
    }
  }

  // Actual recursive function to return an iterator pointing to a specific
  // node
  iterator find(const number_type& x, const number_type& y, Node<number_type,
    label_type>* p)
  {
    // Base case
    if (p == NULL) return end();

    // Check to see which child we should continue recursing down, depending on
    // where the point we are looking at is relative to its child
    if (x < (p->pt).x && y < (p->pt).y)
    {
      return find(x, y, p->children[0]);
    }
    else if (x > (p->pt).x && y < (p->pt).y)
    {
      return find(x, y, p->children[1]);
    }
    else if (x < (p->pt).x && y > (p->pt).y)
    {
      return find(x, y, p->children[2]);
    }
    else if (x > (p->pt).x && y > (p->pt).y)
    {
      return find(x, y, p->children[3]);
    }
    // Once we've found it and the checks return false, return an iterator
    // pointing to that node
    else
    {
      return iterator(p);
    }
  }

  pair<iterator, bool> insert(const Point<number_type> &point, 
    const label_type &label, Node<number_type, label_type>*& p,
    Node<number_type, label_type>* the_parent)
  {
    // base case
    if (p == NULL)
    {
      p = new Node<number_type, label_type>(point, label);
      p->parent = the_parent;
      this->size_++;
      return pair<iterator, bool>(iterator(p), true);
    }

    // check to see which quadrant the point belongs in, and continually recurse
    // until p == NULL and the base case is executed
    else if (point.x < (p->pt).x && point.y < (p->pt).y)
    {
      return insert(point, label, p->children[0], p);
    }
    else if (point.x > (p->pt).x && point.y < (p->pt).y)
    {
      return insert(point, label, p->children[1], p);
    }
    else if (point.x < (p->pt).x && point.y > (p->pt).y)
    {
      return insert(point, label, p->children[2], p);
    }
    else if (point.x > (p->pt).x && point.y > (p->pt).y)
    {
      return insert(point, label, p->children[3], p);
    }
    else
    {
      return pair<iterator, bool>(iterator(p), false);
    }
  }

  int height(Node<number_type,label_type>*& p) const
  {
    if (!p) return -1;

    int a = height(p->children[0]);
    int b = height(p->children[1]);
    int c = height(p->children[2]);
    int d = height(p->children[3]);

    if (a > b && a > c && c > d) return a+1;
    if (b > a && b > c && b > d) return b+1;
    if (c > a && c > b && c > d) return c+1;
    if (d > a && d > b && d > c) return d+1;

    return 1 + max(max(max(a,b),c),d);
  }

  // NOTE: this function only works for quad trees with non negative
  // integer coordinates and char labels
  // NOTE2: this function assumes that no two points have the same x
  // coordinate or the same y coordinate.
  // plot driver function
  // takes in the maximum x and y coordinates for these data points
  // the optional argument draw_lines defaults to true
  // actual recursive function for plotting
  void plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
            int x_min, int x_max, int y_min, int y_max, bool draw_lines) const 
  {
    // base case, draw nothing if this node is NULL
    if (p == NULL) return;
    // check that the dimensions range of this node make sense
    assert (x_min >= 0 && x_min <= x_max);
    assert (y_min >= 0 && y_min <= y_max);
    assert (board.size() >= y_max);
    assert (board[0].size() >= x_max);
    // verify that the point stored at this node fits on the board
    assert (p->pt.y >= 0 && p->pt.y < board.size());
    assert (p->pt.x >= 0 && p->pt.x < board[0].size());
    // draw the vertical and horizontal bars extending across the
    // range of this node
    if (draw_lines) {
      for (int x = x_min; x <= x_max; x++) {
        board[p->pt.y][x] = '-';
      }
      for (int y = y_min; y <= y_max; y++) {
        board[y][p->pt.x] = '|';
      }
    }
    // draw this label
    board[p->pt.y][p->pt.x] = p->label;
    // recurse on the 4 children
    plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
    plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
    plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
    plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
  }

  // actual recursive function
  void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const {
    // base case
    if (p == NULL) return;
    // print out this node
    std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
    // recurse on each of the children trees
    // increasing the indentation
    print_sideways(p->children[0],indent+"  ");
    print_sideways(p->children[1],indent+"  ");
    print_sideways(p->children[2],indent+"  ");
    print_sideways(p->children[3],indent+"  ");
  }
};


#endif
#ifndef NODE_HPP
#define NODE_HPP

//  `Node`
/*
  Containing all necessary structs, classes and functions for the implementation of a doubly linked class.
*/

// # Header Declarations

/*
  ### `template <class T> class Node{...};`
  *Accessible Template Class*

  Template Class Node contains functions and members implementing a doubly linked class.
*/
template<class T>
class Node
{
public:
  Node<T> *prev,*next;
  T data;
  Node() {
    prev = nullptr;
    next = nullptr;
    data = T();
  }
  Node<T>* NewAfter();
  Node<T>* NewBefore();
  void InsertAfter(Node<T>*);
  void InsertBefore(Node<T>*);
  Node<T>* Delete();    
};

// # Public Class Functions

/*
  ### `template <class T> Node<T>* Node<T>::NewAfter();`
  *Public Class Function*

  Returns a pointer to the Node<T> which is `next` of the current `link`. Takes in a nothing.
  Basically this function initializes the `next` member in the chain and returns a pointer to it.
*/
template<class T>
Node<T>* Node<T>::NewAfter() {
  Node<T>* temp = new Node<T>;
  InsertAfter(temp);
  return temp;
}

/*
  ### `template <class T> Node<T>* Node<T>::NewBefore();`
  *Public Class Function*

  Returns a pointer to the Node<T> which is `prev` of the current `link`. Takes in a nothing.
  Similar to NewAfter, this function initializes the `prev` member in the chain and returns a pointer to it.
*/
template<class T>
Node<T>* Node<T>::NewBefore() {
  Node<T>* temp = new Node<T>;
  InsertBefore(temp);
  return temp;
}

/*
  ### `template <class T> void Node<T>::InsertAfter(Node<T>*);`
  *Public Class Function*

  Returns nothing. Takes in a pointer to the Node<T>. 
  This inserts a `link` (passed as a parameter), at `next`.
  ```
  prev <-> current <-> next                            
  ^ <------ Inserts the link here and becomes
  prev <-> current <-> link <-> next
  ```
*/
template<class T>
void Node<T>::InsertAfter(Node<T>* n) {
  if (next != nullptr) {
    n->next = next;
    next->prev = n;
  }
  next = n;
  n->prev = this;
}

/*
  ### `template <class T> void Node<T>::InsertBefore(Node<T>*);`
  *Public Class Function*

  Returns nothing. Takes in a pointer to the Node<T>. 
  This inserts a `link` (passed as a parameter), at `prev`.
  ```
  prev <-> current <-> next                            
  ^ <------ Inserts the link here and becomes
  prev <-> link <-> current <-> next
  ```
*/
template<class T>
void Node<T>::InsertBefore(Node<T>* n) {
  if (prev != nullptr) {
    n->prev = prev;
    prev->next = n;
    prev = n;
  }
  prev = n;
  n->next = this;
}

/*
  ### `template <class T> Node<T>* Node<T>::Delete();`
  *Public Class Function*

  Returns the pointer to the `prev` link of this node. Takes in nothing.
  Deletes the current `link` and joins the `next` and `prev` links.
  ```
  prev <-> current <-> next                            
  ^ <------ Inserts the link here and becomes
  prev <-> link <-> current <-> next
  ```
*/
template<class T>
Node<T>* Node<T>::Delete() {
  if (prev != nullptr) {
    prev->next = next;
  }
  if (next != nullptr) {
    next->prev = prev;
  }
  return prev;
}

#endif

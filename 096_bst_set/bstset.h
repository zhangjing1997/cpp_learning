#include <stdlib.h>

#include <utility>

#include "set.h"

template<typename T>
class BstSet : public Set<T>
{
 protected:
  class Node
  {
   public:
    T key;
    Node * left;
    Node * right;
    Node(const T & item) : key(item), left(NULL), right(NULL) {}
  };

  Node * root;

 public:
  //default constructor
  BstSet() : root(NULL){};

  //copy constructor
  void copy(Node ** n, const Node * m) {
    if (m != NULL) {
      *n = new Node(m->key);
      copy(&(*n)->left, m->left);
      copy(&(*n)->right, m->right);
    }
  }
  BstSet(const BstSet & rhs) : root(NULL) { copy(&root, rhs.root); }

  //assignment constructor
  const BstSet & operator=(const BstSet & rhs) {
    if (this != &rhs) {
      BstSet temp(rhs);
      Node * t = root;
      root = temp.root;
      temp.root = t;
    }
    return *this;
  }

  //destructor
  void destroy(Node * curr) {
    if (curr != NULL) {
      destroy(curr->left);
      destroy(curr->right);
      delete curr;
    }
  }
  virtual ~BstSet() { destroy(root); }

  //lookup address according to data
  virtual Node ** lookupAddress(const T & key) const {
    Node ** curr = const_cast<Node **>(&root);
    while (*curr != NULL) {
      if (key < (*curr)->key) {
        curr = &(*curr)->left;
      }
      else if (key > (*curr)->key) {
        curr = &(*curr)->right;
      }
      else {
        break;
      }
    }
    return curr;
  }

  //add - keep pointer to pointer to Node
  virtual void add(const T & key) {
    Node ** curr = lookupAddress(key);
    if (*curr == NULL) {
      *curr = new Node(key);
    }
  }

  //lookup
  virtual bool contains(const T & key) const {
    Node ** curr = lookupAddress(key);
    if (*curr != NULL) {
      return true;
    }
    else {
      return false;
    }
  }

  //remove
  virtual void remove(const T & key) {
    Node ** curr = lookupAddress(key);
    Node * temp;
    if ((*curr)->left == NULL) {
      temp = (*curr)->right;
      delete *curr;
      *curr = temp;
    }
    else if ((*curr)->right == NULL) {
      temp = (*curr)->left;
      delete *curr;
      *curr = temp;
    }
    else {
      Node ** similar = &(*curr)->left;
      while ((*similar)->right != NULL) {
        similar = &(*similar)->right;
      }
      (*curr)->key = (*similar)->key;
      temp = (*similar)->left;
      delete *similar;
      *similar = temp;
    }
  }
};

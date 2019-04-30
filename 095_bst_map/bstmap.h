#include <exception>
#include <sstream>

#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V>
{
 protected:
  class Node
  {
   public:
    K key;
    V value;
    Node * left;
    Node * right;
    //Node() : key(), value(), left(NULL), right(NULL) {}
    Node(const K & itemKey, const V & itemValue) :
        key(itemKey),
        value(itemValue),
        left(NULL),
        right(NULL) {}
  };
  Node * root;

 public:
  //default constructor
  BstMap() : root(NULL) {}
  Node * getroot() { return root; }

  //copy constructor
  void copy(Node ** n, const Node * m) {
    if (m != NULL) {
      *n = new Node(m->key, m->value);
      copy(&(*n)->left, m->left);
      copy(&(*n)->right, m->right);
    }
  }
  BstMap(const BstMap & rhs) : root(NULL) { copy(&root, rhs.root); }

  //assignment constructor
  const BstMap & operator=(const BstMap & rhs) {
    if (this != &rhs) {
      BstMap temp(rhs);
      Node * t = temp.root;
      temp.root = root;
      root = t;
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
  virtual ~BstMap() { destroy(root); }

  //add - keep pointer to pointer to Node
  virtual void add(const K & keyToAdd, const V & valueToAdd) {
    Node ** curr = lookupAddress(keyToAdd);
    if (*curr == NULL) {
      *curr = new Node(keyToAdd, valueToAdd);
    }
    else {
      (*curr)->value = valueToAdd;
    }
  }

  //lookup
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node ** curr = lookupAddress(key);
    if (*curr != NULL) {
      return (*curr)->value;
    }
    else {
      std::ostringstream outinfo;
      outinfo << "cannot find the key: " << key;
      throw std::invalid_argument(outinfo.str());
    }
  }

  //lookup address according to key
  virtual Node ** lookupAddress(const K & key) const {
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

  //remove
  virtual void remove(const K & keyToRemove) {
    Node ** curr = lookupAddress(keyToRemove);
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
      (*curr)->value = (*similar)->value;
      temp = (*similar)->left;
      delete *similar;
      *similar = temp;
    }
  }
};

#include "node.h"

Node * copyNode(Node * n) {
  Node * m = new Node(n->sym, n->freq);
  if (n->left != NULL) {
    m->left = copyNode(n->left);
  }
  if (n->right != NULL) {
    m->right = copyNode(n->right);
  }
  return m;
}

Node * buildTree(uint64_t * counts) {
  //WRITE ME!
  priority_queue_t pq;
  for (int i = 0; i < 257; i++) {
    if (counts[i] > 0) {
      pq.push(new Node(i, counts[i]));
    }
  }
  while (pq.size() > 1) {
    Node * node1 = pq.top();
    pq.pop();
    Node * node2 = pq.top();
    pq.pop();
    Node * newnode = new Node(node1, node2);
    pq.push(newnode);
  }
  return pq.top();
}

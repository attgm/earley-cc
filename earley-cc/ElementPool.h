//  ElementPool.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef ELEMENT_POOL_H_
#define ELEMENT_POOL_H_

#include <list>

class Quadruplet;

struct Element
{
  double prob;
  Quadruplet *ptr;
  Element *bp1, *bp2;
};

inline int operator<(const Element &a, const Element &b)
{
  return a.prob < b.prob;
};

inline int operator>(const Element &a, const Element &b)
{
  return a.prob > b.prob;
};

inline int operator==(const Element &a, const Element &b)
{
  return a.prob == b.prob;
};

inline int operator!=(const Element &a, const Element &b)
{
  return a.prob != b.prob;
};

inline int operator<=(const Element &a, const Element &b)
{
  return a.prob <= b.prob;
};

inline int operator>=(const Element &a, const Element &b)
{
  return a.prob >= b.prob;
};

class ElementPool
{
public:
  ElementPool(void);
  ~ElementPool();

  Element *create_new_element(Element *a, Element *b, double p);
  Element *create_new_element(Element *a = NULL, Element *b = NULL);
  Element *create_new_element(double inProb) { return create_new_element(NULL, NULL, inProb); };

  void clear(void);

protected:
  void insert(Element *inElement) { element_list_.push_back(inElement); };

private:
  static ElementPool *element_pool_;
  std::list<Element *> element_list_;
};

inline Element *
ElementPool::create_new_element(Element *a, Element *b)
{
  return create_new_element(a, b, (a != NULL) ? ((b != NULL) ? (a->prob * b->prob) : a->prob) : 0);
};

#endif //ELEMENT_POOL_H_

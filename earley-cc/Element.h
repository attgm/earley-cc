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

struct Element {
  double prob_;
  Quadruplet *quadruplet_;
  Element *back_ptr_1, *back_ptr_2;
};

inline int operator<(const Element &a, const Element &b) {
  return a.prob_ < b.prob_;
};

inline int operator>(const Element &a, const Element &b) {
  return a.prob_ > b.prob_;
};

inline int operator==(const Element &a, const Element &b) {
  return a.prob_ == b.prob_;
};

inline int operator!=(const Element &a, const Element &b) {
  return a.prob_ != b.prob_;
};

inline int operator<=(const Element &a, const Element &b) {
  return a.prob_ <= b.prob_;
};

inline int operator>=(const Element &a, const Element &b) {
  return a.prob_ >= b.prob_;
};

class ElementPool {
public:
  ElementPool(void);
  ~ElementPool();

  Element *create_new_element(Element *a, Element *b, double p);
  Element *create_new_element(Element *a = nullptr, Element *b = nullptr);
  Element *create_new_element(double inProb) {
    return create_new_element(nullptr, nullptr, inProb);
  };

  void clear(void);

protected:
  void insert(Element *element) { element_list_.push_back(element); };

private:
  static ElementPool *element_pool_;
  std::list<Element *> element_list_;
};

inline Element *ElementPool::create_new_element(Element *a, Element *b) {
  return create_new_element(
      a, b, (a != NULL) ? ((b != NULL) ? (a->prob_ * b->prob_) : a->prob_) : 0);
};

#endif // ELEMENT_POOL_H_

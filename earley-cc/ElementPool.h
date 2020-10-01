// ElementPool.h
//  2000 Atsushi Tagami
// $Id: ElementPool.h,v 1.5 2000/01/31 23:43:10 atsushi Exp $
#ifndef __ELEMENT_POOL_H__
#define __ELEMENT_POOL_H__
#include <list>

class  Quadruplet;

struct Element {
  double        dProb;
  Quadruplet    *ptr;
  Element       *bp1, *bp2;
};

inline int operator<(const Element& a, const Element& b) {
  return a.dProb < b.dProb;
};

inline int operator>(const Element& a, const Element& b) {
  return a.dProb > b.dProb;
};

inline int operator==(const Element& a, const Element& b) {
  return a.dProb == b.dProb;
};

inline int operator!=(const Element& a, const Element& b) {
  return a.dProb != b.dProb;
};

inline int operator<=(const Element& a, const Element& b) {
  return a.dProb <= b.dProb;
};

inline int operator>=(const Element& a, const Element& b) {
  return a.dProb >= b.dProb;
};


class ElementPool {
 public:
  ElementPool(void);
  ~ElementPool();
  
  Element* New(Element* a, Element* b, double p);
  Element* New(Element* a = NULL, Element* b = NULL);
  Element* New(double inProb){ return New(NULL, NULL, inProb); };

  void Clear(void);
  
 protected:
  void Insert(Element* inElement){ mElementList.push_back(inElement); };
  
 private:
	static ElementPool* sElementPool;
	std::list<Element*> mElementList;
};


inline Element*
ElementPool::New(Element* a, Element* b){
  return New(a, b, (a != NULL) ? 
	     ((b != NULL) ? (a->dProb * b->dProb) : a->dProb) : 0);
};

#endif //__ELEMENT_POOL_H__

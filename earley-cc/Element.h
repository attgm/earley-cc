// Elements.h
//   1999 Atsushi Tagami
// $Id: Element.h,v 1.1.1.1 2000/01/05 23:10:57 atsushi Exp $
#ifndef __ELEMENTS_H__
#define __ELEMENTS_H__


class Element
{
 protected:
  Element(Element* bp1 = 0, Element* bp2 = 0)
    : backpointer1(bp1), backpointer2(bp2){};
  
 public:
  Insert(Element*) = 0;
  Multiply(list<double> &inList) = 0;

 private:
  Element *backpointer1, *backpointer2;
};


 
#endif //__ELEMENTS_H__

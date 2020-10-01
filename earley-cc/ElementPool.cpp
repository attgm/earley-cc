//  ElementPool.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "ElementPool.h"

//----------- ElementPool
// コンストラクタ
ElementPool::ElementPool(void)
{
}

//----------- ~ElementPool
// ディストラクタ
ElementPool::~ElementPool(void)
{
  clear();
}

//---------- create_new_element
//  新しいエレメントを作成する
Element *
ElementPool::create_new_element(Element *a, Element *b, double p)
{
  Element *element = new Element;

  element->prob = p;
  element->bp1 = a;
  element->bp2 = b;

  insert(element);

  return element;
};

//----------- Clear
// すべての要素を削除する
void ElementPool::clear(void)
{
  for (std::list<Element *>::iterator it = element_list_.begin();
       it != element_list_.end(); it++)
  {
    delete *it;
  }

  element_list_.clear();
}

// ElementPool.h
//  2000 Atsushi Tagami
// $Id: ElementPool.cc,v 1.4 2000/01/31 23:43:10 atsushi Exp $
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
  Clear();
}


//---------- New
//  新しいエレメントを作成する
Element*
ElementPool::New(Element* a, Element* b, double p) {
  Element* element = new Element;
  
  element->dProb = p;
  element->bp1 = a;
  element->bp2 = b;
    
  Insert(element);
  
  return element;
};


//----------- Clear
// すべての要素を削除する
void
ElementPool::Clear(void)
{
	for(std::list<Element*>::iterator it = mElementList.begin(); 
	it != mElementList.end(); it++){
		delete *it;
	}	
	
	mElementList.clear();
}

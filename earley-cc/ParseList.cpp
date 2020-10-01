// ParseList.cc
//  1999 Atsushi Tagami
// $Id: ParseList.cc,v 1.4 2000/01/31 23:43:10 atsushi Exp $
#include <iostream>
#include "ParseList.h"

#pragma mark -- TermTable --
// TermTable::TermTable
//  コンストラクタ
TermTable::TermTable(void)
{
}


// TermTable::~TermTable
//  ディストラクタ
TermTable::~TermTable()
{
  for(QuadSetMap::iterator it = mQuadSetMap.begin(); 
      it != mQuadSetMap.end(); it++){
    for(QuadSet::iterator si = it->second->begin(); 
	si != it->second->end(); si++){
      delete *si;
    }
    delete it->second;
  }
}


// TermTable::Insert
//   TermTableの所定の位置に要素を挿入する
void
TermTable::Insert(int inTermNo, Quadruplet* inQuad)
{
  // ドットの次の番号がTermNoのQuadSetを得る
  QuadSetMap::iterator qm_it = mQuadSetMap.find(inTermNo);
  QuadSet* unit;
  if(qm_it == mQuadSetMap.end()){
    //  もし存在しないなら新しいParseUnitを作成する
    unit = new QuadSet;
    mQuadSetMap.insert(make_pair(inTermNo, unit));
  }else{
    unit = qm_it->second;
  }
  
  // ルール番号とドットの位置が等しい要素を検索する
  QuadSet::iterator qs_it = unit->find(inQuad);
  if(qs_it == unit->end()){
    unit->insert(inQuad);
  }else{
    // もし既にParseElementが存在するなら,
    // 確率値のlistをMargeする
    (*qs_it)->Marge(inQuad);
    delete inQuad;
  }
};



// TermTable::Find
//  ドットの次のTermがtermnoの要素を検索する.
//  発見できなかった場合は nullを返す
QuadSet*
TermTable::Find(int inTermNo)
{
  QuadSetMap::iterator it = mQuadSetMap.find(inTermNo);
  return (it == mQuadSetMap.end()) ? (QuadSet*)NULL : it->second;
}


#pragma mark -- ParseList --
// ParseList
//  コンストラクタ
ParseList::ParseList(int n)
  : mSize(n)
{
  InitParseList(n);
}


// ~ParseList
//   ディストラクタ
ParseList::~ParseList(void)
{
  Clear();
}


// InitParseList 
//    大きさnのParseListを作成し，初期化する
void
ParseList::InitParseList(int n)
{
  mTermTable = new TermTable*[mSize*mSize];
  
  // 配列の左下三角形部にTermTableを割り当てる
  for(int i=1; i<mSize; i++){
    for(int j=0; j<i; j++){
      TTable(j,i) = new TermTable;
    }
  }
  
  // 対角線成分には同じものが入る
  TTable(0,0) = new TermTable;
  for(int i=0; i<mSize; i++){
    TTable(i,i) = TTable(0,0);
  }
};



// Clear
//   ParseListのメモリ空間を解放する
void
ParseList::Clear(void)
{
  for(int i=0; i<mSize; i++){
    for(int j=0; j<i; j++){
      delete TTable(j,i);
    }
  }
  delete TTable(0,0);
  delete[] mTermTable;
};



// Insert
//   ParseListの(x,y)に, ドットの次のTermがtermno
//  であるParseElement peを挿入する
void 
ParseList::Insert(int x, int y, int inTermNo, Quadruplet* inQuad)
{
  if(x > y){
    std::cerr << "Error:Insert ParseElement in PL(" << x << ',' << y << ')' << std::endl;
    return;
  }
  // (x,y)のTermTableを得る
  TermTable* termtable = TTable(x,y);
  // ドットの次の番号がTermNoの位置に挿入する
  termtable->Insert(inTermNo, inQuad);
}


// ParseList::Find
//   ParseListに(x,y)ドットの次のTermがtermnoの要素をParseListから
//   探索する  
QuadSet* 
ParseList::Find(int x, int y, int inTermNo)
{
  // (x,y)のTermTableを得る
  TermTable* termtable = TTable(x,y);
  // TermTableを検索する.
  return termtable->Find(inTermNo);
};

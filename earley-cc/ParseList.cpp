//  ParseList.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "ParseList.h"

#include <iostream>

#pragma mark-- TermTable --
// TermTable::TermTable
//  コンストラクタ
TermTable::TermTable(void) {}

// TermTable::~TermTable
//  ディストラクタ
TermTable::~TermTable() {
  for (QuadSetMap::iterator it = quad_set_map_.begin();
       it != quad_set_map_.end(); it++) {
    for (QuadSet::iterator si = it->second->begin(); si != it->second->end();
         si++) {
      delete *si;
    }
    delete it->second;
  }
}

// TermTable::Insert
//   TermTableの所定の位置に要素を挿入する
void TermTable::insert(int inTermNo, Quadruplet *inQuad) {
  // ドットの次の番号がTermNoのQuadSetを得る
  QuadSetMap::iterator qm_it = quad_set_map_.find(inTermNo);
  QuadSet *unit;
  if (qm_it == quad_set_map_.end()) {
    //  もし存在しないなら新しいParseUnitを作成する
    unit = new QuadSet;
    quad_set_map_.insert(make_pair(inTermNo, unit));
  } else {
    unit = qm_it->second;
  }

  // ルール番号とドットの位置が等しい要素を検索する
  QuadSet::iterator qs_it = unit->find(inQuad);
  if (qs_it == unit->end()) {
    unit->insert(inQuad);
  } else {
    // もし既にParseElementが存在するなら,
    // 確率値のlistをMargeする
    (*qs_it)->marge(inQuad);
    delete inQuad;
  }
};

// TermTable::Find
//  ドットの次のTermがtermnoの要素を検索する.
//  発見できなかった場合は nullを返す
QuadSet *TermTable::find(int inTermNo) {
  QuadSetMap::iterator it = quad_set_map_.find(inTermNo);
  return (it == quad_set_map_.end()) ? (QuadSet *)NULL : it->second;
}

#pragma mark-- ParseList --
// ParseList
//  コンストラクタ
ParseList::ParseList(int n) : size_(n) { init_parse_list(n); }

// ~ParseList
//   ディストラクタ
ParseList::~ParseList(void) { clear(); }

// InitParseList
//    大きさnのParseListを作成し，初期化する
void ParseList::init_parse_list(int n) {
  term_table_ = new TermTable *[size_ * size_];

  // 配列の左下三角形部にTermTableを割り当てる
  for (int i = 1; i < size_; i++) {
    for (int j = 0; j < i; j++) {
      term_table(j, i) = new TermTable;
    }
  }

  // 対角線成分には同じものが入る
  term_table(0, 0) = new TermTable;
  for (int i = 0; i < size_; i++) {
    term_table(i, i) = term_table(0, 0);
  }
};

// Clear
//   ParseListのメモリ空間を解放する
void ParseList::clear(void) {
  for (int i = 0; i < size_; i++) {
    for (int j = 0; j < i; j++) {
      delete term_table(j, i);
    }
  }
  delete term_table(0, 0);
  delete[] term_table_;
};

// Insert
//   ParseListの(x,y)に, ドットの次のTermがtermno
//  であるParseElement peを挿入する
void ParseList::insert(int x, int y, int inTermNo, Quadruplet *inQuad) {
  if (x > y) {
    std::cerr << "Error:Insert ParseElement in PL(" << x << ',' << y << ')'
              << std::endl;
    return;
  }
  // (x,y)のTermTableを得る
  TermTable *termtable = term_table(x, y);
  // ドットの次の番号がTermNoの位置に挿入する
  termtable->insert(inTermNo, inQuad);
}

// ParseList::Find
//   ParseListに(x,y)ドットの次のTermがtermnoの要素をParseListから
//   探索する
QuadSet *ParseList::find(int x, int y, int inTermNo) {
  // (x,y)のTermTableを得る
  TermTable *termtable = term_table(x, y);
  // TermTableを検索する.
  return termtable->find(inTermNo);
};

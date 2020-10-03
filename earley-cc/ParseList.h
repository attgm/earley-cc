//  ParseList.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_LIST_H_
#define PARSE_LIST_H_

#include <map>
#include <set>

#include "Quadruplet.h"

struct LessQuadruplet : std::binary_function<Quadruplet *, Quadruplet *, bool> {
  bool operator()(const Quadruplet *a, const Quadruplet *b) const {
    return *a < *b;
  };
};

typedef std::set<Quadruplet *, LessQuadruplet> QuadSet;
typedef std::map<int, QuadSet *> QuadSetMap;

class TermTable {
public:
  TermTable();
  ~TermTable();

  void insert(int inTermNo, Quadruplet *inQuad);
  QuadSet *find(int inTermNo);

private:
  QuadSetMap quad_set_map_;
};

class ParseList {
public:
  ParseList(int n);
  ~ParseList(void);

  void insert(int x, int y, int inTermNo, Quadruplet *pe);
  QuadSet *find(int x, int y, int inTermNo);

protected:
  void init_parse_list(int n);
  void clear(void);
  TermTable *&term_table(int i, int j) { return term_table_[i + j * size_]; };

private:
  int size_;
  TermTable **term_table_;
};

#endif // PARSE_LIST_H_

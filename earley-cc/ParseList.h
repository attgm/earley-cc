// ParseList.h
//  1999 Atsushi Tagami
// $Id: ParseList.h,v 1.6 2000/03/07 11:49:09 atsushi Exp $
#ifndef __PARSE_LIST_H__
#define __PARSE_LIST_H__
#include <set>
#include <map>
#include "Registration.h"

struct LessQuadruplet : std::binary_function<Quadruplet*, Quadruplet*, bool> {
  bool operator() (const Quadruplet* a, const Quadruplet* b) const {
    return *a < *b;
  };
};
 
typedef std::set<Quadruplet*, LessQuadruplet> QuadSet;
typedef std::map<int, QuadSet*> QuadSetMap;

class TermTable
{
 public:
  TermTable();
  ~TermTable();
  
  void Insert(int inTermNo, Quadruplet* inQuad);
  QuadSet* Find(int inTermNo);
  
 private:
  QuadSetMap mQuadSetMap;
};


class ParseList
{
 public:
  ParseList(int n);
  ~ParseList(void);
  
  void Insert(int x, int y, int inTermNo, Quadruplet* pe);
  QuadSet* Find(int x, int y, int inTermNo);
  
 protected:
  void InitParseList(int n);
  void Clear(void);
  TermTable* &TTable(int i, int j) { return mTermTable[i + j*mSize]; };
  
 private:
   int mSize;
  TermTable** mTermTable;
};

#endif // __PARSE_LIST_H__

//  ParseList.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_LIST_H_
#define PARSE_LIST_H_

#include "Quadruplet.h"
#include <cassert>
#include <map>
#include <set>

template <class Q>
struct LessQuadruplet
    : std::binary_function<std::unique_ptr<Q>, std::unique_ptr<Q>, bool> {
  bool operator()(std::unique_ptr<Q> const &a,
                  std::unique_ptr<Q> const &b) const {
    return *a < *b;
  };
};

template <class Q>
using QuadSet = std::set<std::unique_ptr<Q>, LessQuadruplet<Q>>;

template <class Q> class ParseList {
public:
  ParseList(int n) : size_(n){};

  void insert(int i, int j, int term_id, std::unique_ptr<Q> quadruplet) {
    if (i == j) {
      i = 0;
      j = 0;
    }
    if (auto it = term_table_.find(std::make_tuple(i, j, term_id)); it != end(term_table_)) {
      auto unit = it->second.get();
      if (auto unit_it = unit->find(quadruplet); unit_it != end(*unit)) {
        (*unit_it)->merge(quadruplet.get());
      } else {
        unit->insert(std::move(quadruplet));
      }
    } else {
      auto new_unit = std::make_unique<QuadSet<Q>>();
      new_unit->insert(std::move(quadruplet));
      term_table_.emplace(std::make_tuple(i, j, term_id), std::move(new_unit));
    }
  };

  const QuadSet<Q> *find(int i, int j, int term_id) const {
    assert(i >= 0 && j >= 0 && i < size_ && j < size_ && i <= j);
    if (i == j) {
      return &(*(term_table_.at(std::make_tuple(0, 0, term_id))));
    } else {
      return &(*(term_table_.at(std::make_tuple(i, j, term_id))));
    }
  };

private:
  int size_;
  std::map<std::tuple<int, int, int>, std::unique_ptr<QuadSet<Q>>> term_table_;
};

#endif // PARSE_LIST_H_

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

template <class Q>
using QuadSetMap = std::map<int, std::unique_ptr<QuadSet<Q>>>;

template <class Q> class ParseList {
public:
  ParseList(int n) : size_(n) { init_parse_list(); };

  void insert(int i, int j, int term_id, std::unique_ptr<Q> quadruplet) {

    auto table = term_table(i, j);
    if (auto it = table->find(term_id); it != end(*table)) {
      auto unit = it->second.get();
      if (auto unit_it = unit->find(quadruplet); unit_it != end(*unit)) {
        (*unit_it)->marge(quadruplet.get());
      } else {
        unit->insert(std::move(quadruplet));
      }
    } else {
      auto new_unit = std::make_unique<QuadSet<Q>>();
      new_unit->insert(std::move(quadruplet));
      table->emplace(term_id, std::move(new_unit));
    }
  };

  const QuadSet<Q> *find(int i, int j, int term_id) {
    auto table = term_table(i, j);
    auto it = table->find(term_id);
    return (it != end(*table) ? it->second.get() : nullptr);
  };

protected:
  void init_parse_list() {
    for (int i = 1; i < size_; i++) {
      for (int j = 0; j < i; j++) {
        term_table_.emplace(std::make_pair(j, i),
                            std::make_shared<QuadSetMap<Q>>());
      }
    }
    auto diagonal = std::make_shared<QuadSetMap<Q>>();
    for (int i = 0; i < size_; i++) {
      term_table_.emplace(std::make_pair(i, i), diagonal);
    }
  }

  std::shared_ptr<QuadSetMap<Q>> term_table(int i, int j) {
    assert(i >= 0 && j >= 0 && i < size_ && j < size_ && i <= j);
    return term_table_.at(std::make_pair(i, j));
  };

private:
  int size_;
  std::map<std::pair<int, int>, std::shared_ptr<QuadSetMap<Q>>> term_table_;
};

#endif // PARSE_LIST_H_

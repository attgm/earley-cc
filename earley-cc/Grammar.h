//  Grammar.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::map<std::string, int> TermList;

const int TERM_UNKNOWN = -1;
const int TERM_EPSILON = 0;

typedef struct rule {
  int left;
  std::vector<int> right;
  double prob;
} Rule;

class Grammar {
 public:
  Grammar(){};

  int term_to_id(const std::string &term);
  int term_to_id_or_create(const std::string &term);
  const std::string &id_to_term(int term_id);
  std::string Grammar::id_to_rule(int rule_id);

  int term_after_dot(int rule_id, int dot_locate);

  int term_num(void) { return terms_.size(); };
  int rule_num(void) { return rules_.size(); };

  const Rule *get_rule(int rule_no) { return rules_[rule_no].get(); };
  int get_root_term_id(void) { return root_term_; };

  void load_rule(std::istream &is);

 private:
  std::vector<std::unique_ptr<Rule>> rules_;
  std::map<std::string, int> terms_;

  int root_term_;
};

#endif  // GRAMMAR_H_

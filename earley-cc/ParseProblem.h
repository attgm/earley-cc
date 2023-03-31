//  ParseProblem.h
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_PROBLEM_H_
#define PARSE_PROBLEM_H_

#include <algorithm>
#include <vector>
#include <memory>
#include "Grammar.h"
#include "Registration.h"

//-- Element
//
struct Element {
  double prob_;
  std::vector<int> rule_list_;
};

inline int operator<(const Element &a, const Element &b) {
  return a.prob_ < b.prob_;
};

inline int operator>(const Element &a, const Element &b) {
  return a.prob_ > b.prob_;
};



//-- ParseQuad
// Quadruplet for parsing problem
class ParseQuad : public Quadruplet {
public:
  ParseQuad(int rule_no, int dot_loc, int limit)
      : Quadruplet(rule_no, dot_loc), limit_(limit){};
  virtual ~ParseQuad(){};

  void add(int rule_id, double prob);
  void merge(std::unique_ptr<ParseQuad> const &quadruplet);
  void multiply(std::unique_ptr<ParseQuad> const &quadruplet);

  const std::list<Element> &get_prob_list(void) const { return probs_; };

protected:
  void limit();

private:
  std::list<Element> probs_;
  int limit_;
};


//-- ParseRegistration
class ParseRegistration : public Registration<ParseQuad> {
public:
  enum { mode_Number, mode_MinNumber };

  ParseRegistration(std::shared_ptr<Grammar> grammar);
  virtual ~ParseRegistration();

  void set_limit(int inLimit);

  void set_mode(int new_mode) { mode_ = new_mode; };

  void print_result_at(std::unique_ptr<ParseQuad> const &results, int index);
  void print_all_results(std::unique_ptr<ParseQuad> const &results);

protected:
  std::unique_ptr<ParseQuad> create_quad(int rule_no, int dot_loc) {
    return std::make_unique<ParseQuad>(rule_no, dot_loc, limit_);
  }
  void init_registration(void);

private:
  std::unique_ptr<ParseQuad> results_;

  int limit_;
  int mode_;
};

#endif // PARSE_PROBLEM_H_
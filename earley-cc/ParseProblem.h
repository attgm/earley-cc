//  ParseProblem.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_PROGLEM_H_
#define PARSE_PROGLEM_H_

#include <algorithm>
#include "Grammar.h"
#include "Registration.h"

//-- Element
//
struct Element {
  double prob_;
  std::list<int> rule_id_;
};

inline int operator<(const Element &a, const Element &b) {
  return a.prob_ < b.prob_;
};

inline int operator>(const Element &a, const Element &b) {
  return a.prob_ > b.prob_;
};

inline int operator==(const Element &a, const Element &b) {
  return std::equal(a.rule_id_.begin(), a.rule_id_.end(), b.rule_id_.begin());
};


//-- ParseQuad
// Quadruplet for parsing problem
class ParseQuad : public Quadruplet {
public:
  ParseQuad(int rule_no, int dot_loc, int limit)
      : Quadruplet(rule_no, dot_loc), limit_(limit){};
  virtual ~ParseQuad(){};

  void add(int rule_id, double prob);
  void merge(const ParseQuad *quadruplet);
  void multiply(const ParseQuad *quadruplet);

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
  void regist(const std::string &inString);

  int get_result_num(void);
  void set_mode(int new_mode) { mode_ = new_mode; };

  void back_trace(int index);
  void back_trace_all();

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

#endif // PARSE_PROGLEM_H_
//  ParseProblem.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_PROGLEM_H_
#define PARSE_PROGLEM_H_

#include "Element.h"
#include "Grammar.h"
#include "Registration.h"

//-- ParseQuad
// Quadruplet for parsing problem
class ParseQuad : public Quadruplet {
public:
  ParseQuad(int rule_no, int dot_loc, ElementPool *allocator, int limit)
      : Quadruplet(rule_no, dot_loc), element_pool_(allocator), limit_(limit){};
  virtual ~ParseQuad(){};

  void add(double inProb);
  void add_next(ParseQuad *quadruplet);
  void marge(ParseQuad *quadruplet);
  void multiply(ParseQuad *quadruplet);

  std::list<Element *> &get_prob_list(void) { return probs_; };

protected:
  void limit(std::list<Element *> &inElement);

private:
  std::list<Element *> probs_;
  ElementPool *element_pool_;
  int limit_;
};

//--
class Tracer {
public:
  Tracer(std::shared_ptr<Grammar> grammar = NULL);
  virtual ~Tracer();

  void init(double inProb);
  void reverse(Element *e);
  void finish(void);

protected:
  void reverse_self(Element *e);

  std::shared_ptr<Grammar> grammar_;
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
  void back_trace(int inIndex, Tracer *inTracer = NULL);
  void back_trace_all(Tracer *inTracer = NULL);

  void set_mode(int inNewMode) { mode_ = inNewMode; };

protected:
  std::unique_ptr<ParseQuad> create_quad(int inRuleNo, int inDotLoc) {
    return std::make_unique<ParseQuad>(inRuleNo, inDotLoc, element_pool_,
                                       limit_);
  }
  void init_registration(void);

private:
  ElementPool *element_pool_;
  ParseQuad *results_;

  int limit_;
  int mode_;
};

#endif // PARSE_PROGLEM_H_
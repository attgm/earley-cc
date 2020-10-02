//  ParseProblem.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef PARSE_PROGLEM_H_
#define PARSE_PROGLEM_H_

#include "ElementPool.h"
#include "Grammar.h"
#include "Registration.h"

//-- ParseQuad
// 構文解析問題用Quadruplet
class ParseQuad : public Quadruplet {
 public:
  ParseQuad(int inRuleNo, int inDotLoc, ElementPool *inAllocator, int inLimit,
            int inMode)
      : Quadruplet(inRuleNo, inDotLoc),
        element_pool_(inAllocator),
        limit_(inLimit),
        mode_(inMode){};
  virtual ~ParseQuad(){};

  void add(double inProb);
  void add_next(Quadruplet *inQuadruplet);
  void marge(Quadruplet *inQuadruplet);
  void multiply(Quadruplet *inElement);

  std::list<Element *> &get_prob_list(void) { return probs_; };

 protected:
  void limit(std::list<Element *> &inElement);

 private:
  std::list<Element *> probs_;
  ElementPool *element_pool_;
  int limit_;
  int mode_;
};

//--
class Tracer {
 public:
  Tracer(Grammar *inGrammar = NULL);
  virtual ~Tracer();

  virtual void init(double inProb);
  virtual void reverse(Element *e);
  virtual void finish(void);

 protected:
  virtual void reverse_self(Element *e);

  Grammar *grammar_;
};

//--
class ParseRegistration : public Registration {
 public:
  enum { mode_Number, mode_MinNumber };

  ParseRegistration(Grammar *inGrammar);
  virtual ~ParseRegistration();

  void set_limit(int inLimit);

  virtual void regist(const std::string &inString);

  int get_result_num(void);
  void back_trace(int inIndex, Tracer *inTracer = NULL);
  void back_trace_all(Tracer *inTracer = NULL);

  void set_mode(int inNewMode) { mode_ = inNewMode; };

 protected:
  Quadruplet *create_quad(int inRuleNo, int inDotLoc);
  void init_registration(void);

 private:
  ElementPool *element_pool_;
  ParseQuad *results_;

  int limit_;
  int mode_;
};

#endif  // PARSE_PROGLEM_H_
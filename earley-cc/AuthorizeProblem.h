//  AuthorizeProblem.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef AUTHORIZE_PROGLEM_H_
#define AUTHORIZE_PROGLEM_H_

#include "Grammar.h"
#include "Registration.h"

//-- AuthorizeQuad
// 構文解析問題用Quadruplet
class AuthorizeQuad : public Quadruplet {
 public:
  AuthorizeQuad(int inRuleNo, int inDotLoc)
      : Quadruplet(inRuleNo, inDotLoc), prob_(0.0){};
  ~AuthorizeQuad(){};

  void add(double inProb);
  void add_next(Quadruplet *inQuadruplet);
  void marge(Quadruplet *inQuadruplet);
  void multiply(Quadruplet *inElement);

  double get_probability(void) { return prob_; };

 private:
  double prob_;
};

//--
class AuthorizeRegistration : public Registration {
 public:
  AuthorizeRegistration(Grammar *inGrammar);
  ~AuthorizeRegistration();

  double calc_probability(void);

 protected:
  Quadruplet *create_quad(int inRuleNo, int inDotLoc);
};

#endif  // AUTHORIZE_PROGLEM_H_

//  AuthorizeProblem.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef AUTHORIZE_PROGLEM_H_
#define AUTHORIZE_PROGLEM_H_

#include "Grammar.h"
#include "Quadruplet.h"
#include "Registration.h"

//-- AuthorizeQuad
// 構文解析問題用Quadruplet
class AuthorizeQuad : public Quadruplet {
public:
  AuthorizeQuad(int rule_id, int dot_loc)
      : Quadruplet(rule_id, dot_loc), prob_(0.0){};

  void add(double prob);
  void add_next(AuthorizeQuad *quadruplet);
  void marge(AuthorizeQuad *quadruplet);
  void multiply(AuthorizeQuad *quadruplet);

  double get_probability(void) const { return prob_; };

private:
  double prob_;
};

//--
class AuthorizeRegistration : public Registration<AuthorizeQuad> {
public:
  AuthorizeRegistration(std::shared_ptr<Grammar> grammar);

  double calc_probability(void);

protected:
  std::unique_ptr<AuthorizeQuad> create_quad(int rule_id, int dot_loc) {
    return std::make_unique<AuthorizeQuad>(rule_id, dot_loc);
  }
};

#endif // AUTHORIZE_PROGLEM_H_

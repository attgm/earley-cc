//  AuthorizeProblem.h
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef AUTHORIZE_PROGLEM_H_
#define AUTHORIZE_PROGLEM_H_

#include <memory>
#include "Grammar.h"
#include "Quadruplet.h"
#include "Registration.h"

//-- AuthorizeQuad
// Quadruplet for authorize fcreate_new_elementproblem
class AuthorizeQuad : public Quadruplet {
public:
  AuthorizeQuad(int rule_id, int dot_loc)
      : Quadruplet(rule_id, dot_loc), prob_(0.0){};


  void add(int /* rule_id */, double prob);
  void merge(std::unique_ptr<AuthorizeQuad> const &quadruplet);
  void multiply(std::unique_ptr<AuthorizeQuad> const &quadruplet);

  double get_probability(void) const { return prob_; };

private:
  double prob_;
};

//--
class AuthorizeRegistration : public Registration<AuthorizeQuad> {
public:
  AuthorizeRegistration(std::shared_ptr<Grammar> grammar);
  
protected:
  std::unique_ptr<AuthorizeQuad> create_quad(int rule_id, int dot_loc) {
    return std::make_unique<AuthorizeQuad>(rule_id, dot_loc);
  }
};

#endif // AUTHORIZE_PROGLEM_H_

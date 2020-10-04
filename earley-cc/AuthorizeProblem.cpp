//  AuthorizeProblem.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "AuthorizeProblem.h"

#include "ParseList.h"

//-- add
void AuthorizeQuad::add(double prob) { prob_ += prob; }

//-- add_next
void AuthorizeQuad::add_next(const AuthorizeQuad *quadruplet) {
  prob_ += quadruplet->get_probability();
}

//-- marge
void AuthorizeQuad::marge(const AuthorizeQuad *quadruplet) {
  prob_ += quadruplet->get_probability();
}

//-- multiply
void AuthorizeQuad::multiply(const AuthorizeQuad *quadruplet) {
  prob_ *= quadruplet->get_probability();
}

//-- AuthorizeRegistration
// constractor
AuthorizeRegistration::AuthorizeRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<AuthorizeQuad>(grammar) {}

//-- calc_probability
// Summation of all the quadriplets starting with a root term
double AuthorizeRegistration::calc_probability(void) {
  // search [*->*.,0,n]
  auto unit = parse_list_->find(0, input_length_, 0);
  if (!unit) {
    return 0.0;
  }

  double results = 0.0;
  for (const auto &quad : *unit) {
    // filtered [S->*., 0, n]
    if ((grammar_->get_rule(quad->get_rule_id()))->left ==
        grammar_->get_root_term_id()) {
      results += quad->get_probability();
    }
  }
  return results;
};

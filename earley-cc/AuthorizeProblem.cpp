//  AuthorizeProblem.cpp
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "AuthorizeProblem.h"
#include "ParseList.h"
#include "Grammar.h"
#include "iostream"

//-- add
void AuthorizeQuad::add(int /* rule_id */, double prob) { prob_ += prob; }

//-- merge
void AuthorizeQuad::merge(std::unique_ptr<AuthorizeQuad> const &quadruplet) {
  prob_ += quadruplet->get_probability();
}

//-- multiply
void AuthorizeQuad::multiply(std::unique_ptr<AuthorizeQuad> const &quadruplet) {
  prob_ *= quadruplet->get_probability();
}

//-- AuthorizeRegistration
// constructor
AuthorizeRegistration::AuthorizeRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<AuthorizeQuad>(grammar) {}

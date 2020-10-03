//  AuthorizeProblem.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "AuthorizeProblem.h"

#include "ParseList.h"

//-- add(double inProb)
void AuthorizeQuad::add(double prob) { prob_ += prob; }

//----------- AuthorizeQuad::add_next
// Quadrupletの「次」(dotが1つ右に移動した)要素を挿入する
void AuthorizeQuad::add_next(AuthorizeQuad *quadruplet) {
  prob_ += quadruplet->get_probability();
}

//----------- AuthorizeQuad::Marge
// inQuadの要素とマージする
void AuthorizeQuad::marge(AuthorizeQuad *quadruplet) {
  prob_ += quadruplet->get_probability();
}

//----------- AuthorizeQuad::Multiply
void AuthorizeQuad::multiply(AuthorizeQuad *quadruplet) {
  prob_ *= quadruplet->get_probability();
}

#pragma mark-- AuthorizeRegistration --
//---------- AuthorizeRegistration
// constractor
AuthorizeRegistration::AuthorizeRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<AuthorizeQuad>(grammar) {}

//----------AuthorizeRegistration::CalcProbability
// 開始記号で始まるQuadを全部足し合わせた結果を返す
double AuthorizeRegistration::calc_probability(void) {
  // [S->γ.,0,n]なる要素を検索する
  //   (実際は[*->*.,0,n] )
  auto unit = parse_list_->find(0, input_length_, 0);
  if (!unit) {
    return 0.0;
  }

  double results = 0.0;
  for (auto it = unit->begin(); it != unit->end(); it++) {
    auto quad = (*it).get();
    if ((grammar_->get_rule(quad->get_rule_id()))->left ==
        grammar_->get_root_term_id()) {
      results += quad->get_probability();
    }
  }
  return results;
};

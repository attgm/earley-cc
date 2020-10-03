//  AuthorizeProblem.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "AuthorizeProblem.h"

#include "ParseList.h"

//----------- AuthorizeQuad::add(double inProb)
void AuthorizeQuad::add(double inProb) { prob_ += inProb; }

//----------- AuthorizeQuad::add_next
// Quadrupletの「次」(dotが1つ右に移動した)要素を挿入する
void AuthorizeQuad::add_next(Quadruplet *inQuadruplet) {
  AuthorizeQuad *quad = dynamic_cast<AuthorizeQuad *>(inQuadruplet);
  prob_ += quad->get_probability();
}

//----------- AuthorizeQuad::Marge
// inQuadの要素とマージする
void AuthorizeQuad::marge(Quadruplet *inQuadruplet) {
  AuthorizeQuad *quad = dynamic_cast<AuthorizeQuad *>(inQuadruplet);
  prob_ += quad->get_probability();
}

//----------- AuthorizeQuad::Multiply
void AuthorizeQuad::multiply(Quadruplet *inQuadruplet) {
  AuthorizeQuad *quad = dynamic_cast<AuthorizeQuad *>(inQuadruplet);
  prob_ *= quad->get_probability();
}

#pragma mark-- AuthorizeRegistration --
//---------- AuthorizeRegistration
// constractor
AuthorizeRegistration::AuthorizeRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<AuthorizeQuad>(grammar) {}

//---------- AuthorizeRegistration::~AuthorizeRegistration
// distractor
AuthorizeRegistration::~AuthorizeRegistration() {}

//---------- AuthorizeRegistration::CreateQuad [protected]
//
AuthorizeQuad *AuthorizeRegistration::create_quad(int inRuleNo, int inDotLoc) {
  return new AuthorizeQuad(inRuleNo, inDotLoc);
}

//----------AuthorizeRegistration::CalcProbability
// 開始記号で始まるQuadを全部足し合わせた結果を返す
double AuthorizeRegistration::calc_probability(void) {
  // [S->γ.,0,n]なる要素を検索する
  //   (実際は[*->*.,0,n] )
  QuadSet *unit = parse_list_->find(0, input_length_, 0);
  if (!unit) {
    return 0.0;
  }

  double results = 0.0;
  for (QuadSet::iterator it = unit->begin(); it != unit->end(); it++) {
    AuthorizeQuad *quad = dynamic_cast<AuthorizeQuad *>(*it);
    if ((grammar_->get_rule(quad->get_rule_no()))->left ==
        grammar_->get_root_term_id()) {
      results += quad->get_probability();
    }
  }
  return results;
};

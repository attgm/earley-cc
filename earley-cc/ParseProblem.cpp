//  ParseProblem.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "ParseProblem.h"

#include <iostream>

#include "ParseList.h"

const int kDefaultLimit = 10;
const int kDefaultMode = ParseRegistration::mode_Number;

#pragma mark-- ParseQuad --
//----------- ParseQuad::Add
// double inProb : 追加する確率値
void ParseQuad::add(double prob) {
  Element *newElement = element_pool_->create_new_element(prob);
  newElement->quadruplet_ = this;
  probs_.push_back(newElement);

  limit(probs_);
}

//----------- AddNext
// Quadrupletの「次」(dotが1つ右に移動した)要素を挿入する
void ParseQuad::add_next(ParseQuad *quadruplet) {
  std::list<Element *> &baseList = quadruplet->get_prob_list();

  for (std::list<Element *>::iterator it = baseList.begin();
       it != baseList.end(); it++) {
    Element *new_element = element_pool_->create_new_element(*it);
    new_element->quadruplet_ = this;
    probs_.push_back(new_element);
  }
}

//----------- ParseQuad::Marge
// inQuadの要素とマージする
void ParseQuad::marge(ParseQuad *quadruplet) {
  // 確率値のリストへの参照を得る
  std::list<Element *> &sl = quadruplet->get_prob_list();
  // 元のQuadrupletを始めから見て,
  std::list<Element *>::iterator it = probs_.begin();
  for (std::list<Element *>::iterator i = sl.begin(); i != sl.end(); i++) {
    while (it != probs_.end() && *(*it) > *(*i))
      it++;
    (*i)->quadruplet_ = this;
    probs_.insert(it, *i);
  }
  limit(probs_);
}

//----------- ParseQuad::Multiply
// quadrupletとの積をとる
void ParseQuad::multiply(ParseQuad *quadruplet) {
  std::list<Element *> probs;
  std::list<Element *> &sl = quadruplet->get_prob_list();

  for (std::list<Element *>::iterator i = probs_.begin(); i != probs_.end();
       i++) {
    std::list<Element *>::iterator it = probs.begin();
    for (std::list<Element *>::iterator j = sl.begin(); j != sl.end(); j++) {
      Element *newElement = element_pool_->create_new_element(*i, *j);
      newElement->quadruplet_ = this;
      while (it != probs.end() && *(*it) > *newElement)
        it++;
      if (it == probs.end() && probs.size() > limit_) {
        // ElementPool::RemoveElement(newElement);
        break;
      } else {
        probs.insert(it, newElement);
      }
    }
  }
  limit(probs);
  probs_ = probs;
}

//---------- ParseQuad::Limit
// 確率値を mLimit 個に制限する.
void ParseQuad::limit(std::list<Element *> &inQuad) {
  while (inQuad.size() > limit_) {
    inQuad.pop_back();
  }
}

#pragma mark-- ParseRegistration --
//---------- ParseRegistration
// constractor
ParseRegistration::ParseRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<ParseQuad>(grammar), results_(NULL), limit_(kDefaultLimit),
      mode_(kDefaultMode) {
  element_pool_ = new ElementPool;
}

//---------- ParseRegistration::~ParseRegistration
// distractor
ParseRegistration::~ParseRegistration() {
  if (element_pool_) {
    delete element_pool_;
  }
}

//---------- ParseRegistration::InitRegistration
// Registの前処理
void ParseRegistration::init_registration(void) {
  Registration::init_registration();
  element_pool_->clear();
}

//---------- ParseRegistration::SetLimit
void ParseRegistration::set_limit(int inLimit) {
  if (inLimit > 0) {
    limit_ = inLimit;
  }
}

//---------- ParseRegistration::GetResultNum
int ParseRegistration::get_result_num(void) {
  return results_->get_prob_list().size();
};

//---------- ParseRegistration::Regist
//
void ParseRegistration::regist(const std::string &inString) {
  // 以前の結果が残っていたら削除する
  if (results_) {
    delete results_;
  }
  // 構文解析を行う
  Registration::regist(inString);

  // 開始記号から始まる要素を取り出す
  // [S->γ.,0,n]なる要素を検索する(実際は[*->*.,0,n] )
  try {
    auto unit = parse_list_->find(0, input_length_, 0);
    // 結果の列を取り出す
    results_ = create_quad(-1, -1).get();
    for (auto it = unit->begin(); it != unit->end(); it++) {
      auto element = (*it).get();
      // もし左辺が開始記号だった場合
      if ((grammar_->get_rule(element->get_rule_id()))->left ==
          grammar_->get_root_term_id()) {
        // マージソートでk個だけ取り出す
        results_->marge(element);
      }
    }
  } catch (std::out_of_range e) {
    std::cout << "out_of_range" << std::endl;
    return;
  }
}

//----------ParseRegistration::BackTrace
//  index番目の構文木を得る
void ParseRegistration::back_trace(int index, Tracer *default_tracer) {
  if (!results_ || index < 0 || index >= results_->get_prob_list().size())
    return;
  Tracer *tracer = default_tracer ? default_tracer : new Tracer(grammar_);

  std::list<Element *> &probList = results_->get_prob_list();
  // index番目のiteratorを求める
  std::list<Element *>::iterator it = probList.begin();
  advance(it, index);

  if (it != probList.end()) {
    tracer->init((*it)->prob_);
    tracer->reverse(*it);
    tracer->finish();
  }

  if (!default_tracer) {
    delete tracer;
  }
};

//----------ParseRegistration::BackTraceAll
//  先頭のElementを取って
//  バックトレースを行なう
void ParseRegistration::back_trace_all(Tracer *default_tracer) {
  if (!results_)
    return;

  Tracer *tracer = default_tracer ? default_tracer : new Tracer(grammar_);

  // 得た構文木の先頭それぞれからバックトレースを始める
  for (std::list<Element *>::iterator it = results_->get_prob_list().begin();
       it != results_->get_prob_list().end(); it++) {
    tracer->init((*it)->prob_);
    tracer->reverse(*it);
    tracer->finish();
  }

  if (!default_tracer) {
    delete tracer;
  }
};

//== Tracer

//---------- Tracer::Tracer
// back traceを行うクラス
Tracer::Tracer(std::shared_ptr<Grammar> grammar) : grammar_(grammar) {}

//----------- Tracer::~Tracer
Tracer::~Tracer() {}

//----------- Tracer::init
// back trace開始時に呼ばれる関数
void Tracer::init(double inProb) {
  std::cout << "prob :" << inProb << std::endl;
}

//----------- Tracer::reverse
//   実際にバックトレースを行なうルーチン
//   再帰的に呼び出される
void Tracer::reverse(Element *e) {
  if (e->back_ptr_1 == NULL) {
    reverse_self(e);
  } else {
    if (e->back_ptr_2 != NULL) {
      reverse(e->back_ptr_2);
    }
    reverse(e->back_ptr_1);
  }
}

//----------- Tracer::reverse_self
//  [A -> ・γ, i, j] となった時に呼び出される.
//  実際ここだけをover writeすればいいと思われる.
void Tracer::reverse_self(Element *e) {
  auto rule = grammar_->id_to_rule(e->quadruplet_->get_rule_id());
  std::cout << rule << std::endl;
}

//----------- Tracer::finish
//   バックトレース終了時に呼び出される
void Tracer::finish(void) { std::cout << "--" << std::endl; }

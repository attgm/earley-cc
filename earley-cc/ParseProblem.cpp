//  ParseProblem.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "ParseProblem.h"

#include <iostream>
#include <algorithm>

#include "ParseList.h"

const int DEFAULT_LIMIT = 10;
const int DEFAULT_MODE = ParseRegistration::mode_Number;

#pragma mark-- ParseQuad --
//----------- ParseQuad::add
// add a prob
void ParseQuad::add(int rule_id, double prob) {
  Element element;
  element.prob_ = prob;
  element.rule_id_.push_back(rule_id);
  probs_.push_back(element);
  limit();
}


//----------- ParseQuad::merge
// merge with quadruplet
void ParseQuad::merge(const ParseQuad* quadruplet) {
  auto baseList = quadruplet->get_prob_list();

  for (auto it : baseList){
    Element element;
    element.prob_ = it.prob_;
    element.rule_id_.insert(element.rule_id_.begin(), it.rule_id_.begin(), it.rule_id_.end());
    probs_.push_back(element);
  }
  limit();
}

//----------- ParseQuad::Multiply
// multiply two quadruplets
void ParseQuad::multiply(const ParseQuad *quadruplet) {
  auto prob_list = quadruplet->get_prob_list();
  std::list<Element> new_prob_list;

  for (auto i : probs_) {
    for (auto j : prob_list) {
      Element new_element;
      new_element.prob_ = i.prob_ * j.prob_;
      new_element.rule_id_.insert(new_element.rule_id_.begin(), j.rule_id_.begin(), j.rule_id_.end());
      new_element.rule_id_.insert(new_element.rule_id_.begin(),i.rule_id_.begin(), i.rule_id_.end());
      new_prob_list.push_back(new_element);
    }
  }

  probs_ = new_prob_list;
  limit();
}

//---------- ParseQuad::Limit
// Limit the number of probability values to limit_.
void ParseQuad::limit() {
  probs_.sort(std::greater<Element>());
  while (probs_.size() > limit_) {
    probs_.pop_back();
  }
}

#pragma mark-- ParseRegistration --
//---------- ParseRegistration
// constructor
ParseRegistration::ParseRegistration(std::shared_ptr<Grammar> grammar)
    : Registration<ParseQuad>(grammar), limit_(DEFAULT_LIMIT),
      mode_(DEFAULT_MODE) {
}

//---------- ParseRegistration::~ParseRegistration
// destructor 
ParseRegistration::~ParseRegistration() {
}

//---------- ParseRegistration::InitRegistration
// Preprocessing of registration
void ParseRegistration::init_registration(void) {
  Registration::init_registration();
}

//---------- ParseRegistration::SetLimit
void ParseRegistration::set_limit(int limit) {
  if (limit > 0) {
    limit_ = limit;
  }
}

//---------- ParseRegistration::GetResultNum
int ParseRegistration::get_result_num(void) {
  return results_->get_prob_list().size();
};

//---------- ParseRegistration::Regist
//
void ParseRegistration::regist(const std::string &string) {
  results_ = create_quad(-1, -1);
  
  // Perform parsing
  Registration::regist(string);

  // Find elements beginning with the root term
  try {
    // find quad [*->*.,0,n]
    auto unit = parse_list_->find(0, input_length_, 0);
    for (const auto &it : *unit) {
      // if left term is start term
      if ((grammar_->get_rule(it->get_rule_id()))->left == grammar_->get_root_term_id()) {
        // get the results
        results_->merge(it.get());
      }
    }
  } catch (std::out_of_range e) {
    std::cout << "out_of_range" << std::endl;
    return;
  }
}

//----------ParseRegistration::BackTrace
//  get the index-th parse tree
void ParseRegistration::back_trace(int index) {
  if (index < 0 || index >= results_->get_prob_list().size())
    return;

  auto probList = results_->get_prob_list();
  auto it = std::next(probList.begin(), index);
  if (it != probList.end()) {
    std::cout << it->prob_ << std::endl;

    for(auto i : it->rule_id_){
      std::cout << grammar_->id_to_rule(i) << std::endl;
    }
    std::cout << "---" << std::endl;
  }
};

//----------ParseRegistration::BackTraceAll
// print all parse tree 
void ParseRegistration::back_trace_all() {
  for (int i = 0; i<results_->get_prob_list().size(); i++){
    back_trace(i);
  }
};

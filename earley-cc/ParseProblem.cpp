//  ParseProblem.cpp
//    1999 - 2023 Atsushi Tagami
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
  element.rule_list_.reserve(1);
  element.rule_list_.push_back(rule_id);
  probs_.push_back(element);
  limit();
}


//----------- ParseQuad::merge
// merge with quadruplet
void ParseQuad::merge(std::unique_ptr<ParseQuad> const &quadruplet) {
  auto baseList = quadruplet->get_prob_list();

  for (auto it : baseList){
    Element element;
    element.prob_ = it.prob_;
    element.rule_list_.reserve(it.rule_list_.size());
    std::copy(it.rule_list_.begin(), it.rule_list_.end(), std::back_inserter(element.rule_list_));
    probs_.push_back(element);
  }
  limit();
}

//----------- ParseQuad::Multiply
// multiply two quadruplets
void ParseQuad::multiply(std::unique_ptr<ParseQuad> const &quadruplet) {
  auto prob_list = quadruplet->get_prob_list();
  std::list<Element> new_prob_list;

  for (auto i : probs_) {
    for (auto j : prob_list) {
      Element element;
      element.prob_ = i.prob_ * j.prob_;
      element.rule_list_.reserve(j.rule_list_.size() + i.rule_list_.size());
      std::copy(i.rule_list_.begin(), i.rule_list_.end(), 
        std::copy(j.rule_list_.begin(), j.rule_list_.end(), std::back_inserter(element.rule_list_)));
      new_prob_list.push_back(element);
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

//----------ParseRegistration::print_result_at
//  get the index-th parse tree
void ParseRegistration::print_result_at(std::unique_ptr<ParseQuad> const &results, int index) {
  auto probList = results->get_prob_list();
  auto it = std::next(probList.begin(), index);
  if (it != probList.end()) {
    std::cout << it->prob_ << std::endl;

    for(auto i : it->rule_list_){
      std::cout << grammar_->id_to_rule(i) << std::endl;
    }
    std::cout << "---" << std::endl;
  }
};

//----------ParseRegistration::print_all_results
// print all parse tree 
void ParseRegistration::print_all_results(std::unique_ptr<ParseQuad> const &results) {
  for (auto it : results->get_prob_list()){
    std::cout << it.prob_ << std::endl;

    for(auto i : it.rule_list_){
      std::cout << grammar_->id_to_rule(i) << std::endl;
    }
    std::cout << "---" << std::endl;
  }
};

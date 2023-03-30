//  Registration.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef REGISTRATION_H_
#define REGISTRATION_H_

#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <strstream>
#include <vector>

#include "Grammar.h"
#include "ParseList.h"
#include "Quadruplet.h"

template <class Q> class Registration {
public:
  Registration(std::shared_ptr<Grammar> grammar)
      : grammar_(grammar), parse_list_(nullptr){};

  void regist(const std::string &input_string) {
    std::istrstream iss(input_string.data(), input_string.length());
    std::string buffer;
    std::vector<int> term_list;

    while (iss >> buffer) {
      int token = grammar_->term_to_id(buffer);
      if (token == TERM_UNKNOWN) {
        throw std::runtime_error("unknown term : " + buffer);
      }
      term_list.push_back(token);
    }
    init_registration();

    input_length_ = term_list.size();
    parse_list_ = std::make_unique<ParseList<Q>>(input_length_ + 1);

    step1();
    for (int i = 1; i < (input_length_ + 1); i++) {
      step2(i, term_list);
      step3(i);
    }
  }

protected:
  void init_registration(void){};
  virtual std::unique_ptr<Q> create_quad(int rule_id, int dot_loc) = 0;

  std::unique_ptr<Q> create_next_quad(Q *quadruplet) {
    auto new_quad =
        create_quad(quadruplet->get_rule_id(), quadruplet->get_dot_loc() + 1);
    new_quad->merge(quadruplet);

    return std::move(new_quad);
  };

  void step1(void);
  void step2(int i, std::vector<int> &input);
  void step3(int i);

  std::shared_ptr<Grammar> grammar_;
  std::unique_ptr<ParseList<Q>> parse_list_;
  int input_length_;
};

// Step1
//   A ->α => A->.α, i, i, 0
//    (i,i) points the same table, thus
//    this function sets only (0,0)
template <class Q> void Registration<Q>::step1(void) {
  for (int i = 0; i < grammar_->rule_num(); i++) {
    const Rule *rule = grammar_->get_rule(i);
    auto quad = create_quad(i, 0);
    quad->add(i, rule->prob);
    parse_list_->insert(0, 0, rule->right[0], std::move(quad));
  }
};

// Step2
//  [ * -> * .a(i-1)* ,j ,i-1] =>[ * -> * a(i-1).* ,j ,i]
template <class Q> void Registration<Q>::step2(int i, std::vector<int> &input) {
  for (int j = 0; j < i; j++) {
    // search [ * -> * .a(i-1)*,j,i-1]
    try {
      auto base = parse_list_->find(j, i - 1, input[i - 1]);

      for (const auto &quad : *base) {
        // insert [ * -> * a(i-1).*,j,i]
        auto quadruplet = create_next_quad(quad.get());
        int term = grammar_->term_after_dot(quadruplet->get_rule_id(),
                                            quadruplet->get_dot_loc());
        parse_list_->insert(j, i, term, std::move(quadruplet));
      }
    } catch (std::out_of_range e) {
    }
  }
};

// Step3
//    [ A -> * .,k.i],[ * ->  *.A *,j,k] => [ * ->  *A .*,j,k]
template <class Q> void Registration<Q>::step3(int i) {
  for (int j = i - 1; j >= 0; j--) {
    for (int k = i - 1; k >= j; k--) {
      // search [ A -> *., k, i]
      try {
        auto first_quads = parse_list_->find(k, i, TERM_EPSILON);
        for (const auto &first_quad : *first_quads) {
          int left_term = grammar_->get_rule(first_quad->get_rule_id())->left;
          // search [ * ->  *.A *, j, k]
          try {
            auto second_quads = parse_list_->find(j, k, left_term);

            for (const auto &second_quad : *second_quads) {
              // insert [ * ->  *A .*, j, i]
              auto new_quad = create_next_quad(second_quad.get());
              new_quad->multiply(first_quad.get());
              int term = grammar_->term_after_dot(new_quad->get_rule_id(),
                                                  new_quad->get_dot_loc());
              parse_list_->insert(j, i, term, std::move(new_quad));
            }
          } catch (std::out_of_range e) {
          }
        }
      } catch (std::out_of_range e) {
      }
    }
  }
};

#endif // REGISTRATION_H_
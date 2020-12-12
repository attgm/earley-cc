//  Grammar.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include "Grammar.h"

#include <iostream>
#include <strstream>

//-- load_rule
// load rule file.
//	If there is some problem, throw runtime error
void Grammar::load_rule(std::istream &is) {
  std::string buffer;
  int line = 0;
  terms_.clear();

  // First line is root term
  is >> buffer;
  root_term_ = term_to_id_or_create(buffer);
  getline(is, buffer); // Skip to the next line
  line++;

  while (getline(is, buffer)) {
    std::cout << buffer << std::endl;
    line++;
    if (buffer.length() == 0) {
      continue;
    }
    std::istrstream iss(buffer.data(), buffer.length());

    auto rule = std::make_unique<Rule>();

    iss >> rule->prob; // probability
    iss >> buffer;     // left term
    rule->left = term_to_id_or_create(buffer);
    // right terms
    std::string token;
    while (iss >> token) {
      rule->right.push_back(term_to_id_or_create(token));
    }
    // if there is no right term, throw
    if (rule->right.size() == 0) {
      throw std::runtime_error("invalid rule : " + buffer + ":" +
                               std::to_string(line));
    }
    rules_.push_back(std::move(rule));
  }
};

//-- term_to_id
// Convert term to term id. If it is unknown id, return -1
int Grammar::term_to_id(const std::string &term) {
  int id = TERM_UNKNOWN;
  TermList::iterator it = terms_.find(term);
  if (it != terms_.end()) {
    id = it->second;
  }
  return id;
};

//-- term_to_id_or_create
// Convert term to term id. If the term is new, it is assigned new id.
int Grammar::term_to_id_or_create(const std::string &term) {
  int id = term_to_id(term);
  if (id == TERM_UNKNOWN) {
    id = term_num() + 1;
    terms_[term] = id;
  }
  return id;
};

//-- id_to_term
// Convert term id to Term Number. If there is no id, return empty string
const std::string &Grammar::id_to_term(int term_id) {
  for (TermList::iterator it = terms_.begin(); it != terms_.end(); ++it) {
    if (term_id == it->second) {
      return it->first;
    }
  }
  throw std::runtime_error("term id" + std::to_string(term_id) +
                           "is not found");
};

//-- term_after_dot
//   return term id after dot
int Grammar::term_after_dot(int rule_id, int dot_locate) {
  if (rule_id >= rule_num()) {
    throw std::runtime_error("out of range :" + std::to_string(rule_id) +
                             " >= " + std::to_string(rule_num()));
  }

  auto rule = get_rule(rule_id);
  if (rule->right.size() > dot_locate) {
    return rule->right[dot_locate];
  } else {
    return TERM_EPSILON;
  }
};

//-- id_to_rule
//  convert rule id to string
std::string Grammar::id_to_rule(int rule_id) {
  auto rule = get_rule(rule_id);
  std::string rule_string = id_to_term(rule->left);
  rule_string += " -> ";

  try {
    for (auto it = rule->right.begin(); it != rule->right.end(); ++it) {
      auto term = id_to_term(*it);
      rule_string += term;
      rule_string += " ";
    }
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return rule_string;
};
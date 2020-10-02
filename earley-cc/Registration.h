//  Registration.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef REGISTRATION_H_
#define REGISTRATION_H_

#include <list>
#include <string>
#include <vector>

class Grammar;
class ParseList;

//-- Quadruplet
// base class
class Quadruplet {
 public:
  Quadruplet(int inRuleNo, int inDotLoc)
      : rule_no_(inRuleNo), dot_loc_(inDotLoc){};
  virtual ~Quadruplet(){};

  virtual void add(double inProb) = 0;
  virtual void add_next(Quadruplet *inQuadruplet) = 0;
  virtual void marge(Quadruplet *inQuadruplet) = 0;
  virtual void multiply(Quadruplet *inQuadruplet) = 0;

  int get_rule_no(void) { return rule_no_; };
  int get_dot_loc(void) { return dot_loc_; };

 private:
  int rule_no_;
  int dot_loc_;

  friend int operator<(const Quadruplet &a, const Quadruplet &b);
};

inline int operator<(const Quadruplet &a, const Quadruplet &b) {
  return ((a.rule_no_ != b.rule_no_) ? (a.rule_no_ < b.rule_no_)
                                     : (a.dot_loc_ < b.dot_loc_));
};

class Registration {
 public:
  Registration(Grammar *inGrammar);
  virtual ~Registration();

  virtual void regist(const std::string &inString);

 protected:
  void step1(void);
  void step2(int i, std::vector<int> &input);
  void step3(int i);

  virtual void init_registration(void);
  virtual Quadruplet *create_quad(int inRuleNo, int inDotLoc) = 0;
  virtual Quadruplet *create_next_quad(Quadruplet *inQuadruplet);

  Grammar *grammar_;
  ParseList *parse_list_;
  int input_length_;
};

#endif  // REGISTRATION_H_

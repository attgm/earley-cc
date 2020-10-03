//  Quadruplet.h
//    2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef QUADRUPLET_H_
#define QUADRUPLET_H_

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

#endif // QUADRUPLET_H_
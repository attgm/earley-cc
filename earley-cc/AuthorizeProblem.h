//---------- AuthorizeProblem.h
// 2000.1 Atsushi Tagami
//  認定問題を解くクラス群
// $Id: AuthorizeProblem.h,v 1.1 2000/02/02 00:14:21 atsushi Exp $
#ifndef __AUTHORIZE_PROGLEM_H___
#define __AUTHORIZE_PROGLEM_H___
#include "Registration.h"
#include "Grammar.h"

//-- AuthorizeQuad
// 構文解析問題用Quadruplet
class AuthorizeQuad : public Quadruplet
{
 public:
  AuthorizeQuad(int inRuleNo, int inDotLoc) 
    : Quadruplet(inRuleNo, inDotLoc), mProb(0.0){};
  ~AuthorizeQuad(){};
 
  void Add(double inProb);
  void AddNext(Quadruplet* inQuadruplet);
  void Marge(Quadruplet* inQuadruplet);
  void Multiply(Quadruplet* inElement);
  
  double GetProbability(void){ return mProb; };
  
 private:
  double mProb;
};


//--
class AuthorizeRegistration : public Registration
{
 public:
  AuthorizeRegistration(Grammar* inGrammar);
  ~AuthorizeRegistration();
  
  double CalcProbability(void);

 protected:
  Quadruplet* CreateQuad(int inRuleNo, int inDotLoc);
};

#endif //__AUTHORIZE_PROGLEM_H__

// AuthorizeProblem.cc
//  2000.1 Atsushi Tagami
///  認定問題を解くクラス群
// $Id: AuthorizeProblem.cc,v 1.1 2000/02/02 00:14:21 atsushi Exp $
#include "AuthorizeProblem.h"
#include "ParseList.h"
 
#pragma mark -- AuthorizeQuad --
//----------- AuthorizeQuad::Add(double inProb)
void
AuthorizeQuad::Add(double inProb)
{
	mProb += inProb;
}


//----------- AuthorizeQuad::AddNext
// Quadrupletの「次」(dotが1つ右に移動した)要素を挿入する
void
AuthorizeQuad::AddNext(Quadruplet* inQuadruplet)
{
	AuthorizeQuad* quad = dynamic_cast<AuthorizeQuad*>(inQuadruplet);
	mProb += quad->GetProbability();
}


//----------- AuthorizeQuad::Marge
// inQuadの要素とマージする
void 
AuthorizeQuad::Marge(Quadruplet* inQuadruplet)
{
  AuthorizeQuad* quad = dynamic_cast<AuthorizeQuad*>(inQuadruplet);
  mProb += quad->GetProbability();
}


//----------- AuthorizeQuad::Multiply
void 
AuthorizeQuad::Multiply(Quadruplet* inQuadruplet)
{
  AuthorizeQuad* quad = dynamic_cast<AuthorizeQuad*>(inQuadruplet);
  mProb *= quad->GetProbability();
}


#pragma mark -- AuthorizeRegistration --
//---------- AuthorizeRegistration
// constractor
AuthorizeRegistration::AuthorizeRegistration(Grammar* inGrammar)
  : Registration(inGrammar)
{
}


//---------- AuthorizeRegistration::~AuthorizeRegistration
// distractor
AuthorizeRegistration::~AuthorizeRegistration()
{
}
  

//---------- AuthorizeRegistration::CreateQuad [protected]
// 
Quadruplet*
AuthorizeRegistration::CreateQuad(int inRuleNo, int inDotLoc)
{
  return new AuthorizeQuad(inRuleNo, inDotLoc);
}



//----------AuthorizeRegistration::CalcProbability
// 開始記号で始まるQuadを全部足し合わせた結果を返す
double
AuthorizeRegistration::CalcProbability(void)
{
  // [S->γ.,0,n]なる要素を検索する
  //   (実際は[*->*.,0,n] )	  
  QuadSet* unit = mParseList->Find(0, mInputLength, 0);
  if(!unit){
    return 0.0;
  }
  
  double results = 0.0;
  for(QuadSet::iterator it = unit->begin(); it != unit->end(); it++){
    AuthorizeQuad* quad = dynamic_cast<AuthorizeQuad*>(*it);
    if((mGrammar->GetRule(quad->GetRuleNo()))->iLeft 
       == mGrammar->GetStartTerm()){
      results += quad->GetProbability();
    }
  }
  return results;
};

// Registration.h
//   1999 Atsushi Tagami
// $Id: Registration.h,v 1.7 2000/03/07 11:49:09 atsushi Exp $
#ifndef __REGISTRATION_H__
#define __REGISTRATION_H__
#include <string>
#include <vector>
#include <list>

class Grammar;
class ParseList;



//-- Quadruplet
// base class
class Quadruplet
{
  public:
	Quadruplet(int inRuleNo, int inDotLoc)
			: mRuleNo(inRuleNo),mDotLoc(inDotLoc){};
	virtual ~Quadruplet(){};
	
	virtual void Add(double inProb) = 0;
	virtual void AddNext(Quadruplet* inQuadruplet)  = 0;
	virtual void Marge(Quadruplet* inQuadruplet)    = 0;
	virtual void Multiply(Quadruplet* inQuadruplet) = 0;

	int GetRuleNo(void){ return mRuleNo; };
	int GetDotLoc(void){ return mDotLoc; };

  private:
	int mRuleNo;
	int mDotLoc;
	
	friend int operator<(const Quadruplet& a, const Quadruplet& b);
}; 


inline int operator<(const Quadruplet& a, const Quadruplet& b) {
	return ((a.mRuleNo != b.mRuleNo) ?
			(a.mRuleNo < b.mRuleNo) : (a.mDotLoc < b.mDotLoc));
};


class Registration
{
public:
  Registration(Grammar* inGrammar);
  virtual ~Registration();
  
  virtual void Regist(const std::string &inString);

protected:
  void Step1(void);
	void Step2(int i, std::vector<int> &input);
  void Step3(int i);

  virtual void        InitRegistration(void);
  virtual Quadruplet* CreateQuad(int inRuleNo, int inDotLoc) = 0;
  virtual Quadruplet* CreateNextQuad(Quadruplet* inQuadruplet);

  Grammar*   mGrammar;
  ParseList* mParseList;
  int mInputLength;
};

#endif //__REGISTRATION_H__

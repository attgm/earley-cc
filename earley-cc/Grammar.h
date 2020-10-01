// Grammar.h
//   1999 Atsushi Tagami
// $Id: Grammar.h,v 1.3 2000/02/01 23:41:18 atsushi Exp $
#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__
#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, int> TermList;
// Rule				  
typedef struct rule{
  int		    iLeft;	       
	std::vector<int>	    iRight;           
  double	    dProb;	      
} Rule;


class Grammar
{
 public:
	Grammar(std::istream& is);
  
	int  TermToNum(const std::string &term);
	void NumToTerm(std::string &outTerm, int inTermID);
	void NumToRule(std::string &outRule, int inRuleNo);
  
  int TermAfterDot(int ruleno, int dotloc);
  
  int TermNum(void){ return mTerms.size(); };
  int RuleNum(void){ return mRules.size(); };
  
  int IsLocked(void){ return mLock; };
  const Rule* GetRule(int inRuleNo){ return mRules[inRuleNo]; };
  int GetStartTerm(void){ return mStartTerm; };
  
  bool Good(void) { return mErrorLine == 0; };
  int  ErrorLine(void) { return mErrorLine; };
  
 protected:
	void Init(std::istream& is);
	void Lock(void){ mLock = true; };
	void UnLock(void){ mLock = false; };
  
 private:
	int mLock;
	int mErrorLine;
	int mTermNum;
	std::vector<Rule*> mRules;
	std::map<std::string, int> mTerms;

  int mStartTerm;
};

#endif// __GRAMMAR_H__

//---------- ParseProblem.h
// 2000.1 Atsushi Tagami
//  構文解析問題を解くクラス群
// $Id: ParseProblem.h,v 1.3 2000/03/07 11:49:09 atsushi Exp $
#ifndef __PARSE_PROGLEM_H__
#define __PARSE_PROGLEM_H__
#include "Registration.h"
#include "Grammar.h"
#include "ElementPool.h"

//-- ParseQuad
// 構文解析問題用Quadruplet
class ParseQuad : public Quadruplet
{
 public:
  
  ParseQuad(int inRuleNo, int inDotLoc,
	    ElementPool* inAllocator, int inLimit, int inMode)
    : Quadruplet(inRuleNo, inDotLoc),
    mElementPool(inAllocator), mLimit(inLimit), mMode(inMode){};
  virtual ~ParseQuad(){};
 
  void Add(double inProb);
  void AddNext(Quadruplet* inQuadruplet);
  void Marge(Quadruplet* inQuadruplet);
  void Multiply(Quadruplet* inElement);
  
	std::list<Element*>& GetProbList(void){ return mProbs; };

 protected:
	void Limit(std::list<Element*>& inElement);

    
 private:
	std::list<Element*>  mProbs;
  ElementPool*    mElementPool;
  int             mLimit;
  int             mMode;
};



//--
class Tracer
{
 public:
          Tracer(Grammar* inGrammar = NULL);
  virtual ~Tracer();
	       
  virtual void Init(double inProb);
  virtual void RutineR(Element* e);
  virtual void Finish(void);
  
 protected:
  virtual void RutineRSelf(Element* e);

  Grammar* mGrammar;
};


//--
class ParseRegistration : public Registration
{
 public:
  
  enum { mode_Number, mode_MinNumber };

  ParseRegistration(Grammar* inGrammar);
  virtual ~ParseRegistration();
  
  void SetLimit(int inLimit);

	virtual void Regist(const std::string &inString);
  
  int  GetResultNum(void);
  void BackTrace(int inIndex, Tracer* inTracer = NULL);
  void BackTraceAll(Tracer* inTracer = NULL);
    
  void SetMode(int inNewMode){ mMode = inNewMode; };

 protected:
  Quadruplet* CreateQuad(int inRuleNo, int inDotLoc);
  void InitRegistration(void);
  
 private:
  ElementPool* mElementPool;
  ParseQuad*   mResults;

  int mLimit;
  int mMode;
};


#endif //__PARSE_PROGLEM_H__

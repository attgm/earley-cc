// ParseProblem.cc
//  2000.1 Atsushi Tagami
///  構文解析問題を解くクラス群
// $Id: ParseProblem.cc,v 1.3 2000/03/07 11:49:09 atsushi Exp $
#include <iostream>

#include "ParseProblem.h"
#include "ElementPool.h"
#include "ParseList.h"

const int kDefaultLimit = 10;
const int kDefaultMode  = ParseRegistration::mode_Number;
 
#pragma mark -- ParseQuad --
//----------- ParseQuad::Add
// double inProb : 追加する確率値
void
ParseQuad::Add(double inProb)
{
	Element* newElement = mElementPool->New(inProb);
	newElement->ptr = this;
	mProbs.push_back(newElement);
	
	Limit(mProbs);
}


//----------- AddNext
// Quadrupletの「次」(dotが1つ右に移動した)要素を挿入する
void
ParseQuad::AddNext(Quadruplet* inQuadruplet)
{
	ParseQuad* quad = dynamic_cast<ParseQuad*>(inQuadruplet);
	std::list<Element*> &baseList = quad->GetProbList();
	
	for(std::list<Element*>::iterator it = baseList.begin(); it != baseList.end(); it++){
		Element* newElement = mElementPool->New(*it);
		newElement->ptr = this;
		mProbs.push_back(newElement);
	}
}


//----------- ParseQuad::Marge
// inQuadの要素とマージする
void 
ParseQuad::Marge(Quadruplet* inQuad)
{
	ParseQuad* quad = dynamic_cast<ParseQuad*>(inQuad);
	// 確率値のリストへの参照を得る
	std::list<Element*> &sl = quad->GetProbList();
	// 元のQuadrupletを始めから見て,  
	std::list<Element*>::iterator it = mProbs.begin();
	for(std::list<Element*>::iterator i = sl.begin(); i != sl.end(); i++){
		while (it != mProbs.end() && *(*it) > *(*i)) it++;
		(*i)->ptr = this;
		mProbs.insert(it, *i);
	}
	Limit(mProbs);
}


//----------- ParseQuad::Multiply
// quadrupletとの積をとる
void 
ParseQuad::Multiply(Quadruplet* inQuad)
{
	ParseQuad* quad = dynamic_cast<ParseQuad*>(inQuad);
	
	std::list<Element*> probs;
	std::list<Element*> &sl = quad->GetProbList();
	
	for(std::list<Element*>::iterator i = mProbs.begin(); i != mProbs.end(); i++) {
		std::list<Element*>::iterator it = probs.begin();
		for(std::list<Element*>::iterator j = sl.begin(); j != sl.end(); j++){
			Element* newElement = mElementPool->New(*i, *j);
			newElement->ptr = this;
			while(it != probs.end() && *(*it) > *newElement) it++;
			if(it == probs.end() && probs.size() > mLimit){
				// ElementPool::RemoveElement(newElement);
				break;
			}else{
				probs.insert(it, newElement);
			}
		}
	}
	Limit(probs);
	mProbs = probs;
}


//---------- ParseQuad::Limit
// 確率値を mLimit 個に制限する.
void 
ParseQuad::Limit(std::list<Element*>& inQuad)
{
	while(inQuad.size() > mLimit){
		inQuad.pop_back();
	}
}

#pragma mark -- ParseRegistration --
//---------- ParseRegistration
// constractor
ParseRegistration::ParseRegistration(Grammar* inGrammar)
  : Registration(inGrammar), mResults(NULL),
    mLimit(kDefaultLimit), mMode(kDefaultMode)
{
  mElementPool = new ElementPool;
}


//---------- ParseRegistration::~ParseRegistration
// distractor
ParseRegistration::~ParseRegistration()
{
	if(mElementPool){
		delete mElementPool;
	}
}
  

//---------- ParseRegistration::InitRegistration
// Registの前処理
void
ParseRegistration::InitRegistration(void)
{
	Registration::InitRegistration();
	mElementPool->Clear();
}


//---------- ParseRegistration::SetLimit
void
ParseRegistration::SetLimit(int inLimit)
{
	if(inLimit > 0){
		mLimit = inLimit;
	}
}


//---------- ParseRegistration::GetResultNum
int
ParseRegistration::GetResultNum(void)
{
	return mResults->GetProbList().size();
};


//---------- ParseRegistration::Regist
//
void
ParseRegistration::Regist(const std::string &inString)
{
  // 以前の結果が残っていたら削除する
	if(mResults){
		delete mResults;
	}
	// 構文解析を行う
	Registration::Regist(inString);
	
	// 開始記号から始まる要素を取り出す
	// [S->γ.,0,n]なる要素を検索する(実際は[*->*.,0,n] )	  
	QuadSet* unit = mParseList->Find(0, mInputLength, 0);
	if(!unit){
		return;
	}
	// 結果の列を取り出す
	mResults = dynamic_cast<ParseQuad*>(CreateQuad(-1, -1));
	for(QuadSet::iterator it = unit->begin(); it != unit->end(); it++){
		Quadruplet *element = *it;
		// もし左辺が開始記号だった場合
		if((mGrammar->GetRule(element->GetRuleNo()))->iLeft 
				== mGrammar->GetStartTerm()){
			// マージソートでk個だけ取り出す
			mResults->Marge(element);
		}
	}
}


//---------- ParseRegistration::CreateQuad [protected]
// 
Quadruplet*
ParseRegistration::CreateQuad(int inRuleNo, int inDotLoc)
{
	return new ParseQuad(inRuleNo, inDotLoc, mElementPool, mLimit, mMode);
}


//----------ParseRegistration::BackTrace
//  inIndex番目の構文木を得る
void
ParseRegistration::BackTrace(int inIndex, Tracer *inTracer)
{
	if (!mResults || inIndex < 0 || inIndex >= mResults->GetProbList().size()) return;
	Tracer* tracer = inTracer ? inTracer : new Tracer(mGrammar);
	
	std::list<Element*> &probList = mResults->GetProbList();
	// inIndex番目のiteratorを求める
	std::list<Element*>::iterator it = probList.begin();
	advance(it, inIndex);
	
	if(it != probList.end()){
		tracer->Init((*it)->dProb);
		tracer->RutineR(*it);
		tracer->Finish();
	}
	
	if(!inTracer){
		delete tracer;
	}
};


//----------ParseRegistration::BackTraceAll
//  先頭のElementを取って  
//  バックトレースを行なう 
void
ParseRegistration::BackTraceAll(Tracer* inTracer)
{
	if (!mResults) return;

	Tracer* tracer = inTracer ? inTracer : new Tracer(mGrammar);
  
	// 得た構文木の先頭それぞれからバックトレースを始める
	for(std::list<Element*>::iterator it = mResults->GetProbList().begin(); 
	it != mResults->GetProbList().end(); 
	it++){
		tracer->Init((*it)->dProb);
		tracer->RutineR(*it);
		tracer->Finish();
	}
	
	if(!inTracer){
		delete tracer;
	}
};


//== Tracer

//---------- Tracer::Tracer
// back traceを行うクラス
Tracer::Tracer(Grammar* inGrammar) 
: mGrammar(inGrammar)
{
}

//----------- Tracer::~Tracer
Tracer::~Tracer()
{
}


//----------- Tracer::Init
// back trace開始時に呼ばれる関数
void
Tracer::Init(double inProb)
{
	std::cout << "prob :" << inProb << std::endl;
}


//----------- Tracer::RutineR
//   実際にバックトレースを行なうルーチン
//   再帰的に呼び出される
void
Tracer::RutineR(Element* e)
{
	if(e->bp1 == NULL){
		RutineRSelf(e);
	}else{
		if(e->bp2 != NULL){
			RutineR(e->bp2);
		}
		RutineR(e->bp1);
	}
}


//----------- Tracer::RutineRSelf
//  [A -> ・γ, i, j] となった時に呼び出される.
//  実際ここだけをover writeすればいいと思われる.
void
Tracer::RutineRSelf(Element* e)
{
	std::string rule;
	mGrammar->NumToRule(rule, e->ptr->GetRuleNo());
	std::cout << rule << std::endl;
}


//----------- Tracer::Finish
//   バックトレース終了時に呼び出される
void
Tracer::Finish(void)
{
	std::cout << "--" << std::endl;
}


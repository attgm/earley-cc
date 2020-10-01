// Registraxtion.cc
//   1999 Atsushi Tagami
// $Id: Registration.cc,v 1.7 2000/03/07 11:49:09 atsushi Exp $
#include <strstream>
#include <iostream>
#include "Grammar.h"
#include "ParseList.h"
#include "Registration.h"

// Registration
//  コンストラクタ
Registration::Registration(Grammar* inGrammar)
  : mGrammar(inGrammar), mParseList(0)
{
}


// ~Registration
//  デストラクタ
Registration::~Registration()
{
  delete mParseList;
}


// Regist
//     文法grammarで入力文字列inStringを構文解析し,
//	parse listに登録するルーチン
void
Registration::Regist(const std::string &inString)
{
	// 入力をstream化する
	std::istrstream iss(inString.data(), inString.length());
	// 空白記号で区切りながら, token listに追加していく
	std::string buffer;
	std::vector<int> input;
	
	while(iss >> buffer){
		int token = mGrammar->TermToNum(buffer);
		if(token == -1){
			std::cerr << "Unknown Term : " << buffer << std::endl;
			exit(0);
		}
		input.push_back(token);
	}
	// すでにParseListがあった場合, そのParseListを開放する
	if(mParseList){
		delete mParseList;
	}
	InitRegistration();
  
	// 入力文字列の長さ + 1のParseListを作成する
	mInputLength = input.size();
	mParseList = new ParseList(mInputLength + 1);
	
	Step1();
	for(int i=1; i<(mInputLength + 1); i++){
		Step2(i, input);
		Step3(i);
	}
};


// Step1
//   A ->α => A->.α, i, i, 0
//    (i,i)はすべて同じテーブルを参照するので
//    (0,0)にだけ挿入すればよい
void 
Registration::Step1(void)
{
	// すべての生成規則に対して
	// 新しくElementをつくり，挿入する
	for(int i=0; i<mGrammar->RuleNum(); i++){
		const Rule* rule = mGrammar->GetRule(i);
		Quadruplet* quad = CreateQuad(i, 0);
		quad->Add(rule->dProb);
		mParseList->Insert(0, 0, rule->iRight[0], quad);
	}
};


// Step2
//  [ * -> * .a(i-1)* ,j ,i-1] =>[ * -> * a(i-1).* ,j ,i]
void
Registration::Step2(int i, std::vector<int> &input)
{
	for(int j=0; j<i; j++){
		// [ * -> * .a(i-1)*,j,i-1] となるElementを探す
		QuadSet* base = mParseList->Find(j, i-1, input[i-1]);
		
		if(base){
			QuadSet::iterator it = base->begin();
			for(; it != base->end(); it++) {
				Quadruplet *newQuad = CreateNextQuad(*it);
				int term 
						= mGrammar->TermAfterDot(newQuad->GetRuleNo(), newQuad->GetDotLoc());
				mParseList->Insert(j, i, term, newQuad);
			}
		}
	}
};


// Step3
//    [ A -> * .,k.i],[ * ->  *.A *,j,k] => [ * ->  *A .*,j,k]
void 
Registration::Step3(int i)
{
	for(int j=i-1; j>=0; j--){
		for(int k=i-1; k>=j; k--){
			// [ A -> *., k, i] となるElementを探す
			QuadSet* qs1 = mParseList->Find(k, i, 0);
			if(qs1 != NULL){
				for(QuadSet::iterator it1 = qs1->begin(); it1 != qs1->end(); it1++){
					// 検索したそれぞれの要素に対して
					// 生成規則の左辺をとりだす
					int leftterm = mGrammar->GetRule((*it1)->GetRuleNo())->iLeft;
					// [ * ->  *.A *,j,k]となるElementを探す
					QuadSet* qs2 = mParseList->Find(j, k, leftterm);
					
					if(qs2 != NULL){
						for(QuadSet::iterator it2 = qs2->begin();
						it2 != qs2->end(); it2++){
							// 検索したそれぞれの要素に対して
							// [ * ->  *A .*,j,i]となるElementを挿入する
							Quadruplet* newQuad = CreateNextQuad(*it2);
							newQuad->Multiply(*it1);
							int term = 
									mGrammar->TermAfterDot(newQuad->GetRuleNo(),
									newQuad->GetDotLoc());
							mParseList->Insert(j, i, term, newQuad);
						}	
					}
				}
			}
		}
	}
};


//----------- CreateNextQuad
Quadruplet*
Registration::CreateNextQuad(Quadruplet* inQuadruplet)
{
	Quadruplet* newQuad = 
			CreateQuad(inQuadruplet->GetRuleNo(), inQuadruplet->GetDotLoc() + 1);
	newQuad->AddNext(inQuadruplet);
	
	return newQuad;
}


//----------- InitRegistraion
// Resigtを始める前に呼び出される
void
Registration::InitRegistration(void)
{
}

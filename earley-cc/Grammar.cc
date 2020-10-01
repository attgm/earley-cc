// Grammar.cc
//   1999 Atsushi Tagami
// $Id: Grammar.cc,v 1.1.1.1 2000/01/05 23:10:57 atsushi Exp $
#include <iostream>
#include <strstream>
#include "Grammar.h"

//--------- Grammar
// constractor
Grammar::Grammar(std::istream& is) : mLock(true), mErrorLine(0), mTermNum(1)
{
	Init(is);
}


//---------- Init
// grammarクラスの初期化をおこなう. 
// 何も問題がなければ 0を何か問題があればその行を返す
void
Grammar::Init(std::istream& is)
{
	std::string buffer;
	UnLock(); // ロック解除
	mErrorLine = 1;
	mTermNum = 1;
	
	// 1行目を読み取って，開始記号を得る
	is >> buffer;
	mStartTerm = TermToNum(buffer);
	getline(is, buffer); // \nまで読み飛ばす
	mErrorLine++;
	
	while(is.good()){
		// 1行読み込んで stream化する
		getline(is, buffer);
		if(buffer.length() == 0){
			mErrorLine++;
			continue;
		}
		std::istrstream iss(buffer.data(), buffer.length());
		
		Rule* rule = new Rule;
		mRules.push_back(rule);
		// 確率値を読み取る
		iss >> rule->dProb;
		// 左辺を読み取る
		iss >> buffer;
		rule->iLeft = TermToNum(buffer);
		// Tokenに分割しながら右辺を読み取る
		std::string token;
		while(iss >> token){
			rule->iRight.push_back(TermToNum(token));
		}
		// もしも右辺がなかった場合はきえる
		if(rule->iRight.size() == 0){
			mRules.pop_back();
			delete rule;
			return;
		}
		mErrorLine++;
	}
	mErrorLine = 0;
	Lock(); // Lock Rules
};


//---------- TermToNum
// Termを対応する番号に変換する 		      
//  もしGrammarがロックされていて，変換が不可能なら-1を返す
//  それ以外は変換した番号を返す			   
int
Grammar::TermToNum(const std::string &term)
{
	int id = -1;
	// 既存のTermListにtermがないかどうか検索する
	TermList::iterator it = mTerms.find(term);
	if (it != mTerms.end()) {
		id = it->second;
	}else if(IsLocked() == false) {
		// Lockがかけられていなければ新規作成
		id = mTermNum++;
		mTerms[term] = id;
	}
	return id;
};


//--------- NumToTerm
// Term Numberを 対応するTermに変換する
//  εもしくは該当なしの場合，""を返す
void 
Grammar::NumToTerm(std::string &outTerm, int inTermID)
{
	for(TermList::iterator it = mTerms.begin(); it != mTerms.end(); ++it){
		if (inTermID == it->second) {
			outTerm = it->first;
			return;
		}
	}
	outTerm = "";
};


// TermAfterDot
//    ドットの次のTermのIDを返す
int
Grammar::TermAfterDot(int inRuleNo, int inDotLocate)
{
	if(inRuleNo >= mRules.size()){
		return 0;
	}
	
	Rule* rule = mRules[inRuleNo];
	if(rule->iRight.size() > inDotLocate){
		/* もし後に文字があるならその文字番号を返す */
		return rule->iRight[inDotLocate];
	}else{
		/* もし最後なら0(ε)を返す */
		return 0;
	}
};


// NumberToRule
//  生成規則番号を生成規則の文字列に変換する
void
Grammar::NumToRule(std::string &outRule, int inRuleNo){
	Rule* rule = mRules[inRuleNo];
	// 左辺をいれる
	NumToTerm(outRule, rule->iLeft);
	outRule += " -> ";
	// 右辺をいれる
	std::string term;
	for(std::vector<int>::iterator it = rule->iRight.begin(); 
	it != rule->iRight.end(); ++it){
		NumToTerm(term, *it);
		outRule += term;
		outRule += " ";
	}
};
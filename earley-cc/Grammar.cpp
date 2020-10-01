//  Grammar.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <iostream>
#include <strstream>
#include "Grammar.h"

//--------- Grammar
// constractor
Grammar::Grammar(std::istream &is) : lock_(true), error_line_(0), term_num_(1)
{
	init(is);
}

//---------- Init
// grammarクラスの初期化をおこなう.
// 何も問題がなければ 0を何か問題があればその行を返す
void Grammar::init(std::istream &is)
{
	std::string buffer;
	unlock(); // ロック解除
	error_line_ = 1;
	term_num_ = 1;

	// 1行目を読み取って，開始記号を得る
	is >> buffer;
	start_term_ = term_to_id(buffer);
	getline(is, buffer); // \nまで読み飛ばす
	error_line_++;

	while (is.good())
	{
		// 1行読み込んで stream化する
		getline(is, buffer);
		if (buffer.length() == 0)
		{
			error_line_++;
			continue;
		}
		std::istrstream iss(buffer.data(), buffer.length());

		Rule *rule = new Rule;
		rules_.push_back(rule);
		// 確率値を読み取る
		iss >> rule->prob;
		// 左辺を読み取る
		iss >> buffer;
		rule->left = term_to_id(buffer);
		// Tokenに分割しながら右辺を読み取る
		std::string token;
		while (iss >> token)
		{
			rule->right.push_back(term_to_id(token));
		}
		// もしも右辺がなかった場合はきえる
		if (rule->right.size() == 0)
		{
			rules_.pop_back();
			delete rule;
			return;
		}
		error_line_++;
	}
	error_line_ = 0;
	lock(); // Lock Rules
};

//---------- term_to_id
// Termを対応する番号に変換する
//  もしGrammarがロックされていて，変換が不可能なら-1を返す
//  それ以外は変換した番号を返す
int Grammar::term_to_id(const std::string &term)
{
	int id = -1;
	// 既存のTermListにtermがないかどうか検索する
	TermList::iterator it = terms_.find(term);
	if (it != terms_.end())
	{
		id = it->second;
	}
	else if (is_locked() == false)
	{
		// Lockがかけられていなければ新規作成
		id = term_num_++;
		terms_[term] = id;
	}
	return id;
};

//--------- id_to_term
// Term Numberを 対応するTermに変換する
//  εもしくは該当なしの場合，""を返す
void Grammar::id_to_term(std::string &outTerm, int inTermID)
{
	for (TermList::iterator it = terms_.begin(); it != terms_.end(); ++it)
	{
		if (inTermID == it->second)
		{
			outTerm = it->first;
			return;
		}
	}
	outTerm = "";
};

// term_after_dot
//    ドットの次のTermのIDを返す
int Grammar::term_after_dot(int inRuleNo, int inDotLocate)
{
	if (inRuleNo >= rules_.size())
	{
		return 0;
	}

	Rule *rule = rules_[inRuleNo];
	if (rule->right.size() > inDotLocate)
	{
		/* もし後に文字があるならその文字番号を返す */
		return rule->right[inDotLocate];
	}
	else
	{
		/* もし最後なら0(ε)を返す */
		return 0;
	}
};

// id_to_rule
//  生成規則番号を生成規則の文字列に変換する
void Grammar::id_to_rule(std::string &outRule, int inRuleNo)
{
	Rule *rule = rules_[inRuleNo];
	// 左辺をいれる
	id_to_term(outRule, rule->left);
	outRule += " -> ";
	// 右辺をいれる
	std::string term;
	for (std::vector<int>::iterator it = rule->right.begin();
		 it != rule->right.end(); ++it)
	{
		id_to_term(term, *it);
		outRule += term;
		outRule += " ";
	}
};
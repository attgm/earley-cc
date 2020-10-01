//  Registration.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <strstream>
#include <iostream>
#include "Grammar.h"
#include "ParseList.h"
#include "Registration.h"

// Registration
//  コンストラクタ
Registration::Registration(Grammar *inGrammar)
	: grammar_(inGrammar), parse_list_(0)
{
}

// ~Registration
//  デストラクタ
Registration::~Registration()
{
	delete parse_list_;
}

// Regist
//     文法grammarで入力文字列inStringを構文解析し,
//	parse listに登録するルーチン
void Registration::regist(const std::string &inString)
{
	// 入力をstream化する
	std::istrstream iss(inString.data(), inString.length());
	// 空白記号で区切りながら, token listに追加していく
	std::string buffer;
	std::vector<int> input;

	while (iss >> buffer)
	{
		int token = grammar_->term_to_id(buffer);
		if (token == -1)
		{
			std::cerr << "Unknown Term : " << buffer << std::endl;
			exit(0);
		}
		input.push_back(token);
	}
	// すでにParseListがあった場合, そのParseListを開放する
	if (parse_list_)
	{
		delete parse_list_;
	}
	init_registration();

	// 入力文字列の長さ + 1のParseListを作成する
	input_length_ = input.size();
	parse_list_ = new ParseList(input_length_ + 1);

	step1();
	for (int i = 1; i < (input_length_ + 1); i++)
	{
		step2(i, input);
		step3(i);
	}
};

// Step1
//   A ->α => A->.α, i, i, 0
//    (i,i)はすべて同じテーブルを参照するので
//    (0,0)にだけ挿入すればよい
void Registration::step1(void)
{
	// すべての生成規則に対して
	// 新しくElementをつくり，挿入する
	for (int i = 0; i < grammar_->rule_num(); i++)
	{
		const Rule *rule = grammar_->get_rule(i);
		Quadruplet *quad = create_quad(i, 0);
		quad->add(rule->prob);
		parse_list_->insert(0, 0, rule->right[0], quad);
	}
};

// Step2
//  [ * -> * .a(i-1)* ,j ,i-1] =>[ * -> * a(i-1).* ,j ,i]
void Registration::step2(int i, std::vector<int> &input)
{
	for (int j = 0; j < i; j++)
	{
		// [ * -> * .a(i-1)*,j,i-1] となるElementを探す
		QuadSet *base = parse_list_->find(j, i - 1, input[i - 1]);

		if (base)
		{
			QuadSet::iterator it = base->begin();
			for (; it != base->end(); it++)
			{
				Quadruplet *newQuad = create_next_quad(*it);
				int term = grammar_->term_after_dot(newQuad->get_rule_no(), newQuad->get_dot_loc());
				parse_list_->insert(j, i, term, newQuad);
			}
		}
	}
};

// Step3
//    [ A -> * .,k.i],[ * ->  *.A *,j,k] => [ * ->  *A .*,j,k]
void Registration::step3(int i)
{
	for (int j = i - 1; j >= 0; j--)
	{
		for (int k = i - 1; k >= j; k--)
		{
			// [ A -> *., k, i] となるElementを探す
			QuadSet *qs1 = parse_list_->find(k, i, 0);
			if (qs1 != NULL)
			{
				for (QuadSet::iterator it1 = qs1->begin(); it1 != qs1->end(); it1++)
				{
					// 検索したそれぞれの要素に対して
					// 生成規則の左辺をとりだす
					int leftterm = grammar_->get_rule((*it1)->get_rule_no())->left;
					// [ * ->  *.A *,j,k]となるElementを探す
					QuadSet *qs2 = parse_list_->find(j, k, leftterm);

					if (qs2 != NULL)
					{
						for (QuadSet::iterator it2 = qs2->begin();
							 it2 != qs2->end(); it2++)
						{
							// 検索したそれぞれの要素に対して
							// [ * ->  *A .*,j,i]となるElementを挿入する
							Quadruplet *newQuad = create_next_quad(*it2);
							newQuad->multiply(*it1);
							int term =
								grammar_->term_after_dot(newQuad->get_rule_no(),
													   newQuad->get_dot_loc());
							parse_list_->insert(j, i, term, newQuad);
						}
					}
				}
			}
		}
	}
};

//----------- CreateNextQuad
Quadruplet *
Registration::create_next_quad(Quadruplet *inQuadruplet)
{
	Quadruplet *newQuad =
		create_quad(inQuadruplet->get_rule_no(), inQuadruplet->get_dot_loc() + 1);
	newQuad->add_next(inQuadruplet);

	return newQuad;
}

//----------- InitRegistraion
// Resigtを始める前に呼び出される
void Registration::init_registration(void)
{
}

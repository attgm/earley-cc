//  Grammar.h
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#pragma once
#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, int> TermList;
// Rule
typedef struct rule
{
	int left;
	std::vector<int> right;
	double prob;
} Rule;

class Grammar
{
public:
	Grammar(std::istream &is);

	int term_to_id(const std::string &term);
	void id_to_term(std::string &outTerm, int inTermID);
	void id_to_rule(std::string &outRule, int inRuleNo);

	int term_after_dot(int ruleno, int dotloc);

	int term_num(void) { return terms_.size(); };
	int rule_num(void) { return rules_.size(); };

	int is_locked(void) { return lock_; };
	const Rule *get_rule(int inRuleNo) { return rules_[inRuleNo]; };
	int get_start_term(void) { return start_term_; };

	bool good(void) { return error_line_ == 0; };
	int error_line(void) { return error_line_; };

protected:
	void init(std::istream &is);
	void lock(void) { lock_ = true; };
	void unlock(void) { lock_ = false; };

private:
	int lock_;
	int error_line_;
	int term_num_;
	std::vector<Rule *> rules_;
	std::map<std::string, int> terms_;

	int start_term_;
};

#endif // GRAMMAR_H_

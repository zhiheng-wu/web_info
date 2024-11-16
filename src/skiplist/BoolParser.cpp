#include "BoolParser.h"
#include <iostream>
#include <set>
using namespace std;

enum STATE {
	S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15
};
struct ParserNode {
	enum TYPE {
		ISSTATE, ISNONTERMINATOR, ISTERMINATOR
	}type;
	enum NONTERMINATOR {
		EL2, E2, EL1, E1, E0
	};
	union
	{
		STATE state;
		NONTERMINATOR nonterminator;
		BoolAnalyser::Node::TERMINATOR terminator;
	};
	int varIdx = 0;
	std::string value;
	ParserNode* left_child = nullptr;
	ParserNode* mid_child = nullptr;
	ParserNode* right_child = nullptr;
	ParserNode();
	ParserNode(STATE s);
	ParserNode(NONTERMINATOR n);
	ParserNode(BoolAnalyser::Node b);
};


enum EQIDX {
	R1, R2, R3, R4, R5, R6, R7, R8, R9
};
struct ACTION {
	enum ACTIONTYPE {
		ERROR, ACC, S, R
	}type;
	union {
		int targetEquation;
		STATE targetState;
	};
	ACTION();
	ACTION(bool accept);
	ACTION(EQIDX eq);
	ACTION(STATE st);
};
struct GOTO {
	bool exsit;
	STATE target;
	GOTO();
	GOTO(STATE t);
};
const static ACTION actionMap[16][7] = {
	//	OR			AND			NOT			(			)			VALUE		$
		ACTION(),	ACTION(),	ACTION(S6),	ACTION(S7),	ACTION(),	ACTION(S8),	ACTION(),
		ACTION(S9),	ACTION(),	ACTION(),	ACTION(),	ACTION(),	ACTION(),	ACTION(true),
		ACTION(R2),	ACTION(),	ACTION(),	ACTION(),	ACTION(R2),	ACTION(),	ACTION(R2),
		ACTION(R3),	ACTION(S10),ACTION(),	ACTION(),	ACTION(R3),	ACTION(),	ACTION(R3),
		ACTION(R5),	ACTION(R5),	ACTION(),	ACTION(),	ACTION(R5),	ACTION(),	ACTION(R5),
		ACTION(R6),	ACTION(R6),	ACTION(),	ACTION(),	ACTION(R6),	ACTION(),	ACTION(R6),
		ACTION(),	ACTION(),	ACTION(S6),	ACTION(S7),	ACTION(),	ACTION(S8),	ACTION(),
		ACTION(),	ACTION(),	ACTION(S6),	ACTION(S7),	ACTION(),	ACTION(S8),	ACTION(),
		ACTION(R9),	ACTION(R9),	ACTION(),	ACTION(),	ACTION(R9),	ACTION(),	ACTION(R9),
		ACTION(),	ACTION(),	ACTION(S6),	ACTION(S7),	ACTION(),	ACTION(S8),	ACTION(),
		ACTION(),	ACTION(),	ACTION(S6),	ACTION(S7),	ACTION(),	ACTION(S8),	ACTION(),
		ACTION(R7),	ACTION(R7),	ACTION(),	ACTION(),	ACTION(R7),	ACTION(),	ACTION(R7),
		ACTION(S9),	ACTION(),	ACTION(),	ACTION(),	ACTION(S15),ACTION(),	ACTION(),
		ACTION(R1),	ACTION(),	ACTION(),	ACTION(),	ACTION(R1),	ACTION(),	ACTION(R1),
		ACTION(R4),	ACTION(R4),	ACTION(),	ACTION(),	ACTION(R4),	ACTION(),	ACTION(R4),
		ACTION(R8),	ACTION(R8),	ACTION(),	ACTION(),	ACTION(R8),	ACTION(),	ACTION(R8)
};
const static GOTO gotoMap[16][5] = {
		GOTO(S1),GOTO(S2),GOTO(S3),GOTO(S4),GOTO(S5), //0
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //1
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //2
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //3
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //4
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //5
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(S11), //6
		GOTO(S12),GOTO(S2),GOTO(S3),GOTO(S4),GOTO(S5), //7
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //8
		GOTO(),GOTO(S13),GOTO(S3),GOTO(S4),GOTO(S5), //9
		GOTO(),GOTO(),GOTO(),GOTO(S14),GOTO(S5), //10
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //11
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //12
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //13
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO(), //14
		GOTO(),GOTO(),GOTO(),GOTO(),GOTO() //15
};
typedef ParserNode* (*FuncType)(std::stack<ParserNode*>&);

ParserNode* func1(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e2 = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* eor = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* el2 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::EL2);
	ret->left_child = el2;
	ret->mid_child = eor;
	ret->right_child = e2;
	return ret;
}
ParserNode* func2(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e2 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::EL2);
	ret->varIdx = 1;
	ret->left_child = e2;
	return ret;
}
ParserNode* func3(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e1 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::E2);
	ret->left_child = e1;
	return ret;
}
ParserNode* func4(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e2 = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* eor = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* el2 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::EL1);
	ret->left_child = el2;
	ret->mid_child = eor;
	ret->right_child = e2;
	return ret;
}
ParserNode* func5(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e1 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::EL1);
	ret->varIdx = 1;
	ret->left_child = e1;
	return ret;
}
ParserNode* func6(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e0 = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::E1);
	ret->left_child = e0;
	return ret;
}
ParserNode* func7(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* e0 = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* enot = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::E0);
	ret->left_child = enot;
	ret->mid_child = e0;
	return ret;
}
ParserNode* func8(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* ebr = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* e2 = s.top();
	s.pop();
	delete s.top();
	s.pop();
	ParserNode* ebl = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::E0);
	ret->varIdx = 1;
	ret->left_child = ebl;
	ret->mid_child = e2;
	ret->right_child = ebr;
	return ret;
}
ParserNode* func9(std::stack<ParserNode*>& s)
{
	delete s.top();
	s.pop();
	ParserNode* ev = s.top();
	s.pop();
	ParserNode* ret = new ParserNode(ParserNode::E0);
	ret->varIdx = 2;
	ret->left_child = ev;
	return ret;
}

const static FuncType equation[9] = {
	func1,func2,func3,func4,func5,func6,func7,func8,func9
};

ASTNode* accept(ParserNode* p);

ASTNode* parse(std::string str)
{
	BoolAnalyser analyser(str);
	BoolAnalyser::Node n = analyser.get();
	stack<ParserNode*> s;
	s.push(new ParserNode(S0));
	ParserNode* np = new ParserNode(n);
	while (1)
	{
		const ACTION& a = actionMap[s.top()->state][np->terminator];
		if (a.type == a.S)
		{
			s.push(np);
			s.push(new ParserNode(a.targetState));
		}
		else if (a.type == a.R)
		{
			ParserNode* A = equation[a.targetEquation](s);
			auto ss = s.top()->state;
			const GOTO& g = gotoMap[s.top()->state][A->nonterminator];
			s.push(A);
			if (g.exsit) s.push(new ParserNode(g.target));
			else
			{
				cout << "语法错误";
				exit(-1);
			}
			continue;
		}
		else if (a.type == a.ACC && s.size() == 3)
		{
			delete s.top();
			s.pop();
			ParserNode* target = s.top();
			s.pop();
			delete s.top();
			s.pop();
			return accept(target);
		}
		else
		{
			cout << "语法错误";
			exit(-1);
		}
		n = analyser.get();
		np = new ParserNode(n);
	}
}

std::string search(std::string str, std::map<std::string, StringSkipList*>* dataset);

ParserNode::ParserNode(STATE s)
{
	type = ISSTATE;
	state = s;
}

ParserNode::ParserNode(NONTERMINATOR n)
{
	type = ISNONTERMINATOR;
	nonterminator = n;
}

ParserNode::ParserNode(BoolAnalyser::Node t)
{
	type = ISTERMINATOR;
	terminator = t.terminator;
	if (t.terminator == BoolAnalyser::Node::VALUE)
		value = t.value;
}

ParserNode::ParserNode()
{
	type = ISTERMINATOR;
	terminator = BoolAnalyser::Node::END;
}

ACTION::ACTION()
{
	type = ACTIONTYPE::ERROR;
}

ACTION::ACTION(bool accept)
{
	type = ACTIONTYPE::ACC;
}

ACTION::ACTION(EQIDX eq)
{
	type = ACTIONTYPE::R;
	targetEquation = eq;
}

ACTION::ACTION(STATE st)
{
	type = ACTIONTYPE::S;
	targetState = st;
}

GOTO::GOTO()
{
	exsit = false;
}

GOTO::GOTO(STATE t)
{
	exsit = true;
	target = t;
}

static void releaseList(list<ResultLinkedList*>* r)
{
	if (r != nullptr)
	{
		for (auto& i : *r)
			delete i;
		delete r;
	}
}

class ASTOr;
class ASTNot;
class ASTValue;

class ASTAnd : public ASTNode {
private:
public:
	const static short TYPE;
	short getType() override
	{
		return ASTAnd::TYPE;
	}
	list<ASTOr*>* orChilds = nullptr;
	list<ASTValue*>* rawChilds = nullptr;
	ASTNot* notChild = nullptr;
	~ASTAnd();
	bool visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv);
};
const short ASTAnd::TYPE = 1;

class ASTNot : public ASTNode {
public:
	const static short TYPE;
	short getType() override
	{
		return ASTNot::TYPE;
	}
	ASTNode* child = nullptr;
	~ASTNot()
	{
		delete child;
	}
	bool visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv)
	{
		return child->visit(dataset, rv, sv);
	}
};
const short ASTNot::TYPE = 2;

class ASTOr : public ASTNode {
public:
	const static short TYPE;
	short getType() override
	{
		return ASTOr::TYPE;
	}
	list<ASTAnd*>* andChilds = nullptr;
	list<ASTValue*>* rawChilds = nullptr;
	~ASTOr();
	bool visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv);
};
const short ASTOr::TYPE = 3;

class ASTValue : public ASTNode {
public:
	const static short TYPE;
	short getType() override
	{
		return ASTValue::TYPE;
	}
	string value;
	ASTValue(string s);
	bool visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv);
};
const short ASTValue::TYPE = 4;

//调用约定： 会删除输入元素中不需要的，但不会设置为 null
template<typename T>
list<T>* MergeNullableList(list<T>* a, list<T>* b)
{
	if (b == nullptr) return a;
	if (a == nullptr) return b;
	else
	{
		set<T> c;
		for (auto& i : *a)c.insert(i);
		for (auto& i : *b)
		{
			if (c.contains(i) == false)
				a->push_back(i);
		}
		delete b;
		return a;
	}
}
template<typename T>
list<T*>* AppendToNullableList(list<T*>* v, T* target)
{
	if (v == nullptr)
		v = new list<T*>();
	v->push_back(target);
	return v;
}
ASTOr* MergeNullableOr(ASTOr* a, ASTOr* b)
{
	if (b == nullptr)return a;
	if (a == nullptr)return b;
	else
	{
		a->rawChilds = MergeNullableList(a->rawChilds, b->rawChilds);
		a->andChilds = MergeNullableList(a->andChilds, b->andChilds);
		b->rawChilds = nullptr;
		b->andChilds = nullptr;
		delete b;
		return a;
	}
}
ASTNot* MergeNullableNotChild(ASTNot* a, ASTNot* b)
{
	if (a == nullptr)return b;
	if (b == nullptr)return a;
	ASTOr* o = new ASTOr();
	if (a->child->getType() == ASTAnd::TYPE)
		o->andChilds = AppendToNullableList(o->andChilds, dynamic_cast<ASTAnd*>(a->child));
	else if (a->child->getType() == ASTValue::TYPE)
		o->rawChilds = AppendToNullableList(o->rawChilds, dynamic_cast<ASTValue*>(a->child));
	else o = MergeNullableOr(o, dynamic_cast<ASTOr*>(a->child));
	if (b->child->getType() == ASTAnd::TYPE)
		o->andChilds = AppendToNullableList(o->andChilds, dynamic_cast<ASTAnd*>(b->child));
	else if (b->child->getType() == ASTValue::TYPE)
		o->rawChilds = AppendToNullableList(o->rawChilds, dynamic_cast<ASTValue*>(b->child));
	else o = MergeNullableOr(o, dynamic_cast<ASTOr*>(b->child));
	b->child = nullptr;
	a->child = o;
	delete b;
	return a;

}
ASTAnd* MergeNullableAnd(ASTAnd* a, ASTAnd* b)
{
	if (b == nullptr)return a;
	if (a == nullptr)return b;
	a->orChilds = MergeNullableList(a->orChilds, b->orChilds);
	a->rawChilds = MergeNullableList(a->rawChilds, b->rawChilds);
	a->notChild = MergeNullableNotChild(a->notChild, b->notChild);
	b->orChilds = nullptr;
	b->rawChilds = nullptr;
	b->notChild = nullptr;
	delete b;
	return a;
}


ASTNode* buildNot(ASTNode* target)
{
	if (target->getType() == ASTNot::TYPE)
	{
		ASTNot* nt = dynamic_cast<ASTNot*>(target);
		ASTNode* ret = nt->child;
		nt->child = nullptr;
		return ret;
	}
	else
	{
		ASTNot* ret = new ASTNot();
		ret->child = target;
		return ret;
	}
}
ASTNode* buildOr(ASTNode* left, ASTNode* right);
ASTNode* buildAnd(ASTNode* left, ASTNode* right)
{
	ASTAnd* ret = new ASTAnd();
	if (left->getType() == ASTNot::TYPE && right->getType() == ASTNot::TYPE)
	{
		ASTNot* n1 = dynamic_cast<ASTNot*>(left);
		ASTNot* n2 = dynamic_cast<ASTNot*>(right);
		auto ret = buildOr(n1->child, n2->child);
		n1->child = ret;
		n2->child = nullptr;
		delete n2;
		return n1;
	}
	if (left->getType() == ASTAnd::TYPE)
	{
		ret = MergeNullableAnd(ret, dynamic_cast<ASTAnd*>(left));
	}
	else if (left->getType() == ASTValue::TYPE)
	{
		ret->rawChilds = AppendToNullableList(ret->rawChilds, dynamic_cast<ASTValue*>(left));
	}
	else if (left->getType() == ASTOr::TYPE)
	{
		ret->orChilds = AppendToNullableList(ret->orChilds, dynamic_cast<ASTOr*>(left));
	}
	else
	{
		ret->notChild = dynamic_cast<ASTNot*>(left);
	}
	if (right->getType() == ASTAnd::TYPE)
	{
		ret = MergeNullableAnd(ret, dynamic_cast<ASTAnd*>(right));
	}
	else if (right->getType() == ASTValue::TYPE)
	{
		ret->rawChilds = AppendToNullableList(ret->rawChilds, dynamic_cast<ASTValue*>(right));
	}
	else if (right->getType() == ASTOr::TYPE)
	{
		ret->orChilds = AppendToNullableList(ret->orChilds, dynamic_cast<ASTOr*>(right));
	}
	else
	{
		ret->notChild = MergeNullableNotChild(ret->notChild, dynamic_cast<ASTNot*>(right));
	}
	return ret;
}
ASTNode* buildOr(ASTNode* left, ASTNode* right)
{
	int t = ((left->getType() == ASTNot::TYPE) << 1) + (right->getType() == ASTNot::TYPE);
	switch (t)
	{
	case 0:
	{
		ASTOr* ret = new ASTOr();
		if (left->getType() == ASTOr::TYPE)
			ret = MergeNullableOr(ret, dynamic_cast<ASTOr*>(left));
		else if (left->getType() == ASTValue::TYPE)
			ret->rawChilds = AppendToNullableList(ret->rawChilds, dynamic_cast<ASTValue*>(left));
		else
			ret->andChilds = AppendToNullableList(ret->andChilds, dynamic_cast<ASTAnd*>(left));
		if (right->getType() == ASTOr::TYPE)
			ret = MergeNullableOr(ret, dynamic_cast<ASTOr*>(right));
		else if (right->getType() == ASTValue::TYPE)
			ret->rawChilds = AppendToNullableList(ret->rawChilds, dynamic_cast<ASTValue*>(right));
		else
			ret->andChilds = AppendToNullableList(ret->andChilds, dynamic_cast<ASTAnd*>(right));
		return ret;
	}
	case 1:
	{
		ASTNot* n = dynamic_cast<ASTNot*>(right);
		ASTNode* in = n->child;
		n->child = left;
		ASTNot* nr = new ASTNot();
		nr->child = buildAnd(n, in);
		return nr;
	}
	case 2:
	{
		ASTNot* n = dynamic_cast<ASTNot*>(left);
		ASTNode* in = n->child;
		n->child = right;
		ASTNot* nr = new ASTNot();
		nr->child = buildAnd(in, n);
		return nr;
	}
	case 3:
	{
		ASTNot* n1 = dynamic_cast<ASTNot*>(left);
		ASTNot* n2 = dynamic_cast<ASTNot*>(right);
		auto ret = buildAnd(n1->child, n2->child);
		n1->child = ret;
		n2->child = nullptr;
		delete n2;
		return n1;
	}
	default:
		cout << "语义分析器错误";
		exit(-1);
	}
}


/*

template<typename T>
void AppendToNullableList(list<ASTAnd*>*& v, T*& target)
{
	if (v == nullptr)
		v = new list<ASTAnd*>();
	v->push_back(dynamic_cast<ASTAnd*>(target));
	target = nullptr;
}
void AppendToNullableList(list<ASTOr*>*& v, ASTOr*& target)
{
	if (v == nullptr)
		v = new list<ASTOr*>();
	v->push_back(target);
	target = nullptr;
}
template<typename T>
void AppendToNullableList(list<ASTOr*>*& v, T*& target)
{
	if (v == nullptr)
		v = new list<ASTOr*>();
	v->push_back(dynamic_cast<ASTOr*>(target));
	target = nullptr;
}
void AppendToNullableList(list<string>*& v, string*& target)
{
	if (v == nullptr)
		v = new list<string>();
	for (auto& i : *v)
	{
		if (*target == i)
		{
			delete target;
			target = nullptr;
			return;
		}
	}
	v->push_back(*target);
	delete target;
	target = nullptr;
}
void AppendToNullableList(list<string>*& v, string target)
{
	if (v == nullptr)
		v = new list<string>();
	for (auto& i : *v)
	{
		if (target == i)
			return;
	}
	v->push_back(target);
}



ASTLogic* compressNot(ASTNot* a)
{
	if (a->child && a->child->getType() == ASTNot::TYPE)
	{
		ASTNot* buf = dynamic_cast<ASTNot*>(a->child);
		ASTLogic* ret = buf->child;
		buf->child = nullptr;
		delete a;
		return buf;
	}
	return a;
}


template<typename T>
void MergeNullableOr(ASTOr*& a, T*& b)
{
	ASTOr* bo = dynamic_cast<ASTOr*>(b);
	if (b == nullptr)return;
	if (a == nullptr)
	{
		a = bo;
		b = nullptr;
	}
	else
	{
		MergeNullableList(a->rawChilds, bo->rawChilds);
		MergeNullableList(a->andChilds, bo->andChilds);
		delete b;
		b = nullptr;
	}
}

template<typename T>
void MergeNullableAnd(ASTAnd*& a, T*& ba)
{
	ASTAnd* b = dynamic_cast<ASTAnd*>(ba);
	if (b == nullptr)return;
	if (a == nullptr)
	{
		a = b;
		b = nullptr;
	}
	else
	{
		MergeNullableList(a->orChilds, b->orChilds);
		MergeNullableList(a->rawChilds, b->rawChilds);
		if (b->notChild == nullptr)
		{
			delete b;
			b = nullptr;
			return;
		}
		if (a->notChild == nullptr)
		{
			a->notChild = b->notChild;
			b->notChild = nullptr;
			delete b;
			b = nullptr;
		}
		else
		{
			ASTNot* an = a->notChild;
			ASTNot* bn = b->notChild;
			ASTOr* o = new ASTOr();
			if (an->rawchild)
				AppendToNullableList(o->rawChilds, an->rawchild);
			else if (an->child->getType() == ASTAnd::TYPE)
				AppendToNullableList(o->andChilds, an->child);
			else
				MergeNullableOr(o, an->child);
			if (bn->rawchild)
				AppendToNullableList(o->rawChilds, bn->rawchild);
			else if (bn->child->getType() == ASTAnd::TYPE)
				AppendToNullableList(o->andChilds, bn->child);
			else
				MergeNullableOr(o, bn->child);
			an->child = o;
			delete b;
			b = nullptr;
		}
	}
}

void AppendChildToAnd(ASTAnd*& a, ASTLogic*& b)
{
	if (b->getType() == ASTAnd::TYPE)
	{
		MergeNullableAnd(a, b);
	}
	else if (b->getType() == ASTOr::TYPE)
	{
		AppendToNullableList(a->orChilds, b);
	}
	else
	{
		ASTNot* n = dynamic_cast<ASTNot*>(b);
		if (n->rawchild)
		{
			AppendToNullableList(a->rawChilds, n->rawchild);
			delete b;
			b = nullptr;
		}
		else
		{
			if (a->notChild == nullptr)
			{
				a->notChild = n;
				n = nullptr;
			}
			else
			{
				ASTNot* an = a->notChild;
				ASTOr* o = new ASTOr();
				if (an->rawchild)
					AppendToNullableList(o->rawChilds, an->rawchild);
				else if (an->child->getType() == ASTAnd::TYPE)
					AppendToNullableList(o->andChilds, an->child);
				else
					MergeNullableOr(o, an->child);
				if (n->rawchild)
					AppendToNullableList(o->rawChilds, n->rawchild);
				else if (n->child->getType() == ASTAnd::TYPE)
					AppendToNullableList(o->andChilds, n->child);
				else
					MergeNullableOr(o, n->child);
				an->child = o;
				delete b;
				b = nullptr;
			}
		}
	}
}
void AppendChildToAnd(ASTAnd*& a, ASTNot*& b)
{
	if (b->rawchild)
	{
		AppendToNullableList(a->rawChilds, b->rawchild);
		delete b;
		b = nullptr;
	}
	else
	{
		if (a->notChild == nullptr)
		{
			a->notChild = b;
			b = nullptr;
		}
		else
		{
			ASTNot* an = a->notChild;
			ASTOr* o = new ASTOr();
			if (an->rawchild)
				AppendToNullableList(o->rawChilds, an->rawchild);
			else if (an->child->getType() == ASTAnd::TYPE)
				AppendToNullableList(o->andChilds, an->child);
			else
				MergeNullableOr(o, an->child);
			if (b->rawchild)
				AppendToNullableList(o->rawChilds, b->rawchild);
			else if (b->child->getType() == ASTAnd::TYPE)
				AppendToNullableList(o->andChilds, b->child);
			else
				MergeNullableOr(o, b->child);
			an->child = o;
			delete b;
			b = nullptr;
		}
	}
}
*/
ASTNode* accept(ParserNode* p)
{
	switch (p->type)
	{
	case ParserNode::ISTERMINATOR:
		switch (p->terminator)
		{
		case	BoolAnalyser::Node::VALUE:
		{
			ASTNode* ret = new ASTValue(p->value);
			delete p;
			return ret;
		}
		default:
			cout << "语法分析器异常";
			exit(-1);
		}
		break;
	case ParserNode::ISNONTERMINATOR:
		switch (p->nonterminator)
		{
		case ParserNode::NONTERMINATOR::EL2:
		{
			ASTNode* ret = nullptr;
			if (p->varIdx == 0)
			{
				ret = buildOr(accept(p->left_child), accept(p->right_child));
				delete p->mid_child;
			}
			else
				ret = accept(p->left_child);
			delete p;
			return ret;
		}
		case ParserNode::NONTERMINATOR::E2:
		{
			ASTNode* ret = accept(p->left_child);
			delete p;
			return ret;
		}
		case ParserNode::NONTERMINATOR::EL1:
		{
			ASTNode* ret = nullptr;
			if (p->varIdx == 0)
			{
				ret = buildAnd(accept(p->left_child), accept(p->right_child));
				delete p->mid_child;
			}
			else
				ret = accept(p->left_child);
			delete p;
			return ret;
		}
		case ParserNode::NONTERMINATOR::E1:
		{
			ASTNode* ret = accept(p->left_child);
			delete p;
			return ret;
		}
		case ParserNode::NONTERMINATOR::E0:
		{
			ASTNode* ret = nullptr;
			if (p->varIdx == 0)
			{
				ret = buildNot(accept(p->mid_child));
				delete p->left_child;
			}
			else if (p->varIdx == 1)
			{
				ret = accept(p->mid_child);
				delete p->left_child;
				delete p->right_child;
			}
			else
				ret = accept(p->left_child);
			delete p;
			return ret;
		}
		default:
			cout << "语法分析器异常";
			exit(-1);
		}
		break;
	default:
		cout << "语法分析器异常";
		exit(-1);
	}
	cout << "语法分析器异常";
	exit(-1);
}

std::string search(std::string str, std::map<std::string, StringSkipList*>* dataset)
{
	auto node = parse(str);
	if (node->getType() == ASTNot::TYPE)
	{
		delete node;
		str += "\ninfinite";
		return str;
	}
	list<ResultLinkedList*>* r = nullptr;
	list<StringSkipList*>* s = nullptr;
	if (node->visit(dataset, r, s))
	{
		delete node;
		if (r != nullptr)
		{
			ResultLinkedList* rl = r->front();
			delete r;
			auto p = rl->getFirst();
			while (p)
			{
				str += '\n' + to_string(p->value);
				p = p->next;
			}
			delete rl;
			return str;
		}
		else
		{
			StringSkipList* sl = s->front();
			delete s;
			auto p = sl->getFirst();
			while (p != -1)
			{
				str += '\n' + to_string(sl->getValue(p));
				p = sl->getNext(p);
			}
			return str;
		}
	}
	else
	{
		delete node;
		return str;
	}
}


const short ASTNode::TYPE = 0;
short ASTNode::getType()
{
	return ASTNode::TYPE;
}

ASTAnd::~ASTAnd()
{
	if (orChilds)
	{
		for (auto& i : *orChilds)
		{
			delete i;
		}
		delete orChilds;
	}
	if (rawChilds)
	{
		for (auto& i : *rawChilds)
		{
			delete i;
		}
		delete rawChilds;
	}
	delete notChild;
}

bool ASTAnd::visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv)
{
	list<ResultLinkedList*>* r = nullptr;
	list<StringSkipList*>* s = nullptr;
	if (rawChilds != nullptr)
		for (auto& i : *rawChilds)
		{
			if (i->visit(dataset, r, s) == false)
			{
				releaseList(r);
				delete s;
				return false;
			}
		}
	if (orChilds != nullptr)
		for (auto& i : *orChilds)
		{
			if (i->visit(dataset, r, s) == false)
			{
				releaseList(r);
				delete s;
				return false;
			}
		}
	if (r == nullptr && s == nullptr)return false;
	if (notChild)
	{
		list<ResultLinkedList*>* rn = nullptr;
		list<StringSkipList*>* sn = nullptr;
		if (notChild->visit(dataset, rn, sn) == false)goto NONOTCHILD;
		ResultLinkedList* rs = nullptr;
		if (rn)
			rs = andNotOperation(s, r, rn->front());
		else
			rs = andNotOperation(s, r, sn->front());
		releaseList(rn);
		delete sn;
		if (rs == nullptr) return false;
		rv = AppendToNullableList(rv, rs);
		return true;
	}
NONOTCHILD:;
	if (r == nullptr && s != nullptr && s->size() == 1)
	{
		sv = AppendToNullableList(sv, s->front());
		delete s;
		return true;
	}
	else
	{
		ResultLinkedList* rs = andOperation(s, r);
		if (rs == nullptr) return false;
		rv = AppendToNullableList(rv, rs);
		return true;
	}
}

ASTOr::~ASTOr()
{
	if (andChilds)
	{
		for (auto& i : *andChilds)
		{
			delete i;
		}
		delete andChilds;
	}
	if (rawChilds)
	{
		for (auto& i : *rawChilds)
		{
			delete i;
		}
		delete rawChilds;
	}
}

bool ASTOr::visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv)
{
	list<ResultLinkedList*>* r = nullptr;
	list<StringSkipList*>* s = nullptr;
	if (rawChilds != nullptr)
		for (auto& i : *rawChilds)
		{
			i->visit(dataset, r, s);
		}
	if (andChilds != nullptr)
		for (auto& i : *andChilds)
		{
			i->visit(dataset, r, s);
		}
	if (r == nullptr)
	{
		if (s == nullptr)
		{
			return false;
		}
		else if (s->size() == 1)
		{
			sv = AppendToNullableList(sv, s->front());
			delete s;
			return true;
		}
	}
	ResultLinkedList* rs = orOperation(s, r);
	rv = AppendToNullableList(rv, rs);
	return true;
}

ASTValue::ASTValue(string s)
{
	value = s;
}

bool ASTValue::visit(map<string, StringSkipList*>* dataset, list<ResultLinkedList*>*& rv, list<StringSkipList*>*& sv)
{
	auto it = dataset->find(value);
	if (it == dataset->end()) return false;
	sv = AppendToNullableList(sv, (*it).second);
	return true;
}

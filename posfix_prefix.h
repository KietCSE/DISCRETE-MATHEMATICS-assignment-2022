#pragma once
#include<iostream>

#include<sstream>
#include<string>
#include<vector>
#include<math.h>
#include<stack>
using namespace std; 

struct Node {
	string data;
	Node* Left;
	Node* Right;
};

struct Logic {
	char data; 
	Logic* Left; 
	Logic* Right;
};

// test 
void SearchNLR(Node* tree);
void CheckTreeNLR(Node* tree);
void CheckTreeNLR(Logic* tree);
//

// stack;
template<class T>
class Stack {
private:
	T* head;  
public:
	Stack(); 
	~Stack(); 
	void push(); 
	void pop(); 
	T top(); 
};


//
void StandardizeString(string& s); // chuan hoa xau truoc khi tao expression tree
void StandardizeString4Value(string& s); // chuan hoa xau truoc khi tinh toan 
bool isOperator(char str);
bool isOperator(string str);
int PrecedenceOperator(string str);  // tinh toan do uu tien toan tu
Node* MakeNode(string s);
void CreatSubTreeRight(Node*& tree, stack<Node*>& operatorStack, stack<Node*>& nodeStack);
void CreatSubTreeLeft(Node*& tree, stack<Node*>& operatorStack, stack<Node*>& nodeStack);
void SeperateString(string s, vector<Node*>& token); // tach cac string tao node cho tree
void CreateTreeExpression(Node*& tree, string infix); // tao expression tree
void PreoderTreeTraversal(Node* tree, string& result); 
void PostoderTreeTraversal(Node* tree, string& result); 
string Infix2Prefix(string infix);
string Infix2Postfix(string infix);
int NumOfNegativeOperator(string s); // so luong dau am trong postfix 
void SolveNegativePostfix(string &postfix); // tim dau am va thay the bang dau #
void SolveNegativePrefix(string& prefix);
double Calculate(double num1, string Operator, double num2); 

string PostfixCalculator(string input);
string PrefixCalculator(string s);
string PostfixPrefixCalculator(string input);

// exercise 2
bool isLogicOperator(char str);
int PrecedenceOperator(char str);
Logic* MakeLogicNode(char str);
void StandardizeLogicExpression(string& s);
void CreatSubTreeRight(Logic*& tree, stack<Logic*>& operatorStack, stack<Logic*>& nodeStack);
void CreatSubTreeLeft(Logic*& tree, stack<Logic*>& operatorStack, stack<Logic*>& nodeStack);
void SolveSequenceLogicOperator(int n, Logic*& tree, stack<Logic*>& logicOperator, stack<Logic*>& logicNode);
void CreateLogicTree(Logic*& tree, string s);
void PreoderLogicTreeTraversal(Logic* tree, string& result);
void PostoderLogicTreeTraversal(Logic* tree, string& result);
string LogicInfix2Postfix(string infix);
string LogicInfix2Prefix(string infix);
void ReadInputLogic(vector<char>& node, vector<char>& value, string s);
void SetValueToString(const vector<char>& node, const vector<char>& value, string& s); // day gia tri 1 0 vao chuoi can tinh 
char CalculateLogic(char node1, char Operator, char node2);
char LogicPostfixCalculator(string input, string value);
char LogicPrefixCalculator(string input, string value);
string LogicPostfixPrefixCalculator(string input, string varlue);
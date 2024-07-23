#pragma once
#include"posfix_prefix.h"

// ====test tree =======
void SearchNLR(Node* tree) {
	if (tree != NULL) {
		cout << tree->data << ' ';
		SearchNLR(tree->Left);
		SearchNLR(tree->Right);
	}
}

void CheckTreeNLR(Node* tree) {
	if (tree != NULL) {
		cout << tree->data << ": "; 
		if (tree->Left == NULL) cout << "left = NULL "; 
		else {
			cout << "left: " << tree->Left->data << ' '; 
		}
		if (tree->Right == NULL) cout << "rigth = NULL ";
		else {
			cout << "right: " << tree->Right->data << ' ';
		}
		cout << endl;
		CheckTreeNLR(tree->Left);
		CheckTreeNLR(tree->Right);
	}
}

void CheckTreeNLR(Logic* tree) {
	if (tree != NULL) {
		cout << tree->data << ": ";
		if (tree->Left == NULL) cout << "left = NULL ";
		else {
			cout << "left: " << tree->Left->data << ' ';
		}
		if (tree->Right == NULL) cout << "rigth = NULL ";
		else {
			cout << "right: " << tree->Right->data << ' ';
		}
		cout << endl;
		CheckTreeNLR(tree->Left);
		CheckTreeNLR(tree->Right);
	}
}

//======

// EXERCISE 1

bool isOperator(char str) {
	if (str == '*' || str == '/' || str == '+' || str == '-' || str == '^') return true;
	else return false;
}

bool isOperator(string str) {
	if (str == "*" || str == "/" || str == "+" || str == "-" || str == "^") return true;
	else return false;
}


int PrecedenceOperator(string str) {
	if (str == "(") return 0;
	if (str == "+" || str == "-") return 1;
	if (str == "*" || str == "/") return 2;
	if (str == "^") return 3;
	if (str == "#") return 4;
}

void StandardizeString(string& s) {
	// xoa khoang trang trong xau
	while (s[0] == ' ') s.erase(s.begin()); 
	while (s[s.length() - 1] == ' ') s.pop_back();
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			s.erase(s.begin() + i);
			i--;
		}
	}
	// thay the dau am bang dau "#"
	for (int i = 0; i < s.length(); i++)
		if (s[i] == '-' && (i == 0 || s[i-1] == '(' || isOperator(s[i-1])))
			s[i] = '#';

	// xoa cap dau () 
	while (s.find("()") != UINT64_MAX) {	
		int pos = s.find("()"); 
		s.erase(s.begin() + pos, s.begin() + pos + 2);
	}
}

Node* MakeNode(string s) {
	Node* root = new Node; 
	root->data = s; 
	root->Left = NULL;
	root->Right = NULL;
	return root;
}

void SeperateString(string s, vector<Node*>& token) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(' || s[i] == ')' || isOperator(s[i]) || s[i] == '#') {
			Node* temp = MakeNode(string(1, s[i]));
			token.push_back(temp);
		}
		if (s[i] >= '0' && s[i] <= '9') {
			int begin = i;
			while (i + 1 < s.length() && s[i + 1] >= '0' && s[i + 1] <= '9') i++;
			int num = i - begin + 1;
			Node* temp = MakeNode(s.substr(begin, num));
			token.push_back(temp);
		}
	}
}

void CreatSubTreeRight(Node*& tree, stack<Node*> &operatorStack, stack<Node*> &nodeStack) {
	Node* Operator = operatorStack.top(); 
	operatorStack.pop();
	if (Operator->data == "#") {
		Operator->data = "-";
		Node* root1 = nodeStack.top(); nodeStack.pop();
		Operator->Right = root1; // node dau truyen ben phai
		Operator->Left = NULL; 
		tree = Operator; // tra ve node goc
	}
	else {
		Node* root1 = nodeStack.top(); nodeStack.pop();
		Node* root2 = nodeStack.top(); nodeStack.pop();
		Operator->Right = root1; // node dau truyen ben phai
		Operator->Left = root2;
		tree = Operator; // tra ve node goc 
	}
	nodeStack.push(Operator);
}

void CreatSubTreeLeft(Node*& tree, stack<Node*>& operatorStack, stack<Node*>& nodeStack) {
	Node* Operator = operatorStack.top();
	operatorStack.pop();
	if (Operator->data == "#") {
		Operator->data = "-";
		Node* root1 = nodeStack.top(); nodeStack.pop();
		Operator->Left = root1; // node dau truyen ben trai
		Operator->Right = NULL;
		tree = Operator; // tra ve node goc
	}
	else {
		Node* root1 = nodeStack.top(); nodeStack.pop();
		Node* root2 = nodeStack.top(); nodeStack.pop();
		Operator->Left = root1; // node dau truyen ben trai
		Operator->Right = root2;
		tree = Operator; // tra ve node goc 
	}
	nodeStack.push(Operator);
}

// convert infix to expression tree
void CreateTreeExpression(Node*& tree, string infix) {
	stack<Node*> operatorStack; 
	stack<Node*> nodeStack; 
	vector<Node*> token; 

	StandardizeString(infix); 
	SeperateString(infix, token); 

	for (int i = 0; i < token.size(); i++) {
		// neu la toan tu 
		if (isOperator(token[i]->data) || token[i]->data == "#") {
			while (!operatorStack.empty() && (PrecedenceOperator(token[i]->data) < PrecedenceOperator(operatorStack.top()->data))) {
				CreatSubTreeRight(tree, operatorStack, nodeStack); 
			}
			operatorStack.push(token[i]);
		}
		// neu la dau mo ngoac
		else if (token[i]->data == "(") operatorStack.push(token[i]);

		// neu la dau dong ngoac
		else if (token[i]->data == ")") {
			// xu li cac dau /*^
			if (operatorStack.top()->data != "+" && operatorStack.top()->data != "-") {
				int num = 0;
				stack<Node*> sub_operator;
				stack<Node*> sub_node;
				while (operatorStack.top()->data != "(" && operatorStack.top()->data != "+" && operatorStack.top()->data != "-") {
					if (operatorStack.top()->data != "#") num++; // dem so luong can lay ra cua nodestack
					sub_operator.push(operatorStack.top());
					operatorStack.pop();
				}
				for (int i = 1; i <= num + 1; i++) {
					sub_node.push(nodeStack.top());
					nodeStack.pop();
				}
				// tao lien ket node cho cay 
				while (!sub_operator.empty()) CreatSubTreeLeft(tree, sub_operator, sub_node);
				nodeStack.push(sub_node.top()); // tra lai vao nodestack
			}
			// xu ly dau + -
			if (operatorStack.top()->data != "(") {
				int num = 0; 
				stack<Node*> sub_operator; 
				stack<Node*> sub_node;
				while (operatorStack.top()->data != "(") {
					if (operatorStack.top()->data != "#") num++; // dem so luong can lay ra cua nodestack
					sub_operator.push(operatorStack.top()); 
					operatorStack.pop();
				}
				operatorStack.pop(); // xoa dau mo ngoac
				for (int i = 1; i <= num+1; i++) {
					sub_node.push(nodeStack.top()); 
					nodeStack.pop();
				}
				// tao lien ket node cho cay 
				while (!sub_operator.empty()) CreatSubTreeLeft(tree, sub_operator, sub_node);
				nodeStack.push(sub_node.top()); // tra lai vao nodestack
			}
			else operatorStack.pop(); // xoa dau mo ngoac
		}
		// neu la toan hang 
		else {
			nodeStack.push(token[i]);
		}
	}

	// ket thuc vong lam nhung stack con toan tu 
	if (!operatorStack.empty()) {
		// xu li cac dau /*^
		if (operatorStack.top()->data != "+" && operatorStack.top()->data != "-") {
			int num = 0;
			stack<Node*> sub_operator;
			stack<Node*> sub_node;
			while (!operatorStack.empty() && operatorStack.top()->data != "+" && operatorStack.top()->data != "-") {
				if (operatorStack.top()->data != "#") num++; // dem so luong can lay ra cua nodestack
				sub_operator.push(operatorStack.top());
				operatorStack.pop();
			}
			for (int i = 1; i <= num + 1; i++) {
				sub_node.push(nodeStack.top());
				nodeStack.pop();
			}
			// tao lien ket node cho cay 
			while (!sub_operator.empty()) CreatSubTreeLeft(tree, sub_operator, sub_node);
			nodeStack.push(sub_node.top()); // tra lai vao nodestack
		}
		// xu ly dau + - 
		if (!operatorStack.empty()) {
			int num = 0;
			stack<Node*> sub_operator;
			stack<Node*> sub_node;
			while (!operatorStack.empty()) {
				if (operatorStack.top()->data != "#") num++; // dem so luong can lay ra cua nodestack
				sub_operator.push(operatorStack.top());
				operatorStack.pop();
			}
			for (int i = 1; i <= num + 1; i++) {
				sub_node.push(nodeStack.top());
				nodeStack.pop();
			}
			// tao lien ket node cho cay 
			while (!sub_operator.empty()) CreatSubTreeLeft(tree, sub_operator, sub_node);
			nodeStack.push(sub_node.top()); // 
		}
	}
}

void PreoderTreeTraversal(Node* tree, string &result) {
	if (tree != NULL) {
		result = result + tree->data + " "; 
		PreoderTreeTraversal(tree->Left, result);
		PreoderTreeTraversal(tree->Right, result);
	}
}

void PostoderTreeTraversal(Node* tree, string& result) {
	if (tree != NULL) {
		PostoderTreeTraversal(tree->Left, result);
		PostoderTreeTraversal(tree->Right, result);
		result = result + tree->data + " ";
	}
}

string Infix2Prefix(string infix) {
	Node* tree = NULL; 
	string result; 
	CreateTreeExpression(tree, infix); 
	PreoderTreeTraversal(tree, result); 
	result.pop_back(); 
	return result;
}

string Infix2Postfix(string infix) {
	Node* tree = NULL; 
	string result; 
	CreateTreeExpression(tree, infix);
	PostoderTreeTraversal(tree, result); 
	result.pop_back(); 
	return result;
}

void StandardizeString4Value(string& s) {
	while (s[0] == ' ') s.erase(s.begin());
	while (s[s.length() - 1] == ' ') s.pop_back();
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ' && s[i+1] == ' ') {
			s.erase(s.begin() + i);
			i--;
		}
	}
}

double Calculate(double num1, string Operator, double num2) {
	if (Operator == "+") return num1 + num2;
	if (Operator == "-") return num1 - num2;
	if (Operator == "*") return num1 * num2;
	if (Operator == "/") return num1 / num2; 
	if (Operator == "^") {
		for (int i = 1; i < num2; i++) num1 = num1 * num1;
		return num1;
	}
}

int NumOfNegativeOperator(string s) {
	// string da duoc chuan hoa 
	s = s + " ";
	int begin = 0; 
	int end = 0;
	int numoperator = 0; 
	int numoperand = 0; 
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			end = i;
			string token = s.substr(begin, end - begin);
			begin = end + 1;
			if (!isOperator(token)) {
				numoperand++; 
			}
			else {
				numoperator++;
			}
		}
	}
	return numoperator - numoperand + 1; 
}

void SolveNegativePostfix(string &postfix) {
	// sau khi chuan hoa xau
	int numnegative = NumOfNegativeOperator(postfix);
	if (numnegative > 0) {
		int num = 0;
		for (int i = 0; i < postfix.length() - 1; i++) {
			if (postfix[i] == '-' && (postfix[i - 2] >= '0' && postfix[i - 2] <= '9')) {
				postfix[i] = '#'; // thay cho dau am
				num++;
				if (num == numnegative) break;
			}
		}
	}	
}

void SolveNegativePrefix(string& prefix) {
	int numnegative = NumOfNegativeOperator(prefix);
	if (numnegative > 0) {
		int num = 0;
		for (int i = 1; i < prefix.length(); i++) {
			if (prefix[i] == '-' && (prefix[i + 2] >= '0' && prefix[i + 2] <= '9')) {
				prefix[i] = '#'; // thay cho dau am
				num++;
				if (num == numnegative) break;
			}
		}
	}
}

string PostfixCalculator(string s) {
	StandardizeString4Value(s); 
	SolveNegativePostfix(s);
	s = s + " "; //  dung de chan khi tach cac token
	int begin = 0; 
	int end = 0; 
	stack<double> value; 

	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			end = i; 
			string token = s.substr(begin, end - begin); 
			begin = end + 1;
		
			if (token == "#") {
				double num1 = 0 - value.top(); value.pop(); // so am 
				value.push(num1);
			}
			else if (!isOperator(token)) {
				double num = stod(token);
				value.push(num);
			}
			else {
				double num2 = value.top();  value.pop(); 
				double num1 = value.top();  value.pop(); 
				double num = Calculate(num1, token, num2);
				value.push(num);
			}
		}
	}

	// lam tron
	double x = round(value.top() * pow(10, 4)) / pow(10, 4);
	string result = to_string(x);
	while (result[result.length() - 1] == '0' && result[result.length() - 1] != '.') result.pop_back();
	if (result[result.length() - 1] == '.') result.pop_back();

	return result; 
}

string PrefixCalculator(string s) {
	StandardizeString4Value(s);
	SolveNegativePrefix(s);
	
	s = " " + s; //  dung de chan khi tach cac token
	int end = s.length()+1;
	int begin = 0;
	stack<double> value;

	for (int i = s.length()-1; i >= 0; i--) {
		if (s[i] == ' ') {
			begin = i + 1;
			string token = s.substr(begin, end - begin);
			end = i;

			if (token == "#") {
				double num1 = 0 - value.top(); value.pop(); // so am 
				value.push(num1);
			}
			else if (!isOperator(token)) {
				double num = stod(token);
				value.push(num);
			}
			else {
				double num1 = value.top();  value.pop();
				double num2 = value.top();  value.pop();
				double num = Calculate(num1, token, num2);
				value.push(num);
			}
		}
	}

	// lam tron
	double x = round(value.top() * pow(10, 4)) / pow(10, 4);
	string result = to_string(x);
	while (result[result.length() - 1] == '0' && result[result.length() - 1] != '.') result.pop_back();
	if (result[result.length() - 1] == '.') result.pop_back();

	return result;
}

string PostfixPrefixCalculator(string input) {
	string result; 
	if (isOperator(input[0])) {
		result = PrefixCalculator(input);
	}
	else {
		result = PostfixCalculator(input);
	}
	return result;
}

// EXERCISE 2









bool isLogicOperator(char str) {
	if (str == '~' || str == '>' || str == '=' || str == '|' || str == '&') return true;
	else return false;
}

int PrecedenceOperator(char str) {
	if (str == '(') return 0;
	if (str == '=') return 1;
	if (str == '>') return 2;
	if (str == '|' || str == '&') return 3;
	if (str == '~') return 4;
}

Logic* MakeLogicNode(char str) {
	Logic* root = new Logic; 
	root->data = str; 
	root->Left = NULL; 
	root->Right = NULL; 
	return root;
}

void StandardizeLogicExpression(string& s) {
	// xoa khoang trang 
	while (s[0] == ' ') s.erase(s.begin());
	while (s[s.length() - 1] == ' ') s.pop_back();
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			s.erase(s.begin() + i);
			i--;
		}
	}

	// xoa cap dau () 
	while (s.find("()") != UINT64_MAX) {
		int pos = s.find("()");
		s.erase(s.begin() + pos, s.begin() + pos + 2);
	}

	// thay dau <-> bang dau =
	while (s.find("<->") != UINT64_MAX) {
		int pos = s.find("<->");
		s.erase(s.begin() + pos, s.begin() + pos + 3);
		s.insert(s.begin() + pos, '=');
	}

	// thay dau -> bang dau >
	while (s.find("->") != UINT64_MAX) {
		int pos = s.find("->"); 
		s.erase(s.begin() + pos, s.begin() + pos + 2); 
		s.insert(s.begin() + pos, '>');
	}
}

void CreatSubTreeRight(Logic*& tree, stack<Logic*>& operatorStack, stack<Logic*>& nodeStack) {
	Logic* Operator = operatorStack.top();
	operatorStack.pop();
	if (Operator->data == '~') {
		Logic* root1 = nodeStack.top(); nodeStack.pop();
		Operator->Right = root1; // node dau truyen ben phai
		Operator->Left = NULL;
		tree = Operator; // tra ve node goc
	}
	else {
		Logic* root1 = nodeStack.top(); nodeStack.pop();
		Logic* root2 = nodeStack.top(); nodeStack.pop();
		Operator->Right = root1; // node dau truyen ben phai
		Operator->Left = root2;
		tree = Operator; // tra ve node goc 
	}
	nodeStack.push(Operator);
}

void CreatSubTreeLeft(Logic*& tree, stack<Logic*>& operatorStack, stack<Logic*>& nodeStack) {
	Logic* Operator = operatorStack.top();
	operatorStack.pop();
	if (Operator->data == '~') {
		Logic* root1 = nodeStack.top(); nodeStack.pop();
		Operator->Left = root1; // node dau truyen ben trai
		Operator->Right = NULL;
		tree = Operator; // tra ve node goc
	}
	else {
		Logic* root1 = nodeStack.top(); nodeStack.pop();
		Logic* root2 = nodeStack.top(); nodeStack.pop();
		Operator->Left = root1; // node dau truyen ben trai
		Operator->Right = root2;
		tree = Operator; // tra ve node goc 
	}
	nodeStack.push(Operator);
}

void SolveSequenceLogicOperator(int n, Logic* &tree, stack<Logic*> &logicOperator, stack<Logic*> &logicNode) {
	// lay cac toan tu can tinh 
	if (PrecedenceOperator(logicOperator.top()->data) == n) {
		int num = 0;
		stack<Logic*> sublogic;
		stack<Logic*> subnode;
		while (!logicOperator.empty() && PrecedenceOperator(logicOperator.top()->data) == n) {
			sublogic.push(logicOperator.top());
			logicOperator.pop();
			num++;
		}
		if (sublogic.top()->data == '~') num = 0;  // num+1 = 1 nen chi lay ra 1 node duy nhat 
		// lay cac node can tinh
		for (int i = 1; i <= num + 1; i++) {
			subnode.push(logicNode.top());
			logicNode.pop();
		}
		// tao cay 
		while (!sublogic.empty()) CreatSubTreeLeft(tree, sublogic, subnode);
		logicNode.push(subnode.top());	
	}
}

void CreateLogicTree(Logic*& tree, string s) {
	stack<Logic*> logicOperator; 
	stack<Logic*> logicNode;
	
	StandardizeLogicExpression(s); 

	for (int i = 0; i < s.length(); i++) {
		// neu la toan tu 
		if (isLogicOperator(s[i])) {
			while (!logicOperator.empty() && (PrecedenceOperator(s[i]) < PrecedenceOperator(logicOperator.top()->data))) {
				CreatSubTreeRight(tree, logicOperator, logicNode);
			}
			// day vao trong stack
			Logic* root = MakeLogicNode(s[i]);
			logicOperator.push(root);
		}
		else if (s[i] == '(') {
			Logic* root = MakeLogicNode(s[i]); 
			logicOperator.push(root);
		}
		else if (s[i] == ')') {
			int i = 4; 
			while (logicOperator.top()->data != '(' && i >= 0) {
				SolveSequenceLogicOperator(i, tree, logicOperator, logicNode);
				i--;
			}
			logicOperator.pop(); // xoa dau mo ngoac 
		}
		else {
			Logic* root = MakeLogicNode(s[i]); 
			logicNode.push(root);
		}
	}
	// ket thuc vong lap nhung stack con phan tu 
	if (!logicOperator.empty()) {
		int i = 4;
		while (!logicOperator.empty() && i >= 0) {
			SolveSequenceLogicOperator(i, tree, logicOperator, logicNode);
			i--;
		}
	}
}

void PreoderLogicTreeTraversal(Logic* tree, string& result) {
	if (tree != NULL) {
		if (tree->data == '>') result = result + "->";
		else if (tree->data == '=') result = result + "<->";
		else result = result + tree->data;
		PreoderLogicTreeTraversal(tree->Left, result);
		PreoderLogicTreeTraversal(tree->Right, result);
	}
}

void PostoderLogicTreeTraversal(Logic* tree, string& result) {
	if (tree != NULL) {
		PostoderLogicTreeTraversal(tree->Left, result);
		PostoderLogicTreeTraversal(tree->Right, result);
		if (tree->data == '>') result = result + "->";
		else if (tree->data == '=') result = result + "<->";
		else result = result + tree->data;
	}
}

string LogicInfix2Postfix(string infix) {
	Logic* tree = NULL; 
	string result;
	CreateLogicTree(tree, infix); 
	PostoderLogicTreeTraversal(tree, result);
	return result;
}

string LogicInfix2Prefix(string infix) {
	Logic* tree = NULL;
	string result;
	CreateLogicTree(tree, infix);
	PreoderLogicTreeTraversal(tree, result);
	return result;
}

void ReadInputLogic(vector<char> &node, vector<char> &value, string s) {
	StandardizeString4Value(s);
	s = s + " "; //  dung de chan khi tach cac token
	for (int i = 1; i < s.length(); i++) {
		if (s[i] == ' ') {
			if (s[i - 1] == '0' || s[i - 1] == '1') {
				value.push_back(s[i-1]);
			}
			else {
				node.push_back(s[i-1]);
			}
		}
	}
}

void SetValueToString(const vector<char>& node, const vector<char>& value, string &s) {
	// xau sau khi da duoc chuan hoa
	for (int i = 0; i < node.size(); i++) {
		for (int j = 0; j < s.length(); j++)
			if (s[j] == node[i]) s[j] = value[i];
	}
}

char CalculateLogic(char node1, char Operator, char node2) {
	if (node2 == '\0' && Operator == '~') {
		if (node1 == '1') return '0'; else return '1';
	}
	else if (Operator == '|') {
		if (node1 == '1' || node2 == '1') return '1';
		else return '0';
	}
	else if (Operator == '&') {
		if (node1 == '0' || node2 == '0') return '0';
		else return '1';
	}
	else if (Operator == '>') {
		if (node2 == '0') return '0';
		else return '1';
	}
	else if (Operator == '=') {
		if (node1 != node2) return '1';
		else return '0';
	}
}

char LogicPostfixCalculator(string input, string value) {
	vector<char> Node;
	vector<char> ValueLogic; 
	StandardizeString4Value(value); 
	ReadInputLogic(Node, ValueLogic, value);
	SetValueToString(Node, ValueLogic, input);

	stack<char> logic; 

	for (int i = 0; i < input.length(); i++) {
		if (!isLogicOperator(input[i])) {
			logic.push(input[i]);
		}
		else if (input[i] == '~') {
			char node1 = logic.top(); logic.pop(); 
			logic.push(CalculateLogic(node1, input[i], '\0'));
		}
		else {
			char node2 = logic.top(); logic.pop(); 
			char node1 = logic.top(); logic.pop(); 
			logic.push(CalculateLogic(node1, input[i], node2));
		}
	}
	
	return logic.top();
}

char LogicPrefixCalculator(string input, string value) {
	vector<char> Node;
	vector<char> ValueLogic;
	StandardizeString4Value(value);
	ReadInputLogic(Node, ValueLogic, value);
	SetValueToString(Node, ValueLogic, input);

	stack<char> logic;

	for (int i = input.length()-1; i>=0; i--) {
		if (!isLogicOperator(input[i])) {
			logic.push(input[i]);
		}
		else if (input[i] == '~') {
			char node1 = logic.top(); logic.pop();
			logic.push(CalculateLogic(node1, input[i], '\0'));
		}
		else {
			char node1 = logic.top(); logic.pop();
			char node2 = logic.top(); logic.pop();
			logic.push(CalculateLogic(node1, input[i], node2));
		}
	}

	return logic.top();
}

string LogicPostfixPrefixCalculator(string input, string value) {
	StandardizeLogicExpression(input);
	string result; 
	if (isLogicOperator(input[0])) {
		if (LogicPrefixCalculator(input, value) == '1') return "TRUE";
		else return "FALSE";
	}
	else {
		if (LogicPostfixCalculator(input, value) == '1') return "TRUE";
		else return "FALSE";
	}
	return result;
}



















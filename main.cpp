#include"posfix_prefix.h"

int main() {

	/*string s, str; 
	getline(cin, s); 
	getline(cin, str);
	cout << LogicPostfixPrefixCalculator(s, str);*/


	while (1) {
		system("cls");
		cout << "\n\n\t\t =============== MENU ============== \n\n"; 
		cout << "\t\t1. Infix to postfix \n"; 
		cout << "\t\t2. Infix to prefix\n"; 
		cout << "\t\t3. Prefix and Postfix Calculator \n";
		cout << "\n\t\t =============== END ============== \n\n";

		cout << "\tYour selection: ";
		int selection; cin >> selection; 
		if (selection == 1) {
			string s; 
			cout << "\n\tYour expressoin: ";
			cin.ignore(); 
			getline(cin, s); 
			cout << Infix2Postfix(s); 
			system("pause");

		}
		if (selection == 2) {
			string s;
			cout << "\n\tYour expressoin: ";
			cin.ignore();
			getline(cin, s);
			cout << Infix2Prefix(s);
			system("pause");

		}
		if (selection == 3) {
			string s;
			cout << "\n\tYour expressoin: ";
			cin.ignore();
			getline(cin, s);
			cout << PostfixPrefixCalculator(s);
			system("pause");
		}
	}
	return 0; 
}
// webLauncher1.cpp
//

#include "pch.h"
#include <windows.h>
#include <iostream>
#include <string>
//#include <bits/stdc++.h> 

using namespace std;

string url;
int a; 
int b = 0;

int main() {
	string baseUrl = "http://www.spanishdict.com/translate/";
	string userAdd = "help";

	while (userAdd != "q") {
	cout << "\nInput a word or q to quit\n";
	getline(cin, userAdd);
	if (userAdd == "q")break;
	ShellExecuteA(NULL, "open", (baseUrl + userAdd).c_str(), NULL, NULL, SW_SHOWNORMAL);
}
}
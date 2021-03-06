// findChars.cpp Its job is just to print out the ASCII code that goes with simbols that I come across. 
//


#include "pch.h"
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <fstream> 
#include <map> 
#include <vector> 
#include <iterator> 
#include<string>

using namespace std;


int main() {
	string str, inFileName, outFileName;
	unsigned int countWords = 0;
	inFileName = "test.txt";
	outFileName = "test_out.txt";

	cout << "\n Attempting to open " << inFileName << endl;
	ifstream in(inFileName);

	map<string, int> m;
	string stringLast;
	bool isHyphen = false;
	if (in.is_open()) {
		while (in >> str) {
			string strCopy = str;
			if (isHyphen) {
				isHyphen = false;
				str = stringLast + str;
				strCopy = str;
			}
//Fix the case of a hyphen separating words across lines. The problem is that you need to combine
//this string with the next (which you don't even have a this points). 
//Assume that this hyphen (45) only occurs at the end of the word. So, start by checking
//to see if this word has a hypen at the end. 
			if (str[str.length() - 1] == 45) {
				stringLast = str;
				isHyphen = true;
				continue;
			}
			int j = 0;
				cout << "\n";
			for (int i = 0; i < str.length(); i++) {
				cout << "\n" << str[i] << " " << int(str[i]) ;
				//Make every capital letter lower case. 
				if (str[i] >= 65 && str[i] <= 90) {
					strCopy[i - j] += 32;
					str[i] += 32;
				}

// Remove normal punctuation plus upside down ? = -65, upside down ! = -95, >> -69 and << -85
				//also long dash -105 ...
				if ((str[i] >= 0 && str[i] < 97) || (str[i] > 122) ||
					str[i] == -65 || str[i] == -95 || str[i] == -69 || str[i] == -85 ||
					str[i] == -105 || str[i] == -111 || str[i] == -67 || str[i] == -68) {
					//					cout << "\nRemoving i-j with i = " << i<< " and j = " << j << endl;
					strCopy.erase(i - j, 1);
					++j;
				}
			}
			if (strCopy.length() > 0) {
				m[strCopy]++;
				++countWords;
			}
		}
	}
	else {
		cerr << "\nError reading " << inFileName << endl;
		exit(1);
	}
	cout << "\nFinished reading file and storing in map. Opening outfile " << outFileName << endl;

	unsigned int countUniqueWords = 0;
	unsigned int countNoRepeaters = 0;
	unsigned int count2_5Repeaters = 0;
	unsigned int count6_15Repeaters = 0;
	unsigned int count16_50Repeaters = 0;
	unsigned int count51_200Repeaters = 0;
	unsigned int count201_AndBeyoundRepeaters = 0;
	ofstream out(outFileName);
	if (out.is_open()) {
		out << "\nTotal number of words " << countWords << endl;
		//Do a quick count of unique words so that you can include it in the header.
		for (map<string, int>::iterator it = m.begin(); it != m.end(); it++) {
			++countUniqueWords;
			if ((*it).second == 1)countNoRepeaters++;
			if ((*it).second > 1 && (*it).second <= 5)count2_5Repeaters++;
			if ((*it).second > 5 && (*it).second <= 15)count6_15Repeaters++;
			if ((*it).second > 15 && (*it).second <= 50)count16_50Repeaters++;
			if ((*it).second > 51 && (*it).second <= 200)count51_200Repeaters++;
			if ((*it).second > 200)count201_AndBeyoundRepeaters++;

		}
		out << "\nNumber of unique words " << countUniqueWords << endl;
		out << "\nNumber of words that only occur once " << countNoRepeaters << endl;
		out << "\nNumber of words that only occur between 2 and 5 times " << count2_5Repeaters << endl;
		out << "\nNumber of words that only occur between 6 and 15 times " << count6_15Repeaters << endl;
		out << "\nNumber of words that only occur between 16 and 50 times " << count16_50Repeaters << endl;
		out << "\nNumber of words that only occur between 51 and 200 times " << count51_200Repeaters << endl;
		out << "\nNumber of words that only occur more than 200 times " << count201_AndBeyoundRepeaters << endl;

		for (map<string, int>::iterator it = m.begin(); it != m.end(); it++) {
			out << (*it).first << "\t" << (*it).second << endl;
		}
	}
	else {
		cerr << "\nError opening output file: " << outFileName << endl;
	}

}
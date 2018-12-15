// SentenceFinder.cpp - The idea here is to go through a text file and find all the individual sentences. The hope
// is that the sentences of a translated text will match up. 
// I'll start with the assumption/hope that all sentences end with a . ? or ! and that each is directly attached to a word. 

#include "pch.h"
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <fstream> 
#include <vector> 
#include <iterator> 
#include<string>

using namespace std;

int main() {
	string str, inFileName, outFileName,root;
	unsigned int countSentencesSp = 0;
	unsigned int countSentencesEn = 0;
	
	root = "../media/principito/principito";
	inFileName = root + ".txt";
	outFileName = root + "Sent.txt";

	cout << "\n Attempting to open " << inFileName << endl;
	vector<string> mSp;	//Put the sentences in a hash table and write them out when you are done.
	ifstream inSp(inFileName);

	string sentence ="";
	string stringLast;

	bool isHyphen = false;
	if (inSp.is_open()) {
		while (inSp >> str) {

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
				continue;	//Go back to the top of the loop.
			}
			sentence += (str+" ");

			if (str[str.length()-1] == '.' || str[str.length()-1] == '!' || str[str.length()-1] == '?') {
				mSp.push_back(sentence);
				countSentencesSp++;
//				cout << "\nFound sentence = " << sentence << endl;
				sentence ="";
			}
		}
	}
	else {
		cerr << "\nError reading " << inFileName << endl;
		exit(1);
	}
	inSp.close();

	//Repeat for the english version
	root = "../media/principito/theLittlePrince";
	inFileName = root + ".txt";
	outFileName = root + "Sent.txt";
	vector<string> mEn;	//Put the sentences in a string vector and write them out when you are done.
	ifstream inEn(inFileName);


	if (inEn.is_open()) {
		while (inEn >> str) {

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
			sentence += (str + " ");

			if (str[str.length() - 1] == '.' || str[str.length() - 1] == '!' || str[str.length() - 1] == '?') {
				mEn.push_back(sentence);
				countSentencesEn++;
				//				cout << "\nFound sentence = " << sentence << endl;
				sentence = "";
			}
		}
	}
	else {
		cerr << "\nError reading " << inFileName << endl;
		exit(1);
	}
	inEn.close();

	cout << "\n Found " << countSentencesSp << " sentences in Spanish and " <<countSentencesEn<<" in English"<< endl;

	ofstream out(outFileName);
	size_t tempSize = mSp.size();
	bool spanishSmaller = true;
	if (mEn.size() <= mSp.size()) {
		tempSize = mEn.size();
		spanishSmaller = false;
	}
	if (out.is_open()) {

		for (int iCnt = 0; iCnt < tempSize; iCnt++) {
			out << mSp[iCnt] << endl;
			out << mEn[iCnt] << endl;
			out << "\n";
		}

// So, you have some number of either spanish or english sentences left over. Go ahead and print them at the end.

		if (spanishSmaller) {
			out << "\n\nExtra english sentences\n" << endl;
			for (size_t iCnt = mSp.size(); iCnt < mEn.size(); iCnt++) {
				out << mEn[iCnt] << endl;
				out << "\n";
			}
		}
		else
		{
			out << "\n\nExtra spanish sentences\n" << endl;
			for (size_t iCnt = mEn.size(); iCnt < mSp.size(); iCnt++) {
				out << mSp[iCnt] << endl;
				out << "\n";
			}
		}
	}
	else
	{
		cerr << "\nError opening output file: " << outFileName << endl;
	}
	out.close();
}
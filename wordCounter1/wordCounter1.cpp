//wordCount.cpp - Basic program for reading words from a text file and comparing against other word 
//files.
//Read a text file.
//Strip all the punctuation (while leaving accents etc.)
//Write the results in two formats. 1) alphabetic listing along with count for each word  (and header).
//									2) just alphabetic listing. 
//Read a file of words to compare and reduce the original file. 
//Write the reduced file in format 1).
//Note: It appears that when I run this on a text file that I've obtained from the web or from converting
//a pdf file, that the special letters (accents, etc.) do not write out properly. However, if I copy 
//the contents into a notepad file from scratch, I do not see the problem. 

#include "pch.h"
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <fstream> 
#include <map> 
#include <vector> 
#include <iterator> 
#include <set>
#include <functional>
#include<string>

using namespace std;
	
int main() {
	string str, inFileName,  outFileNameWHeader,outFileNameNoHeader,root;
	string myWordsFileName, myPtr, resultsOutFile;
	unsigned int countWords = 0;

//	root = "LaLenguaDeLasMariposasCopied";
	root = "principito";
	inFileName = root+".txt";
	outFileNameWHeader = root+"WHeader.txt";
	outFileNameNoHeader = root+"NoHeader.txt";
	myWordsFileName = "myWords.txt";
	
//	myWordsFileName = "DonQCopiedNoHeader.txt";
//	myWordsFileName = "LaLenguaDeLasMariposasCopied.txt";

	resultsOutFile = root+"Reduced.txt";
	
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

			for (int i = 0; i < str.length(); ++i) {

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
					strCopy.erase(i - j, 1);
					++j;
				}
			}
			if (strCopy.length() > 0) {
				++m[strCopy];
				++countWords;
			}
		}
	}
	else {
		cerr << "\nError reading " << inFileName << endl;
		exit(1);
	}
	cout << "\nFinished reading file and storing in map. Opening outfile for With Heading information "
		<<  outFileNameWHeader << endl;

//Make a set that is sorted by the value of the map. The reason for a set is that set can have a
//pair. It can be ordered by the value and not the key. Remember, the key is unique, i.e., if you 
//try to do it with a map (making the key = the value of the original map), the key, which would be
//the frequency, is not a set of unique values. 


	// Declaring the type of Predicate that accepts 2 pairs and return a bool
	typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;

	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
	Comparator compFunctor =
		[](std::pair<std::string, int> elem1, std::pair<std::string, int> elem2)
	{
		return elem1.second >= elem2.second;
	};

	// Declaring a set that will store the pairs using above comparision logic
	std::set<std::pair<std::string, int>, Comparator> setOfWords(
		m.begin(), m.end(), compFunctor);

	unsigned int countUniqueWords = 0;
	unsigned int countNoRepeaters = 0;
	unsigned int count2_5Repeaters = 0;
	unsigned int count6_15Repeaters = 0;
	unsigned int count16_50Repeaters = 0;
	unsigned int count51_200Repeaters = 0;
	unsigned int count201_AndBeyondRepeaters = 0;
	ofstream out( outFileNameWHeader);
	if (out.is_open()) {
		out << "\nTotal number of words " << countWords << endl;
		//Do a quick count of unique words so that you can include it in the header.

		for (std::pair<std::string, int> element : setOfWords){
			 ++countUniqueWords;
			 if (element.second == 1)countNoRepeaters++;
			 if (element.second > 1 && element.second <= 5)count2_5Repeaters++;
			 if (element.second > 5 && element.second <= 15)count6_15Repeaters++;
			 if (element.second > 15 && element.second <= 50)count16_50Repeaters++;
			 if (element.second > 51 && element.second <= 200)count51_200Repeaters++;
			 if (element.second > 200)count201_AndBeyondRepeaters++;
		}
		out << "\nNumber of unique words " << countUniqueWords;
		out << "\nNumber of words that only occur once " << countNoRepeaters;
		out << "\nNumber of words that only occur between 2 and 5 times " << count2_5Repeaters;
		out << "\nNumber of words that only occur between 6 and 15 times " << count6_15Repeaters;
		out << "\nNumber of words that only occur between 16 and 50 times " << count16_50Repeaters;
		out << "\nNumber of words that only occur between 51 and 200 times " << count51_200Repeaters;
		out << "\nNumber of words that only occur more than 200 times " << count201_AndBeyondRepeaters;
		out << "\n\n";
//		for (map<string, int>::iterator it = m.begin(); it != m.end(); it++) {
		for (std::pair<std::string, int> element : setOfWords) {

			out << element.first << "\t" << element.second << endl;
		}
	}
	else {
		cerr << "\nError opening output file: " <<  outFileNameWHeader << endl;
	}
	out.close();

	ofstream outNoHeader(outFileNameNoHeader);
	if (outNoHeader.is_open()) {
		for (map<string, int>::iterator it = m.begin(); it != m.end(); it++) {
			outNoHeader << (*it).first << endl;
		}
	}
	else
	{
		cerr << "\nError opening output file: " << outFileNameNoHeader << endl;
	}
	outNoHeader.close();

//So, at this point you have a map "m" that knows how many words there are for each word. 
//Read in your words and remove them from the map. Then write out all the numbers again.
//It would be nice to also know how many of your words were in it. 

	cout << "\n Attempting to open " << myWordsFileName << endl;
	ifstream inMyWords(myWordsFileName);

	if (inMyWords.is_open()) {
		while (inMyWords >> myPtr) {
			m.erase(myPtr);
		}
	}
	//So, let's see what you have.

	ofstream myOut(resultsOutFile);
	if (myOut.is_open()) {

		countUniqueWords = 0;
		countNoRepeaters = 0;
		count2_5Repeaters = 0;
		count6_15Repeaters = 0;
		count16_50Repeaters = 0;
		count51_200Repeaters = 0;
		count201_AndBeyondRepeaters = 0;
		countWords = 0;
//Count the total remaining words

		for (map<string, int>::iterator it = m.begin(); it != m.end(); ++it) {
			countWords += (*it).second;
			++countUniqueWords;
			if ((*it).second == 1)countNoRepeaters++;
			if ((*it).second > 1 && (*it).second <= 5)count2_5Repeaters++;
			if ((*it).second > 5 && (*it).second <= 15)count6_15Repeaters++;
			if ((*it).second > 15 && (*it).second <= 50)count16_50Repeaters++;
			if ((*it).second > 51 && (*it).second <= 200)count51_200Repeaters++;
			if ((*it).second > 200)count201_AndBeyondRepeaters++;
		}
		myOut << "\nInput files " << inFileName <<" and "<< myWordsFileName<< endl;
		myOut << "\nTotal number of words " << countWords;
		myOut << "\nNumber of unique words " << countUniqueWords;
		myOut << "\nNumber of words that only occur once " << countNoRepeaters;
		myOut << "\nNumber of words that only occur between 2 and 5 times " << count2_5Repeaters;
		myOut << "\nNumber of words that only occur between 6 and 15 times " << count6_15Repeaters;
		myOut << "\nNumber of words that only occur between 16 and 50 times " << count16_50Repeaters;
		myOut << "\nNumber of words that only occur between 51 and 200 times " << count51_200Repeaters;
		myOut << "\nNumber of words that only occur more than 200 times " << count201_AndBeyondRepeaters;
		myOut<<"\n\n";

//Again, let's get these in descending order of value.
// Declaring a set that will store the pairs using above comparision logic
		std::set<std::pair<std::string, int>, Comparator> newSetOfWords(
			m.begin(), m.end(), compFunctor);

		for (std::pair<std::string, int> element : newSetOfWords) {
			myOut << element.first << "\t" << element.second << endl;

//		for (map<string, int>::iterator it = m.begin(); it != m.end(); ++it) {
//			myOut << (*it).first << "\t" << (*it).second << endl;
		}
	}
	else{
			cerr << "\nError opening output file: " << resultsOutFile << endl;

	}
}
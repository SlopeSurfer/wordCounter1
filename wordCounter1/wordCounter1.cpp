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
//the contents into a notepad file from scratch, I do not see the problem. My best geuss is that it has to do with the
//encoding (UTF-8 ?).

#include "pch.h"
#include <iostream> 
#include <algorithm> 
#include <fstream> 
#include <map> 
#include <vector> 
#include <iterator> 
#include<string>

using namespace std;
	
class headerStats {

	unsigned int countUniqueWords;
	unsigned int countNoRepeaters;
	unsigned int count2_5Repeaters;
	unsigned int count6_15Repeaters;
	unsigned int count16_50Repeaters;
	unsigned int count51_200Repeaters;
	unsigned int count201_AndBeyondRepeaters;
public:
	headerStats() {
		initialize();
	}
	void initialize() {
		countUniqueWords = 0;
		countNoRepeaters = 0;
		count2_5Repeaters = 0;
		count6_15Repeaters = 0;
		count16_50Repeaters = 0;
		count51_200Repeaters = 0;
		count201_AndBeyondRepeaters = 0;
	}
	void incrementCounterBins(int inNum) {
		if (inNum == 1)countNoRepeaters++;
		if (inNum > 1 && inNum <= 5)count2_5Repeaters++;
		if (inNum > 5 && inNum <= 15)count6_15Repeaters++;
		if (inNum > 15 && inNum <= 50)count16_50Repeaters++;
		if (inNum > 51 && inNum <= 200)count51_200Repeaters++;
		if (inNum > 200)count201_AndBeyondRepeaters++;
	}
	void incrementUniqueWords() {
		++countUniqueWords;
	}
	void printHeader(ostream& out){
		out << "\nNumber of unique words " << countUniqueWords;
		out << "\nNumber of words that only occur once " << countNoRepeaters;
		out << "\nNumber of words that only occur between 2 and 5 times " << count2_5Repeaters;
		out << "\nNumber of words that only occur between 6 and 15 times " << count6_15Repeaters;
		out << "\nNumber of words that only occur between 16 and 50 times " << count16_50Repeaters;
		out << "\nNumber of words that only occur between 51 and 200 times " << count51_200Repeaters;
		out << "\nNumber of words that only occur more than 200 times " << count201_AndBeyondRepeaters;
		out << "\n\n";
	}
};

//Create a functor to compare second elements of pair(string,int)
class wordSorter {
public:
	bool operator()(pair<string, int> elem1, pair<string, int> elem2){
		return elem1.second > elem2.second;
	}
};

int main() {
	string str, inFileName,  outFileNameWHeader,outFileNameNoHeader,rootMedia;
	string rootPerson,myWordsFileName, myPtr, resultsOutFile;
	unsigned int countWords = 0;

	string mediaName = "principito";
	string personName = "Karl";

	rootMedia = "../media/"+mediaName+"/";
	rootPerson = "../People/"+personName+"/";
	inFileName = rootMedia+mediaName+".txt";
	outFileNameWHeader = rootMedia+"OccurencesWHeader.txt";
	outFileNameNoHeader = rootMedia+"UniqueWordList.txt";
	myWordsFileName = rootPerson+"myWords.txt";
	resultsOutFile = rootPerson + mediaName+"Reduced.txt";
	
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
	cout << "\nFinished reading file and storing in map. Number of unique words "<<"m.size = "<<m.size()<<endl;

//Make a vector that is sorted by the value of the map. The reason for a vector is that Vector can have a
//pair. It can be ordered by the value and not the key. Remember, the key is unique, i.e., if you 
//try to do it with a map (making the key = the value of the original map), the key, which would be
//the frequency, is not a set of unique values. 

// Declaring a vector that will store the pairs using above comparision logic

	wordSorter compFunctor;
	vector<pair<string, int>> setOfWords(m.begin(), m.end());
	sort(setOfWords.begin(), setOfWords.end(), compFunctor);

	cout<<"\nOpening outfile for With Heading information "<< outFileNameWHeader << endl;
	headerStats myStats;
	ofstream out( outFileNameWHeader);
	if (out.is_open()) {
		out << "\nTotal number of words " << countWords << endl;
		//Do a quick count of unique words so that you can include it in the header.

		for (pair<string, int> element : setOfWords){
			myStats.incrementUniqueWords();
			 myStats.incrementCounterBins(element.second);

		}
		myStats.printHeader(out);

		for (pair<string, int> element : setOfWords) {
			out << element.first << "\t" << element.second << endl;
		}
	}
	else {
		cerr << "\nError opening output file: " <<  outFileNameWHeader << endl;
	}
	out.close();

//The following output is just the alphabetized list. So, the map is fine. It's predicted purpose is 
//to be used as a list of words that this media has taught them (provied they learn every word in the media).

	cout << "\nOpening outfile for With Heading No information " << outFileNameNoHeader << endl;
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
	//Again, let's get these in descending order of value.

	myStats.initialize();

	// Declaring a vector that will store the pairs using above comparision logic	
	vector<pair<string, int>> newSetOfWords(m.begin(), m.end());
	sort(newSetOfWords.begin(), newSetOfWords.end(), compFunctor);

		countWords = 0;
//Count the total remaining words

		for (pair<string, int> element : newSetOfWords) {
			countWords += element.second;
			myStats.incrementUniqueWords();
			myStats.incrementCounterBins(element.second);
		}
		ofstream myOut(resultsOutFile);
		if (myOut.is_open()) {
			myOut << "\nTotal number of words " << countWords<<endl;
			myStats.printHeader(myOut);

		for (std::pair<std::string, int> element : newSetOfWords) {
			myOut << element.first << "\t" << element.second << endl;

		}
	}
	else{
			cerr << "\nError opening output file: " << resultsOutFile << endl;
	}
}
// joanPoon_finalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <fstream>
#include <sstream>
using namespace std;

//this class is for storing file info such as stats and links
class fileInfo {
public:
	vector<string> vfile;
	vector<string> links;
	string fileName;
	int wordCount;
	float avgWordLength;
	int linkCount;
	fileInfo(vector<string> v, string f, int w, float a, int lc,vector<string> l) {
		vfile = v;
		fileName = f;
		wordCount = w;
		avgWordLength = a;
		linkCount = lc;
		links = l;
	}
};

//some helper methods
fileInfo AnalyzeFile(string filename);
bool isNumber(string input);
bool isInRange(fileInfo fi, string s);
int findIndexInFiles(vector<fileInfo> fi, string fname);
void PrintStat(fileInfo fi);
void PrintFile(vector<string> vstr);
void FindFilesInPath(string& dirPath, vector<string*>& fileList);

int main() {
	//location of my dox files
	string path = "D:\\Users\\joanp\\Documents\\joanPoon_finalProject\\joanPoon_finalProject\\DoxFiles";
	vector<string*> fileList;

	cout << "Defaulting to current path: ";
	cout << path << endl;

	path.append("\\*.*");

	//get all files from path
	FindFilesInPath(path, fileList);
	//delete unnecessary file
	fileList.erase(fileList.begin(), fileList.begin() + 2);

	//analyze all files
	vector<fileInfo> info;
	for (string* item : fileList) {
		info.push_back(AnalyzeFile(*item));
	}
	
	
	//to store user path and show top.dox 
	vector<int> userPath;
	int currentPath = 13;

	//print file content
	cout << "\nShowing: " << info.at(13).fileName << endl;
	PrintFile(info.at(13).vfile);
	userPath.push_back(13);
	
	//get user command
	string command;
	cout << "please enter your command" << endl;
	cin >> command;

	//run program until user quit
	while (command != "q") {
		if (command == "i") {
			//show stats
			PrintStat(info.at(currentPath));
		}
		else if (command == "b") {
			//make sure there is a last path before we go back
			if (userPath.size() == 1) {
				cout << "Sorry, this is the beginning of your browse history." << endl;
			}
			else {
				//show the path then remove the path 
				userPath.pop_back();
				int lastPath = userPath.back();
				cout << "\nShowing: " << info.at(lastPath).fileName << endl;
				PrintFile(info.at(lastPath).vfile);
				currentPath = lastPath;
			}
		}
		else if (isNumber(command)) {
			//if user entered a number make sure it is in range
			if (isInRange(info.at(currentPath), command)) {
				stringstream geek(command);
				int x = 0;
				geek >> x;
				// minus one to get the index in the vector
				x = x - 1;

				//get file name of next path
				string nextPath = info.at(currentPath).links.at(x);
				//update current path
				currentPath = findIndexInFiles(info, nextPath);
				//add next path to userPath vector
				userPath.push_back(currentPath);

				//print file content
				cout << "\nShowing: " << nextPath << endl;
				PrintFile(info.at(currentPath).vfile);
			}
			else {
				cout << "The number you entered is out of range, please enter another command." << endl;
			}
		}
		else {
			//unrecognized command, print menu
			cout << "Unrecognized command: '" << command << "'" << endl;
			cout << "Commands: " << endl;
			cout << "<#> - Navigate to the requested link number" << endl;
			cout << "b - Back" << endl;
			cout << "i - Display document statistics" << endl;
			cout << "q - Exit the Dox shell" << endl;
		}

		//prompt for next commend
		cout << "Please enter your command.\n" << endl;
		cin >> command;
		cout << endl;

	}

	
}

// Complete a list of files with .dox extention at the given path
void FindFilesInPath(string& dirPath, vector<string*>& fileList) {

	_finddata_t data;
	intptr_t ff = _findfirst(dirPath.c_str(), &data);
	if (ff != -1) {
		int res = 0;
		while (res != -1) {
			fileList.push_back(new string(data.name));
			res = _findnext(ff, &data);
		}
		_findclose(ff);
	}
	return;
}

//Analyze file and return a fileInfo object that contains:
//file content and file stats
fileInfo AnalyzeFile(string filename) {
	vector<fileInfo> info;
	vector <string> vStr;
	vector <string> links;
	string x;
	ifstream inFile;
	int wordCount = 0;
	float avgLength = 0;
	int linkCount = 0;


		cout << "Analyzing :" << filename << endl;
		
		//open the file from DoxFiles folder
		inFile.open("DoxFiles\\" + filename);

		//check if file can be opened
		if (!inFile) {
			cerr << "Cannot open file." << endl;
			exit(1);
		}

		//
		string processedFilename;
		while (inFile >> x) {
			
			if (x.at(0) == '|') {

				//check to see if it is |:filename|
				if (x.at(1) == ':') {
					linkCount++;
				
					links.push_back(filename);
					//change it to number link
					x = "|" + to_string(linkCount) + "|";
				} //check to see if it is |filename.dox:|
				else if (x.at(1) != '#') {
					linkCount++;

					int i = 1;
					while (x.at(i) != ':') {
						processedFilename += x.at(i);
						i++;
					}
					links.push_back(processedFilename);
					//change it to number link
					x = "|" + to_string(linkCount) + "|";
					vStr.push_back(x);
				}

			}
			else {
				//calculate total length and increment word count
				avgLength += x.length();
				wordCount++;
				vStr.push_back(x);
			}

			//reset this variable
			processedFilename = "";
		}

		return(fileInfo(vStr,filename,wordCount , avgLength/wordCount, linkCount,links));

}

//Prints the file content
void PrintFile(vector<string> vstr) {
	for (string s : vstr) {
		cout << s << " ";
	}
	cout << "\n\n " << endl;
}

//Prints the file stats
void PrintStat(fileInfo fi) {
	cout << endl << "Statistics for " << fi.fileName << ":" << endl;
	cout << "Number of words: " << fi.wordCount << endl;
	cout << "Average word length: " << fi.avgWordLength << endl;
	cout << "Number of links: " << fi.linkCount << endl;
	cout << "\n\n" << endl;
}

//check if the string is a number
bool isNumber(string input) {
	for (int i = 0; i < input.length(); i++) 
		if (!isdigit(input.at(i))) return false;
	
	return true;
}

//check if the string number is in range according to the link count of the file
bool isInRange(fileInfo fi, string s) {
	stringstream geek(s);
	int i = 0;
	geek >> i;
	return(i > 0 && i < fi.linkCount + 1 );
}

//using the file name, find it's position in the vector of fileInfo
int findIndexInFiles(vector<fileInfo> fi, string fname) {
	int nextPathIndex = 0;
	//change the first letter to upper case to match the format of vector<fileInfo>
	fname[0] = toupper(fname[0]);

	//compare each element in the vector
	for (fileInfo f : fi) {

		if (f.fileName== fname) {
			//found it, return the index
			return nextPathIndex;
		}
		//not found, increment index since we are going to check the next element
		nextPathIndex++;
	}
}
//
//  main.cpp
//  joan_assignment4
//
//  Created by joan on 1/31/19.
//  Copyright © 2019 joan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void Sort(vector<string>& words) {
    
    for (int i = 0; i < words.size() - 1; ++i) {
        
        // Find index of smallest remaining element
        int indexSmallest = i;
        for (int j = i + 1; j < words.size(); ++j) {
            
            if (words[j] < words[indexSmallest]) {
                indexSmallest = j;
            }
        }
        
        // Swap words[i] and words[indexSmallest]
        string temp = words[i];
        words[i] = words[indexSmallest];
        words[indexSmallest] = temp;
    }
    
}
int main()
{
    string x;
    vector <string> vStr;
    
    ifstream inFile;
    inFile.open("a4.txt");
    
    if(!inFile){
        cerr << "Cannot open file." << endl;
        exit(1);
    }
    
    while(inFile >> x){
        vStr.push_back(x);
    }
    
    Sort(vStr);
    
    for (int i=0; i<vStr.size(); i++)
        cout << vStr[i] << "\n";
    return 0;
}

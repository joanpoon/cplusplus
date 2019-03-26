//
//  main.cpp
//  demo
//
//  Created by joan on 1/3/19.
//  Copyright © 2019 joan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
    std::string line, text;
    std::string filename = "a4.txt";
    
    std::ifstream file(filename.c_str());
    std::stringstream buffer;
    
    buffer << file.rdbuf();
    std::string str = buffer.str();
    std::cout << str;
    
    return 0;
}

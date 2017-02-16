/*
 * CmdOptionParser.cpp
 *
 *  Created on: Feb 9, 2017
 *      Author: jamesm
 */

#include "CmdOptionParser.hpp"
#include <algorithm>


CmdOptionParser::CmdOptionParser(int &argc, const char **argv) {
    for (int i=1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));
}

const std::string& CmdOptionParser::getCmdOption(const std::string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    return empty_string;
}

bool CmdOptionParser::cmdOptionExists(const std::string &option) const{
    return std::find(this->tokens.begin(), this->tokens.end(), option)
    != this->tokens.end();
}

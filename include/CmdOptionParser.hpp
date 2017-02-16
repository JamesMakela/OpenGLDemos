/*
 * CmdOptionParser.hpp
 *
 *  Created on: Feb 9, 2017
 *      Author: jamesm
 */

#ifndef CMDOPTIONPARSER_HPP_
#define CMDOPTIONPARSER_HPP_

#include <iostream>
#include <vector>


class CmdOptionParser
{
public:
    CmdOptionParser(int &argc, const char **argv);

    const std::string& getCmdOption(const std::string &option) const;
    bool cmdOptionExists(const std::string &option) const;

private:
    std::vector<std::string> tokens;
    std::string empty_string;
};


#endif /* CMDOPTIONPARSER_HPP_ */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

#ifndef CONFIG_H
#define CONFIG_H

class Config {
private:
    std::string _path;
    std::ifstream _configFile;

   const char _groupSimbol     = '!';
   const char _parameterSimbol = '-';

   const std::string _equivalenceSymbol = "=";

   std::vector<std::string> _parametersGroup;

    /**
    *@brief return a vector of all parameters of a group
    *@param group name of the group
    *@return a vector of each parameter of that group
    */
   std::vector<std::string> FindParametersGroup(std::string& group);

   std::string GetParameterValue(std::vector<std::string>& groupParameters, std::string& param);

public:
    Config(const std::string& path) : _path(path) {}

    /**
    *@brief Gets the value of a parameter in the config file
    *@param group Which group is going to be pointer
    *@param param Value of the parameter in a group
    *@return string value of the parameter
    */
    std::string GetConfigValue(const std::string& group, const std::string& param);

    // std::string * operator[](std::string index);
    // std::string operator[](std::string index);
};


#endif
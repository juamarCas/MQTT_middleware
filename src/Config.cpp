#include "Config.h"

std::string Config::GetConfigValue(const std::string& group, const std::string& param){
    std::string _g = group;
    std::string _p = param;
    std::vector<std::string> data = FindParametersGroup(_g);
    std::string value = GetParameterValue(data, _p);
    return value;  
}




std::vector<std::string> Config::FindParametersGroup(std::string& group){
    _configFile.open(_path);
    std::string text = "";
    std::string result;

    bool foundParameter = false;
    bool foundGroup     = false;
    std::vector<std::string> parameters;

    if (!_configFile) {
        throw "An error ocurred while opening the file!";
        return parameters;
    }

 

    group.insert(0, sizeof(_groupSimbol), _groupSimbol);


     ///Needed to look in all the file, the group and parameter given
    while (getline(_configFile, text)) {
        //std::remove(text.begin(), text.end(), ' ');
        text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());                
            
        if (text.find(group) != std::string::npos) {
            foundGroup = true;
            continue;
        }
        if (foundGroup) {
            if (text[0] != '-') {
                break;
            }
            parameters.push_back(text);            
        }
    }

     _configFile.close();

    if (!foundGroup) {
        throw "The group does not exist!";
        return parameters;
    }
      
    return parameters;
}

std::string Config::GetParameterValue(std::vector<std::string>& groupParameters, std::string& param){
    bool foundParameter = false;
    param.insert(0, sizeof(_parameterSimbol), _parameterSimbol);
    std::string txt = "";
    std::string res = "";
    std::stringstream ss;

    for(int i = 0; i < groupParameters.size(); i++){
        txt = groupParameters[i];

        if(txt.find(param) != std::string::npos){
            foundParameter = true;

            if (txt.find(_equivalenceSymbol) == std::string::npos) {
                _configFile.close();
                throw "Error reading parameter!";
                return param;
            }

            std::size_t simbol = txt.find(_equivalenceSymbol);
            simbol++;
            ss.str(txt);
            ss.seekg(simbol);
            ss >> res;
            break;

        }
        
    }
    return res;
}

// std::string * Config::operator[] (std::string& group){
//     _parametersGroup.Clear();
// }

// std::string Config::operator[] (std::string& param){

// }
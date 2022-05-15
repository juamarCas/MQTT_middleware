#include "GPIO.h"

 bool GPIO::GPIO_Write(const std::string& state) {
    _currentState = state;
    if (write(_file, _currentState.c_str(), 1) != 1) {
        std::cout << "error" << std::endl;
        return false;
    }
    return true;
}

bool GPIO::GPIO_Toggle() {
    std::string _res;
    if (_currentState == "1") _res = "0";
    else if (_currentState == "0") _res = "1";
    

    if (write(_file, _res.c_str(), 1) != 1) {
        std::cout << "error" << std::endl;
        return false;
    }
    _currentState = _res;
}
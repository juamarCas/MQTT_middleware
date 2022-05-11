#ifndef GPIO_H
#define GPIO_H
#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>


class GPIO{
    
    private: 
        std::string m_gpioNumber;
        std::string m_direction;
        
        std::string _currentState;
        int _file;

    public:
            GPIO(const std::string& gpio, const std::string& direction): m_gpioNumber(gpio), m_direction(direction){

            std::string path = "/sys/class/gpio/gpio" + gpio + "/direction";

            _file = open(path.c_str(), O_WRONLY);
            if(_file == -1){
                std::cout<<"failed opening"<<std::endl;
            }
            

            if(write(_file, direction.c_str(), 3) != 3){
                std::cout<<"error writing"<<std::endl;
            }
            close(_file);

            //be sure that the gpio is off
            path = "/sys/class/gpio/gpio" + gpio + "/value";         
            _file = open(path.c_str(), O_WRONLY);
            
            if (_file == -1) {
                std::cout<<"error"<<std::endl;
            }

            if (write(_file, "0", 1) != 1) {
                std::cout<<"error"<<std::endl;
            }

            _currentState = "0";
        }

        ~GPIO(){
            close(_file);
        }
        bool GPIO_Write(const std::string& state);
        bool GPIO_Toggle();

};

#endif
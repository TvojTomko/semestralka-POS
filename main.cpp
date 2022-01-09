#include <boost/asio.hpp>
#include <iostream>
#include "console.h"
#include "download.h"
#include "fileHandler.h"
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string


using boost::asio::ip::tcp;

void separateCommand(std::vector<std::string> &v, std::string &command) {
    size_t pos;
    while ((pos = command.find(":")) != std::string::npos) {
        v.push_back(command.substr(0, pos));
        command.erase(0, pos + 1);
    }
    v.push_back(command);
}



int main() {
/*
    //Zo suboru na time_t
    struct std::tm tm;
    std::istringstream sss("17:30:12");
    sss >> std::get_time(&tm, "%H:%M:%S"); // or just %T in this case
    std::time_t time = mktime(&tm);
    //std::cout<<sss.str();


    //aktualny cas na time_t
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%X");
    struct std::tm tm1;
    std::istringstream sss1(ss.str());
    sss1 >> std::get_time(&tm1, "%H:%M:%S"); // or just %T in this case
    std::time_t time1 = mktime(&tm1);

    std::cout<<sss.str()<<std::endl;
    std::cout<<sss1.str()<<std::endl;

    if(time<time1){
        std::cout<<"menej"<<std::endl;

    } else{
        std::cout<<"viac"<<std::endl;

    }
*/
/*
    std::string time="17:34:20";
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%X");
    std::cout<<time<<std::endl;
    std::cout<<ss.str()<<std::endl;

    std::vector<std::string> timeE;
    separateCommand(timeE,time);
    std::vector<std::string> CtimeE;
    std::string cTime= ss.str();
    separateCommand(CtimeE,cTime);

    //std::cout<<timeE.at(2)<<std::endl;
    //std::cout<<CtimeE.at(2)<<std::endl;

    if(std::stoi(timeE.at(0))<std::stoi(CtimeE.at(0))){
        std::cout<<"spustit0"<<std::endl;
    }else if(std::stoi(timeE.at(0))>std::stoi(CtimeE.at(0))){
        std::cout<<"nespustit0"<<std::endl;
    }else {
        if(std::stoi(timeE.at(1))<std::stoi(CtimeE.at(1))){
            std::cout<<"spustit1"<<std::endl;
        }else if(std::stoi(timeE.at(1))>std::stoi(CtimeE.at(1))){
            std::cout<<"nespustit1"<<std::endl;
        }else{
            if(std::stoi(timeE.at(2))<std::stoi(CtimeE.at(2))){
                std::cout<<"spustit2"<<std::endl;
            }else if(std::stoi(timeE.at(2))>std::stoi(CtimeE.at(2))){
                std::cout<<"nespustit2"<<std::endl;
            }else{
                std::cout<<"spustit2"<<std::endl;
            }
        }
    }

*/


    console asd;
    asd.listener();
    /*std::string asd;
    fileHandler d;
    d.checkSchedule(asd);
    std::cout<<asd;*/
    return 0;
}


//
// Created by looper on 30. 12. 2021.
//

#include "console.h"


void console::listener() {
    bool exit=false;
    std::vector<std::string>bashCommands{"ls",
                                         "mkdir",
                                         "rmdir",
                                         "rm",
                                         "cd"
                                        };
    std::vector<std::string>customCommands{"download",
                                           "history",
                                           "pause",
                                           "resume",
                                           "stop",
                                           "setpath",
                                           "help",
                                           "exit"
                                            };
    std::string space_delimiter = " ";

    while(!exit){
        std::string command;
        getline(std::cin,command);
        std::string first=command.substr(0,command.find(space_delimiter));
        if(std::find(bashCommands.begin(),bashCommands.end(),first)!=bashCommands.end()){
            system(command.c_str());
        }else if(std::find(customCommands.begin(),customCommands.end(),first)!=customCommands.end()){
            //separate custom command to individual elements
            std::vector<std::string> elements{};
            size_t pos = 0;
            while ((pos = command.find(space_delimiter)) != std::string::npos) {
                elements.push_back(command.substr(0, pos));
                command.erase(0, pos + space_delimiter.length());
            }
            if(first=="download"){
                //TODO download
            }else if(first=="history"){
                //TODO history
            }else if(first=="pause"){
                //TODO pause
            }else if(first=="resume"){
                //TODO resume
            }else if(first=="stop"){
                //TODO stop
            }else if(first=="setpath"){
                //TODO setpath
            }else if(first=="help"){
                //TODO help
            }else if(first=="exit"){
                    exit=true;
            }else if(first=="info"){
                //TODO info
            }

        }else{
            std::cout<<"Unsuported command"<<std::endl;
        }
    }
}

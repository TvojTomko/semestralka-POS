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
                                           "exit",
                                           "info"
                                            };
    std::string space_delimiter = " ";
    std::string path;

    while(!exit){
        std::string command;
        getline(std::cin,command);
        std::string first=command.substr(0,command.find(space_delimiter));
        if(std::find(bashCommands.begin(),bashCommands.end(),first)!=bashCommands.end()){
            std::string toConsole;
            if (!getPathtoDownload().empty()){
            toConsole+="cd ";
            toConsole+=getPathtoDownload();
            toConsole+=" && ";
            }
            toConsole+=command;
            system(toConsole.c_str());
        }else if(std::find(customCommands.begin(),customCommands.end(),first)!=customCommands.end()){
            //separate custom command to individual elements
            std::vector<std::string> elements{};
            size_t pos = 0;
            while ((pos = command.find(space_delimiter)) != std::string::npos) {
                elements.push_back(command.substr(0, pos));
                command.erase(0, pos + space_delimiter.length());
            }
            if(first=="download"){
                downloads.push_back(new download(elements.at(1),command,elements.at(2),getPathtoDownload()));
            }else if(first=="history"){
                //TODO history
            }else if(first=="pause"){
                //TODO pause
            }else if(first=="resume"){
                //TODO resume
            }else if(first=="stop"){
                //TODO stop

            }else if(first=="setpath"){
                setPathtoDownload(command);
            }else if(first=="help"){
                std::cout<<"To exit the program type exit"<<std::endl;
            }else if(first=="exit"){
                long unsigned int i=0;
                //stop all threads
                while(i<downloads.size()){
                   downloads.at(i)->stop();
                    i++;
                }
                    exit=true;
            }else if(first=="info"){
                checkFinishedDownloads();
                long unsigned int i=0;
                std::cout<<"Current downloading directory: "<<getPathtoDownload();
                std::cout<<"Number "<<"Protocol "<<"Hostname "<<"Filename "<<"Finished "<<std::endl;
                while(i<downloads.size()){
                    std::cout<<i<<". "<<downloads.at(i)->getAProtocol()<<" "<<downloads.at(i)->getHostname()<<" "<<downloads.at(i)->getFilename()<<" ";
                    std::cout<<downloads.at(i)->getMsg()<<std::endl;
                    i++;
                }
            }


        }else{
            std::cout<<"Unsuported command"<<std::endl;
        }
    }


}
bool IsMarkedToDelete(const download* o)
{
    return o->msg=="je";
}
void console::checkFinishedDownloads() {
    downloads.erase(
            std::remove_if(downloads.begin(), downloads.end(), IsMarkedToDelete),
            downloads.end());
}


const std::string &console::getPathtoDownload() const {
    return pathtoDownload;
}

void console::setPathtoDownload(const std::string &pathtoDownload) {
    console::pathtoDownload = pathtoDownload;
}

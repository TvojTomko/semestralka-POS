//
// Created by looper on 30. 12. 2021.
//

#include "console.h"


void console::listener() {
    dH = new downloadHandler();
    while (!exit) {
        std::string command;
        getline(std::cin, command);
        std::string first = command.substr(0, command.find(space_delimiter));
        if (resolveCommand(first) == 0) {
            makeConsoleCommand(command);
        } else if (resolveCommand(first) == 1) {
            //separate custom command to individual elements
            std::vector<std::string> elements{};
            if (separateCommand(elements, command)) {
                if (first == "download") {
                    int priority = elements.size() > 4 ? std::stoi(elements.at(4)) : 0;
                    dH->addDownload(
                            new download(elements.at(1), elements.at(3), elements.at(2), getPathtoDownload(),
                                         priority));
                } else if (first == "history") {
                    //TODO history
                } else if (first == "pause") {
                    dH->pauseDownloading(std::stoi(elements.at(1)));
                } else if (first == "resume") {
                    dH->resumeDownloading(std::stoi(elements.at(1)));
                } else if (first == "stop") {
                    dH->stopDownloading(std::stoi(elements.at(1)));
                } else if (first == "start") {
                    dH->startDownloading(std::stoi(elements.at(1)));
                } else if (first == "setpath") {
                    pathtoDownload = elements.at(1);
                } else if (first == "help") {
                    std::cout << "To exit the program type exit" << std::endl;
                } else if (first == "exit") {
                    exitProgram();
                } else if (first == "info") {
                    info();
                } else if (first == "startAll") {
                    std::jthread th(&downloadHandler::startAll, dH);
                    th.detach();
                } else if (first == "pauseAll") {
                    std::jthread th(&downloadHandler::pauseAll, dH);
                    th.detach();
                } else if (first == "stopAll") {
                    std::jthread th(&downloadHandler::stopAll, dH);
                    th.detach();
                } else if (first == "resumeAll") {
                    std::jthread th(&downloadHandler::resumeAll, dH);
                    th.detach();
                } else if (first == "manage") {
                    dH->manageDownloadings();
                } else if (first == "setPriority") {
                    dH->setPriority(std::stoi(elements.at(1)), std::stoi(elements.at(2)));
                }
            }
        } else {
            std::cout << "Unsuported command" << std::endl;
        }


    }
}

const std::string &console::getPathtoDownload() const {
    return pathtoDownload;
}

void console::exitProgram() {
    exit = true;
    dH->exitProgram();
    delete dH;
}

int console::resolveCommand(std::string input) {
    if (std::find(bashCommands.begin(), bashCommands.end(), input) != bashCommands.end()) {
        return 0;
    }
    if (std::find(customCommands.begin(), customCommands.end(), input) != customCommands.end()) {
        return 1;
    }
    return 2;
}

void console::makeConsoleCommand(std::string &input) {
    std::string toConsole;
    if (!getPathtoDownload().empty()) {
        toConsole += "cd ";
        toConsole += getPathtoDownload();
        toConsole += " && ";
    }
    toConsole += input;
    system(toConsole.c_str());
}

bool console::checkInput(std::vector<std::string> &v) {
    if (v.at(0) == "download") {
        if (v.size() > 5 || v.size() < 4) {
            std::cout << "Wrong number of arguments" << std::endl;
            return false;
        }
    }
    return true;
}

void console::info() {
    std::string p = pathtoDownload.empty() ? "default" : pathtoDownload;
    std::cout << "Current path: " << p << std::endl;
    dH->listOfDownloads();
}

bool console::separateCommand(std::vector<std::string> &v, std::string &command) {
    size_t pos = 0;
    while ((pos = command.find(space_delimiter)) != std::string::npos) {
        v.push_back(command.substr(0, pos));
        command.erase(0, pos + space_delimiter.length());
    }
    v.push_back(command);
    return checkInput(v);
}
//
// Created by looper on 30. 12. 2021.
//

#include "console.h"


void console::listener() {
    dH = new downloadHandler();

    std::jthread th(&downloadHandler::checkSchedule, dH);
    th.detach();
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
                    std::string username = elements.size() > 5 ? elements.at(5) : "";
                    std::string password = elements.size() > 6 ? elements.at(6) : "";
                    dH->addDownload(
                            new download(elements.at(1), elements.at(3), elements.at(2), pathToDownload,
                                         priority, username, password));
                } else if (first == "history") {
                    dH->history();
                } else if (first == "pause") {
                    dH->pauseDownloading(std::stoi(elements.at(1)));
                } else if (first == "resume") {
                    dH->resumeDownloading(std::stoi(elements.at(1)));
                } else if (first == "stop") {
                    dH->stopDownloading(std::stoi(elements.at(1)));
                } else if (first == "start") {
                    dH->startDownloading(std::stoi(elements.at(1)));
                } else if (first == "setPath") {
                    pathToDownload = elements.at(1);
                } else if (first == "help") {
                    std::cout << "To exit the program type exit" << std::endl;
                } else if (first == "exit") {
                    exitProgram();
                    sleep(2);
                } else if (first == "info") {
                    info();
                } else if (first == "startAll") {
                    dH->startAll();
                } else if (first == "pauseAll") {
                    dH->pauseAll();
                } else if (first == "stopAll") {
                    dH->stopAll();
                } else if (first == "resumeAll") {
                    dH->resumeAll();
                } else if (first == "manage") {
                    dH->manageDownloadings();
                } else if (first == "setPriority") {
                    dH->setPriority(std::stoi(elements.at(1)), std::stoi(elements.at(2)));
                } else if (first == "plan") {
                    std::string priority = elements.size() > 6 ? elements.at(6) : "0";
                    std::string username = elements.size() > 7 ? elements.at(7) : "";
                    std::string password = elements.size() > 8 ? elements.at(8) : "";

                    dH->plan(elements.at(1), elements.at(2), elements.at(3), elements.at(4), elements.at(5),
                             priority, username, password);
                    //plan protocol hostname filename date time priority username password
                } else if (first == "schedule") {
                    dH->schedule();
                }
            }
        } else {
            std::cout << "Unsupported command" << std::endl;
        }


    }
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
    if (!pathToDownload.empty()) {
        toConsole += "cd ";
        toConsole += pathToDownload;
        toConsole += " && ";
    }
    toConsole += input;
    system(toConsole.c_str());
}

bool console::checkInput(std::vector<std::string> &v) {
    if (v.at(0) == "download") {
        if (v.size() > 7 || v.size() < 4) {
            std::cout << "Wrong number of arguments " << v.size() << " required 4-7 " << std::endl;
            return false;
        } else if (!(std::find(protocols.begin(), protocols.end(), v.at(1)) != protocols.end())) {
            std::cout << "Wrong protocol " << v.at(1) << " required http/https/ftp/ftps" << std::endl;
            return false;
        } else if (v.size() == 5 && !is_number(v.at(4))) {
            std::cout << "Wrong priority " << v.at(1) << " required integer" << std::endl;
            return false;
        } else {
            return true;
        }
    } else if (v.at(0) == "pause" || v.at(0) == "start" || v.at(0) == "resume" || v.at(0) == "stop") {
        if (v.size() != 2) {
            std::cout << "Wrong number of arguments " << v.size() << " required 2" << std::endl;
            return false;
        } else if (!is_number(v.at(1)) || std::stoi(v.at(1)) < 0) {
            std::cout << "Wrong number " << v.at(1) << " required positive integer" << std::endl;
            return false;
        } else if (std::stoi(v.at(1)) > dH->getNumberOdDownloads() - 1) {
            std::cout << "Wrong index " << v.at(1) << " required maximum index: " << dH->getNumberOdDownloads() - 1
                      << std::endl;
            return false;
        } else {
            return true;
        }

    } else if ((v.at(0) == "setPath")) {
        if (v.size() != 2) {
            std::cout << "Wrong number of arguments " << v.size() << " required 2" << std::endl;
            return false;
        } else {
            return true;
        }
    } else if (v.at(0) == "help" || v.at(0) == "exit" || v.at(0) == "info" || v.at(0) == "startAll" ||
               v.at(0) == "pauseAll" || v.at(0) == "resumeAll" || v.at(0) == "stopAll") {
        if (v.size() != 1) {
            std::cout << "Wrong number of arguments " << v.size() << " required 1" << std::endl;
            return false;
        } else {
            return true;
        }
    } else if (v.at(0) == "manage") {
        if (v.size() != 1) {
            std::cout << "Wrong number of arguments " << v.size() << " required 1" << std::endl;
            return false;
        } else if (dH->isManage()) {
            std::cout << "Automatic management is already running " << std::endl;
            return false;
        } else {
            return true;
        }
    } else if (v.at(0) == "setPriority") {
        if (v.size() != 3) {
            std::cout << "Wrong number of arguments " << v.size() << " required 3" << std::endl;
            return false;
        } else if (!is_number(v.at(1)) || std::stoi(v.at(1)) < 0) {
            std::cout << "Wrong index " << v.at(1) << " required positive integer" << std::endl;
            return false;
        } else if (!is_number(v.at(2))) {
            std::cout << "Wrong number " << v.at(2) << " required integer" << std::endl;
            return false;
        } else if (std::stoi(v.at(1)) > dH->getNumberOdDownloads() - 1) {
            std::cout << "Wrong index " << v.at(1) << " required maximum index: " << dH->getNumberOdDownloads() - 1
                      << std::endl;
            return false;
        } else {
            return true;
        }
    }

    return true;
}

void console::info() {
    std::string p = pathToDownload.empty() ? "default" : pathToDownload;
    std::cout << "Current path: " << p << std::endl;
    dH->listOfDownloads();
}

bool console::separateCommand(std::vector<std::string> &v, std::string &command) {
    size_t pos;
    while ((pos = command.find(space_delimiter)) != std::string::npos) {
        v.push_back(command.substr(0, pos));
        command.erase(0, pos + space_delimiter.length());
    }
    v.push_back(command);
    return checkInput(v);
}

bool console::is_number(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
//
// Created by looper on 30. 12. 2021.
//

#include "console.h"


void console::listener() {
    bool exit = false;
    downloadHandler *dH = new downloadHandler();

    while (!exit) {
        std::string command;
        getline(std::cin, command);
        std::string first = command.substr(0, command.find(space_delimiter));
        if (resolveCommand(first) == 0) {
            std::string toConsole;
            makeConsoleCommand(toConsole, command);
            system(toConsole.c_str());
        } else if (resolveCommand(first) == 1) {
            //separate custom command to individual elements
            std::vector<std::string> elements{};
            size_t pos = 0;
            while ((pos = command.find(space_delimiter)) != std::string::npos) {
                elements.push_back(command.substr(0, pos));
                command.erase(0, pos + space_delimiter.length());
            }
            if (first == "download") {
                dH->addDownload(new download(elements.at(1), command, elements.at(2), getPathtoDownload()));
            } else if (first == "history") {
                //TODO history
            } else if (first == "pause") {
                dH->pauseDownloading(std::stoi(command));
            } else if (first == "resume") {
                dH->resumeDownloading(std::stoi(command));
            } else if (first == "stop") {
                dH->stopDownloading(std::stoi(command));
            } else if (first == "setpath") {
                setPathtoDownload(command);
            } else if (first == "help") {
                std::cout << "To exit the program type exit" << std::endl;
            } else if (first == "exit") {
                exitt(exit);
                dH->pauseAll();
            } else if (first == "info") {
                std::jthread th(&downloadHandler::listOfDownloads, dH);
                th.detach();
            } else if (first == "startAll") {
                std::jthread th(&downloadHandler::startAll, dH);
                th.detach();
            } else if (first == "pauseAll") {
                std::jthread th(&downloadHandler::pauseAll, dH);
                th.detach();
            } else if (first == "manage") {
                dH->manageDownloadings();
            } else if (first == "setPriority") {
                dH->setPriority(std::stoi(elements.at(1)), std::stoi(command));
            }
        } else {
            std::cout << "Unsuported command" << std::endl;
        }


    }
}

/*
bool IsMarkedToDelete(const download* o)
{
    return o->msg=="je";
}
void console::checkFinishedDownloads() {
    downloads.erase(
            std::remove_if(downloads.begin(), downloads.end(), IsMarkedToDelete),
            downloads.end());
}*/


const std::string &console::getPathtoDownload() const {
    return pathtoDownload;
}

void console::setPathtoDownload(const std::string &pathtoDownload) {
    console::pathtoDownload = pathtoDownload;
}

void console::exitt(bool &exit) {
    exit = true;
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

void console::makeConsoleCommand(std::string &command, std::string &input) {
    if (!getPathtoDownload().empty()) {
        command += "cd ";
        command += getPathtoDownload();
        command += " && ";
    }
    command += input;
}

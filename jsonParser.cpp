
#include "jsonParser.h"

using namespace std;

FILE *fp;
char *buffer;
size_t filesNum;

struct json_object *file;

struct json_object *fileh;

struct json_object *fileContent;


void jsonReadSchedule() {
    struct json_object *files;

    long sz;

    fp = fopen("json-test.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);


    files = json_object_new_array();
    json_object_array_add(files, json_tokener_parse(buffer));

    filesNum = json_object_array_length(files);

    fileContent = json_object_array_get_idx(files, 0);

    cout << "Reading json... " << endl << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE) << endl;

    json_object_put(files);
    free(buffer);
}
void jsonWrite(std::string protocolp, std::string hostnamep, std::string filenamep,std::string date, std::string timep,
               std::string priorityp, std::string usernamep, std::string passwordp) {
    struct json_object *object;
    long sz;

    fp = fopen("json-test.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);


    int num = 1;
    string name;


    for (int i = 0; i < json_object_object_length(object); ++i) {
        name = "file" + to_string(num);

        if (json_object_object_get(object, name.c_str()) != nullptr) {
            //json_object_object_get(object, name.c_str());
            num++;
        } else {
            i--;
            num++;
        }
    }

    string key = "file" + to_string(num);

    cout << "Inserting: " << key << " ..." << endl;

    file = json_object_new_object();


    //fileh = json_object_new_object();

    //plan protocol hostname filename time priority username password

    json_object_object_add(file, "protocol", json_object_new_string(protocolp.c_str()));
    json_object_object_add(file, "hostname", json_object_new_string(hostnamep.c_str()));
    json_object_object_add(file, "filename", json_object_new_string(filenamep.c_str()));
    json_object_object_add(file, "date",json_object_new_string(date.c_str()));
    json_object_object_add(file, "scheduled-time",json_object_new_string(timep.c_str()));
    json_object_object_add(file, "priority", json_object_new_string(priorityp.c_str()));
    json_object_object_add(file, "username", json_object_new_string(usernamep.c_str()));
    json_object_object_add(file, "password", json_object_new_string(passwordp.c_str()));


    json_object_object_add(object, key.c_str(), file);
    //json_object_object_add(history, key.c_str(), fileh);

    json_object_to_file_ext("currentdownload.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    //json_object_to_file_ext("history.json", file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    //json_object_put(history);
    //json_object_put(file);
    json_object_put(object);
    free(buffer);
    addToHistory(key);

}
void jsonGetAllInfo(std::string &s) {

    //plan protocol hostname filename time priority username password

    struct json_object *object;

    struct json_object *protocol;
    struct json_object *hostname;
    struct json_object *filename;
    struct json_object *date;
    struct json_object *time;
    struct json_object *priority;
    struct json_object *username;
    struct json_object *password;


    string final;
    long sz;

    fp = fopen("json-test.json", "r");

    if (fp == nullptr) {
      //  cout << "Error opening file...";
        return;
    }

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("json-test.json", "r");

    if (fp == nullptr) {
        //cout << "Error opening file...";
        return;
    }

    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    int length = json_object_object_length(object);

    string name;
    int flnum = 1;

    for(int i = 0; i < length; i++) {
        name = "file" + to_string(flnum);

        if (json_object_object_get(object, name.c_str()) != nullptr) {
           // cout << name << endl;

            //cout << json_object_to_json_string(json_object_object_get(object, name.c_str())) << endl;

            json_object_object_get_ex(object, name.c_str(), &file);

            //cout << json_object_to_json_string_ext(file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

            //plan protocol hostname filename time priority username password

            json_object_object_get_ex(file, "protocol", &protocol);
            json_object_object_get_ex(file, "hostname", &hostname);
            json_object_object_get_ex(file, "filename", &filename);
            json_object_object_get_ex(file, "date", &date);
            json_object_object_get_ex(file, "scheduled-time", &time);
            json_object_object_get_ex(file, "priority", &priority);
            json_object_object_get_ex(file, "username", &username);
            json_object_object_get_ex(file, "password", &password);

            //datum
            bool datumSpusti;
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%d.%m.%Y");
           // std::cout<<ss.str()<<std::endl;

            std::string today=ss.str();
            std::vector<std::string> dates;
            //separateCommand(timeE,stime);
            size_t posD;
            while ((posD = today.find(".")) != std::string::npos) {
                dates.push_back(today.substr(0, posD));
                today.erase(0, posD + 1);
            }
            dates.push_back(today);

            string cdate = json_object_get_string(date);

            std::vector<std::string> datesF;
            //separateCommand(timeE,stime);
            size_t posDF;
            while ((posDF = cdate.find(".")) != std::string::npos) {
                datesF.push_back(cdate.substr(0, posDF));
                cdate.erase(0, posDF + 1);
            }
            datesF.push_back(cdate);

            if(std::stoi(dates.at(2))>std::stoi(datesF.at(2))){
                //std::cout << "spustit2" << std::endl;
            datumSpusti=true;
            }else if(std::stoi(dates.at(2))<std::stoi(datesF.at(2))){
               // std::cout << "nespustit2" << std::endl;
               datumSpusti=false;
            }else{
                if(std::stoi(dates.at(1))>std::stoi(datesF.at(1))){
                  //  std::cout << "spustit1" << std::endl;
                    datumSpusti=true;
                }else if(std::stoi(dates.at(1))<std::stoi(datesF.at(1))){
                   // std::cout << "nespustit1" << std::endl;
                  //  std::cout <<std::stoi(dates.at(1))<<endl;
                  //  std::cout <<std::stoi(datesF.at(1))<<endl;
                    datumSpusti=false;
                }else{
                    if(std::stoi(dates.at(0))>std::stoi(datesF.at(0))){
                   //     std::cout << "spustit0" << std::endl;
                        datumSpusti=true;
                    }else if(std::stoi(dates.at(0))<std::stoi(datesF.at(0))){
                     //   std::cout << "nespustit0" << std::endl;
                     //   std::cout <<std::stoi(dates.at(0))<<endl;
                     //   std::cout <<std::stoi(datesF.at(0))<<endl;
                        datumSpusti=false;
                    }else {
                     //   std::cout << "spustit00" << std::endl;
                        datumSpusti=true;
                    }
                }

            }
           // cout<<datumSpusti<<std::endl;

            //cas

            bool spustit=false;

            if(datumSpusti) {
                string stime = json_object_get_string(time);
                auto now = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);
                std::stringstream ss;
                ss << std::put_time(std::localtime(&in_time_t), "%X");


                std::vector<std::string> timeE;
                //separateCommand(timeE,stime);
                size_t pos;
                while ((pos = stime.find(":")) != std::string::npos) {
                    timeE.push_back(stime.substr(0, pos));
                    stime.erase(0, pos + 1);
                }
                timeE.push_back(stime);

                std::vector<std::string> CtimeE;
                std::string cTime = ss.str();


                size_t pos1;
                while ((pos1 = cTime.find(":")) != std::string::npos) {
                    CtimeE.push_back(cTime.substr(0, pos1));
                    cTime.erase(0, pos1 + 1);
                }
                CtimeE.push_back(cTime);

                if (std::stoi(timeE.at(0)) < std::stoi(CtimeE.at(0))) {
              //      std::cout << "spustit0" << std::endl;
                    spustit = true;
                } else if (std::stoi(timeE.at(0)) > std::stoi(CtimeE.at(0))) {
                //    std::cout << "nespustit0" << std::endl;
                    spustit = false;
                } else {
                    if (std::stoi(timeE.at(1)) < std::stoi(CtimeE.at(1))) {
                //        std::cout << "spustit1" << std::endl;
                        spustit = true;
                    } else if (std::stoi(timeE.at(1)) > std::stoi(CtimeE.at(1))) {
                 //       std::cout << "nespustit1" << std::endl;
                        spustit = false;

                    } else {
                        if (std::stoi(timeE.at(2)) < std::stoi(CtimeE.at(2))) {
                  //          std::cout << "spustit2" << std::endl;
                            spustit = true;
                        } else if (std::stoi(timeE.at(2)) > std::stoi(CtimeE.at(2))) {
                     //       std::cout << "nespustit2" << std::endl;
                            spustit = false;
                        } else {
                     //       std::cout << "spustit2" << std::endl;
                            spustit = true;
                        }
                    }
                }


            }
            if (spustit) {
               // cout << "start download..." << endl;

                //plan protocol hostname filename time priority username password

                final += json_object_get_string(protocol);
                final += " ";
                final += json_object_get_string(hostname);
                final += " ";
                final += json_object_get_string(filename);
                final += " ";
                final += json_object_get_string(priority);
                 // final += " ";
                 //final += json_object_get_string(username);
                 // final += " ";
                 //final += json_object_get_string(password);
                i=length;

                //cout << final << endl;

                json_object_object_del(object, name.c_str());
                json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);
            }
            flnum++;
        } else {
            //cout << name << " not found, searching again..." << endl;
            flnum++;
            i--;
        }

       // cout << "*******************************************************************************************" << endl;
    }
    s=final;
    json_object_put(object);
    free(buffer);
}
void addToHistory(string fn) {
    struct json_object *object1;

    long sz;

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object1 = json_tokener_parse(buffer);

    free(buffer);

    fp = fopen("currentdownload.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("currentdownload.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);

    json_object_object_add(object1, fn.c_str(), json_tokener_parse(buffer));

    json_object_to_file_ext("history.json", object1,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    json_object_put(object1);

    free(buffer);
}
void jsonReadHistory() {
    struct json_object *files;

    long sz;

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);


    files = json_object_new_array();
    json_object_array_add(files, json_tokener_parse(buffer));

    filesNum = json_object_array_length(files);

    fileContent = json_object_array_get_idx(files, 0);

    cout << "Reading json... " << endl << json_object_to_json_string_ext(fileContent,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE) << endl;

    json_object_put(files);
    free(buffer);
}
void addObjectToHistory(string protocolp, string hostnamep, string filenamep, string priorityp, string usernamep, string passwordp) {
    struct json_object *object;

    long sz;

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
    //cout << sz;

    buffer = (char*)malloc(sz);

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }

    fp = fopen("history.json", "r");
    if (fp == nullptr) {
        cout << "Error opening file...";
        return;
    }
    fread(buffer, sz, 1, fp);
    fclose(fp);

    object = json_tokener_parse(buffer);

    int num = 1;
    string name;


    for (int i = 0; i < json_object_object_length(object); ++i) {
        name = "file" + to_string(num);

        if (json_object_object_get(object, name.c_str()) != nullptr) {
            //json_object_object_get(object, name.c_str());
            num++;
        } else {
            i--;
            num++;
        }
    }

    string key = "file" + to_string(num);
    file = json_object_new_object();

    cout << "Inserting: " << key << " ..." << endl;

    json_object_object_add(file, "protocol", json_object_new_string(protocolp.c_str()));
    json_object_object_add(file, "hostname", json_object_new_string(hostnamep.c_str()));
    json_object_object_add(file, "filename", json_object_new_string(filenamep.c_str()));
    json_object_object_add(file, "priority", json_object_new_string(priorityp.c_str()));
    json_object_object_add(file, "username", json_object_new_string(usernamep.c_str()));
    json_object_object_add(file, "password", json_object_new_string(passwordp.c_str()));

    json_object_object_add(object, key.c_str(), file);

    json_object_to_file_ext("history.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);

    json_object_put(object);
    free(buffer);
}
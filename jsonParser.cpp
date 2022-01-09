#include <json-c/json.h>
#include <iostream>
#include <chrono>
#include <time.h>

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

void jsonWrite(std::string protocolp, std::string hostnamep, std::string filenamep, std::string timep,
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

void jsonGetAllInfo() {

    //plan protocol hostname filename time priority username password

    struct json_object *object;

    struct json_object *protocol;
    struct json_object *hostname;
    struct json_object *filename;
    struct json_object *time;
    struct json_object *priority;
    struct json_object *username;
    struct json_object *password;

    string final;

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

    int length = json_object_object_length(object);

    string name;
    int flnum = 1;

    for(int i = 0; i < length; i++) {
        name = "file" + to_string(flnum);

        if (json_object_object_get(object, name.c_str()) != nullptr) {
            cout << name << endl;

            //cout << json_object_to_json_string(json_object_object_get(object, name.c_str())) << endl;

            json_object_object_get_ex(object, name.c_str(), &file);

            //cout << json_object_to_json_string_ext(file, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) << endl;

            //plan protocol hostname filename time priority username password

            json_object_object_get_ex(file, "protocol", &protocol);
            json_object_object_get_ex(file, "hostname", &hostname);
            json_object_object_get_ex(file, "filename", &filename);
            json_object_object_get_ex(file, "scheduled-time", &time);
            json_object_object_get_ex(file, "priority", &priority);
            json_object_object_get_ex(file, "username", &username);
            json_object_object_get_ex(file, "password", &password);

            string stime = json_object_get_string(time);
            std::tm schedtime = {};

            strptime(stime.c_str(), "%a %b %d %H:%M:%S %Y", &schedtime);

            std::time_t schedtm = mktime(&schedtime);

            auto current = std::chrono::system_clock::now();
            std::time_t currenttm = std::chrono::system_clock::to_time_t(current);

            cout << "scheduled time: " << asctime(&schedtime);
            cout << "current time: " << std::ctime(&currenttm) << endl;

            if(schedtm > currenttm)
                cout << "you have enough time... chill" << endl;
            if (schedtm <= currenttm) {
                cout << "start download..." << endl;

                //plan protocol hostname filename time priority username password

                final += json_object_get_string(protocol);
                final += " ";
                final += json_object_get_string(hostname);
                final += " ";
                final += json_object_get_string(filename);
                final += " ";
                final += json_object_get_string(time);
                final += " ";
                final += json_object_get_string(priority);
                final += " ";
                final += json_object_get_string(username);
                final += " ";
                final += json_object_get_string(password);

                cout << final << endl;

                json_object_object_del(object, name.c_str());
                json_object_to_file_ext("json-test.json", object, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_NOSLASHESCAPE);
            }
            flnum++;
        } else {
            cout << name << " not found, searching again..." << endl;
            flnum++;
            i--;
        }

        cout << "*******************************************************************************************" << endl;
    }

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
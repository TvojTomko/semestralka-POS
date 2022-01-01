#include <iostream>
#include <cstdio>
#include <json-c/json.h>

using namespace std;

int main() {
    FILE *fp;
    char buffer[1024];

    struct json_object *files;
    struct json_object *parsed_json;
    struct json_object *fileContent;
    //struct json_object *age;
    //struct json_object *location;


    struct json_object *file2;

    size_t filesNum;

    fp = fopen("json-test.txt", "r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    //json_object_object_get_ex(parsed_json, "files", &files);
    //filesNum = json_object_array_length(files);

    /*for (int i = 0; i < filesNum; i++) {
        fileContent = json_object_array_get_idx(files, i);
        //json_object_object_get_ex(files, "fileContent", &fileContent);
        //json_object_object_get_ex(files, "age", &age);
        //json_object_object_get_ex(files, "location", &location);
        cout << "File " << (i)+1 << ": " << json_object_get_string(fileContent) << endl;
    }
    */
    file2 = json_object_new_object();
    //json_object_array_add(files, file2);

    string nameN = "meno";
    json_object_object_add(file2,"name", json_object_new_string(nameN.c_str()));

    int ageN = 20;
    json_object_object_add(file2, "age", json_object_new_int(ageN));

    string locationN = "location";
    json_object_object_add(file2,"location", json_object_new_string(locationN.c_str()));

    json_object_to_file("json-test.txt", file2);

    return 0;
}

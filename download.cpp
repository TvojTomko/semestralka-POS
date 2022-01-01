//
// Created by looper on 30. 12. 2021.
//

#include "download.h"
download::download(std::string pprotocol,std::string pfilename,std::string phostname,std::string ppath) {
    protocol=pprotocol;
    filename=pfilename;
    hostname=phostname;
    path=ppath;
    size=0;
    currentSize=0;
    msg="No";
    priority=0;

}

void download::startDownload() {
if(protocol=="http"){
    http();
}else if(protocol=="https"){
    https();
}
}
void download::pauseDownload() {
    pause=true;
    resume=false;
    setMsg("PAUSED");
}
void download::resumeDownload() {
    resume=true;
    pause=false;
    if(stop){
        return;
    }
    if(protocol=="http"){
        http();
    }else if(protocol=="https"){
        https();
    }

}
void download::stopDownload() {
    stop=true;
    pause=true;
    msg="STOP";
    std::ostringstream oss;
    oss<<"rm "<<filepath;
    system(oss.str().c_str());
    localFilename="Deleted";
}

int download::http(){
    try {
        if (hostname.empty() || filename.empty()) {
            std::cout << "not enough arguments\n";
            return 1;
        }
        boost::asio::io_context io_context;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(hostname, "http");

        // Try each endpoint until we successfully establish a connection.
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << filename << " HTTP/1.0\r\n";
        request_stream << "Host: " << hostname << "\r\n";
        if(resume){
            request_stream << "Range: bytes="<<std::to_string((int)currentSize)<<"-" << "\r\n";
        }
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Invalid response\n";
            return 1;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {

            if (!resume) {
                if (header.find("Content-Length") != std::string::npos) {
                    setSize(std::stoi(header.substr(header.find(" "), header.size())));
                }
            }
        }
        if(!resume){
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename=filenamenew;
            filepath=path+"/"+localFilename;
        }
        std::ofstream MyFile;
        MyFile.open((filepath),std::ios::app);

        // Write whatever content we already have to output.
        int size=currentSize;
        if (response.size() > 0) {
            size+=response.size();
            MyFile << &response;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
                                 boost::asio::transfer_at_least(1), error)) {
            size+=response.size();
            MyFile << &response;
            currentSize=size;
            if(pause){
                MyFile.close();
                return 0;
            }
        }


        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        MyFile.close();
        setMsg("Yes");
        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}

int download::https(){
    try {
        if (hostname.empty() || filename.empty()) {
            std::cout << "not enough arguments\n";
            return 1;
        }
        boost::asio::io_service io_service;
        // Create a context that uses the default paths for
        // finding CA certificates.
        ssl::context ctx(ssl::context::sslv23);
        ctx.set_default_verify_paths();

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(hostname, "https");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // Try each endpoint until we successfully establish a connection.
        ssl_socket socket(io_service, ctx);
        boost::asio::connect(socket.lowest_layer(), endpoint_iterator);
        socket.lowest_layer().set_option(tcp::no_delay(true));

        // Perform SSL handshake and verify the remote host's
        // certificate.
        socket.set_verify_mode(boost::asio::ssl::verify_none);
        socket.set_verify_callback(ssl::rfc2818_verification(hostname));
        socket.handshake(ssl_socket::client);

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << filename << " HTTP/1.0\r\n";
        request_stream << "Host: " << hostname << "\r\n";
        if(resume){
            request_stream << "Range: bytes="<<std::to_string((int)currentSize)<<"-" << "\r\n";
        }
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Invalid response\n";
            return 1;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {

            if (!resume) {
                if (header.find("Content-Length") != std::string::npos) {
                    setSize(std::stoi(header.substr(header.find(" "), header.size())));
                }
            }
        }
        if(!resume){
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename=filenamenew;
            filepath=path+"/"+localFilename;
        }
        std::ofstream MyFile;
        MyFile.open((filepath),std::ios::app);

        // Write whatever content we already have to output.
        int size=currentSize;
        if (response.size() > 0) {
            size+=response.size();
            MyFile << &response;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
                                 boost::asio::transfer_at_least(1), error)) {
            size+=response.size();
            MyFile << &response;
            currentSize=size;
            if(pause){
                MyFile.close();
                return 0;
            }
        }


        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        MyFile.close();
        setMsg("Yes");
        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;

}

const std::string &download::getFilename() const {
    return filename;
}

void download::setFilename(const std::string &filename) {
    download::filename = filename;
}

void download::setHostname(const std::string &hostname) {
    download::hostname = hostname;
}

void download::setSize(long long int size) {
    download::size = size;
}

void download::setCurrentSize(long long int currentSize) {
    download::currentSize = currentSize;
}

const std::string &download::getHostname() const {
    return hostname;
}

long long int download::getSize() const {
    return size;
}

long long int download::getCurrentSize() const {
    return currentSize;
}

const std::string &download::getAProtocol() const {
    return protocol;
}

void download::setAProtocol(const std::string &aProtocol) {
    protocol = aProtocol;
}

const std::string &download::getMsg() const {
    return msg;
}

void download::setMsg(const std::string &msg) {
    download::msg = msg;
}

const std::string &download::getPpath() const {
    return path;
}

void download::setPpath(const std::string &ppath) {
    download::path = ppath;
}

double download::progress() {
    return currentSize/size*100;
}

void download::checkfile(std::string &filename) {
    bool exists = true;
    int i=1;
    std::string newFilenamecheck=filename;

    while (exists) {

        if (!path.empty()) {
            std::filesystem::current_path(path);
        }
        if (std::filesystem::exists(newFilenamecheck)) {
            if(i>1){
                newFilenamecheck=filename;
            }
            std::string token=newFilenamecheck.substr(0, newFilenamecheck.find("."));
            token+=std::to_string(i);
            token+=newFilenamecheck.substr( newFilenamecheck.find("."),newFilenamecheck.length());
            newFilenamecheck=token;
            i++;
        }else{
            exists=false;
            filename=newFilenamecheck;
            return;
        }

    }

}

const std::string &download::getLocalFilename() const {
    return localFilename;
}

const std::string &download::getPath() const {
    return path;
}

int download::getPriority() const {
    return priority;
}

const std::string &download::getFilepath() const {
    return filepath;
}

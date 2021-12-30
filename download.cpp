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
    msg="nieje";
    priority=0;
    t=std::jthread(download::http,hostname,filename,this);
}

int download::http(std::string hostname,std::string filename,download* instance){
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
        if (status_code != 200) {
            std::cout << "Response returned with status code " << status_code << "\n";
            return 1;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r"){
            std::cout << header << "\n";
            if (header.find("Content-Length") != std::string::npos) {
                instance->setSize(std::stoi(header.substr( header.find(" ") ,header.size())));
            }
        }
        std::cout << "\n";
        std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
        std::ofstream MyFile(filenamenew);
        // Write whatever content we already have to output.
        int size=0;
        if (response.size() > 0) {
            //std::cout << response.size() << std::endl;
            size+=response.size();
            MyFile << &response;
            //std::cout << response.size() << std::endl;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
                                 boost::asio::transfer_at_least(1), error)) {
            //std::cout << response.size() << std::endl;
            size+=response.size();
            MyFile << &response;
            instance->currentSize=size;
            //std::cout << response.size() << std::endl;
        }
        std::cout <<size<<std::endl;


        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        MyFile.close();
        std::cout<<"Checkpoint"<<std::endl;
        instance->stop();
        instance->setMsg("je");
        if(!instance->getPpath().empty()){
            std::ostringstream oss;
            oss<<"mv "<<filenamenew<<" "<<instance->getPpath();
            system(oss.str().c_str());
        }

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

void download::stop(){
    t.request_stop();
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

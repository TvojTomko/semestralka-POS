//
// Created by looper on 30. 12. 2021.
//

#include "download.h"

download::download(std::string pprotocol, std::string pfilename, std::string phostname, std::string ppath,
                   int ppriority, std::string pusername, std::string ppassword) {
    protocol = pprotocol;
    filename = pfilename;
    hostname = phostname;
    path = ppath;
    size = 0;
    currentSize = 0;
    msg = "No";
    priority = ppriority;
    pause = false;
    username = pusername;
    password = ppassword;

}

void download::startDownload() {
    downloading = true;
    if (pause) {
        resumeDownload();
        return;
    }
    if (protocol == "http") {
        http();
    } else if (protocol == "https") {
        https();
    } else if (protocol == "ftp") {
        ftp();
    } else if (protocol == "ftps") {
        ftps();
    }
}

void download::pauseDownload() {
    downloading = false;
    pause = true;
    resume = false;
    setMsg("PAUSED");
}

void download::resumeDownload() {
    downloading = true;
    resume = true;
    pause = false;
    if (stop) {
        return;
    }
    if (protocol == "http") {
        http();
    } else if (protocol == "https") {
        https();
    } else if (protocol == "ftp") {
        ftp();
    } else if (protocol == "ftps") {
        ftps();
    }

}

void download::stopDownload() {
    downloading = false;
    stop = true;
    pause = true;
    msg = "STOP";
    std::ostringstream oss;
    oss << "rm " << filepath;
    system(oss.str().c_str());
    localFilename = "Deleted";
}

int download::http() {
    try {
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
        if (resume) {
            request_stream << "Range: bytes=" << std::to_string((int) currentSize) << "-" << "\r\n";
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

            if (!resume || size == 0) {
                if (header.find("Content-Length") != std::string::npos) {
                    setSize(std::stoi(header.substr(header.find(" "), header.size())));
                }
            }
        }
        if (!resume || filepath.empty() || localFilename.empty()) {
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename = filenamenew;
            if (path.empty()) {
                filepath = "./" + localFilename;
            } else {
                filepath = path + "/" + localFilename;
            }

        }
        std::ofstream MyFile;
        MyFile.open((filepath), std::ios::app);

        // Write whatever content we already have to output.
        int size = currentSize;
        if (response.size() > 0) {
            size += response.size();
            MyFile << &response;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
                                 boost::asio::transfer_at_least(1), error)) {
            size += response.size();
            MyFile << &response;
            currentSize = size;
            if (pause) {
                MyFile.close();
                return 0;
            }
        }


        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        MyFile.close();
        setMsg("Yes");
        downloading = false;
        return 0;
    }
    catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}

int download::https() {
    try {
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
        if (resume) {
            request_stream << "Range: bytes=" << std::to_string((int) currentSize) << "-" << "\r\n";
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

            if (!resume || size == 0) {
                if (header.find("Content-Length") != std::string::npos) {
                    setSize(std::stoi(header.substr(header.find(" "), header.size())));
                }
            }
        }
        if (!resume || filepath.empty() || localFilename.empty()) {
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename = filenamenew;
            if (path.empty()) {
                filepath = "./" + localFilename;
            } else {
                filepath = path + "/" + localFilename;
            }
        }
        std::ofstream MyFile;
        MyFile.open((filepath), std::ios::app);

        // Write whatever content we already have to output.
        int size = currentSize;
        if (response.size() > 0) {
            size += response.size();
            MyFile << &response;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
                                 boost::asio::transfer_at_least(1), error)) {
            size += response.size();
            MyFile << &response;
            currentSize = size;
            if (pause) {
                MyFile.close();
                return 0;
            }
        }


        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        MyFile.close();
        setMsg("Yes");
        downloading = false;
    }
    catch (std::exception &e) {
        setMsg("Yes");
        downloading = false;
        //std::cout << "Exception: " << e.what() << "\n";

    }

    return 0;

}

int download::ftp() {
    try {
        boost::asio::io_service io_service;

        tcp::resolver time_server_resolver(io_service);

        tcp::resolver::query query(hostname, "ftp");

        tcp::resolver::iterator endpoint_iterator = time_server_resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socket socket(io_service);

        boost::system::error_code error = boost::asio::error::host_not_found;

        while (error && endpoint_iterator != end) {
            socket.close();
            socket.connect(*endpoint_iterator++, error);;
        }
        if (error) {
            throw boost::system::system_error(error);
        }
        boost::asio::streambuf request;
        boost::asio::streambuf response;
        //std::string strLine;
        std::ostream request_stream(&request);
        std::istream response_stream(&response);

        std::string strResult;

        request_stream << "USER " << username << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;
        if (getRetCode(strResult) == 220) {
            // strResult.clear();
            boost::asio::read_until(socket, response, "\r\n");
            std::getline(response_stream, strResult);
            //std::cout << strResult << std::endl;

        }

        request_stream << "PASS " << password << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;

        request_stream << "TYPE I" << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;

        request_stream << "PASV" << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;

        std::string strPort = getHostAndPort(strResult);

        request_stream << "SIZE " << filename << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;
        size = std::stoi(strResult.substr(strResult.find_first_of(" "), strResult.length() - 1));
        //std::cout << size << std::endl;

        if (resume) {
            request_stream << "REST " << std::to_string((int) currentSize) << "\r\n";
            boost::asio::write(socket, request);
        }

        request_stream << "RETR " << filename << "\r\n";
        boost::asio::write(socket, request);

        tcp::socket data_socket(io_service);
        tcp::resolver::query data_query(hostname, strPort);

        tcp::resolver::iterator data_endpoint_iterator = time_server_resolver.resolve(data_query);
        error = boost::asio::error::host_not_found;

        if (error && data_endpoint_iterator != end) {
            data_socket.close();
            data_socket.connect(*data_endpoint_iterator++, error);
        }
        if (error) {
            throw boost::system::system_error(error);
        }

        boost::asio::streambuf responseData;
        boost::system::error_code errorData;

        if (!resume || filepath.empty() || localFilename.empty()) {
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename = filenamenew;
            if (path.empty()) {
                filepath = "./" + localFilename;
            } else {
                filepath = path + "/" + localFilename;
            }

        }
        std::ofstream MyFile;
        MyFile.open((filepath), std::ios::app);

        // Write whatever content we already have to output.
        int size = currentSize;
        if (responseData.size() > 0) {
            size += responseData.size();
            MyFile << &responseData;

        }

        while (boost::asio::read(data_socket, responseData,
                                 boost::asio::transfer_at_least(1), errorData)) {
            size += responseData.size();
            MyFile << &responseData;
            currentSize = size;
            if (pause) {
                MyFile.close();
                return 0;
            }
        }

        if (errorData != boost::asio::error::eof) {
            throw boost::system::error_code(errorData);
        }

        MyFile.close();
        setMsg("Yes");
        downloading = false;
        return 0;


    }
    catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;

}

int download::ftps() {
    try {

        boost::asio::io_service io_service;
        // Create a context that uses the default paths for
        // finding CA certificates.
        ssl::context ctx(ssl::context::sslv23);
        ctx.set_default_verify_paths();

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(hostname, "ftps");
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
        tcp::resolver time_server_resolver(io_service);

        boost::asio::streambuf request;
        boost::asio::streambuf response;
        //std::string strLine;
        std::ostream request_stream(&request);
        std::istream response_stream(&response);

        std::string strResult;

        request_stream << "USER " << username << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;
        if (getRetCode(strResult) == 220) {
            boost::asio::read_until(socket, response, "\r\n");
            std::getline(response_stream, strResult);
            // std::cout << strResult << std::endl;

        }

        request_stream << "PASS " << password << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        //std::cout << strResult << std::endl;

        request_stream << "TYPE I" << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        // std::cout << strResult << std::endl;

        request_stream << "PASV" << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        // std::cout << strResult << std::endl;

        std::string strPort = getHostAndPort(strResult);
        //std::cout << strPort << std::endl;

        request_stream << "SIZE " << filename << "\r\n";
        boost::asio::write(socket, request);
        boost::asio::read_until(socket, response, "\r\n");
        std::getline(response_stream, strResult);
        // std::cout << strResult << std::endl;
        size = std::stoi(strResult.substr(strResult.find_first_of(" "), strResult.length() - 1));
        //std::cout << size << std::endl;

        if (resume) {
            request_stream << "REST " << std::to_string((int) currentSize) << "\r\n";
            boost::asio::write(socket, request);
        }

        request_stream << "RETR " << filename << "\r\n";
        boost::asio::write(socket, request);

        //tcp::socket data_socket(io_service);
        boost::asio::io_service data_io_service;
        ssl::context data_ctx(ssl::context::sslv23);
        data_ctx.set_default_verify_paths();

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver data_resolver(data_io_service);
        tcp::resolver::query data_query(hostname, strPort);
        tcp::resolver::iterator data_endpoint_iterator = resolver.resolve(data_query);

        // Try each endpoint until we successfully establish a connection.
        ssl_socket data_socket(data_io_service, ctx);
        boost::asio::connect(data_socket.lowest_layer(), data_endpoint_iterator);
        data_socket.lowest_layer().set_option(tcp::no_delay(true));

        // Perform SSL handshake and verify the remote host's
        // certificate.
        data_socket.set_verify_mode(boost::asio::ssl::verify_none);
        data_socket.set_verify_callback(ssl::rfc2818_verification(hostname));
        data_socket.handshake(ssl_socket::client);
        tcp::resolver data_time_server_resolver(io_service);

        boost::asio::streambuf responseData;
        boost::system::error_code errorData;

        if (!resume || filepath.empty() || localFilename.empty()) {
            std::string filenamenew = std::string(filename).substr(std::string(filename).find_last_of("/\\") + 1);
            checkfile(filenamenew);
            localFilename = filenamenew;
            if (path.empty()) {
                filepath = "./" + localFilename;
            } else {
                filepath = path + "/" + localFilename;
            }

        }
        std::ofstream MyFile;
        MyFile.open((filepath), std::ios::app);

        // Write whatever content we already have to output.
        int size = currentSize;
        if (responseData.size() > 0) {
            size += responseData.size();
            MyFile << &responseData;

        }

        while (boost::asio::read(data_socket, responseData,
                                 boost::asio::transfer_at_least(1), errorData)) {
            size += responseData.size();
            MyFile << &responseData;
            currentSize = size;
            if (pause) {
                MyFile.close();
                return 0;
            }
        }

        if (errorData != boost::asio::error::eof) {
            throw boost::system::error_code(errorData);
        }

        MyFile.close();
        setMsg("Yes");
        downloading = false;
        return 0;


    }
    catch (std::exception &e) {
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
    return currentSize / size * 100;
}

void download::checkfile(std::string &filename) {
    bool exists = true;
    int i = 1;
    std::string newFilenamecheck = filename;

    while (exists) {

        if (!path.empty()) {
            std::filesystem::current_path(path);
        }
        if (std::filesystem::exists(newFilenamecheck)) {
            if (i > 1) {
                newFilenamecheck = filename;
            }
            std::string token = newFilenamecheck.substr(0, newFilenamecheck.find("."));
            token += std::to_string(i);
            token += newFilenamecheck.substr(newFilenamecheck.find("."), newFilenamecheck.length());
            newFilenamecheck = token;
            i++;
        } else {
            exists = false;
            filename = newFilenamecheck;
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

bool download::isPause() const {
    return pause;
}

bool download::isResume() const {
    return resume;
}

bool download::isStop() const {
    return stop;
}

bool download::isDownloading() const {
    return downloading;
}

void download::setPriority(int priority) {
    download::priority = priority;
}

int download::getRetCode(const std::string &stringLine) {
    //std::cout << "som tu " << stringLine << std::endl;
    if (stringLine.size() < 3) {
        return -1;
    }
    size_t nPos = stringLine.find_first_of(" ");
    std::stringstream stream;
    std::string strCode = stringLine.substr(0, nPos);
    int nRetCode = std::stoi(strCode);
    //std::cout << "som tu code je " << nRetCode << std::endl;
    return nRetCode;
}


std::string download::getHostAndPort(std::string &strResult) {
    size_t nBegin = strResult.find("(");
    size_t nEnd = strResult.find(")");

    if (nBegin == -1 || nEnd == -1)
        return "";

    std::string strHost = strResult.substr(nBegin + 1, nEnd - nBegin - 1);
    nBegin = strHost.find_last_of(",");
    std::string strPort = strHost.substr(nBegin + 1, nEnd - nBegin + 1);
    int nPort = 0, tmp = 0;

    tmp = boost::lexical_cast<int>(strPort);
    strHost = strHost.substr(0, nBegin);

    nBegin = strHost.find_last_of(",");
    strPort = strHost.substr(nBegin + 1, nEnd - nBegin + 1);
    nPort = boost::lexical_cast<int>(strPort);
    nPort = 256 * nPort + tmp;
    strPort = boost::lexical_cast<std::string>(nPort);
    return strPort;
}


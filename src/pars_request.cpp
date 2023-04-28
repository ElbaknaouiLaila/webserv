
#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"
#include <sys/stat.h>
#include <dirent.h>

int hexToDigit(const std::string &hexStr) {
    char *endptr;
    long int num = strtol(hexStr.c_str(), &endptr, 16);
    if (*endptr != '\0') {
        return -1;
    }
    return num;
}

bool isHexadecimal(const std::string &str)
{
    std::stringstream ss(str); // to read integer(hexa) from a string
    int n;
    ss >> std::hex >> n; // to extract the integers one by one
    return !ss.fail() && ss.eof();
    // we use func fail() to check the extration opr performed by ss >> ..; was succ or not
}

bool Client::isDirectory(const char *path)
{
    struct stat info;
    if (stat(path, &info) != 0)
    {
        // Error occurred
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

//------------------------ Body of request------------------------//
int Client::getFirstBodyPart(std::string httpRequest, char *str, std::string delimiter, size_t size_read)
{
    if (Method == "GET")
    {
        if (GET_Response())
            return 1;
    }
    else if (Method == "DELETE")
    {
        if (access((URI).c_str(), F_OK) == -1)
        {
            StatusCode = "404 Not Found";
            set_error(true);
            set_has_request(true);
            return 1;
        }
        if (isDirectory(URI.c_str()))
        {
            if (URI.back() != '/')
            {
                StatusCode = "409 Conflict";
                set_error(true);
                set_has_request(true);
                return 1;
            }else{
                // if hase index file
            }
            int result = removeDirectory(URI.c_str());
            if (result != 0)
            {
                std::cout << "Error deleting directory" << std::endl;
                StatusCode = "403 Forbidden";
                set_error(true);
                set_has_request(true);
                return 1;
            }
            else
            {
                StatusCode = "204 No Content";
                set_error(true);
                set_has_request(true);
                return 0;
            }
        }
        else
        {
            int result = removeFile(URI.c_str());
            if (result == 0)
            {
                std::cout << "File deleted successfully" << std::endl;
                StatusCode = "204 No Content";
                set_error(true);
                set_has_request(true);
                return 0;
            }else{
                std::cout << "Error deleting file." << std::endl;
                set_has_request(true);
                return 1;
            }
            // error deleting file?
        }
    }
    else if (Method == "POST")
    {
        std::ofstream requestBody;
        size_t PosChunked = RequestHeaders["Transfer-Encoding"].find("chunked");
        requestBody.open("bodyRequestCopy" + FileExtension(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
        std::string ss(str, size_read);
        lenghtHeader = ss.find("\r\n\r\n") + 4;
        body.assign(ss.erase(0, lenghtHeader));
        if (PosChunked != std::string::npos)
        {
            size_t pos = 0;
            std::string token;
            while ((pos = body.find("\r\n")) != std::string::npos)
            {
                token = body.substr(0, pos);
                if (isHexadecimal(token))
                {
                    body.erase(0, pos + 2);
                    continue;
                }
                requestBody << token + "\r\n";
                body.erase(0, pos + 2);
            }
        }
        requestBody << body;
    }
    return 0;
}

void Client::getBody(const char *str, size_t size_read)
{
    size_t content_lenght = stoi(RequestHeaders["Content-Length"]);
    size_t PosBoundary = RequestHeaders["Content-Type"].find("boundary");
    size_t PosChunked = RequestHeaders["Transfer-Encoding"].find("chunked");

    if (PosChunked != std::string::npos)
    {
        ParseChunked(str, size_read);
    }
    else
    {
        Binary(str, size_read);
    }
}

size_t Client::FindStringInBuffer(const char *buffer, size_t buffer_size, const char *str, int pos)
{
    size_t str_len = strlen(str);
    if (buffer_size < str_len)
        return std::string::npos;
    if (pos == 0)
    {
        for (size_t i = 0; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0)
                return i;
    }
    else
    {
        for (size_t i = pos; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0)
                return i;
    }
    return std::string::npos;
}

void Client::ParseChunked(const char *str, size_t size_read)
{
    std::fstream requestBody;
    const char *buffer = str;
    size_t buffer_size = size_read;

    requestBody.open("bodyRequestCopy" + FileExtension(), std::ios::out | std::ios::app | std::ios::binary);
    while (buffer_size > 0)
    {
        size_t pos = 0;
        std::string token;
        while ((pos = FindStringInBuffer(buffer, buffer_size, "\r\n", 0)) != std::string::npos)
        {
            token = std::string(buffer, pos);
            if (isHexadecimal(token))
            {
                if (token == "0")
                {
                    requestBody.close();
                    set_has_request(true);
                    return;
                }
                buffer += pos + 2;
                buffer_size -= pos + 2;
                pos = 0;
                continue;
            }
            else
            {
                size_t pos2 = FindStringInBuffer(buffer, buffer_size, "\r\n", pos + 2);
                if (pos2 != std::string::npos)
                {
                    std::string nextToken = std::string(buffer + pos + 2, pos2 - pos - 2);
                    if (!isHexadecimal(nextToken))
                    {
                        token.append("\r\n");
                    }
                }
                else
                {
                    token.append("\r\n");
                }
            }
            requestBody.write(token.c_str(), token.size());
            buffer += pos + 2;
            buffer_size -= pos + 2;
            pos = 0;
        }
        requestBody.write(buffer, buffer_size);
        buffer_size = 0;
    }
    // move file pointer to the end of the file
    requestBody.seekg(0, std::ios::end);
    // get current position of file pointer, which is the file size
    lenghtBodyFile = requestBody.tellg();
    setlenghtBodyFile(lenghtBodyFile);
    requestBody.seekg(0, std::ios::beg);
    requestBody.close();
}

void Client::Binary(const char *str, size_t size_read)
{
    std::fstream requestBody;
    requestBody.open("bodyRequestCopy" + FileExtension(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
    requestBody.write(str, size_read);
    lenghtBody += size_read;
    if (stoi(RequestHeaders["Content-Length"]) == i - lenghtHeader)
    {
        // requestBody.seekg(0, std::ios::end);
        // lenghtBodyFile = requestBody.tellg();
        // setlenghtBodyFile(lenghtBodyFile);
        // requestBody.seekg(0, std::ios::beg);

        // ******compte contente of file

        set_has_request(true);
        requestBody.close();
    }
}

std::string Client::FileExtension()
{
    std::string path = RequestHeaders["Content-Type"];
    if (path.find("text/css") != std::string::npos)
        return ".css";
    if (path.find("text/csv") != std::string::npos)
        return ".csv";
    if (path.find("image/gif") != std::string::npos)
        return "gif";
    if (path.find("text/htm") != std::string::npos)
        return ".html";
    if (path.find("text/html") != std::string::npos)
        return ".html";
    if (path.find("video/mp4") != std::string::npos)
        return ".mp4";
    if (path.find("image/x-icon") != std::string::npos)
        return ".ico";
    if (path.find("image/jpeg") != std::string::npos)
        return ".jpeg";
    if (path.find("image/jpg") != std::string::npos)
        return ".jpeg";
    if (path.find("application/javascript") != std::string::npos)
        return ".js";
    if (path.find("application/json") != std::string::npos)
        return ".json";
    if (path.find("image/png") != std::string::npos)
        return ".png";
    if (path.find("application/pdf") != std::string::npos)
        return ".pdf";
    if (path.find("image/svg+xml") != std::string::npos)
        return ".svg";
    if (path.find("text/plain") != std::string::npos)
        return ".txt";
    return "";
}

void Client::RequestLine(size_t pos, std::string Request_headers)
{
    //------------------------ Method ------------------------//
    pos = Request_headers.find(" ");
    RequestHeaders["Method"] = Request_headers.substr(0, pos);
    Method = Request_headers.substr(0, pos);
    //------------------------ Request-URI ------------------------//
    size_t pos2 = Request_headers.find(' ', pos + 1);
    RequestHeaders["URI"] = Request_headers.substr(pos + 1, pos2 - pos - 1);
    URI = Request_headers.substr(pos + 1, pos2 - pos - 1);
    //------------------------ HTTP-Version ------------------------//
    pos = Request_headers.find("\r\n");
    RequestHeaders["Version"] = Request_headers.substr(pos2 + 1, pos - pos2);
    RequestHeaders["Version_nbr"] = Request_headers.substr(pos2 + 6, pos - pos2);
    Version = Request_headers.substr(pos2 + 1, pos - pos2);
    pos = URI.find("?");
    if (pos != std::string::npos)
    {
        RequestHeaders["Query"] = URI.substr(pos + 1);
        URI = URI.substr(0, pos);
    }
}

//------------------------ Parse the headers ------------------------//
void Client::ParseRequestHeaders(std::string Request_headers)
{
    size_t prev_pos = 0;
    size_t pos = 0;
    while ((pos = Request_headers.find("\r\n", prev_pos)) != std::string::npos)
    {
        std::string header_line = Request_headers.substr(prev_pos, pos - prev_pos);
        size_t colon_pos = header_line.find(':');
        if (colon_pos != std::string::npos)
        {
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            RequestHeaders[key] = value;
        }
        prev_pos = pos + 2;
    }
    pos = Request_headers.find("\r\n\r\n", prev_pos);
    std::string header_line = Request_headers.substr(prev_pos, pos - prev_pos);
    size_t colon_pos = header_line.find(':');
    if (colon_pos != std::string::npos)
    {
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 1);
        RequestHeaders[key] = value;
    }
    //------------------------ Port ------------------------//
    pos = RequestHeaders["Host"].find("localhost");
    Port = RequestHeaders["Host"].substr(pos + 10);
    RequestHeaders["Port"] = Port;
}

void Client::HTTP_Request_pars(std::string httpRequest, int size_read, const char *str, int i)
{
    std::string delimiter = "\r\n\r\n";
    size_t pos = httpRequest.find(delimiter);
    std::string Request_headers;
    if (pos != std::string::npos && isParsed == false)
    {
        Request_headers = httpRequest.substr(0, pos); // Here we store the header of the request
        RequestLine(pos, Request_headers);            // here we parse the Request line (Method, URI, http Version)
        ParseRequestHeaders(Request_headers);         // here we parse the header data
        if (FailCaseHeaderRequest() == 1)
        {
            isParsed = false;
            return;
        } // error handling
        if (!loc->getRedirectName().empty())
        {
            StatusCode = "302 Found";
            has_rediretion = true;
            URI = loc->getRedirectName();
            set_has_request(true);
            return;
        }
        if (getFirstBodyPart(httpRequest, (char *)str, delimiter, size_read))
        {
            return;
        }
        isParsed = true;
    }
    else if (Method == "POST" && size_read)
    {
        getBody(str, size_read);
        if (send413Response() == 1)
            return;
        if (get_has_request() == true)
            POST_Response();
    }
}
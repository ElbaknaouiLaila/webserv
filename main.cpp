#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <map>
int main() {
    std::string httpRequest = "POST / HTTP/1.1\r\n"
                              "Host: localhost:8080\r\n"
                              "Connection: keep-alive\r\n"
                              "Content-Length: 383\r\n"
                              "Cache-Control: max-age=0\r\n"
                              "sec-ch-ua-mobile: ?0\r\n"
                              "sec-ch-ua-platform: \"macOS\"\r\n"
                              "Upgrade-Insecure-Requests: 1\r\n"
                              "Origin: http://localhost:8080\r\n"
                              "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary1UdhAnnxETAnq932\r\n"
                              "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36\r\n"
                              "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
                              "Sec-Fetch-Site: same-origin\r\n"
                              "Sec-Fetch-Mode: navigate\r\n"
                              "Sec-Fetch-User: ?1\r\n"
                             " Sec-Fetch-Dest: document\r\n"
                              "Referer: http://localhost:8080/form.html\r\n"
                              "Accept-Encoding: gzip, deflate, br\r\n"
                              "Accept-Language: en,fr-FR;q=0.9,fr;q=0.8,en-US;q=0.7,ar;q=0.6,fi;q=0.5\r\n"
                              "\r\n"
                              "------WebKitFormBoundary1UdhAnnxETAnq932\r\n"
                              "Content-Disposition: form-data; name=comment\r\n"
                              "Content-Type: application/octet-stream\r\n"
                              "------WebKitFormBoundary1UdhAnnxETAnq932--\r\n";
    // Method
    std::string delimiter = "\r\n\r\n";
    size_t pos = httpRequest.find(delimiter);
    std::string header = httpRequest.substr(0, pos);
    // std::string body = httpRequest.substr(pos + delimiter.length());
    pos = header.find(" ");
    std::string method = header.substr(0, pos);
    if (method == "GET")
        std::cout<<"implement GET"<<std::endl;
    else if (method == "POST")
        std::cout<<"implement POST"<<std::endl;
    else if (method == "DELETE")
        std::cout<<"implement DELETE"<<std::endl;
    // Request-URI
    size_t pos2 = header.find(' ', pos + 1);
    std::string uri = header.substr(pos + 1, pos2 - pos - 1);
    // HTTP-Version
    pos = header.find("\r\n");
    std::string version = header.substr(pos2 + 1, pos - pos2);
    // Parse the headers
    std::map<std::string, std::string> header_map;
    size_t prev_pos = 0;
    while ((pos = header.find("\r\n", prev_pos)) != std::string::npos) {
        std::string header_line = header.substr(prev_pos, pos - prev_pos);
        size_t colon_pos = header_line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            header_map[key] = value;
        }
        prev_pos = pos + 2;
    }
    // Body of request

    std::string body;
    size_t content_lenght;
    std::map<std::string, std::string>::iterator itr = header_map.begin();
    for (; itr != header_map.end(); itr++)
    {
        std::string nbr = itr->second;
        if (itr->first == "Content-Length")
            content_lenght =stoi(nbr);
    }
    body = httpRequest.substr(httpRequest.find(delimiter) + delimiter.length(), content_lenght);
    std::ofstream request_body;
    request_body.open("body.txt");
    request_body << body;
    std::istringstream ss(body);
    std::string line;

    // while (std::getline(ss, line, '\r')) {
    // {
    //     std::cout<<line<<std::endl;
    //     /* code */
    // }
    
    request_body.close();
    
    return 0;
}

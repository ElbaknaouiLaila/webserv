
#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"
#include <sys/stat.h>
#include <dirent.h>

int hexToDigit(const std::string& hexStr)
{
    return std::stoi(hexStr, nullptr, 16);
}

bool isHexadecimal(const std::string& str) {
    std::stringstream ss(str);//to read integer(hexa) from a string
    int n;
    ss >> std::hex >> n; //to extract the integers one by one
    return !ss.fail() && ss.eof();
    //we use func fail() to check the extration opr performed by ss >> ..; was succ or not
}

bool isExiste(const std::string &str, std::map<std::string, std::string> header_map, std::string &val){
    std::string                                     str_find;
    std::map<std::string, std::string>::iterator    itr = header_map.begin();
    for (; itr != header_map.end(); itr++)
    {
        std::string key = itr->first;
        if (str == key){
            val = itr->second;
            return (0);
        }
    }
    return(1);
}

// [1xxs – Informational responses]
// [2xxs – Success!...............]
// [3xxs – Redirection............]
// [4xxs – Client errors..........]
// [5xxs – Server errors..........]

int error501( std::map<std::string, std::string> header_map){
    std::string val;
    std::string str = "Transfer-Encoding";
    if (isExiste(str, header_map, val) == 0)
    {
        if(val != " chunked"){
            std::cout<<"page HTML error 501 Not Implemented"<<std::endl;
            return 1;
        }
    }
    return 0;
}

int Client::error400(std::map<std::string, std::string> header_map, std::string uri)
{
    std::string valTe;
    std::string valCl;
    std::string strCl = "Content-Length";
    std::string strTe = "Transfer-Encoding";
    std::string buff = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";

    if ((isExiste(strTe, header_map, valTe) != 0 or isExiste(strCl, header_map, valCl) != 0 )and Method == "POST"){
        // if(valTe != " chunked"){
        //     std::cout<<"response error 400 Bad Request"<<std::endl;
        //     return 1;
        // }else if (isExiste(strCl, header_map, valCl) != 0){
        //     std::cout<<"response error 400 Bad Request"<<std::endl;
        //     return 1;
        // }
        std::cout<<"response error 400 Bad Request strTe "<<std::endl;
            return 1;
    }
    size_t i = 0;
    while (uri[i]){
        int ret = buff.find(uri[i]);
        if (ret == -1){
            std::cout<<"response error 400 Bad Request"<<std::endl;
            return(1);
        }
        i++;
    }
    return 0;
}

int error414(std::string uri)
{
    if (uri.size() > 2048){
        std::cout<<"response error 414 Request-URI Too Long"<<std::endl;
        return(1);
    }
    return(0);
}

int Client::error405(std::string method)
{
    std::vector<std::string>	AllowedMethod;
	std::vector<std::string>::iterator	AM;

    AllowedMethod = loc->getAllowedMethods();
    for (AM = AllowedMethod.begin(); AM != AllowedMethod.end(); AM++)
    {
        if ((*AM) == Method)
            return 0;
    }
    return 1;
}
int Client::error413()
{
    std::vector<Server *> ::iterator it;
    int lenght;
    for (it = ser.begin() ; it != ser.end(); it++)
    {
        if ((*it)->getPort() == stoi(Port))
        {
            lenght = (*it)->getClientSize();
            break;
        }
    }
    // std::cout<<"lenght "<<lenght<<std::endl;
    // std::cout<<"getlenghtBodyFile() "<<getlenghtBodyFile()<<std::endl;
    if (getlenghtBodyFile() > lenght)
    {
        StatusCode = "413 Request Entity Too Large";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    return 0;
}

// char find_string_before_substr(const std::string& str, const std::string& substr) {
//     size_t pos = str.find(substr);
//     if (pos != std::string::npos && pos > 0) {
//         return str[pos];
//     }
//     return '\0';
// }
std::string find_string(const std::string& str, const std::string& substr) {
    size_t pos = str.find(substr);
    std::cout << pos << std::endl;
    if (pos != std::string::npos) {
        std::cout << "pppp\n";
        if (substr[0] == '/')
            return str.substr(substr.size());
        return str.substr(substr.size()+1);
    }
    return str;
}

int Client::error404()
{
    std::vector<Server *>::iterator it;
    std::map<std::string, Location*>::iterator itr;
    std::map<std::string, Location*>	loc_;
    std::string UriCopy = URI;
    bool found = false;
    bool flag = false;
    std::cout<< "honaaaaaa\n";
    for (it = ser.begin() ; it != ser.end(); it++)
    {
        if ((*it)->getPort() == stoi(Port))
        {
            while (UriCopy.find_last_of('/') != 0 || found == false)
            {
                std::map<std::string, Location*>	location;
                location = (*it)->getContent();
                for (itr = location.begin(); itr != location.end(); itr++)
                {
                    std::cout<<"itr->first =========================================="<<(itr->first)<<std::endl;
                    std::cout<<"URICOPY    =========================================="<<(UriCopy)<<std::endl;
                    if (UriCopy == itr->first)
                    {
                        URI = (itr->second)->getRoot() + find_string(URI, (itr->second)->getRoot());
                        loc = itr->second;
                        std::cout << "inside URI : " << URI << std::endl;
                        return (0);
                    }
                    // if (itr->first == UriCopy && UriCopy.find_last_of('/') != 0){
                    //     std::cout<<"dose not hase /"<< "location : "<<itr->first <<"  uri : "<<UriCopy<<std::endl;
                    //     // if (URI != UriCopy)
                    //     // {
                    //     //     URI_loc = URI.substr(UriCopy.size());
                    //     //     URI = URI_loc;
                    //     // }
                    //         URI =(itr->second)->getRoot() + find_string(URI, (itr->second)->getRoot());
                    //     loc = itr->second;
                    //     DefaultIndex = (*it)->getDefaultIndex();
                    //     return 0;
                    // }
                    // if (UriCopy.find_last_of('/') == 0)
                    // {
                    //     std::cout<<"hase / "<<"location : "<<itr->first <<"  uri : "<<UriCopy<<std::endl;
                    //     if (itr->first == UriCopy)
                    //     {
                    //         // URI = URI;
                    //         std::cout<<"***URI "<<URI<< " +++ "<<std::endl;
                    //         flag = true;
                    //         loc = itr->second;
                    //         DefaultIndex = (*it)->getDefaultIndex();
                    //         URI =(itr->second)->getRoot() + find_string(URI, (itr->second)->getRoot());
                    //         return 0;
                    //     }
                    //     if (itr->first == "/")
                    //     {
                    //         found = true;
                    //         loc = itr->second;
                    //         DefaultIndex = (*it)->getDefaultIndex();
                    //         URI =(itr->second)->getRoot() + find_string(URI, (itr->second)->getRoot());
                    //     }
                    // }
                }
                // if (UriCopy.find_last_of('/') == 0 && found == false)
                // {
                //     std::cout<<"not found"<<std::endl;
                //     return(1);
                // }
                // if (UriCopy.find_last_of('/') != 0)
                UriCopy.erase(UriCopy.find_last_of('/'));
                if (UriCopy.size() == 0)
                {
                    UriCopy = "/";
                }
            }
                std::cout<<"----------------------------URI "<<URI<<std::endl;
                // std::cout<<"------ flag "<<flag<<std::endl;
                // std::cout<<"------ found "<<found<<std::endl;
            // if (flag == false && URI != "/")
            // {
            //     URI = URI.substr(1);
            // }
            if (found == true)
            {
                // URI = "/";
                return 0;
            }
            if (found == false)
            {
                return 1;
            }
        }
    }
    return(1);
}

int Client::GETUriError()
{
    if (URI.back() == '/')
    {
        std::string index_file_path = URI + "index.html";
        std::ifstream index_file(index_file_path.c_str());
        // If the index file exists
        if (index_file.good())
        {
            StatusCode = "200 OK";
        //     // set_error(true);
        //     // set_has_request(true);
            return 0;
        }
        else // If the index file doesn't exist
        {
        // std::cerr<<"URI--------------------"<<URI<<std::endl;;

            StatusCode = "404 Not Found"; // 403 Forbidden
            set_error(true);
            set_has_request(true);
            return 1;
        }
    }else // If the requested URI doesn't end with a slash
    {
        URI += "/";
        StatusCode = "301 Moved Permanently";
        set_error(true);
        set_has_request(true);
        return(1);
    }
    return(0);
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
        std::cout << "$$$$$$$ " << URI << std::endl;
        // URI = "/plankcvket";
    //------------------------ HTTP-Version ------------------------//
        pos = Request_headers.find("\r\n");
        RequestHeaders["Version"] = Request_headers.substr(pos2 + 1, pos - pos2);
        RequestHeaders["Version_nbr"] = Request_headers.substr(pos2 + 6, pos - pos2);
        Version = Request_headers.substr(pos2 + 1, pos - pos2);
        pos = RequestHeaders["Version"].find("?");
        if (pos != std::string::npos)
            RequestHeaders["Query"] = Version.substr(pos + 1);
}

//------------------------ Parse the headers ------------------------//
void Client::ParseRequestHeaders(std::string Request_headers)
{
    size_t prev_pos = 0;
    size_t pos = 0;
    while ((pos = Request_headers.find("\r\n", prev_pos)) != std::string::npos ) {
        std::string header_line = Request_headers.substr(prev_pos, pos - prev_pos);
        size_t colon_pos = header_line.find(':');
        if (colon_pos != std::string::npos){
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            RequestHeaders[key] = value;
        }
        prev_pos = pos + 2;
    }
    pos = Request_headers.find("\r\n\r\n", prev_pos);
    std::string header_line = Request_headers.substr(prev_pos, pos - prev_pos);
    size_t colon_pos = header_line.find(':');
    if (colon_pos != std::string::npos) {
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 1);
        RequestHeaders[key] = value;
    }
    //------------------------ Port ------------------------//
        pos = RequestHeaders["Host"].find("localhost");
        Port =  RequestHeaders["Host"].substr(pos + 10);
}

//------------------------ Fail case in request ------------------------//
int    Client::FailCaseHeaderRequest()
{
    if(error501(RequestHeaders))
    {
        StatusCode = "501 Not Implemented";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (error400(RequestHeaders, URI))
    {
        StatusCode = "400 Bad Request";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (error414(URI))
    {
        StatusCode = "414 Request-URI Too Long";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (error404())
    {
        std::cout<<"40000000000000004\n";
        std::cout<<Method<<std::endl;
        std::cout<<URI<<std::endl;
        StatusCode = "404 Not Found";
        set_error(true);
        set_has_request(true);
        return 1;
    }else 
    {
        std::cout<<"\n\nis done\n\n";
        // std::cout<<loc->getRedirectName()<<std::endl;
    }
    if (error405(Method)){//if method not allowded
        StatusCode = "405 Method Not Allowed";
        set_error(true);
            std::cout<<"\n\nMethod Allowed is done\n\n";
        set_has_request(true);
        return 1;
    }
    // if (error413(getlenghtBodyFile()))
    // {
    //     StatusCode = "413 Request Entity Too Large";
    //     set_error(true);
    //     set_has_request(true);
    //     return 1;
    // }
    
    // if (error413()) //befor body !!!
    // {
    //     StatusCode = "413 Request Entity Too Large";
    //     set_error(true);
    //     set_has_request(true);
    //     return 1;
    // }
    // if (UriError()){
    //     // StatusCode = "301 Moved Permanently";
    //     // set_error(true);
    //     // set_has_request(true);
    //     return 1;
    // }
    // std::cerr<<"--------------------NO ERROR HEADER--------------------"<<std::endl;
    // std::cerr<<"======================== "<< Method <<" ================================\n";

    return 0;
}
void Client::list_directory(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    // char *nedle[1] = '/';
    path = strstr(path, "/");
    std::cout<<"path-------------------->"<<path<<std::endl;
    struct dirent* entry;
    std::string html = "<html>\n<head>\n<title>Directory Listing</title>\n<h1>List Directory "+std::string(path)+"</h1>\n<hr>\n</head>\n<body>\n<ul>\n<li><a href=\"../\">../</a></li>\n";
    while ((entry = readdir(dir)) != NULL) {
       if (entry->d_name[0] != '.')
               html += "<li><a href=\""+ std::string(path)+ std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a></li>\n";
                std::cout << "hreeeef : " <<std::string(path)+ std::string(entry->d_name) << std::endl;
    }
       html += "</ul>\n<hr>\n</body>\n</html>";
        set_html_body_response(html);
        htmlString = true;
    closedir(dir);
}

bool isDirectory(const char* path) {
    struct stat info;
    if (stat(path, &info) != 0) {
        // Error occurred
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

int Client::GET_Request_Method()
{
    std::string URL;
    // if (URI == loc->getRoot())
        // URL = find_string_before_substr(loc->getRoot(), URI);
        // std::cout<<"find_string_before_substr(loc->getRoot(), URI)"<<find_string_before_substr(loc->getRoot(), URI)
    // URL = loc->getRoot()+URI;
    if (Method == "GET" and access((URI).c_str(), F_OK) == -1)// path and mode(F_OK: file exists)
        {
            std::cout<<"4040 Not Found access (Root+URI)"<<(URI)<<std::endl;
            StatusCode = "404 Not Found";
            set_error(true);
            set_has_request(true);
            return 1;
        }
        // URI = URI;
        DIR* is_dir = opendir(URI.c_str());
        // bool is_di = isDirectory(URI.c_str());
        // if (is_di == true)
            std::cout<<URI <<"rani directoryyyyyyyyyyyy\n";
        
        if (is_dir != NULL and Method == "GET")
        {
            if (Method == "GET" and URI.back() != '/' and access((URI).c_str(), F_OK) != -1) {
                // Return a 301 Moved Permanently response with the updated URL
                URI = URI + "/";
                StatusCode = "301 Moved Permanently";
            }
            if (loc->getAutoIndex() == "off" )
            {
                if(access(((URI) + (loc->getIndex())).c_str(), F_OK) != -1)
                {
                    URI = (URI) + (loc->getIndex());
                    set_has_request(true);
                    return(0);
                    //status code issssssssss?
                }else
                {
                    StatusCode = "403 Forbidden";
                    set_error(true);
                    set_has_request(true);
                    return 1;
                }
            }else if (loc->getAutoIndex() == "on")
            {
                if (loc->getIndex().size() > 0 && access(((URI) + (loc->getIndex())).c_str(), F_OK) != -1)
                {
                        std::cout << URI + loc->getIndex() << std::endl;
                        std::cout << "5645654564645\n";
                        URI = (URI) + (loc->getIndex());
                        set_has_request(true);
                        return(0);
                        //status code issssssssss?
                }
                else
                {
                    std::cout<<"*************(Root + URI )************* "<<(URI)<<std::endl;
                    list_directory((URI).c_str());
                    StatusCode = "200 OK";
                    set_has_request(true);
                }
            }
        }
        set_has_request(true);
        return 0;
}
//------------------------ Body of request------------------------//
int Client::getFirstBodyPart(std::string httpRequest, char *str, std::string delimiter, size_t size_read)
{
    if (Method == "GET" || Method == "DELETE")
    {
        if (GET_Request_Method())
            return 1;
    }
    if (Method == "POST")
    {
        std::ofstream   requestBody;
        size_t          PosChunked = RequestHeaders["Transfer-Encoding"].find("chunked");
        requestBody.open("bodyRequestCopy" +FileName(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
        std::string ss(str, size_read);
        lenghtHeader = ss.find("\r\n\r\n")+4;
        body.assign(ss.erase(0, lenghtHeader));
        if (PosChunked != std::string::npos)
        {
            size_t pos = 0;
            std::string token;
            while ((pos = body.find("\r\n")) != std::string::npos) {
            token = body.substr(0, pos);
                if (isHexadecimal(token))
                {
                    body.erase(0, pos + 2);
                    continue;
                }
                requestBody<<token + "\r\n";
                body.erase(0, pos + 2);
            }
        }
            requestBody << body;
    }
    return 0;
}

void    Client::getBody(const char *str, size_t size_read)
{
    // std::cerr<<"======================== header ================================\n";
    size_t content_lenght = stoi(RequestHeaders["Content-Length"]);
    size_t PosBoundary = RequestHeaders["Content-Type"].find("boundary");
    size_t PosChunked = RequestHeaders["Transfer-Encoding"].find("chunked");

    if (PosBoundary != std::string::npos){
        ParseBoundary(str, size_read);
    }else if (PosChunked != std::string::npos){
        ParseChunked(str, size_read);
    }else{
        Binary(str,  size_read);
    }
}

size_t Client::FindStringInBuffer(const char* buffer, size_t buffer_size, const char* str,int pos)
{
    size_t str_len = strlen(str);
    if (buffer_size < str_len)
        return std::string::npos;
    if (pos == 0)
    {
        for (size_t i = 0; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0) 
                return i;
    }else{
        for (size_t i = pos; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0)
                return i;
    }
    return std::string::npos;
}
void Client::ParseChunked(const char* str, size_t size_read)
{
    std::fstream   requestBody;
    const char*     buffer = str;
    size_t          buffer_size = size_read;

    // std::cerr<<"==========================chunked==============================\n";
    requestBody.open("bodyRequestCopy"+FileName(), std::ios::out | std::ios::app | std::ios::binary);
    while (buffer_size > 0) {
        size_t pos = 0;
        std::string token;
        while ((pos = FindStringInBuffer(buffer, buffer_size, "\r\n", 0)) != std::string::npos) {
            token = std::string(buffer, pos);
            if (isHexadecimal(token))
            {
                // std::cout << token << std::endl;
                if (token == "0"){
                    std::cout << "set to trueeeee\n";
                    // exit(1);
                    requestBody.close();
                    set_has_request(true);
                    return;
                }
                buffer += pos + 2;
                buffer_size -= pos + 2;
                pos = 0;
                continue;
            }else{
                size_t pos2 = FindStringInBuffer(buffer, buffer_size, "\r\n", pos+2);
                if (pos2 != std::string::npos){
                    std::string nextToken = std::string(buffer + pos + 2, pos2 - pos - 2);
                    if(!isHexadecimal(nextToken)){
                        token.append("\r\n");
                    }
                }else{
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
    // std::cout<<"lenghtBodyFile  : *****************************"<<getlenghtBodyFile()<<std::endl;
    //  set_has_request(true);
    requestBody.close();
}

void Client::ParseBoundary(const char *str, size_t size_read)
{
    int             pos;
    const char      *buffer = str;
    std::ofstream   requestBody;
    size_t          PosBoundary = RequestHeaders["Content-Type"].find("boundary");
    std::string     boundary_str = RequestHeaders["Content-Type"].substr(PosBoundary + 9);
    std::string     lastBoundary = "--" + boundary_str + "--";
    requestBody.open("bodyRequestCopy"+FileName(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
    requestBody.write(str, size_read);
    if (stoi(RequestHeaders["Content-Length"]) == i - lenghtHeader)
    {
        set_has_request(true);
        requestBody.close();
    }
    // if ((pos = FindStringInBuffer(buffer, size_read, lastBoundary.c_str(), 0)) != std::string::npos){
    //     set_has_request(true);
    //     requestBody.close();
    // }
}

void    Client::Binary(const char *str, size_t size_read)
{
    std::ofstream requestBody;
    requestBody.open("bodyRequestCopy"+FileName(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
    requestBody.write(str, size_read);
    lenghtBody += size_read;
    if (stoi(RequestHeaders["Content-Length"]) == i - lenghtHeader)
    {
        set_has_request(true);
        requestBody.close();
    }
}

std::string Client::FileName()
{
    std::string path = RequestHeaders["Content-Type"];
    if (path.find("text/css") != std::string::npos) return ".css";
    if (path.find("text/csv") != std::string::npos) return ".csv";
    if (path.find("image/gif") != std::string::npos) return "gif";
    if (path.find("text/htm") != std::string::npos) return ".html";
    if (path.find("text/html") != std::string::npos) return ".html";
    if (path.find("image/x-icon") != std::string::npos) return ".ico";
    if (path.find("image/jpeg") != std::string::npos) return ".jpeg";
    if (path.find("image/jpg") != std::string::npos) return ".jpeg";
    if (path.find("application/javascript") != std::string::npos) return ".js";
    if (path.find("application/json") != std::string::npos) return ".json";
    if (path.find("image/png") != std::string::npos) return ".png";
    if (path.find("application/pdf") != std::string::npos) return ".pdf";
    if (path.find("image/svg+xml") != std::string::npos) return ".svg";
    if (path.find("text/plain") != std::string::npos) return ".txt";
    return "";
}

void Client::HTTP_Request_pars(std::string httpRequest, int size_read, const char *str, int i)
{
    std::string delimiter = "\r\n\r\n";
    size_t      pos = httpRequest.find(delimiter);
    std::string Request_headers;
    if (pos != std::string::npos && isParsed == false)
    {
        Request_headers = httpRequest.substr(0, pos);               // Here we store the header of the request 
        RequestLine(pos, Request_headers);                          // here we parse the Request line (Method, URI, http Version)
        ParseRequestHeaders(Request_headers);                       // here we parse the header data
        if (FailCaseHeaderRequest() == 1){ isParsed = false; return ; }                                          // error handling
        if (!loc->getRedirectName().empty())
        {
            std::cout<<"redirection"<<std::endl;
            StatusCode = "302 Found";
            has_rediretion = true;
            URI = loc->getRedirectName();
            set_has_request(true);
            return;
        }
        if (getFirstBodyPart(httpRequest, (char *)str ,delimiter, size_read)){return;}
        // if  (error413() == 1){return;}
        std::cout<< "method : " << Method << std::endl;
        std::cout << "has_request : " << has_request << std::endl;
        isParsed = true;
    }
    else if (Method == "POST" && size_read)
    {
    // std::cerr<<"=========================ME"<<Method<<"===============================\n";
        getBody(str, size_read);
        if  (error413() == 1){return;}

    }
    // std::cerr<<"=========================ME"<<Method<<"++++++++===============================\n";

    //  if  (error413(lenghtBodyFile) == 1){return;}
}

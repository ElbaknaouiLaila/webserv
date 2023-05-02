#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"
#include <sys/stat.h>
#include <dirent.h>

// [1xxs – Informational responses]
// [2xxs – Success!...............]
// [3xxs – Redirection............]
// [4xxs – Client errors..........]
// [5xxs – Server errors..........]

int Client::stringToInt(const std::string& str) {
    int result;
    std::stringstream ss(str);
    ss >> result;
    return result;
}

std::string find_string(const std::string& str, const std::string& substr) {
    size_t pos = str.find(substr);
    if (pos != std::string::npos) {
        if (substr[0] == '/')
            return str.substr(substr.size());
        return str.substr(substr.size()+1);
    }
    return str;
}

// -----------------------[4xxs – Client errors..........]-----------------------[
int Client::send400Response(std::map<std::string, std::string> header_map, std::string uri)
{
    std::string buff = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";

    if (header_map.count("Content-Length") == 0 and header_map.count("Transfer-Encoding") == 0 and Method == "POST")
        return 1;
    if(uri.find_first_not_of(buff) != std::string::npos)
        return(1);
    return 0;
}

int Client::get_matched_location_for_request_uri()
{
    std::vector<Server *>::iterator it;
    std::map<std::string, Location*>::iterator itr;
    std::map<std::string, Location*>	loc_;
    std::string UriCopy = URI;
    bool found = false;
    // bool flag = false;
    for (it = ser.begin() ; it != ser.end(); it++)
    {
        if ((*it)->getPort() == stringToInt(Port))
        {
            while (UriCopy.find_last_of('/') != 0 || found == false)
            {
                std::map<std::string, Location*>	location;
                location = (*it)->getContent();
                for (itr = location.begin(); itr != location.end(); itr++)
                {
                    if (UriCopy == itr->first)
                    {
                        URI = (itr->second)->getRoot() + find_string(URI, (itr->second)->getRoot());
                        loc = itr->second;
                        //start check
                            if ((itr->second)->getRoot().find(URI) != std::string::npos)
                                std::cout<<"----------------> Found URI : "<< (itr->second)->getRoot()<<std::endl;
                        //end check
                        std::cout << "----------------> Found : "<< URI << "------>  " << itr->first << std::endl;
                        return (0);
                    }
                }
                UriCopy.erase(UriCopy.find_last_of('/'));
                if (UriCopy.size() == 0)
                    UriCopy = "/";
            }
            if (found == true)
                return 0;
            if (found == false)
                return 1;
        }
    }
    return(1);
}

int Client::is_method_allowed_in_location(std::string method)
{
    (void)method;
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

int Client::send413Response()
{
    std::vector<Server *> ::iterator it;
    int lenght = 0;
    for (it = ser.begin() ; it != ser.end(); it++)
    {
        if ((*it)->getPort() == stringToInt(Port))
        {
            lenght = (*it)->getClientSize();
            break;
        }
    }
    if (lenght and (getlenghtBodyFile() > lenght || lenght == 0))
    {
        StatusCode = "413 Request Entity Too Large";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    return 0;
}

int send414Response(std::string uri)
{
    if (uri.size() > 2048){
        std::cout<<"response error 414 Request-URI Too Long"<<std::endl;
        return(1);
    }
    return(0);
}

// -----------------------[5xxs – Server errors..........]-----------------------//
bool send501Response( std::map<std::string, std::string> header_map){
    std::string val;
    std::string str = "Transfer-Encoding";
    if (header_map["Method"] == "POST" && header_map.count("Transfer-Encoding") && header_map["Transfer-Encoding"] != " chunked")
    {
        std::cout<<"header_map[Transfer-Encoding] : "<<header_map["Transfer-Encoding"]<<std::endl;
        return 1;
    }
    if(header_map["Method"] != "POST" && header_map["Method"] != "GET" && header_map["Method"] != "DELETE")
        return 1;
    return 0;
}



int Client::GET_Response()
{
    std::string URL;
    DIR* is_dir = opendir(URI.c_str());
    //check index file

    if (Method == "GET" and access((URI).c_str(), F_OK) == -1)// path and mode(F_OK: file exists)
    {
        std::cout << "----------------> Not Found : "<< URI << std::endl;
        StatusCode = "404 Not Found";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (is_dir != NULL and Method == "GET")
    {
        if (Method == "GET" and URI.back() != '/' and access((URI).c_str(), F_OK) != -1) {
            // Return a 301 Moved Permanently response with the updated URL
            URI = URI + "/";
            StatusCode = "301 Moved Permanently";
        }
        if (loc->getIndex().size() > 0 and access(((URI) + (loc->getIndex())).c_str(), F_OK) != -1)
        {
                std::cout << "------> "<< find_extension(loc->getIndex()) << std::endl;
                std::cout << "------> "<< (URI)+loc->getIndex() << std::endl;
             if (find_extension(loc->getIndex()) == "py" and loc->getCgiPy().size())
            {
                cgi_GET_response((URI) + loc->getIndex());
                set_has_request(true);
                return(0);
            }else
            {
                StatusCode = "200 OK";
                // set_error(true);
                set_has_request(true);
            }
        }
        if (loc->getAutoIndex() == "off" )
        {
            if(access(((URI) + (loc->getIndex())).c_str(), F_OK) != -1){
                URI = (URI) + (loc->getIndex());
                set_has_request(true);
                return(0);
            }else{
                StatusCode = "403 Forbidden";
                set_error(true);
                set_has_request(true);
                return 1;
            }
        }else if (loc->getAutoIndex() == "on")
        {
            // if (loc->getIndex().size() > 0 && access(((URI) + (loc->getIndex())).c_str(), F_OK) != -1){
            //         URI = (URI) + (loc->getIndex());
            //         set_has_request(true);
            //         return(0);
            // }
            // else{
                list_directory((URI).c_str());
                StatusCode = "200 OK";
                set_has_request(true);
            // }
        }

    }
    else if (is_dir == NULL and Method == "GET")
    {
        if (find_extension(URI) == "py" and loc->getCgiPy().size())
        {
            std::cout << "rah dkhal hna " << std::endl;
            cgi_GET_response(URI);
            set_has_request(true);
            return(0);
        }
        StatusCode = "200 OK";
        set_has_request(true);
    }
    set_has_request(true);// ****set has request to true
    return 0;
}

int Client::removeFile(const char* filePath) {
    return std::remove(filePath);
}

int Client::removeDirectory(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* entry;
    int result = 0;

    if (dir == NULL) {
        std::cout << "Error opening directory" << std::endl;
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        std::string file_name = entry->d_name;
        std::string full_file_path = std::string(path) + "/" + file_name;

        if (file_name == "." || file_name == "..") {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            result = removeDirectory(full_file_path.c_str());
            if (result != 0) {
                return result;
            }
        } else {
            result = removeFile(full_file_path.c_str());
            if (result != 0) {
                return result;
            }
        }
    }

    closedir(dir);

    return rmdir(path);
}

std::string Client::find_extension(std::string filename)
{
    std::string extractedExtension;
    std::size_t dotPos = filename.rfind(".");
    if (filename.empty())
        return ("");
    if (dotPos == std::string::npos) {
        std::cout << "File has no extension" << std::endl;
    } else {
        extractedExtension = filename.substr(dotPos + 1);
        if (loc->getCgiPhp().size() and (extractedExtension == "php")) {
            set_cgiExtension("php");
            return "php";
        } else if(loc->getCgiPy().size() and (extractedExtension == "py")){
            set_cgiExtension("py");
            return "py";
        }
    }
    return extractedExtension;
}

int Client::POST_Response()
{
                std::cout <<"=== >> ELSE OF CGI "<<std::endl;
    //  struct stat info;
    std::string URI_upload;
    std::string upload_path = loc->getUpload();
    std::string file_name = "bodyRequest"+FileExtension();
    std::fstream   requestBody;
    std::fstream   upload_file;
    requestBody.open(file_name, std::ios::out | std::ios::app | std::ios::binary);
    // std::cout<<"====>URI : "<<URI<<std::endl;
    // upload_file.open((upload_path + file_name), std::ios::out | std::ios::app | std::ios::binary);
    if (upload_path.size())
    {
        std::string mv_path = upload_path + std::string("/");
        std::cout<<"====>Path : "<<mv_path<<std::endl;
        if (isDirectory((mv_path).c_str()) and access(upload_path.c_str(), F_OK) != -1)
        {
            std::cout<<"====================>is Directory"<<std::endl;
            if (rename(file_name.c_str(), (mv_path + file_name).c_str()) != 0)
            {
                std::cerr << "Failed to move file. or 404 Not Found"<<mv_path << std::endl;
                StatusCode = "404 Not Found";
                set_error(true);
                set_has_request(true);
                return 1;
            }else
            {
                std::cerr << "file move." << std::endl;
                std::cerr << RequestHeaders["Content-Type"] << std::endl;
                StatusCode = "201 Created";
                set_error(true);
                set_has_request(true);
                return(0);
            }
        }
    }else
    {
        if (isDirectory(URI.c_str()))
        {
            std::cout<<"RequestHeaders'info  "<<std::endl;
            std::map<std::string, std::string> ::iterator data;
            for (data= RequestHeaders.begin(); data != RequestHeaders.end(); data++)
		    {
		    	std::cout <<"* NAME  		 			:"<<data->first<<" ";
		    	std::cout <<"* VALUE          			:"<<data->second<<"|"<<std::endl;
		    }
            if (URI.back() != '/' and access((URI).c_str(), F_OK) != -1)
            {
                URI = URI + "/";
                StatusCode = "301 Moved Permanently";
            }
            if (URI.back() == '/' and find_extension(loc->getIndex()) == "php" and access((URI + loc->getIndex()).c_str(), F_OK) != -1 and loc->getCgiPhp().size() != 0)
            {
                set_file_CGI_name((URI + loc->getIndex()));
                //call cgi program
                cgi_response((*this), file_name,RequestHeaders,loc->getCgiPhp(), URI);

                
            }else if (URI.back() == '/' and find_extension(loc->getIndex()) == "py" and access((URI + loc->getIndex()).c_str(), F_OK) != -1 and loc->getCgiPy().size() != 0)
            {
                set_file_CGI_name((URI + loc->getIndex()));
                // int fd = open("bodyRequest" + FileExtension())
                // call cgi program
                cgi_response((*this),file_name,RequestHeaders,loc->getCgiPy(), URI);
                // cgi_response(RequestHeaders, "bodyRequest" + FileExtension());

            }else if (URI.back() == '/' and find_extension(loc->getIndex()) == "html" and access((URI + loc->getIndex()).c_str(), F_OK) != -1)
            {
                URI = (URI) + (loc->getIndex());
                set_has_request(true);
                return(0);
            }
            else
            {
                StatusCode = "403 Forbidden";
                set_error(true);
                set_has_request(true);
                return 1;
            }
        }else
        {
            std::string filename = URI.substr(URI.rfind("/"));
            find_extension(filename);
            // if requested file
                // run cgi on request file
                //ckeck extension of file if it is php or py
            std::cout << "==========> "<< (URI) << std::endl;
            if (get_cgiExtension() == "php" or get_cgiExtension() == "py")
            {
                if(get_cgiExtension() == "php"and access(URI.c_str(), F_OK) != -1 and loc->getCgiPhp().size() != 0)
                    cgi_response((*this),file_name,RequestHeaders,loc->getCgiPhp(), URI);
                else if(get_cgiExtension() == "py" and access(URI.c_str(), F_OK) != -1 and loc->getCgiPy().size() != 0)
                    cgi_response((*this),file_name,RequestHeaders,loc->getCgiPy(), URI);
                else
                {
                    StatusCode = "403 Forbidden";
                    set_error(true);
                    set_has_request(true);
                    return 1;
                }
            }else if(access(URI.c_str(), F_OK) != -1)
            {
                set_has_request(true);
                return(0);
            }else
            {
                StatusCode = "404 Not Found";
                set_error(true);
                set_has_request(true);
                return 1;
            }
        }
    }
    return(0);
}

void Client::list_directory(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    // char *nedle[1] = '/';
    std::cout << "path: " << path << std::endl;
    // path = strstr(path, "/");
    struct dirent* entry;
    std::string html = "<html>\n<head>\n<title>Directory Listing</title>\n<h1>List Directory "+std::string(path)+"</h1>\n<hr>\n</head>\n<body>\n<ul>\n<li><a href=\"../\">../</a></li>\n";
    while ((entry = readdir(dir)) != NULL) {
       if (entry->d_name[0] != '.')
               html += "<li><a href=\""+ std::string(path)+ std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a></li>\n";
    }
       html += "</ul>\n<hr>\n</body>\n</html>";
        set_html_body_response(html);
        htmlString = true;
    closedir(dir);
}


//------------------------ Fail case in request ------------------------//
int    Client::FailCaseHeaderRequest()
{
    if(send501Response(RequestHeaders))
    {
        StatusCode = "501 Not Implemented";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (send400Response(RequestHeaders, URI))
    {
        StatusCode = "400 Bad Request";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (send414Response(URI))
    {
        StatusCode = "414 Request-URI Too Long";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (get_matched_location_for_request_uri())
    {
        std::cout<<"=====================>here"<<std::endl;
        StatusCode = "404 Not Found";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    if (is_method_allowed_in_location(Method)){
        StatusCode = "405 Method Not Allowed";
        set_error(true);
        set_has_request(true);
        return 1;
    }
    // StatusCode = "301 Moved Permanently";
    return 0;
}
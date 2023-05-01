/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 22:51:57 by rmoujan           #+#    #+#             */
/*   Updated: 2023/05/01 23:08:30 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"


void Client::cgi_GET_response(std::string filename)
{
   char *args[3];
        args[0] = strdup("python");
           args[1] = strdup(filename.c_str());
        args[2]=  nullptr; // replace "script.py" with the path to your Python file

        char *env[] = { nullptr };

        int fd = open("./GET_files/outputfile.txt",  O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        int outpt = dup(1);
        dup2(fd, 1);

        if (fork() == 0)
        {
            if (execve("/usr/bin/python", args, env) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(NULL);
            dup2(outpt, 1);
            set_URI("GET_files/outputfile.txt");
            std::cout <<"safii salat \n";
        }
}
void cgi_response(Client &object, std::string body_req, std::map<std::string, std::string> RequestHeaders, std::string script,std::string URI)
{
    std::string extension;
    std::string fullpath;
    std::cout<<"=========== >> BEGIN CGI RESPONSE "<<std::endl;
    // trim(str, c);
    // str.erase(0, str.find_first_not_of(" "));
    std::string content_type = std::string("CONTENT_TYPE=") + RequestHeaders["Content-Type"].erase(0, RequestHeaders["Content-Type"].find_first_not_of(" "));
    std::cout <<"Content type "<<content_type<<std::endl;
    std::string content_length = std::string("CONTENT_LENGTH=") + RequestHeaders["Content-Length"].erase(0, RequestHeaders["Content-Length"].find_first_not_of(" "));
    std::cout <<"Content length "<<content_length<<std::endl;
    std::string script_name = std::string("SCRIPT_NAME=") + URI.erase(0, URI.find_first_not_of(" "));
    std::cout <<"Script name "<<script_name<<std::endl;
    std::string script_filename = std::string ("SCRIPT_FILENAME=") + URI.erase(0, URI.find_first_not_of(" "));
    std::cout <<"Script filename "<<script_filename<<std::endl;
    std::string path_info = std::string("PATH_INFO=") + URI.erase(0, URI.find_first_not_of(" "));
    std::cout <<"Path info "<<path_info<<std::endl;
    std::string query_str  = std::string ("QUERY_STRING=") + RequestHeaders["Query"].erase(0, RequestHeaders["Query"].find_first_not_of(" "));
    std::cout <<"Query string "<<query_str<<std::endl;
    std::string http_cookie = std::string ("HTTP_COOKIE=") + RequestHeaders["Cookie"].erase(0, RequestHeaders["Cookie"].find_first_not_of(" "));
    std::cout <<"Http cookie "<<http_cookie<<std::endl;
    std::string request_method = std::string ("REQUEST_METHOD=") + RequestHeaders["Method"].erase(0, RequestHeaders["Method"].find_first_not_of(" "));
    std::cout <<"Request method "<<request_method<<std::endl;
    // std::string protocol = std::string ("SERVER_PROTOCOL=HTTP/1.1") ;
    // std::cout <<"00000000000 Version:"<<RequestHeaders["Version"]<<std::endl;
    std::string protocol = std::string ("SERVER_PROTOCOL=") +  RequestHeaders["Version"].erase(0, RequestHeaders["Version"].find_first_not_of(" "));
    std::cout <<"Protocol "<<protocol<<std::endl;
    std::string redirect = std::string ("REDIRECT_STATUS=true");
    std::string url = std::string("REQUEST_url=") + URI.erase(0, URI.find_first_not_of(" "));
    std::cout <<"Url "<<url<<std::endl;
    std::string host = std::string("HTTP_HOST=") + RequestHeaders["Host"].erase(0, RequestHeaders["Host"].find_first_not_of(" "));
    std::cout<<"Host "<<host<<std::endl;
    std::string port = std::string("SERVER_PORT=") + RequestHeaders["Port"].erase(0, RequestHeaders["Port"].find_first_not_of(" "));
    std::cout<<"Port "<<port<<std::endl;
    std::string gateway_interface = std::string("GATEWAY_INTERFACE=CGI/1.1");
    std::cout<<"********script to run"<<script<<std::endl;
    std::cout <<"=========== >> END CGI RESPONSE "<<std::endl;
    char *envp[] = {
    (char *) content_length.c_str(),
    (char *) content_type.c_str(),
    (char *) request_method.c_str(),
    (char *) query_str.c_str(),
    (char *) script_name.c_str(),
    (char *) path_info.c_str(),
    (char *) http_cookie.c_str(),
    (char *) script_filename.c_str(),
    (char *) protocol.c_str(),
    (char *) redirect.c_str(),
    (char *) url.c_str(),
    (char *) host.c_str(),
    (char *) port.c_str(),
    (char *) gateway_interface.c_str(),
    NULL
  };

    char *args[] = {
    (char *) script.c_str(),
    (char *) (URI.erase(0, URI.find_first_not_of(" "))).c_str(),
    NULL 
  };
	const char* var = "outfilecgi";
  int fd_file = open(var, O_CREAT | O_RDWR | O_TRUNC, 0666);
  int fd_req  = open(body_req.c_str(), O_RDWR  , 0666);
  int stdout_copy = dup(1);
  // int stdin_copy = dup(0);
  pid_t pid;
	pid = fork();
  if (pid < 0) {
      std::cout << "Fork failed." << std::endl;
      return ;
  }
  else if (pid == 0) {
    dup2(fd_file,1);
    dup2(fd_req, 0);
    close(fd_file);
    close(fd_req);
    std::cerr << "Child process." << std::endl;
    // args[1] = "/Users/rmoujan/Desktop/webserv_team/GET_files/process_form.php";
    std::cerr <<"args[0]:"<<args[0]<<std::endl;
    std::cerr <<"args[1]:"<<args[1]<<std::endl;
    if (execve(args[0], args, envp) == -1)
    {
      std::cerr << "Error: " << strerror(errno) << std::endl;
    }
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
    std::cout << "Parent process." << std::endl;
    dup2(stdout_copy,1);
    close(stdout_copy);
    close(fd_file);
    close(fd_req);
    
    //must put the output of outputcgi in astring after removing the header from it :
    // int fd = open(var, O_RDWR, 0666);
    // std::string result;
    // result = read(fd, (*int)1024);
    std::cout <<"================ 2090 =============== "<<std::endl;
     std::ofstream outfile("outfilecgi", std::ios_base::app);
    std::string vers = RequestHeaders["Version"].erase(0, RequestHeaders["Version"].find_first_not_of(" "));
    std::size_t last_char_pos = vers.find_last_not_of("\r\n");

    // If a non-newline character was found, erase the newline characters and everything after them
    if (last_char_pos != std::string::npos) {
        vers.erase(last_char_pos + 1);
    }
    std::ifstream infile("outfilecgi");
    std::ostringstream contents;
    contents << infile.rdbuf();
    size_t pod =  contents.str().find("\r\n\r\n");
    std::cout << "contents: " << contents.str().substr(pod+4) << std::endl;
    std::string str_cgi = "<!DOCTYPE html>" + std::string("<html><body><p>") +contents.str().substr(pod+4)  + "</p></body></html>";
    // object.set_str_cgi(str_cgi);
    object.set_html_body_response(str_cgi);
    object.set_status(" 200 OK");
     object.set_html_string(true);
    // std::cout <<"str_cgi: "<<contents.str().substr(pod+4)<<std::endl;
    infile.close();
    // std::ofstream outfile("resultcgi");
    // if (outfile.is_open()) {
    // outfile << vers <<" 200 OK" << std::endl;
    // outfile << contents.str();
    // outfile.close();
    std::cout << "Content added to beginning of file." << std::endl;
    // }
    // std::ifstream infile("outfilecgi");
    // std::string line;
    // std::
    // while (std::getline(infile, line)) {
    //   std::cout << line << std::endl;
    // }
    // infile.close();
  }
  std::remove("/Users/lelbakna/Desktop/webserv_team/bodyRequest");
  std::remove("/Users/lelbakna/Desktop/webserv_team/outfilecgi");
  std::cout <<"================= END ================= "<<std::endl;
}
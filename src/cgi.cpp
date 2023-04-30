/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 22:51:57 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/29 15:29:43 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"



// arg[0] =  /Users/rmoujan/Desktop/Mainweb/cgi-bin/php-cgi //THIS IS THE EXECUTABLE
// arg[1] = /Users/rmoujan/Desktop/Mainweb/test/test.php // THIS IS THE FILE REQUESTED
// arg[2] = NULL
// args[1] howa li aykon f script name script filename and path info

void Client::cgi_GET_response(std::string filename)
{
   char *args[3];
        args[0] = strdup("python");
           args[1] = strdup(filename.c_str());
        args[2]=  nullptr; // replace "script.py" with the path to your Python file

        char *env[] = { nullptr };

        int fd = open("./GET_files/outputfile",  O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
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
            set_URI("GET_files/outputfile");
            std::cout <<"safii salat \n";
        }
}
void Client::cgi_response(std::string body_req, std::map<std::string, std::string> RequestHeaders, std::string script,std::string URI)
{
    std::string extension;
    std::string fullpath;
  std::cout<<"=========== >> FROM CGI RESPONSE "<<std::endl;
  //   std::map<std::string, std::string> ::iterator data;
  // for (data= RequestHeaders.begin(); data != RequestHeaders.end(); data++)
	// 	{
	// 		std::cout <<"* NAME  		 			:"<<data->first<<" ";
	// 		std::cout <<"* VALUE          			:"<<data->second<<"|"<<std::endl;
	// 	}
  
    // if (extension.compare("php") == 0)
    // {
    //   script = "php-cgi";//the name of script that will be executed by cgi
    //   fullpath = "./scripts/php-cgi";//the full path of script
    // }
    // else if (extension.compare("py") == 0)
    // {
    //   script = "py-cgi";//the name of script that will be executed by cgi
    //   fullpath = "./scripts/py-cgi";//the full path of script
    // }
    //location/file_requested.php ==>homa li at7tihum f path info 
    std::string content_type = std::string("CONTENT_TYPE=") + RequestHeaders["Content-Type"];
    if (content_type.find('\r') != std::string::npos)
      std::cout<<"found"<<std::endl;
      // content_type.erase(content_type.find('\r'));
    std::cout <<"Content type "<<content_type<<std::endl;
    std::string content_length = std::string("CONTENT_LENGTH=") + RequestHeaders["Content-Length"];
    std::cout <<"Content length "<<content_length<<std::endl;
    std::string script_name = std::string("SCRIPT_NAME=") + URI;
    std::cout <<"Script name "<<script_name<<std::endl;
    std::string script_filename = std::string ("SCRIPT_FILENAME=") + URI;
    std::cout <<"Script filename "<<script_filename<<std::endl;
    std::string path_info = std::string("PATH_INFO=") + URI;
    std::cout <<"Path info "<<path_info<<std::endl;
    // ==> homa galo f google que PATH_INFO HOWA MORA SCRIPT li F URL (o mashi URL kaml)
    //sara7a ma3rftsh shnu dawer deyal had script, 7it and deja kan seti script_name and script_filename , so what is role of script li f URL ??
    std::string query_str  = std::string ("QUERY_STRING=") + RequestHeaders["Query"];
    std::cout <<"Query string "<<query_str<<std::endl;
    std::string http_cookie = std::string ("HTTP_COOKIE=") + RequestHeaders["Cookie"];//still confused ??
    std::cout <<"Http cookie "<<http_cookie<<std::endl;
    std::string request_method = std::string ("REQUEST_METHOD=") + RequestHeaders["Method"];
    std::cout <<"Request method "<<request_method<<std::endl;

  
    std::string protocol = std::string ("SERVER_PROTOCOL=") +RequestHeaders["Version"];
    std::cout <<"Protocol "<<protocol<<std::endl;
    // std::string software = std::string ("SERVER_SOFTWARE=WEBSERV");
    std::string redirect = std::string ("REDIRECT_STATUS=true");
    std::string url = std::string("REQUEST_url=") + URI;
    std::cout <<"Url "<<url<<std::endl;
    std::string host = std::string("HTTP_HOST=") + RequestHeaders["Host"];
    std::cout<<"Host "<<host<<std::endl;
    std::string port = std::string("SERVER_PORT=") + RequestHeaders["Port"];
    std::cout<<"Port "<<port<<std::endl;
    std::string gateway_interface = std::string("GATEWAY_INTERFACE=CGI/1.1");
    std::cout<<"********script to run "<<script<<std::endl;
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
    // (char *) software.c_str(),
    (char *) redirect.c_str(),
    (char *) url.c_str(),
    (char *) host.c_str(),
    (char *) port.c_str(),
    (char *) gateway_interface.c_str(),
    NULL
  };

    char *args[] = {
    (char *) script.c_str(),   // Name of the executable to run
    (char *) script_filename.c_str(),   // Name of file requested
    NULL   // End of the argument list
  };
	const char* var = "outfilecgi";
  int fd_file = open(var, O_CREAT | O_RDWR | O_TRUNC, 0666);
  int fd_req  = open(body_req.c_str(), O_RDWR , 0666);

  // char buffer[256];
  // ssize_t bytesRead = read(fd_req, buffer, sizeof(buffer)); // read from file
  // if (bytesRead < 0) {
  //     std::cerr << "Error reading file." << std::endl;
  //       // std::cout <<"Error reading file." << endl;
  //       return ;
  //   }

  // std::cout << "Read " << bytesRead << " bytes from file: " << buffer << std::endl;

  // std::cout<<"fd_req "<<fd_req<<std::endl;
  int stdout_copy = dup(1);
  int stdin_copy = dup(0);
  // (void)stdin_copy;
  // (void)stdout_copy;
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
    std::cout << "Child process." << std::endl;
    
    if (execve(args[0], args, envp) == -1)
    {
      std::cerr<<"Error in execve"<<std::endl;
        ft_errnoo(2);
    }
      // Code to be executed by child process
  }
  else
  {
    // int fd_file = open(var, O_CREAT | O_RDWR | O_TRUNC, 0666);
    
    //restart 1 and 0 :
    //hena ate9eray dak file output bash teb9ay tesiftih lserver
    dup2(stdout_copy,1);
    dup2(stdin_copy, 0);
    // close(stdin_copy);
    // close(stdout_copy);
      int status;
        waitpid(pid, &status, 0);
    std::cout << "Parent process." << std::endl;
    // Code to be executed by parent process
  }

}
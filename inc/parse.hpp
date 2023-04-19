/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 21:11:33 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/16 13:37:47 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdio.h>  

typedef struct help myhelp;
struct help{
 	int								brace_server;
	int								brace_location;
	help()
	{
		brace_location = 0;
		brace_server = 0;
	}
};

// hena andir data li katkon west locations !!!!
class Location{

	std::vector<std::string>	allowed_methods;
	std::vector<std::string>	cgi;
	std::string					redirect_name;
	std::string					autoindex;
	std::string					root;
	std::string					index;
	public:
	Location()
	{
	}
	~Location()
	{
	}
	void setAllowedMethods(std::vector<std::string> value)
	{
		allowed_methods = value;
	}

	std::vector<std::string> getAllowedMethods()const{
		return (allowed_methods);
	}
	void setRedirectName(std::string value)
	{
		redirect_name = value;
	}

	std::string getRedirectName()const{
		return (redirect_name);
	}
	
	void setAutoIndex(std::string value)
	{
		autoindex = value;
	}

	std::string getAutoIndex()const{
		return (autoindex);
	}

	void setRoot(std::string value)
	{
		root = value;
	}

	std::string getRoot()const{
		return (root);
	}


	void setIndex(std::string value)
	{
		index = value;
	}

	std::string getIndex()const{
		return (index);
	}

	void setCgi(std::vector<std::string> value)
	{
		cgi = value;
	}

	std::vector<std::string> getCgi()const{
		return (cgi);
	}
};

class Server{

	std::map<int, std::string>		error_page;
	std::map<std::string, Location*>	content;
	std::string						server_name;
	std::string						default_root;
	std::string						location_name;
	std::string						default_index;
	std::string						host;
	int								port;
	int								client_size;

	public:
	Server()
	{

		this->client_size = 0;
		this->port = 0;

	}
	~Server()
	{

	}

	void setDefaultIndex(std::string value)
	{
		default_index = value;
	}
	void setDefaultRoot(std::string value)
	{
		default_root = value;
	}
	void setServerName(std::string value)
	{
		server_name = value;
	}
	void setLocationName(std::string value)
	{
		location_name = value;
	}
	void setHost(std::string  value)
	{
		host = value;
	}
	void setPort(int value)
	{
		port = value;
	}
	void setClientSize(int value)
	{
		client_size = value;
	}
	void setErrorPage(std::map<int, std::string> &error_map)
	{
		error_page = error_map;
	}
	void setContent(std::map<std::string, Location*>	&content_map)
	{
		content = content_map;
	}
	//getters :
	std::string getDefaultIndex()const
	{
		return (default_index);
	}
	std::string getDefaultRoot()const
	{
		return (default_root);
	}
	std::string getServerName()const
	{
		return (server_name);
	}
	std::string getLocationName()const
	{
		return (location_name);
	}
	std::string getHost()const
	{
		return (host);
	}
	int getPort()const
	{
		return (port);
	}
	int getClientSize()const
	{
		return (client_size);
	}
	std::map<int, std::string> getErrorPage()
	{
		return(this->error_page);
	}
	std::map<std::string, Location*> getContent()const
	{
		return (content);
	}
};

#endif

void ft_errnoo(int code);
int check_input_file(char *filee);
Server* newServer();
Server * get_data_of_server(std::ifstream &infile, myhelp &flag);
void get_servers(char *filee, std::vector<Server *> &servers);
void listen_directive(std::string data, Server *obj);
void server_name_directive(std::string server_name, Server *obj);
void client_max_directive(std::string client_max, Server *obj);
void error_page_directive(std::string error, std::map<int, std::string> &error_map);
void root_directive(std::string root, Server *obj);
void index_directive(std::string index, Server *obj);

void directive_allow_methods(Location *loc, std::string methods);
void directive_redirect(Location *loc, std::string redirect_name);
void directive_autoindex(Location *loc, std::string autoindex);
void root_location(Location *loc, std::string root);
void index_location(Location *loc, std::string index);
void directive_accept_cgi(Location *loc, std::string cgi);

Location* location_directive(std::ifstream &infile, Server *obj, std::string name);
int rbrace_check(std::string data);
int lbrace_check(std::string data);
int skipe_whitespace_all(std::string data);



    // location name2
    // {     
    //     allow_methods POST 
    //     redirect http://www.youtube.com/ 
    //     autoindex 20 
    //     root fullpath2090
    //     index rima.html
    //     accept_cgi jee spring
    // }
    // location name3
    // {     
    //     allow_methods DELETE 
    //     redirect http://www.facebook.com/ 
    //     autoindex 30 
    //     root fullpath7800
    //     index page.html
    //     accept_cgi .php .py
    // }

// 	#include <iostream>
// #include <string>
// #include <vector>

// using namespace std;

// int main()
// {
//     // Get the requested URI from the HTTP request.
//     string requested_uri = "/some/path";

//     // Define the list of locations that your web server can handle.
//     vector<string> locations = {"/", "/home", "/about", "/contact"};

//     // Check if the requested URI matches any location.
//     bool found = false;
//     for (int i = 0; i < locations.size(); i++) {
//         if (requested_uri == locations[i]) {
//             found = true;
//             break;
//         }
//     }

//     // If there is a match, serve the corresponding content.
//     if (found) {
//         // Serve content for the matched location.
//         cout << "Content for " << requested_uri << endl;
//     }
//     // If there is no match, return a 404 error.
//     else {
//         // Return a 404 error with a custom error message.
//         cout << "HTTP/1.1 404 Not Found\r\n";
//         cout << "Content-Type: text/html\r\n\r\n";
//         cout << "<html><body><h1>404 Not Found</h1></body></html>" << endl;
//     }

//     return 0;
// }

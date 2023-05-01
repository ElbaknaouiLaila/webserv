/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:23:15 by mmanouze          #+#    #+#             */
/*   Updated: 2023/05/01 00:12:14 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/servers.hpp"
#include "../inc/clients.hpp"
#include "../inc/parse.hpp"

void output_servers(std::vector<Server *> servers)
{
	std::vector<Server *> ::iterator it;
	std::map<int, std::string>  error;
	std::map<int, std::string> ::iterator er;
	std::map<std::string, Location*>	location;
	std::map<std::string, Location*>::iterator	loc;
	std::vector<std::string>	cgi;
	std::vector<std::string>::iterator	cg;
	std::vector<std::string>	ways;
	std::vector<std::string>::iterator	wy;
	
	std::cout <<"\n\n\n\n\n"<<std::endl;
	for (it = servers.begin(); it != servers.end(); it++)
	{
		std::cout <<"Server name is					:"<<(*it)->getServerName()<<"."<<std::endl;
		std::cout <<"* HOST is 					:"<<(*it)->getHost()<<"."<<std::endl;
		std::cout <<"* PORT is 					:"<<(*it)->getPort()<<"."<<std::endl;
		std::cout <<"* client is 					:"<<(*it)->getClientSize()<<"."<<std::endl;
		std::cout <<"* DEFAULT ROOT is 				:"<<(*it)->getDefaultRoot()<<"."<<std::endl;
		std::cout <<"* DEFAULT INDEX is 				:"<<(*it)->getDefaultIndex()<<"."<<std::endl;
		std::cout <<"------ERROR PAGES ARE---------"<<std::endl;
		error = (*it)->getErrorPage();
		for (er = error.begin(); er != error.end(); er++)
		{
			std::cout <<"* ERROR CODE 		 			:"<<er->first<<" ";
			std::cout <<"* ERROR PAGE          			:"<<er->second<<"|"<<std::endl;
		}
		std::cout <<"------LOCATIONS  ARE---------"<<std::endl;
		location = (*it)->getContent();
		int i = 1;
		for (loc = location.begin(); loc != location.end(); loc++)
			{
				std::cout <<i<<") Location "<<std::endl;
				std::cout <<"* Location name is 				:"<<(loc->first)<<"."<<std::endl;
				std::cout <<"* Redirect_name is 				:"<<(loc->second)->getRedirectName()<<"."<<std::endl;
				std::cout <<"* Autoindex is 					:"<<(loc->second)->getAutoIndex()<<"."<<std::endl;
				std::cout <<"* ROOT is 					:"<<(loc->second)->getRoot()<<"."<<std::endl;
				std::cout <<"* INDEX is 					:"<<(loc->second)->getIndex()<<"."<<std::endl;
				// cgi = (loc->second)->getCgi();
				// std::cout <<"------CGI are---------"<<std::endl;
				// for (cg = cgi.begin(); cg != cgi.end(); cg++)
				// {
				// 	std::cout <<"* "<<(*cg)<<". ";
				// }
				std::cout <<"* CGI_PHP is 					:"<<(loc->second)->getCgiPhp()<<"."<<std::endl;
				std::cout <<"* CGI_PYTHON is 					:"<<(loc->second)->getCgiPy()<<"."<<std::endl;
				std::cout <<"* UPLOAD is 					:"<<(loc->second)->getUpload()<<"."<<std::endl;
				std::cout <<"\n------ALLOWED METHODS ARE---------"<<std::endl;
				ways = (loc->second)->getAllowedMethods();
				for (wy = ways.begin(); wy != ways.end(); wy++)
				{
					std::cout <<"* "<<(*wy)<<". ";
				}
				std::cout <<std::endl;
				i++;
			}
	}

}


int main(int argc, char *argv[])
{

	servers serv;
	std::vector<Server *> ser;
	if (argc == 2)
	{
		check_input_file(argv[1]);
		get_servers(argv[1], ser);
		output_servers(ser);
		// return (0);
	}

	serv.create_server_sockets(ser);

	serv.fill_fd();

	while (1)
	{
		serv.clients_waiting_room(ser);		\
	}

//  std::string str ("hello world.");
////  str.back() = '!';
//  std::cout << str.back() << '\n';


	return 0;
}

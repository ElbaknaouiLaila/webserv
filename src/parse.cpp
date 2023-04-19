/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:36:45 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/12 12:59:02 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"

Server* newServer()
{
	Server *obj = new Server();
	return (obj);
}

void skipe_empty_lines(std::ifstream &infile, std::string &data)
{
	while (data.empty() || skipe_whitespace_all(data))
	{
		std::cout <<"<"<<data<<">"<<std::endl;
		getline(infile, data);
	}
}

Server * get_data_of_server(std::ifstream &infile)
{
	std::string							data;
	std::map<int, std::string>			error_page;
	std::map<std::string, Location*>	content_location;
	Server								*obj;
	Location							*loc;

	getline(infile, data);
	skipe_empty_lines(infile, data);
	// std::cout <<"<"<<data<<">"<<std::endl;
	if (data.find("{") != (std::string::npos) && rbrace_check(data))
	{
		obj = newServer();
		while (getline(infile, data) && lbrace_check(data) == 0)//khas condition tegad (mzl mamfixiyash mzn)
		{
			// std::cout <<"data ("<<data<<")"<<std::endl;
			if (data.find("listen") != (std::string::npos))
			{
				listen_directive(data, obj);
			}
			else if (data.find("server_name")  != (std::string::npos))
			{
				server_name_directive(data, obj);
			}
			else if (data.find("client_max_body_size")  != (std::string::npos))
			{
				client_max_directive(data, obj);
			}
			else if (data.find("error_page")  != (std::string::npos))
			{
				error_page_directive(data, error_page);
			}
			else if (data.find("root")  != (std::string::npos))
			{
				root_directive(data, obj);
			}
			else if (data.find("index")  != (std::string::npos))
			{
				 index_directive(data, obj);
			}
			else if (data.find("location")  != (std::string::npos))
			{
				loc = location_directive(infile, obj, data);
				content_location.insert(std::pair<std::string, Location*>(obj->getLocationName(), loc));
				// std::cout <<"data loc |"<<data<<"|"<<std::endl;
			}
			else if (!data.empty())
			{
				std::cout <<"9"<<std::endl;	
				ft_errnoo(2);
			}
		}
	}
	else if (!data.empty() && skipe_whitespace_all(data) == 0)
	{
		ft_errnoo(2);
	}
	// skipe_empty_lines(infile, data);
	// std::cout <<"TEST"<<std::endl;
	if(!lbrace_check(data))
	{
		std::cout <<"APPLE |"<<data<<"|"<<std::endl;
		ft_errnoo(2);
	}
	obj->setErrorPage(error_page);
	obj->setContent(content_location);
	// std::cout <<"WELT |"<<data<<"|"<<std::endl;
	return (obj);
}

int skipe_whitespace_server(std::string data)
{
	int i = 0;
	while (data[i])
	{
		if (data[i] != ' ')
			break;
		i++;
	}
	if (data[i] != 's')
		return (0);
	return (1);
}

int skipe_whitespace_all(std::string data)
{
	int i = 0;
	while (data[i])
	{
		if (data[i] != ' ')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void get_servers(char *filee, std::vector<Server *> &servers)
{
	std::ifstream infile;
	std::string	  data;
	// myhelp flag;
	infile.open(filee);
	while (getline(infile, data))
	{
		if (data.find("server") != (std::string::npos) && skipe_whitespace_server(data))
		{
			//std::cout <<"1"<<std::endl;
			servers.push_back(get_data_of_server(infile));
		}
		else if (!data.empty() && skipe_whitespace_all(data) == 0)
		{
			ft_errnoo(2);
		}
	}
}

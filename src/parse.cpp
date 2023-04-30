/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:36:45 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/29 13:34:02 by lelbakna         ###   ########.fr       */
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
	if (infile.eof())
		ft_errnoo(6);
	while (data.empty() || skipe_whitespace_all(data))
	{
		if (infile.eof())
			ft_errnoo(6);
		getline(infile, data);
	}
}
// void check_empty_server(std::ifstream infile)
// {
// 	std::string	data;
// 	getline(infile, data);
// 	std::cout <<"From checking empty servers "<<data<<std::endl;
// 	if (data.find('}') == (std::string::npos))
// 	{
// 		ft_errnoo(2);
// 	}
// }

Server * get_data_of_server(std::ifstream &infile)
{
	std::map<std::string, Location*>	content_location;
	std::map<int, std::string>			error_page;
	std::string							data;
	Server								*obj = NULL;
	Location							*loc;

	getline(infile, data);
	skipe_empty_lines(infile, data);
	std::cout <<"data is |"<<data<<"|"<<std::endl;
	if ((data.find("{") != (std::string::npos)) && rbrace_check(data))
	{
		std::cout <<"data is "<<std::endl;
		obj = newServer();
		while (getline(infile, data) && lbrace_check(data) == 0)
		{
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
				check_duplicate_locations(obj->getLocationName(), content_location);
				content_location.insert(std::pair<std::string, Location*>(obj->getLocationName(), loc));

			}
			else if (!data.empty())
			{
				ft_errnoo(2);
			}
		}
	}
	else if (!data.empty() && skipe_whitespace_all(data) == 0)
	{
		ft_errnoo(2);
	}
	if(!lbrace_check(data) || data.empty())
	{
		ft_errnoo(2);
	}
	obj->setErrorPage(error_page);
	obj->setContent(content_location);
	return (obj);
}

int skipe_whitespace_server(std::string data)
{
	int i = 0;
	while (data[i])
	{
		if (data[i] != ' ')
		{
			std::cout <<"server check "<<std::endl;
			break;
		}
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

	infile.open(filee);
	while (getline(infile, data))
	{
		if (data.compare("server") == 0)
		{
			servers.push_back(get_data_of_server(infile));
		}
		else if (!data.empty() && skipe_whitespace_all(data) == 0)
		{
			ft_errnoo(2);
		}
	}
}
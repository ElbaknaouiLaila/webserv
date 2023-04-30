/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive_server.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:16:10 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/29 13:34:23 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"

void listen_directive(std::string listen, Server *obj)
{
	std::string		data;
	std::string		host;
	std::string		port;
	int number;
	int i = 0;

 	std::stringstream in(listen);
 	in >> data;
	if (data.compare("listen"))
		ft_errnoo(2);
	while ((in >> data))
	{
		std::stringstream str(data);
		std::getline(str, host, ':');
		std::getline(str, port, ':');
		if (host.empty() || port.empty())
			ft_errnoo(2);
		std::stringstream object;
    	object << port;
    	object >> number;
		obj->setHost(host);
		obj->setPort(number);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void server_name_directive(std::string server_name, Server *obj)
{
	std::string		data;
	std::string		host;
	std::string		port;
	int i = 0;

	std::stringstream in(server_name);
 	in >> data;
	if (data.compare("server_name"))
		ft_errnoo(2);
	while (( in >> data))
	{
		obj->setServerName(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void client_max_directive(std::string client_max, Server *obj)
{
	std::string		data;
	int number;
	int i = 0;

	std::stringstream in(client_max);
 	in >> data;
	if (data.compare("client_max_body_size"))
		ft_errnoo(2);
	while (( in >> data))
	{
		std::stringstream object;
    	object << data;
    	object >> number;
		obj->setClientSize(number);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void error_page_directive(std::string error, std::map<int, std::string>		&error_map)
{
	std::string		data;
	int number;
	int i = 0;

	std::stringstream in(error);
 	in >> data;
	if (data.compare("error_page"))
	{
		ft_errnoo(2);
	}	
	while (( in >> data))
	{
		std::stringstream object;
    	object << data;
    	object >> number;
		i++;
		if (in >> data)
		{
			error_map.insert(std::pair<int, std::string>(number, data));
			i++;
		}
	}
	if (i != 2)
	{
		ft_errnoo(2);
	}
}

void root_directive(std::string root, Server *obj)
{
	std::string		data;
	int i = 0;

	std::stringstream in(root);
 	in >> data;
	if (data.compare("root"))
		ft_errnoo(2);
	while (( in >> data))
	{
		obj->setDefaultRoot(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void index_directive(std::string index, Server *obj)
{
	std::string		data;
	int i = 0;

	std::stringstream in(index);
 	in >> data;
	if (data.compare("index"))
		ft_errnoo(2);
	while (( in >> data))
	{
		obj->setDefaultIndex(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

int rbrace_check(std::string data)
{
	int i = 0;
	while (data[i])
	{
		if (data[i] != '{' && data[i] != ' ')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int lbrace_check(std::string data)
{
	int i = 0;

	while (data[i])
	{
		if (data[i] != '}' && data[i] != ' ')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

Location* newLocation()
{
	Location *obj = new Location();
	return (obj);
}

void location_name(Server *obj, std::string name)
{
	std::string		data;
	std::string		host;
	std::string		port;
	int i = 0;

	std::stringstream in(name);
 	in >> data;
	if (data.compare("location"))
		ft_errnoo(2);
	while (( in >> data))
	{
		obj->setLocationName(data);
		i++;
	}
	
	if (i != 1)
		ft_errnoo(2);
}

Location* location_directive(std::ifstream &infile, Server *obj, std::string name)
{
	std::string data;
	Location *loc = NULL;
	location_name(obj, name);
	getline(infile, data);
	
	if (data.find("{")  != (std::string::npos) && rbrace_check(data))
	{
		loc = newLocation();
		while (getline(infile, data) && lbrace_check(data) == 0)
		{
			if (data.find("allow_methods") != (std::string::npos))
			{
				directive_allow_methods(loc, data);
			}
			else if (data.find("redirect")  != (std::string::npos))
			{
				directive_redirect(loc, data);
			}
			else if (data.find("autoindex")  != (std::string::npos))
			{
				directive_autoindex(loc, data);
			}
			else if (data.find("root")  != (std::string::npos))
			{
				root_location(loc, data);
			}
			else if (data.find("index")  != (std::string::npos))
			{
				index_location(loc, data);
			}
			else if (data.find("cgi_php")  != (std::string::npos))
			{
				// directive_accept_cgi(loc, data);
				 directive_cgi_php(loc,data);
			}
			else if (data.find("cgi_py")  != (std::string::npos))
			{
				// directive_accept_cgi(loc, data);
				 directive_cgi_py(loc,data);

			}
			else if (data.find("upload_pass")  != (std::string::npos))
			{
				// directive_accept_cgi(loc, data);
				 directive_upload(loc,data);

			}
			else if (!data.empty())
			{
				ft_errnoo(2);
			}
		}
		if (data.find('}') == (std::string::npos))
		{
			ft_errnoo(2);
		}
	}
	else
	{
		ft_errnoo(2);
	}
	return (loc);
}
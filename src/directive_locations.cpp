/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive_locations.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 02:17:14 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/29 13:34:35 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"


void directive_allow_methods(Location *loc, std::string methods)
{
	std::vector<std::string>			allowed_ways;
	std::vector<std::string>::iterator	it;
	std::string		data;
	// int number;
	int i = 0;

	std::stringstream in(methods);
 	in >> data;
	if (data.compare("allow_methods"))
	{
		std::cout <<"99"<<std::endl;
		ft_errnoo(2);
	}	
	while (( in >> data))
	{
		allowed_ways.push_back(data);
		i++;
	}
	if (i ==  0 || i > 3)
	{
		ft_errnoo(2);
	}
	loc->setAllowedMethods(allowed_ways);
}

void directive_redirect(Location *loc, std::string redirect_name)
{
	std::string		data;
	std::string		host;
	std::string		port;
	int i = 0;

	std::stringstream in(redirect_name);
 	in >> data;
	if (data.compare("redirect"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setRedirectName(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void directive_autoindex(Location *loc, std::string autoindex)
{
	std::string		data;
	std::string		host;
	std::string		port;
	int i = 0;

	std::stringstream in(autoindex);
 	in >> data;
	if (data.compare("autoindex"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setAutoIndex(data);

		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void root_location(Location *loc, std::string root)
{
	std::string		data;
	int i = 0;

	std::stringstream in(root);
 	in >> data;
	if (data.compare("root"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setRoot(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void index_location(Location *loc, std::string index)
{
	std::string		data;
	int i = 0;

	std::stringstream in(index);
 	in >> data;
	if (data.compare("index"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setIndex(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}


// void directive_accept_cgi(Location *loc, std::string cgi_name)
// {
// 	std::vector<std::string>			cgi;
// 	std::vector<std::string>::iterator	it;
// 	std::string		data;
// 	int number;
// 	int i = 0;

// 	std::stringstream in(cgi_name);
//  	in >> data;
// 	if (data.compare("accept_cgi"))
// 	{
// 		ft_errnoo(2);
// 	}	
// 	while (( in >> data))
// 	{
// 		cgi.push_back(data);
// 		i++;
// 	}
// 	if (i ==  0)
// 	{
// 		ft_errnoo(2);
// 	}
// 	loc->setCgi(cgi);
// }

void directive_cgi_php(Location *loc, std::string cgi_name)
{
	std::string		data;
	int i = 0;

	std::stringstream in(cgi_name);
 	in >> data;
	if (data.compare("cgi_php"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setCgiPhp(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void directive_cgi_py(Location *loc, std::string cgi_name)
{
	std::string		data;
	int i = 0;

	std::stringstream in(cgi_name);
 	in >> data;
	if (data.compare("cgi_py"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setCgiPy(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}

void directive_upload(Location *loc, std::string upload_name)
{
	std::string		data;
	int i = 0;

	std::stringstream in(upload_name);
 	in >> data;
	if (data.compare("upload_pass"))
		ft_errnoo(2);
	while (( in >> data))
	{
		loc->setUpload(data);
		i++;
	}
	if (i != 1)
		ft_errnoo(2);
}
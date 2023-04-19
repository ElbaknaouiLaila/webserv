/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive_locations.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 02:17:14 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/12 12:57:15 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"


void directive_allow_methods(Location *loc, std::string methods)
{
	std::vector<std::string>	allowed_ways;
	std::vector<std::string>::iterator	it;
	std::string		data;
	int number;
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


void directive_accept_cgi(Location *loc, std::string cgi_name)
{
	std::vector<std::string>	cgi;
	std::vector<std::string>::iterator	it;
	std::string		data;
	int number;
	int i = 0;

	std::stringstream in(cgi_name);
 	in >> data;
	if (data.compare("accept_cgi"))
	{
		ft_errnoo(2);
	}	
	while (( in >> data))
	{
		cgi.push_back(data);
		i++;
	}
	if (i ==  0)
	{
		ft_errnoo(2);
	}
	loc->setCgi(cgi);
}


// server    
// {                                                                                         
//     listen     localhost:8080
//     server_name    Reshe
//     client_max_body_size 100
//     error_page 400 ../errors/400.html
//     error_page 404 ../errors/404.html
//     error_page 700 ../errors/500.html 
//     error_page 800 ../errors/400.html
//     error_page 100 ../errors/404.html
//     error_page 300 ../errors/500.html
//     root allah.com
//     index index.html
//     location name1
//     {     
//         allow_methods GET POST DELETE 
//         redirect http://www.google.com/ 
//         autoindex 10 
//         root fullpath2023
//         index life.html
//         accept_cgi java script
//     }
//     location name2
//     {     
//         allow_methods POST 
//         redirect http://www.youtube.com/ 
//         autoindex 20 
//         root fullpath2090
//         index rima.html
//         accept_cgi jee spring
//     }
//     location name3
//     {     
//         allow_methods DELETE 
//         redirect http://www.facebook.com/ 
//         autoindex 30 
//         root fullpath7800
//         index page.html
//         accept_cgi .php .py
//     }
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers_utiles.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:05:19 by mmanouze          #+#    #+#             */
/*   Updated: 2023/04/19 02:07:14 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/servers.hpp"
#include "../inc/clients.hpp"


void servers::full_path(Client &object)
{
	std::string ok;
	std::string take;
	ok.append(object.get_URI());
	std::cout << "URI fih : " << object.get_URI() << std::endl;
	//ok = object.get_URI();
	std::cout << "dkhaaaaaaaaaal                 " << object.get_URI() << std::endl;

	if (object.get_URI().find("/favicon.ico") != std::string::npos)
	{
		take = "GET_files/ICON.ico";
		object.set_URI(take);
	}
	else if (object.get_error() == true)
	{
		if (object.get_status() == "501 Not Implemented")
			object.set_URI("ErrorPage/Error501.html");
		else if (object.get_status() == "400 Bad Request")
			object.set_URI("ErrorPage/Error400.html");
		else if (object.get_status() == "414 Request-URI Too Long")
			object.set_URI("ErrorPage/Error414.html");
		else if (object.get_status() == "405 Method Not Allowed")
			object.set_URI("ErrorPage/Error405.html");
		else if (object.get_status() == "413 Request Entity Too Large")
			object.set_URI("ErrorPage/Error413.html");
		else if (object.get_status() == "404 Not Found")
			object.set_URI("ErrorPage/Error404.html");
		else if (object.get_status() == "301 Moved Permanently")
			object.set_URI("ErrorPage/Error301.html");
		else if (object.get_status() == "403 Forbidden")
			object.set_URI("ErrorPage/Error403.html");
	}
	// else if (object.get_URI() == "/")
	// 	object.set_URI("GET_files/index.html");
	// else if (object.get_URI() != "GET_files/index.html")
	// {
	// 		take = "GET_files" + ok;
	// 		// take = ok;
	// 	object.set_URI(take);
	// }
}
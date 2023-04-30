/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmoujan <rmoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:18:39 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/28 19:45:39 by rmoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"

void free_servers(std::vector<Server *> servers)
{
	std::map<std::string, Location*>			location;
	std::vector<Server *> ::iterator			it;
	std::map<std::string, Location*>::iterator	loc;

	for (it = servers.begin(); it != servers.end(); it++)
	{
		location =  (*it)->getContent();
		for (loc = location.begin(); loc != location.end(); loc++)
		{
			// std::cout <<"from delete "<<(loc->second)->getRoot()<<std::endl;
			delete loc->second;
		}
		delete (*it);
	}
}
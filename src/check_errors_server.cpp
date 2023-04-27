/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors_server.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 01:35:49 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/26 19:55:58 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"

//must check if the name of location already exist in map :if yes show an error 
void check_duplicate_locations(std::string name, std::map<std::string, Location*> content_location)
{
	std::map<std::string, Location*>::iterator	loc;
	for (loc = content_location.begin(); loc != content_location.end(); loc++)
	{
		if (name.compare(loc->first) == 0)
		{
			ft_errnoo(5);
		}
	}
}
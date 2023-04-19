/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:19:55 by rmoujan           #+#    #+#             */
/*   Updated: 2023/04/12 13:00:27 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.hpp"
void ft_errnoo(int code) {

	if (code == 1) {
		std::cout << "ERROR: COULD NOT OPEN FILE" << std::endl;
	} else if (code == 2) {
		std::cout << "ERROR: WRONG FORMAT OF DATA" << std::endl;
	} else if (code == 3) {
		std::cout << "ERROR: FILE IS EMPTY" << std::endl;
	} else if (code == 4) {
		std::cout << "ERROR: PLEASE ENTER A FILE" << std::endl;
	}
	exit(0);
}

int check_input_file(char *filee)
{
	std::ifstream infile;
	std::string	  data;

	infile.open(filee);
	if (infile == 0)
	{
		ft_errnoo(1);
		return (0);
	}
	infile >> data;
	if (data.empty())
	{
		ft_errnoo(3);
		return (0);
	}
	return (1);
}
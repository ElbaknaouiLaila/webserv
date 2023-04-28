/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:00:20 by mmanouze          #+#    #+#             */
/*   Updated: 2023/04/28 18:16:17 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/clients.hpp"

Client::Client():request("NONE"), method_type("walo"),has_resp(false),isError(false), isWriting(false),has_request(false) ,sent_size(0), actual_size(0), file_fd(0),get_del(false), isParsed(false),htmlString(false),has_rediretion(false) ,i(0) 
{
}

Client::~Client() 
{
}

std::string Client::get_request() const
{
	return (request);
}

std::string Client::get_method() const
{
	return (Method);
}

size_t Client::get_sent_size() const
{
	return (sent_size);
}

size_t Client::get_actual_size() const
{
	return (actual_size);
}

FILE* Client::get_file_fd()
{
	return (file_fd);
}

bool Client::get_has_request() const 
{
	return (has_request);
}

void Client::set_request(const char* str)
{
	 if (request == "NONE")
		request.clear();
	request.append(str);

}

void Client::set_method(std::string type)
{
	if (method_type == "walo")
		method_type.clear();
	method_type = type;
}

void Client::set_sent_size(size_t s_size)
{
	sent_size = s_size;
}

void Client::set_actual_size(size_t a_size)
{
	actual_size = a_size;
}

void Client::set_file_fd(FILE * fd)
{
	file_fd = fd;
}

void Client::set_has_request(bool condition)
{
	has_request = condition;
}

void Client::clear()
{
	i = 0;
	get_del = false;
	has_resp = false;
	isParsed = false;
	has_request = false;

	file_fd = 0;
	sent_size = 0;
	actual_size = 0;
	lenghtHeader = 0;

	URI.clear();
	body.clear();
	Method.clear();
	request.clear();
	Version.clear();
	Response.clear();
	method_type.clear();


	//fclose(file_fd);

	//std::ofstream requestBody;
    //requestBody.open(FileExtension(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
	//requestBody.clear();
	//requestBody.close();
	//file->clear();
	//file->close();
}
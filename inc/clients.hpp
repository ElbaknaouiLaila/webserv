/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:52:04 by mmanouze          #+#    #+#             */
/*   Updated: 2023/05/02 13:35:20 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP


#include <errno.h>
#include <iostream>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/time.h>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "parse.hpp"

class Client
{
	private :
		bool 			get_del;
		bool 			has_resp;
		bool 			isParsed;
		bool 			has_request;
		bool			isError;
		bool 			isWriting;
		bool 			htmlString;
		bool 			has_rediretion;
		
		FILE			*file_fd;
		size_t 			sent_size;
		size_t			lenghtBody;
		size_t			actual_size;
		size_t			lenghtchunk;
		size_t			lenghtHeader;
		size_t			ContentLength;
		std::streampos	lenghtBodyFile;

		std::string		Port;
    	std::string		URI;
    	std::string		URI_loc;
		std::string 	body;
		std::string		Method;
		std::string 	request;
		std::string		Version;
		std::string 	Response;
		std::string 	method_type;
		std::string		StatusCode;
		std::string		DefaultIndex;
		std::string		cgiExtension;
		std::string 	file_CGI_name;
		std::fstream 	*file;
		std::vector<Server *> ser;
		std::string html_body_response;
		Location*	loc;
		std::string		n;
		std::string		str_cgi;
	public :
		int i ;
		std::map<std::string, std::string> RequestHeaders;
		// std::fstream request_body;
		Client();
		~Client();
		std::string 	get_str_cgi() const { return (str_cgi); }
		void 			set_str_cgi(std::string str)  { str_cgi = str; }
		std::string 	get_file_CGI_name() const { return (file_CGI_name); }
		void 			set_file_CGI_name(std::string filename)  { file_CGI_name = filename; }
		std::string 	get_html_body_response() const { return (html_body_response); }
		void 			set_html_body_response(std::string body)  { html_body_response = body; }
		std::vector<Server *>	getServer(){return (ser);}
		void	setServer(std::vector<Server *> serv){ser = serv;}
		std::streampos	getlenghtBodyFile(){return (lenghtBodyFile);}
		void	setlenghtBodyFile(std::streampos len){lenghtBodyFile = len;}
		void 			set_iswriting( bool cond) { isWriting = cond; }
		void			set_error(bool set) { isError = set; }
		void 			set_status(std::string state) { StatusCode = state; }

		Location* 		get_location() const { return (loc); }
		bool 			get_iswriting() const { return (isWriting); }
		bool 			get_error() const { return (isError); }
		bool 			get_met() const { return (get_del); }
		bool 			&has_response()  { return (has_resp); }
		bool 			get_has_request() const;
		bool 			get_html_string() const { return (htmlString); }
		bool 			get_has_redirection() const { return (has_rediretion); }
		void 			set_cgiExtension(std::string extension) { cgiExtension = extension; }
		std::string 	get_cgiExtension() const { return (cgiExtension); }
		std::string 	get_status() const { return (StatusCode); }
		std::string 	get_body() const { return (body); }
		std::string 	get_URI()  { return (URI); }
		std::string 	&getbody() {return body;};
		std::string 	get_request() const;
		std::string 	get_method() const;
		
		size_t 			get_sent_size() const;
		size_t 			get_actual_size() const;
		
		FILE* 			get_file_fd() ;
		
		void 			HTTP_Request_pars(std::string httpRequest, int size_read, const char *str, int i);
		void 			RequestLine(size_t pos, std::string Request_headers);
		void 			ParseRequestHeaders(std::string Request_headers);
		int 			getFirstBodyPart(char *str, size_t size_read);
		void    		getBody(const char *str, size_t size_read);
		void 			ParseChunked(const char *str, size_t size_read);
		void 			ParseBoundary(const char *str, size_t size_read);
		void    		getBodyType(std::string Request_headers);
		void    		Binary(const char *str, size_t size_read);
		size_t 			FindStringInBuffer(const char* buffer, size_t buffer_size, const char* str,int pos);
		int				FailCaseHeaderRequest();
		std::string 	FileExtension();
		void    		httpMethod();
		void 			set_file_fd(FILE* fd);
		void 			set_method(std::string type);
		void 			set_request(const char* str);
		void 			set_sent_size(size_t s_size);
		void 			set_actual_size(size_t a_size);
		void 			set_has_request(bool condition);
		int 			send400Response(std::map<std::string, std::string> header_map, std::string uri);
		int 			send413Response();
		int 			get_matched_location_for_request_uri();
		int 			is_method_allowed_in_location(std::string method);
		int 			GETUriError();
		int 			GET_Response();
		int 			POST_Response();
		void 			list_directory(const char* path);
		bool			isDirectory(const char* path);
		int 			removeDirectory(const char* path);
		int 			removeFile(const char* filePath);
		int 			stringToInt(const std::string& str);
		std::string 	find_extension(std::string filename);
		void 			cgi_GET_response(std::string filename);
		std::fstream 	openFile();
		void 			set_html_string(bool set) { htmlString = set; }
		void 			set_URI(const std::string &set)
		{
			URI.clear();
			URI = set;
		}
		void 			clear();
};
void cgi_response(Client &object, std::string body_req, std::map<std::string, std::string> RequestHeaders, std::string script, std::string URI);
// std::fstream 	out;
#endif

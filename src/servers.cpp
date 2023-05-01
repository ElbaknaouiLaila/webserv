/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 18:11:29 by mmanouze          #+#    #+#             */
/*   Updated: 2023/05/01 22:29:29 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/servers.hpp"

const char *get_content_type(const char* path) {
    const char *last_dot = strrchr(path, '.');
    if (last_dot) {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
		if (strcmp(last_dot, ".c") == 0) return ("text/plain");
		if (strcmp(last_dot, ".php") == 0) return ("text/html");
    }

    return "text/plain";
}




//////////////////////////////////////////////////////////////////////////////




servers::servers():received(0)
{
	(void)received;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&original);
	//file.open("kitab", std::ios_base::out | std::ios_base::app | std::ios_base::binary);
}


//////////////////////////////////////////////////////////////////////////////



servers::~servers()
{
	for (size_t i = 0; i < server_list.size(); i++)
		FD_CLR(server_list[i], &original);
}


//////////////////////////////////////////////////////////////////////////////


std::vector<int> servers::get_list() const {
	return (this->server_list);
}


//////////////////////////////////////////////////////////////////////////////


void servers::fill_fd(){
	for (std::vector<int>::iterator it = server_list.begin(); it != server_list.end(); it++)
		FD_SET(*it, &original);
	max = server_list.back();
}




//////////////////////////////////////////////////////////////////////////////





void servers::clients_waiting_room(std::vector<Server *> ser)
{
	readfds = original;
	writefds = original;


	if (select(max + 1 , &readfds, &writefds, 0, 0) < 0) {
        perror("select() failed.");
        exit(1);
	}
	// std::cout << "dora" << std::endl;
	//printf("after select ************************\n");

	for (std::vector<int>::iterator it = server_list.begin(); it != server_list.end(); it++)
	{
		if (FD_ISSET(*it, &readfds))
		{
			accept_clients(*it);
			return ;
		}
	}
	std::map<int, Client>::iterator it = clients.begin();
	std::map<int, Client>::iterator ite = clients.end();
	while (it != ite)
	{
		//std::cout << it->first << " kay9lab 3a raso\n";
		if (FD_ISSET(it->first, &readfds) and it->second.get_has_request() == false) // && it->second.get_met() == false)
		{
			read_request(it->first, it->second, ser);
			//if (i == 1)
			//	return ;
		}
		if (FD_ISSET(it->first, &writefds) and it->second.get_has_request() == true)
		{
			std::map<int, Client>::iterator tmp = it;
			tmp++;
			//std::cout << it->second.get_URI() << std::endl;
			send_response(it->first, it->second);
			it = tmp;
			continue;
		}
			// std::cout << it->first << " has request " << " ++ " << FD_ISSET(it->first, &readfds) << " --- "<<  it->second.get_has_request() << std::endl;
		it++;
	}
	return ;
}



////////////////////////////////////////////////////////////////////////



int servers::read_request(int newsockfd, Client &object, std::vector<Server *> ser)
{
	// bzero(buffer, BUFFER_SIZE);
	object.setServer(ser);
	// std::cout << "helllo\n";
	int valread = recv(newsockfd, buffer , BUFFER_SIZE , 0);
	if (valread < 0 && object.get_has_request() == false) {
		std::cout << newsockfd << " is having a problem in reading \n";
		object.clear();
		// exit(1);
		clients.erase(newsockfd);
		close(newsockfd);
		FD_CLR(newsockfd, &original);
		FD_CLR(newsockfd, &readfds);
		FD_CLR(newsockfd, &writefds);
		perror("webserver (read)");
		return 1;
	}
	buffer[valread] = 0;
	// if (valread > 0)
		// std::cout << buffer << std::endl;
	object.i = object.i + valread;
	//file.write(buffer, valread);
	//file.flush();
	 
	if (valread > 0)
		object.set_request(buffer);
	if (valread > 0)
		object.HTTP_Request_pars(object.get_request(), valread, buffer, object.i);

	return (0);
}



/////////////////////////////////////////////////////////////////////////



void servers::send_response(int newsockfd, Client &object)
{
	if (object.get_has_redirection() == true)
	{
		// "HTTP/1.1 302 Found\r\n"
		std::string buff = "HTTP/1.1 " + object.get_status() + "\r\n" 
							+ "Connection: close\r\n"
							"Location: " + object.get_URI() + "\r\n"
        					"\r\n";
		send(newsockfd, buff.c_str(), buff.size(), 0);

		std::cout << newsockfd << " sad1111 lbab\n";
		fclose(object.get_file_fd());
		object.clear();
		clients.erase(newsockfd);
		close(newsockfd);
		FD_CLR(newsockfd, &original);
		FD_CLR(newsockfd, &readfds);
		FD_CLR(newsockfd, &writefds);
		max -= 1;
		return ;
	}
	if (object.get_html_string() == true)
	{
		std::string buff = "HTTP/1.1 " + object.get_status() + "\r\n" 
							+ "Connection: close\r\n"
							+ "Content-Length: " +toString( object.get_html_body_response().size() )+ "\r\n"
							+ "Content-Type: "+ "text/html" +"\r\n"
							+ "\r\n";
					buff+= object.get_html_body_response();
		send(newsockfd, buff.c_str(), buff.size(), 0);

		std::cout << newsockfd << " sad1111 lbab\n";
		fclose(object.get_file_fd());
		object.clear();
		clients.erase(newsockfd);
		close(newsockfd);
		FD_CLR(newsockfd, &original);
		FD_CLR(newsockfd, &readfds);
		FD_CLR(newsockfd, &writefds);
		max -= 1;
		return ;
	}
	
	else if (object.has_response() == false && object.get_iswriting() == false)
	{
		full_path(object);

		std::cout << newsockfd << " bgha i7al l file :  "<< object.get_URI() << std::endl;
		FILE *fp = fopen(object.get_URI().c_str(), "rb");

		std::cout << " nmra dyal l file : "<< fp->_file << std::endl;

		fseek(fp, 0L, SEEK_END);

		std::cout << "\nseeeeeeeeek  the file " << fp->_file << std::endl;

		size_t cl = ftell(fp);

		std::cout << "\nteeeeeeeeellllll  the file " << fp->_file << std::endl;

		rewind(fp);

		std::cout << "\nreeeeeeeeewind the file " << fp->_file << std::endl;

		if (object.get_status().size() == 0)
			object.set_status("200 OK");

		const char *ct = get_content_type(object.get_URI().c_str());
		std::cout << "ct : " << ct << std::endl;
		std::string buff = "HTTP/1.1 " + object.get_status() + "\r\n" 
							+ "Connection: close\r\n"
							+ "Content-Length: "+ toString(cl) + "\r\n"
							+ "Content-Type: "+ ct +"\r\n"
							+ "\r\n";
					buff+= object.get_html_body_response();
		send(newsockfd, buff.c_str(), buff.size(), 0);

		object.set_file_fd(fp);

		object.has_response() = true;
		
		std::cout << "\n sir diiir khdamtk l " << fp->_file << std::endl;
		object.set_iswriting(true);
	}

	// std::cout << "\\\\\\ " << object.get_URI() << " " <<  object.get_file_fd()->_file << std::endl;

	int r = fread(buffer_res, 1, BSIZE, object.get_file_fd());
		// std::cout << "has request  : "<< object.get_has_request() << std::endl;
		// std::cout << "is writing   : " << object.get_iswriting() << std::endl;
		// std::cout << "has response : " << object.has_response() << std::endl;
		// std::cout << "error        : " << object.get_error() << std::endl;
		// std::cout << newsockfd << " kayssfit f " << object.get_file_fd()->_file << " b : " << r << std::endl;
		// std::cout << buffer_res << std::endl;
	if (r != 0)
	{
		// std::cout << newsockfd << " kaysssift f " << object.get_URI() << " b " << r << std::endl;
	    send(newsockfd, buffer_res, r, 0);
	}
	else if (r == 0)
	{
		std::cout << newsockfd << " sad1111 lbab\n";
		fclose(object.get_file_fd());
		object.clear();
		// exit(1);
		clients.erase(newsockfd);
		close(newsockfd);
		FD_CLR(newsockfd, &original);
		FD_CLR(newsockfd, &readfds);
		FD_CLR(newsockfd, &writefds);
		max -= 1;
	}
	else if (r < 0)
	{
		std::cout << newsockfd << " sad3333 lbab\n";
		// exit(1);
		fclose(object.get_file_fd());
		object.clear();
		clients.erase(newsockfd);
		close(newsockfd);
		FD_CLR(newsockfd, &original);
		FD_CLR(newsockfd, &readfds);
		FD_CLR(newsockfd, &writefds);
		max -= 1;
	}
	bzero(buffer_res, 2000);
	return ;
}




//////////////////////////////////////////////////////////////////////////




int servers::accept_clients(int sockfd)
{
	 int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
	//int newsockfd = accept(sockfd, NULL, NULL);

	if (newsockfd < 0) {
		perror("webserver (accept)");
		return 0;
	}

	std::cout << newsockfd << " t connecta\n";

	fcntl(newsockfd, F_SETFL, O_NONBLOCK);

	int set = 1;
	setsockopt(newsockfd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	// printf("connection accepted\n");
	// int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);
	//int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);

	//if (sockn < 0) {
	//	perror("webserver (getsockname)");
	//	return 1;
	//}

	clients.insert(std::pair<int, Client>(newsockfd, Client()));
	FD_SET(newsockfd, &original);

	if (newsockfd > max)
		max = newsockfd;

	std::cout << "\n\n maaaaaaaax \n\n";

	return (newsockfd);
}



//////////////////////////////////////////////////////////////////////////




void servers::create_server_sockets(std::vector<Server *> ser)
{
	for (std::vector<Server*>::iterator it = ser.begin(); it < ser.end(); it++)
	{
		host_addr.sin_family = AF_INET;
		host_addr.sin_addr.s_addr = inet_addr((*it)->getHost().c_str());
		host_addr.sin_port = htons((*it)->getPort());

		host_addrlen = sizeof(host_addr);
		client_addrlen = sizeof(client_addr);

		int sock = socket(AF_INET, SOCK_STREAM, 0);
		
		int optval = 1;

		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		
		if (sock == -1)
		{
			perror("SOCKET :");
			exit(1);
		}
		printf("port : %d\n", (*it)->getPort());
		this->server_list.push_back(sock);

		if (bind(sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0)
		{
			perror("BIND :");
			exit(1);
		}

		if (listen(sock, SOMAXCONN) != 0)
		{
			perror("LISTEN :");
			exit(1);
		}
	}
}
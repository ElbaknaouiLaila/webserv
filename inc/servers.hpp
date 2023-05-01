/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 18:08:08 by mmanouze          #+#    #+#             */
/*   Updated: 2023/05/01 01:39:28 by lelbakna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP

#include <errno.h>
#include <iostream>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <map>
#include <sys/time.h>
#include <fcntl.h>


#include "clients.hpp"
#include "parse.hpp"

	#define BSIZE 2000
#define BUFFER_SIZE 60000

class servers
{
	private :

		char 							buffer_res[BSIZE];
		int 							host_addrlen;
		int 							client_addrlen;
		int 							received;
		int 							max;

		char 							buffer[BUFFER_SIZE + 1];

		fd_set 							readfds;
		fd_set 							writefds;
		fd_set 							original;

 		std::vector<int> 				server_list;

		std::map<int, Client> 			clients;

		struct sockaddr_in 				client_addr;
	    struct sockaddr_in 				host_addr;
		std::fstream 					file;

	public :

		servers();
		~servers();
		void 				fill_fd();
		void 				clients_waiting_room(std::vector<Server *> ser);
		int 				accept_clients(int sockfd);
		int 				read_request(int newsockfd, Client &object, std::vector<Server *> ser);
		void 				send_response(int newsockfd, Client &object);
		std::vector<int> 	get_list() const;
		void 				create_server_sockets(std::vector<Server *> ser);
		void 				full_path(Client &object);
};

template <typename T>
std::string toString(T val) {
    std::stringstream ss;
    ss << val;
    return ss.str();
}


#endif
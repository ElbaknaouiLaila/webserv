# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lelbakna <lelbakna@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/21 08:16:43 by rmoujan           #+#    #+#              #
#    Updated: 2023/04/29 14:14:36 by lelbakna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME=myServer

CPP = c++

FLAGS= -Wall -Wextra -Werror -std=c++98  -fsanitize=address -g3

RM=rm -rf

SRC=./src/main.cpp\
	./src/parse.cpp\
	./src/directive_server.cpp\
	./src/ft_errors.cpp\
	./src/directive_locations.cpp\
	./src/check_errors_server.cpp\
	./src/free_data.cpp\
	./src/clients.cpp\
	./src/pars_request.cpp\
	./src/response.cpp\
	./src/servers_utiles.cpp\
	./src/servers.cpp\
	./src/cgi.cpp\
	

OBJ = $(subst .cpp,.o,$(SRC))

all:$(NAME)

$(NAME):$(OBJ)
	$(CPP) $(OBJ) -o $(NAME)

%.o : %.cpp
	$(CPP) -c $(FLAGS) $< -o $@ 

clean:
	$(RM) $(OBJ)

fclean:clean
	$(RM) $(NAME)

re:fclean all
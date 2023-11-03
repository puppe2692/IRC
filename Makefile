# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/28 21:18:35 by nwyseur           #+#    #+#              #
#    Updated: 2023/11/03 11:21:20 by nwyseur          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= Test2

SRCS_FILES	:= test2.cpp \

HEADER_FILE	:= irc2.hpp \

SRCS_DIR	:= ./srcs/
INC_DIR		:= ./includes/

SRCS		:= $(addprefix ${SRCS_DIR}, ${SRCS_FILES})
OBJS		:= ${SRCS:.cpp=.o}
HEADER		:= $(addprefix ${INC_DIR}, ${HEADER_FILE})

CC			:= c++
CC_FLAGS	= -std=c++98 -Wall -Wextra -Werror -g3

RM			:= rm -f

%.o : %.cpp	${HEADER}
			${CC} ${CC_FLAGS} -I${INC_DIR} -c $< -o $@

all:		${NAME}

${NAME}:	${OBJS}
			${CC} ${CC_FLAGS} -I${INC_DIR} -o ${NAME} ${OBJS} 
			@echo "$(GREEN)$(NAME) created!$(DEFAULT)"

clean:
			${RM} ${OBJS}
			@echo "$(YELLOW)object files deleted!$(DEFAULT)"

fclean:		clean
			${RM} ${NAME}
			@echo "$(RED)all deleted!$(DEFAULT)"

re:			fclean all

.PHONY: 	all clean fclean re

#COLORS
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
DEFAULT = \033[0m

# ************************************************************************** #
#                                                                            #
#                                                                            #
#   Makefile                                                                 #
#                                                                            #
#   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             #
#                                                             / \__          #
#                                                            (    @\___      #
#                                                             /         O    #
#   Created: 2024/05/15 23:13:39 by Tiago                    /   (_____/     #
#   Updated: 2023/05/15 23:46:56 by Tiago                  /_____/ U         #
#                                                                            #
# ************************************************************************** #

NAME		=	SERVER
CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -Wshadow -std=c++98 -pedantic
CXXFLAGS	=	-fsanitize=address -g3
INCLUDES	=	-Iincs
RM			=	rm -rf
MKDIR		=	mkdir -p

SRC_PATH	=	srcs
OBJ_PATH	=	objs
ANNOYING	=	*.dSYM .vscode
MAIN		=	srcs/main.cpp
SRC			=	Serv.cpp
OBJ			=	${addprefix ${OBJ_PATH}/, ${SRC:.cpp=.o}}

all: ${NAME}

${NAME}: ${OBJ} $(MAIN)
	${CXX} ${CXXFLAGS} ${INCLUDES} ${MAIN} ${OBJ} -o ${NAME}

${OBJ_PATH}/%.o: ${SRC_PATH}/%.cpp
	${MKDIR} ${@D}
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

test: fclean all
	./${NAME}

clean:
	${RM} ${ANNOYING} ${OBJ_PATH}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY = all clean fclean re
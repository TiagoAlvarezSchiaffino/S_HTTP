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
#   Updated: 2024/06/06 07:48:19 by Tiago                  /_____/ U         #
#                                                                            #
# ************************************************************************** #

NAME		=	SERVER
CXX			=	g++
CXXFLAGS	=	-Wall -Wextra -Werror -Wshadow -std=c++98 -pedantic
CXXFLAGS	+=	-fsanitize=thread -g3
INCLUDES	=	-Iincs
RM			=	rm -rf
MKDIR		=	mkdir -p
SRC_PATH	=	srcs
OBJ_PATH	=	objs
ANNOYING	=	*.dSYM ../.vscode
MAIN		=	srcs/main.cpp
SRC			=	Token.cpp \
				CookieJar.cpp \
				Serv.cpp \
				EuleeHand.cpp \
				EuleeWallet.cpp \
				EuleePocket.cpp \
				ConfigManager.cpp \
				HttpCgiResponse.cpp \
				HttpGetResponse.cpp \
				HttpPutResponse.cpp \
				HttpPostResponse.cpp \
				HttpHeadResponse.cpp \
				HttpDeleteResponse.cpp
OBJ			=	${addprefix ${OBJ_PATH}/, ${SRC:.cpp=.o}}

all: 
	${MKDIR} ${OBJ_PATH}
	make ${NAME}

${NAME}: ${OBJ} $(MAIN)
	${CXX} ${CXXFLAGS} ${INCLUDES} ${MAIN} ${OBJ} -o ${NAME}

${OBJ_PATH}/%.o: ${SRC_PATH}/%.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

test: fclean all
	./${NAME}

clean:
	${RM} ${ANNOYING} ${OBJ_PATH}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY = all clean fclean re
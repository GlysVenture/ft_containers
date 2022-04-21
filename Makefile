SRCS		= main.cpp

OBJS		= ${SRCS:.cpp=.o}

NAME		= tests_ctainers

CXX			= c++

CFLAGS		= -Wall -Werror -Wextra -std=c++98 -pedantic

RM			= rm -rf

.cpp.o:
			${CXX} ${CFLAGS} -c $< -o $@

all:		${NAME}

${NAME}:	${OBJS}
			${CXX} ${CFLAGS} -o ${NAME} ${OBJS}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
SRCS_DIR	=	srcs/

SRC			=	main.c\
				utils.c\

SRCS		=	$(addprefix $(SRCS_DIR), $(SRC))

OBJS_DIR	=	objs/

OBJ			=	${SRC:.c=.o}

OBJS		=	$(addprefix $(OBJS_DIR), $(OBJ))

NAME		=	philo

CC			=	gcc -pthread

CFLAGS		=	-Wall -Wextra -Werror -g3

INCS		=	-I ./includes/

all:		${OBJS_DIR} ${NAME}

${OBJS_DIR}:
			mkdir ${OBJS_DIR}

${OBJS_DIR}%.o: ${SRCS_DIR}%.c
			${CC} ${CFLAGS} -c $< -o $@ ${INCS}

${NAME}:	${OBJS}
			${CC} ${FLAGS} ${OBJS} -o ${NAME}

clean:
			rm -rf ${OBJS_DIR}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re

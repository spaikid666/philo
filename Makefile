NAME = philo

SRCS = main.c \
		instructions.c \


OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Werror -Wextra -lpthread

CC = cc

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
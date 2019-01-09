NAME	= projet3D

SRC	= src/main.cpp\
	  src/tgaimage.cpp


OBJ	= $(SRC:.c=.o)

CC	= g++ -g

RM	= rm -f

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

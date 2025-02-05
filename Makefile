CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

SRCS =  ft_split.c  ft_strncmp.c ft_strnstr.c  \
	get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)
#BONUS = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c \
        ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c \
        ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c

BONUS_OBJS = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

.PHONY: all clean fclean re bonus

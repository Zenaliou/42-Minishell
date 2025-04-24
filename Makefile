#COMPILATION
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Ilibft -g3

#DIRECTORIES
SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = includes/
LIBFT_DIR = libft/

#SOURCES
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

#NAME
NAME = minishell

#LIBFT
LIBFT = $(LIBFT_DIR)libft.a

#COLORS
GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

#RULES
all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ Compilation de libft réussie!$(NC)"

$(NAME): $(OBJ_DIR) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses -o $(NAME)
	@echo "$(GREEN)✓ Compilation de $(NAME) réussie!$(NC)"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compilation de $< réussie.$(NC)"

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(RED)Fichiers objets supprimés.$(NC)"

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(RED)Exécutable $(NAME) supprimé.$(NC)"

re: fclean all
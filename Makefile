# ==============================
#         MINISHELL MAKEFILE
# ==============================

# COMPILATION
CC      = cc
CFLAGS  = -Wall -Werror -Wextra -Iincludes -Ilibft -g3

# DIRECTORIES
SRC_DIR     = src/
OBJ_DIR     = obj/
INCLUDE_DIR = includes/
LIBFT_DIR   = libft/

# SOURCES
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS:$(SRC_DIR)/%=%))

# NAME
NAME = minishell

# LIBFT
LIBFT = $(LIBFT_DIR)libft.a

# COLORS & STYLE
GREEN   = \033[1;32m
RED     = \033[1;31m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
CYAN    = \033[1;36m
NC      = \033[0m

# RULES
all: $(LIBFT) $(NAME)
	@echo "$(CYAN)🚀 Tout est prêt !$(NC)"

$(LIBFT):
	@echo "$(YELLOW)⏳ Compilation de libft...$(NC)"
	@$(MAKE) -s -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ Compilation de libft réussie!$(NC)"

$(NAME): $(OBJ_DIR) $(OBJS) $(LIBFT)
	@echo "$(YELLOW)⏳ Compilation de $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses -o $(NAME)
	@echo "$(GREEN)✓ Compilation de $(NAME) réussie!$(NC)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)📁 Dossier obj créé.$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "$(RED)🧹 Fichiers objets supprimés.$(NC)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(RED)🗑️ Exécutable $(NAME) supprimé.$(NC)"

re: fclean all
	@echo "$(CYAN)🔁 Recompilation terminée.$(NC)"

.PHONY: all clean
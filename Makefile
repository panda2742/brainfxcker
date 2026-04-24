# **************************************************************************** #
# 1. BUILD VARIABLES                                                           #
# **************************************************************************** #

NAME			:=	brainfxcker
HEADER_DIR		:=	./inc/
SOURCE_DIR		:=	./src/

# **************************************************************************** #
# 2. SOURCE CODE                                                               #
# **************************************************************************** #

override HEADER_FILES		:=	brainfxcker lexer parser debug ir optimizer codegen output
override SOURCE_FILES		:=	main lexer parser debug ir optimizer codegen output

# **************************************************************************** #
# 3. OTHER COMPILATION VARIABLES                                               #
# **************************************************************************** #

override BUILD_DIR	:=	build/
override SOURCE		:=	$(addprefix $(SOURCE_DIR), $(addsuffix .c, $(SOURCE_FILES)))
override HEADER		:=	$(addprefix $(HEADER_DIR), $(addsuffix .h, $(HEADER_FILES)))
override OBJ		:=	$(patsubst $(SOURCE_DIR)%.c, $(BUILD_DIR)%.o, $(SOURCE))
override DEPS		:=	$(patsubst %.o, %.d, $(OBJ))
override DIRS		:=	$(sort $(dir $(OBJ) $(DEPS)))

# **************************************************************************** #
# 4. FLAGS AND VARIABLES                                                       #
# **************************************************************************** #

TURBO_FLAGS			:=	-O3 -flto -mtune=native -funroll-loops -ffast-math -falign-functions=32
DEBUG_FLAGS			:=	-g3
CFLAGS				:=	-Wall -Wextra -Werror --std=c99 -MD $(DEBUG_FLAGS) $(TURBO_FLAGS)
MAKEFLAGS			:=	--no-print-directory
RMFLAGS				:=	-rf
VG					:=	valgrind
VGFLAGS				:=	--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes
override CC			:=	cc
override RM			:=	rm
override CLEAR		:=	clear
CALLGRIND_PRFL		:=	hotrace.profile
override VGCALL		:=	--tool=callgrind --callgrind-out-file=$(CALLGRIND_PRFL)
override KCACHE		:=	kcachegrind


# **************************************************************************** #
# 5. COMPILATION RULES                                                         #
# **************************************************************************** #

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.c $(HEADER) Makefile | $(DIRS)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

# **************************************************************************** #
# 6. GENERAL RULES                                                             #
# **************************************************************************** #

.PHONY: clean
clean:
	$(RM) $(RMFLAGS) $(BUILD_DIR) $(CALLGRIND_PRFL)

.PHONY: fclean
fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

PHONY: re
re: fclean
	$(MAKE)

.PHONY: run
run: all
	./$(NAME) <programs/main.bf

.PHONY: vg
vg: all
	$(VG) $(VGFLAGS) ./$(NAME) <programs/main.bf

.PHONY: cg
cg: all
	$(MAKE)
	$(RM) $(RMFLAGS) $(CALLGRIND_PRFL)
	$(VG) $(VGCALL) ./$(NAME) <programs/main.bf
	$(KCACHE) $(CALLGRIND_PRFL)

$(DIRS):
	@mkdir -p $@

-include $(DEPS)
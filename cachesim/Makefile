NAME := $(shell basename $(PWD))
export MODULE := Lab4

all: $(NAME)-64
run: all
	@./$(NAME)-64

gdb: all
	@gdb --tui ./$(NAME)-64

include ../Makefile


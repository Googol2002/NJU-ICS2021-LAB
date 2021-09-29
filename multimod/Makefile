NAME := $(shell basename $(PWD))
run: all
	$(./multimod-64)

all: $(NAME)-64 $(NAME)-32
export MODULE := Lab1
include ../Makefile
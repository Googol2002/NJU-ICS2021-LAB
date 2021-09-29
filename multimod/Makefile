NAME := $(shell basename $(PWD))
.PHONY : run
run: all
	$(multimod-64)

all: $(NAME)-64 $(NAME)-32
export MODULE := Lab1
include ../Makefile
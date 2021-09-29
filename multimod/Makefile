NAME := $(shell basename $(PWD))

all: $(NAME)-64 $(NAME)-32
export MODULE := Lab1
include ../Makefile

.PHONY : run
run: all
	$(multimod-64)
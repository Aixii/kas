
CC := gcc
AR := ar

CFLAGS := -Wall -g -O \
	-DDEBUG \
	-DKAS_MEMORY_MANAGE

ARFLAGS := rcs 

OUT_DIR := out
OBJ_DIR := obj

all: prepare lib

prepare:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)

kas.o : kas.c kas.h kasdef.h
	@$(CC) -c $< $(CFLAGS) -o $(OBJ_DIR)/$@

lib: kas.o
	@$(AR) $(ARFLAGS) libkas.a $(OBJ_DIR)/$< 

test: test.c all
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/test1 $< -L. -lkas

clean:
	@$(RM) $(OUT_DIR)/*
	@$(RM) libkas.a 
	@$(RM) $(OBJ_DIR)/*
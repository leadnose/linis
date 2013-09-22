CC=clang++
CFLAGS= -g -Wall -Wextra -Werror -std=c++11

default:
	$(CC) $(CFLAGS) -o matrix matrix.cc

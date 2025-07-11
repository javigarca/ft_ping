# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: javi <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 19:13:49 by javi              #+#    #+#              #
#    Updated: 2024/04/09 18:36:42 by javi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = $(BINDIR)/ft_ping



OBJDIR = obj
SRCDIR = sources
HDRDIR = headers
BINDIR = bin

HEADERS = -I./$(HDRDIR)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
DEPS = $(OBJECTS:.o=.d)

CC = gcc
CFLAGS = -Wall -Wextra -Werror $(HEADERS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.d

fclean: clean
	rm -f $(TARGET)
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all clean fclean re

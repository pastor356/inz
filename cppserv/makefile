CC := g++
CFLAGS := -g -Wall -Wextra -Wpedantic -I./include -O0
OUTERFLAGS := 

INCLUDEDIR 	:= ./include
SRCDIR 		:= ./src
OBJDIR 		:= ./obj

SRC 		:= $(filter-out $(wildcard $(SRCDIR)/main*), $(wildcard $(SRCDIR)/*.cpp))
OBJ 	 	:= $(subst .cpp,.o, $(subst $(SRCDIR),$(OBJDIR), $(SRC)))


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(OUTERFLAGS) -c $(SRCDIR)/$*.cpp -o $(OBJDIR)/$*.o


serv: $(OBJ) $(SRCDIR)/mains.cpp
	$(CC) $(CFLAGS) $(OUTERFLAGS) $(SRCDIR)/mains.cpp $(OBJ) -o serv


client: $(OBJ) $(SRCDIR)/mainc.cpp
	$(CC) $(CFLAGS) $(OUTERFLAGS) $(SRCDIR)/mainc.cpp $(OBJ) -o client

debug: 
	@echo $(OBJ)

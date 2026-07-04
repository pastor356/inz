CC 			:= gcc
CFLAGS 		:= -g -Wall -Wpedantic -Wextra -pipe -I./include -std=c11 -D_POSIX_C_SOURCE=200809L -O0

HDR 		:= ./include
SRC 		:= ./src
OBJDIR 		:= ./obj
TESTDIR 	:= ./test

SERVSRC 	:= serv_funcs.c serv.c
CLIENTSRC 	:= client_funcs.c client.c
SHAREDSRC  	:= shared.c fdarr.c list_tinfo.c msg.c client_info.c serializer.c serv_info.c
TESTSRC 	:= test_tlist.c main_test.c test_tinfo_fdarr.c test_cinfo_ser.c test_checkof_topic_exists.c test_serv_loadtest.c test_newserializer.c test_servinfo.c

SERVOBJ 	:= $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SERVSRC)))
CLIENTOBJ 	:= $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(CLIENTSRC)))
SHAREDOBJ 	:= $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SHAREDSRC)))

TESTOBJ 	:= $(addprefix $(TESTDIR)/, $(patsubst %.c, %.o, $(TESTSRC)))

SERVBIN 	:= serv
CLIENTBIN 	:= client
TESTBIN 	:= test


$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


$(TESTDIR)/%.o: $(TESTDIR)/%.c # for test source
	$(CC) $(CFLAGS) -c $< -o $@


s: $(SHAREDOBJ) $(SERVOBJ)
	$(CC) $(CFLAGS) $^ -o $(SERVBIN)


c: $(SHAREDOBJ) $(CLIENTOBJ)
	$(CC) $(CFLAGS) $^ -o $(CLIENTBIN)

test: $(TESTOBJ) $(word 1, $(SERVOBJ)) $(word 1, $(CLIENTOBJ)) $(SHAREDOBJ)
	@echo "TEST RULE"
	$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$(TESTBIN)

testv: $(TESTOBJ) $(word 1, $(SERVOBJ)) $(word 1, $(CLIENTOBJ)) $(SHAREDOBJ)
	@echo "TEST RULE"
	$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$(TESTBIN)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(TESTDIR)/$(TESTBIN)

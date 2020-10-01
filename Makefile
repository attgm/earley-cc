# MakeFile 
#  $Id:$
CFLAGS = -O -I$(SOURCE_DIR)
CPLUSPLUS = g++

SOURCE_DIR = earley-cc
EXAMPLE_DIR = example

LINK = $(CPLUSPLUS) $(CFLAGS)

EARLEY_OBJS = $(SOURCE_DIR)/ElementPool.o \
		$(SOURCE_DIR)/Grammar.o \
		$(SOURCE_DIR)/Registration.o \
		$(SOURCE_DIR)/ParseList.o \
		$(SOURCE_DIR)/ParseProblem.o \
		$(SOURCE_DIR)/AuthorizeProblem.o

EARLEY_SRCS = $(SOURCE_DIR)/ElementPool.cc \
		$(SOURCE_DIR)/Grammar.cc \
		$(SOURCE_DIR)/Registration.cc \
		$(SOURCE_DIR)/ParseList.cc \
		$(SOURCE_DIR)/ParseProblem.cc \
		$(SOURCE_DIR)/AuthorizeProblem.cc

PARSE_BIN    = test_parse
PARSE_OBJ    = $(EXAMPLE_DIR)/test_parse.o
PARSE_MAIN   = $(EXAMPLE_DIR)/test_parse.cc

AUTHORIZE_BIN  = test_authorize
AUTHORIZE_OBJ  = $(EXAMPLE_DIR)/test_authorize.o
AUTHORIZE_MAIN = $(EXAMPLE_DIR)/test_authorize.cc

EARLEY_MAIN = main.cc
EARLEY_OBJ  = main.o
EARLEY_BIN = earley

OBJS = $(EARLEY_OBJS)
SRCS = $(EARLEY_SRCS) $(EARLEY_MAIN) $(PARSE_MAIN) $(AUTHORIZE_MAIN)
PROG = $(PARSE_BIN) $(AUTHORIZE_BIN) $(EARLEY_BIN)

# c.o
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# cc.o
.cc.o:
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

all: $(EARLEY_BIN)


test: $(PARSE_BIN)  $(AUTHORIZE_BIN) 


$(EARLEY_BIN) : $(OBJS) $(EARLEY_OBJ)
	$(LINK) $(OBJS) $(EARLEY_OBJ) -o $(EARLEY_BIN)


$(AUTHORIZE_BIN) : $(OBJS) $(AUTHORIZE_OBJ)
	$(LINK) $(AUTHORIZE_MAIN) $(OBJS) -o $(AUTHORIZE_BIN)

$(PARSE_BIN) : $(OBJS) $(PARSE_OBJ)
	$(LINK) $(PARSE_OBJ) $(OBJS) -o $(PARSE_BIN)

clean   : 
	rm -f *.o $(SOURCE_DIR)/*.o $(EXAMPLE_DIR)/*.o $(PROG)

depend: $(SRCS)
	cp Makefile Makefile.bak
	makedepend -- $(CFLAGS) $(SRCS) > depends

# ��¸�ط�

# DO NOT DELETE THIS LINE -- make depend uses it


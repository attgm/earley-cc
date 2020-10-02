SOURCE_DIR		= ./earley-cc
EXAMPLE_DIR		= ./example
LIB_DIR			= ./lib
TARGET_DIR		= ./bin
OBJECT_DIR		= ./obj

CXXFLAGS 		= -g -MMD -Wall -std=c++17 -I$(SOURCE_DIR)

LIB_EARLEY		= $(LIB_DIR)/libEarley.a
LIB_EARLEY_SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
LIB_EARLEY_OBJS = $(addprefix $(OBJECT_DIR)/, $(notdir $(LIB_EARLEY_SRCS:.cpp=.o)))

PARSE_BIN    = $(TARGET_DIR)/test_parse
PARSE_MAIN   = $(EXAMPLE_DIR)/test_parse.cpp
PARSE_OBJS  = $(addprefix $(OBJECT_DIR)/, $(notdir $(PARSE_MAIN:.cpp=.o)))


AUTHORIZE_BIN  = $(TARGET_DIR)/test_authorize
AUTHORIZE_MAIN = $(EXAMPLE_DIR)/test_authorize.cpp
AUTHORIZE_OBJS = $(addprefix $(OBJECT_DIR)/, $(notdir $(AUTHORIZE_MAIN:.cpp=.o)))

EARLEY_BIN  = $(TARGET_DIR)/earley
EARLEY_MAIN = ./main.cpp
EARLEY_OBJS = $(addprefix $(OBJECT_DIR)/, $(notdir $(EARLEY_MAIN:.cpp=.o)))

OBJS = $(LIB_EARLEY_OBJS) $(PARSE_OBJS) $(AUTHORIZE_OBJS) $(EARLEY_OBJS)
SRCS = $(LIB_EARLEY_SRCS) $(PARSE_MAIN) $(AUTHORIZE_MAIN) $(EARLEY_MAIN)
BINS = $(PARSE_BIN) $(AUTHORIZE_BIN) $(EARLEY_BIN)
DEPS = $(OBJS:.o=.d)

.PHONY: all clean depend

all: $(EARLEY_BIN) $(PARSE_BIN) $(AUTHORIZE_BIN)

$(LIB_EARLEY) : $(LIB_EARLEY_OBJS)
	@-mkdir -p $(LIB_DIR)
	$(AR) cr $@ $(LIB_EARLEY_OBJS)

$(EARLEY_BIN) : $(LIB_EARLEY) $(EARLEY_OBJS)
	@-mkdir -p $(TARGET_DIR)
	$(LINK.cc) $(LIB_EARLEY) $(EARLEY_OBJS) -o $(EARLEY_BIN)

$(AUTHORIZE_BIN) : $(LIB_EARLEY) $(AUTHORIZE_OBJS)
	@-mkdir -p $(TARGET_DIR)
	$(LINK.cc) $(LIB_EARLEY) $(AUTHORIZE_OBJS) -o $(AUTHORIZE_BIN)

$(PARSE_BIN) : $(LIB_EARLEY) $(PARSE_OBJS)
	@-mkdir -p $(TARGET_DIR)
	$(LINK.cc) $(LIB_EARLEY) $(PARSE_OBJS) -o $(PARSE_BIN)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp	
	@-mkdir -p $(OBJECT_DIR)
	$(COMPILE.cc) $<  -o $@ 

$(OBJECT_DIR)/%.o: $(EXAMPLE_DIR)/%.cpp	
	@-mkdir -p $(OBJECT_DIR)
	$(COMPILE.cc) $<  -o $@ 

clean   : 
	-$(RM) $(BINS) $(DEPS) $(OBJS) \
		*~ .*~ core

-include  $(DEPS)
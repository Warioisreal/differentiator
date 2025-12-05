CFLAGS = -g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
         -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body \
         -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
         -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls \
         -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default \
         -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
         -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector \
         -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer \
         -Wlarger-than=8192 -fPIE -Werror=vla \
         #-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

LDFLAGS =
OBJDIR = 0_objects
CC = g++
SOURCES = main.cpp differentiator.cpp math_func.cpp optimazer.cpp gnuplot.cpp
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE = binary_file

TREE_SRC_DIR = tree
TREE_OBJDIR = 0_tree_obj
TREE_SOURCES = tree_func.cpp tree_DB_func.cpp tree_node_func.cpp checkers.cpp logger.cpp dot.cpp latex.cpp
TREE_OBJECTS = $(addprefix $(TREE_OBJDIR)/, $(TREE_SOURCES:.cpp=.o))
TREE_LIB = tree_lib.a

STACK_SRC_DIR = tree/stack
STACK_OBJDIR = 0_stack_obj
STACK_SOURCES = stack_func.cpp defender_system.cpp
STACK_OBJECTS = $(addprefix $(STACK_OBJDIR)/, $(STACK_SOURCES:.cpp=.o))
STACK_LIB = stack_lib.a

all: tree $(EXECUTABLE)

log: CFLAGS += -DLOG_TREE
log: all

tree: stack $(TREE_LIB)

$(TREE_LIB): $(TREE_OBJECTS)
	@ar rcs $@ $(TREE_OBJECTS)

$(TREE_OBJDIR)/%.o: $(TREE_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@



stack: $(STACK_LIB)

$(STACK_LIB): $(STACK_OBJECTS)
	@ar rcs $@ $(STACK_OBJECTS)

$(STACK_OBJDIR)/%.o: $(STACK_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@



$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TREE_LIB) -o $@

clean: cleantree
	rm -rf $(OBJDIR) $(EXECUTABLE)

cleantree: cleanstack
	rm -rf $(TREE_OBJDIR) $(TREE_LIB)

cleanstack:
	rm -rf $(STACK_OBJDIR) $(STACK_LIB)

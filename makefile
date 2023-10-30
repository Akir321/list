CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline\
 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op\
 -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual\
 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE
LD_FLAGS = 


SOURCE_DIR = source/
OBJ_DIR    = object/

INCLUDES =  $(SOURCE_DIR)list.h

OBJECTS  =  $(OBJ_DIR)test_list.o \
		    $(OBJ_DIR)list.o

all: test_list.exe

test_list.exe: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(CXX_FLAGS)

$(OBJ_DIR)test_list.o: $(SOURCE_DIR)test_list.cpp $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)list.o:      $(SOURCE_DIR)list.cpp      $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

.PHONY: clean

clean:
	rm $(OBJECTS) *.exe


DOT_DIR = dot_tests/

graph1:
	dot $(DOT_DIR)graph1.dot -T png -o $(DOT_DIR)graph1.png

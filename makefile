CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline\
 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op\
 -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual\
 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE 

SRC_DIR  = source/
OBJ_DIR  = object/
DMP_DIR  = gr_dump/
LOG_DIR  = log/

INCLUDES =  $(SRC_DIR)list.h           \
			$(SRC_DIR)graphic_dump.h   \
			$(SRC_DIR)html_logfile.h

OBJECTS  =  $(OBJ_DIR)test_list.o      \
		    $(OBJ_DIR)list.o           \
			$(OBJ_DIR)graphic_dump.o   \
			$(OBJ_DIR)html_logfile.o

DUMPS    =  $(DMP_DIR)*.dot            \
			$(DMP_DIR)*.png

LOGS     =  $(LOG_DIR)*.txt            \
			$(LOG_DIR)*.log            \
			$(LOG_DIR)*.html           

all: test_list.exe

test_list.exe: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(CXX_FLAGS)


$(OBJ_DIR)test_list.o:    $(SRC_DIR)test_list.cpp    $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)list.o:         $(SRC_DIR)list.cpp         $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)graphic_dump.o: $(SRC_DIR)graphic_dump.cpp $(INCLUDES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)html_logfile.o: $(SRC_DIR)html_logfile.cpp $(INCLUDES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)



.PHONY: clean clean_dumps clean_logs

clean:
	rm $(OBJECTS) *.exe 
	
clean_dumps:
	rm $(DUMPS)

clean_logs:
	rm $(LOG_DIR)*.html


DOT_DIR = dot_tests/

graph1:
	dot $(DOT_DIR)graph1.dot -T png -o $(DOT_DIR)graph1.png

graph2:
	dot $(DOT_DIR)graph2.dot -T png -o $(DOT_DIR)graph2.png

graph3:
	dot $(DOT_DIR)graph3.dot -T png -o $(DOT_DIR)graph3.png

graph4:
	dot $(DOT_DIR)graph4.dot -T png -o $(DOT_DIR)graph4.png

graph5:
	dot $(DOT_DIR)graph5.dot -T png -o $(DOT_DIR)graph5.png

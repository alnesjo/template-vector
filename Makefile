SRC = test_template_vec.cpp

OBJS = $(SRC:.cpp=.o)

CO = g++
CFLAGS = -Wall -std=c++0x -pedantic -g
LFLAGS = 
LINK_TARGET = simple_container

all : $(SRC) depend $(LINK_TARGET)

submit : kth_cprog_template_container.hpp
	echo y | ~/scripts/submit.py $^ -p kth:cprog:templatevector -l c++

clean :
	rm -f $(LINK_TARGET) $(OBJS) ./.depend

depend : .depend

.depend : $(SRC)
	rm -f ./.depend
	$(CO) -MM $^ $(CFLAGS) -MF ./.depend

$(LINK_TARGET) : $(OBJS)
	$(CO) $^ $(LFLAGS) -o $@

%.o : %.cpp
	$(CO) $< $(CFLAGS) -c

include .depend

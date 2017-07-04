EXE     = a.out
LIBS    = -O3

CXX     = g++ -std=c++11
SRC     = src
SRCS    = $(shell find $(SRC) -name '*.cpp')
HEADERS = $(shell find $(SRC) -name '*.hpp')
OBJS    = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o)))

$(EXE): $(OBJS)
	$(CXX) $(LIBS) $(OBJS) -o $@

obj/%.o: $(SRC)/%.cpp $(HEADERS)
	mkdir -p obj
	$(CXX) $(LIBS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(EXE) obj


# CXX=clang++-9	
CXX=g++
CXXVERSION=c++2a
CXXFLAGS=-std=$(CXXVERSION) -Werror -Wsign-conversion -I$(SOURCE_PATH)
SOURCE_PATH=.
SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
HEADERS=$(wildcard $(SOURCE_PATH)/*.h*)
TIDY_FLAGS=-extra-arg=-std=$(CXXVERSION) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=*
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99
OBJECTS=$(subst .cpp,.o,$(SOURCES))
KNOWN_TARGETS = target
args := $(filter-out $(KNOWN_TARGETS),$(MAKECMDGOALS))

# run: client server test
run: client server test 

client:client.o
	$(CXX) $(CXXFLAGS) $^ -o $@

server:server.o 
	$(CXX) $(CXXFLAGS)  $^ -o  $@ 


test: TestCounter.o Test.o 
	$(CXX) $(CXXFLAGS) $^ -o test -pthread

%.o: %.c*  $(HEADERS) 
	$(CXX) $(CXXFLAGS) --compile $< -o $@


$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.c*  $(HEADERS) 
	$(CXX) $(CXXFLAGS) --compile $< -o $@

tidy:
	clang-tidy $(SOURCES) $(TIDY_FLAGS) --

valgrind: server client
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./server 

clean:
	rm -f $(OBJECTS) *.o client server test malloc *.a *.out
	
iclient:
	./client 

iserver:
	./server 

tester:
	./test 
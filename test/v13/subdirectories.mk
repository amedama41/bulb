INCLUDES = -I../../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic $(INCLUDES)
# CXX = g++-4.9
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

OBJS = $(SRCS:.cpp=.o)

TARGET = all_test.out

ALL_TESTS = $(OBJS:.o=)

DEPENDS = $(addprefix .,$(OBJS:.o=.depends))

define build_test
$(CXX) -DBOOST_TEST_MODULE=$@ $(CXXFLAGS) -c ../../driver.cpp -o ../../driver.o
$(CXX) $(CXXFLAGS) -o $@ ../../driver.o $^ $(LDFLAGS) -rpath @executable_path/.
endef

.PHONY: all clean clean_all run

all: $(TARGET)

.%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@

%: %.o
	$(build_test)

$(TARGET_LIB): $(OBJS)
	$(CXX) -dynamiclib -install_name @rpath/$@ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET): $(TARGET_LIB)
	$(build_test)

run: all
	./$(TARGET) -l message

clean:
	-rm *.o $(ALL_TESTS) $(TARGET_LIB) $(TARGET) 2> /dev/null

clean_all: clean
	-rm $(DEPENDS)

-include $(DEPENDS)


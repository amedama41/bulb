INCLUDES = -I../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
# CXX = g++-4.9
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic $(INCLUDES)
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

SUB_DIRECTORIES = action \
				  common_type
TEST_LIBRARIES = $(foreach dir,$(SUB_DIRECTORIES),./$(dir)/lib$(dir)_test.dylib)
LINKED_LIBRARIES = $(foreach dir,$(SUB_DIRECTORIES),-L./$(dir) -l$(dir)_test)
LIBRARIES_RPATH = $(foreach dir,$(SUB_DIRECTORIES),-rpath @executable_path/./$(dir))

TARGET = all_test.out

define build_test
$(CXX) -DBOOST_TEST_MODULE=$@ $(CXXFLAGS) -c ../driver.cpp -o ../driver.o
$(CXX) $(CXXFLAGS) -o $@ ../driver.o $^ $(LDFLAGS) $(LIBRARIES_RPATH)
endef

define make_each_subdirectories
@for dir in $(SUB_DIRECTORIES); do \
	$(MAKE) -C ./$$dir $@; \
done
endef

.PHONY: all clean clean_all run

all: $(TARGET)

.%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@

%: %.o
	$(build_test)

$(TARGET): $(TEST_LIBRARIES) $(OBJS)
	$(build_test)

$(TEST_LIBRARIES):
	$(MAKE) -C $(dir $@) $(notdir $@)

run: all
	./$(TARGET)

clean:
	-rm *.o $(ALL_TESTS) $(TARGET) 2> /dev/null
	$(make_each_subdirectories)

clean_all:
	-rm *.o $(DEPENDS) $(ALL_TESTS) $(TARGET) 2> /dev/null
	$(make_each_subdirectories)

-include $(DEPENDS)


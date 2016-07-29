INCLUDES = -I../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
# CXX = g++-4.9
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic $(INCLUDES)
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

SUB_DIRECTORIES = action \
				  common_type \
				  instruction \
				  message \
				  multipart \
				  oxm_match \
				  queue_property \
				  table_feature_property
TEST_LIBRARIES = $(foreach dir,$(SUB_DIRECTORIES),./$(dir)/lib$(dir)_test.dylib)
LINKED_LIBRARIES = $(foreach dir,$(SUB_DIRECTORIES),-L./$(dir) -l$(dir)_test)
LIBRARIES_RPATH = $(foreach dir,$(SUB_DIRECTORIES),-rpath @executable_path/./$(dir))

UTILITY_TEST_SRCS = byteorder_test.cpp \
					packet_queue_test.cpp \
					port_test.cpp
UTILITY_TEST_OBJS = $(UTILITY_TEST_SRCS:.cpp=.o)

MESSAGE_TEST_SRCS = hello_test.cpp hello_elements_test.cpp \
					group_mod_test.cpp \
					table_features_test.cpp
MESSAGE_TEST_OBJS = $(MESSAGE_TEST_SRCS:.cpp=.o)

SRCS = $(UTILITY_TEST_SRCS) \
	   $(MESSAGE_TEST_SRCS)
OBJS = $(SRCS:.cpp=.o)

TARGET = all_test.out

ALL_TESTS = $(OBJS:.o=)

DEPENDS = $(addprefix .,$(OBJS:.o=.depends))

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


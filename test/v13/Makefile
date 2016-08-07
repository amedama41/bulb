INCLUDES = -I../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
# CXX = g++-4.9
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic -DCANARD_NET_OFP_SUPPRESS_IMPLICIT_INSTANTIATION $(INCLUDES)
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

LIBBULB_DIR = libbulb
LIBBULB_LIB = ./$(LIBBULB_DIR)/libbulb.dylib
LIBBULB_RPATH = -rpath @executable_path/./$(LIBBULB_DIR)

TEST_DIRS = action \
			common_type \
			instruction \
			message \
			multipart \
			oxm_match \
			queue_property \
			table_feature_property
TEST_LIBS = $(foreach dir,$(TEST_DIRS),./$(dir)/lib$(dir)_test.dylib)
TEST_RPATH = $(foreach dir,$(TEST_DIRS),-rpath @executable_path/./$(dir))

ALL_SUBDIRS = $(LIBBULB_DIR) $(TEST_DIRS)
ALL_LIBS = $(LIBBULB_LIB) $(TEST_LIBS)
ALL_RPATH = $(LIBBULB_RPATH) $(TEST_RPATH)

UTILITY_TEST_SRCS = byteorder_test.cpp \
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
$(CXX) $(CXXFLAGS) -o $@ ../driver.o $^ $(LDFLAGS) $(ALL_RPATH)
endef

define make_each_subdirectories
@for dir in $(ALL_SUBDIRS); do \
	$(MAKE) -C ./$$dir $@; \
done
endef

.PHONY: all lib clean clean_dep clean_all dep run

all: lib $(TARGET)

dep: $(DEPENDS)
	$(make_each_subdirectories)

.%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@

%: %.o
	$(build_test)

$(TARGET): $(ALL_LIBS) $(OBJS)
	$(build_test)

lib:
	$(make_each_subdirectories)

run: all
	./$(TARGET)

clean:
	-rm *.o $(ALL_TESTS) $(TARGET) 2> /dev/null
	$(make_each_subdirectories)

clean_dep:
	-rm $(DEPENDS)
	$(make_each_subdirectories)

clean_all:
	-rm *.o $(DEPENDS) $(ALL_TESTS) $(TARGET) 2> /dev/null
	$(make_each_subdirectories)

-include $(DEPENDS)


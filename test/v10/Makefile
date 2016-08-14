INCLUDES = -I../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
# CXX = g++-4.9
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic -DCANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION $(INCLUDES)
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

LIBBULB_DIR = libbulb
LIBBULB_LIB = ./$(LIBBULB_DIR)/libbulb.dylib
LIBBULB_RPATH = -rpath @executable_path/./$(LIBBULB_DIR)

TEST_DIRS = action \
			common_type \
			queue_property \
			utility
TEST_LIBS = $(foreach dir,$(TEST_DIRS),./$(dir)/lib$(dir)_test.dylib)
TEST_RPATH = $(foreach dir,$(TEST_DIRS),-rpath @executable_path/./$(dir))

ALL_SUBDIRS = $(LIBBULB_DIR) $(TEST_DIRS)
ALL_LIBS = $(LIBBULB_LIB) $(TEST_LIBS)
ALL_RPATH = $(LIBBULB_RPATH) $(TEST_RPATH)

TARGET = all_test.out

define make_each_subdirectories
@for dir in $(ALL_SUBDIRS); do \
	$(MAKE) -C ./$$dir $@; \
done
endef

.PHONY: all lib run dep clean clean_dep clean_all

all: lib $(TARGET)

lib:
	$(make_each_subdirectories)

run: all
	./$(TARGET)

dep:
	$(make_each_subdirectories)

clean:
	$(make_each_subdirectories)

clean_dep:
	$(make_each_subdirectories)

clean_all:
	$(make_each_subdirectories)

$(TARGET): $(ALL_LIBS)
	$(CXX) -DBOOST_TEST_MODULE=$@ $(CXXFLAGS) -c ../driver.cpp -o ../driver.o
	$(CXX) $(CXXFLAGS) -o $@ ../driver.o $^ $(LDFLAGS) $(ALL_RPATH)


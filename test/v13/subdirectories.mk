INCLUDES = -I../../../include
LDFLAGS = -lboost_unit_test_framework-mt -lboost_system-mt
CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic -DCANARD_NET_OFP_SUPPRESS_IMPLICIT_INSTANTIATION -DCANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION $(INCLUDES)
# CXX = g++-4.9
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

OBJ_DIR = obj
DEPENDS_DIR = depends

LIBBULB_DIR = ../libbulb
LIBBULB = $(LIBBULB_DIR)/libbulb.dylib

OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
DEPENDS = $(addprefix $(DEPENDS_DIR)/,$(SRCS:.cpp=.depends))
ALL_TESTS = $(SRCS:.cpp=.out)
TARGET = all_test.out

define build_test
$(CXX) -DBOOST_TEST_MODULE=$@ $(CXXFLAGS) -c ../../driver.cpp -o ../../driver.o
$(CXX) $(CXXFLAGS) -o $@ ../../driver.o $^ $(LDFLAGS) \
	-rpath @executable_path/. -rpath @executable_path/$(LIBBULB_DIR)
endef

.PHONY: all lib clean clean_dep clean_all dep run

all: $(TARGET)

lib: $(TARGET_LIB)

$(LIBBULB):
	$(MAKE) -C $(LIBBULB_DIR) all

$(DEPENDS_DIR)/%.depends: %.cpp
	@mkdir -p $(DEPENDS_DIR)
	$(CXX) -MM $(CXXFLAGS) $< \
	| sed 's:\($(notdir $(@:.depends=.o))\):$(OBJ_DIR)/\1:' > $@
	@[ -s $@ ] || rm -f $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.out: $(OBJ_DIR)/%.o
	$(build_test)

$(TARGET_LIB): $(LIBBULB) $(OBJS)
	$(CXX) -dynamiclib -install_name @rpath/$@ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET): $(TARGET_LIB)
	$(build_test)

run: all
	./$(TARGET) -l message

clean:
	-rm -rf $(OBJ_DIR) $(ALL_TESTS) $(TARGET_LIB) $(TARGET) 2> /dev/null

clean_dep:
	-rm -rf $(DEPENDS_DIR)

clean_all: clean_dep clean

-include $(DEPENDS)


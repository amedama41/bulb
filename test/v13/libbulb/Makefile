INCLUDES = -I../../../include
CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -pedantic -DCANARD_NET_OFP_IMPOSE_EXPLICIT_INSTANTIATION $(INCLUDES)
# CXX = g++-4.9
# CXXFLAGS = -std=c++11 -Wall -pedantic $(INCLUDES)

SRCS = instantiation_def.cpp
LDFLAGS = -lboost_system-mt
TARGET = libbulb.dylib

OBJ_DIR = obj
DEPENDS_DIR = depends

OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
DEPENDS = $(addprefix $(DEPENDS_DIR)/,$(SRCS:.cpp=.depends))

.PHONY: all lib clean clean_dep clean_all dep

all: lib

lib: $(TARGET)

dep: $(DEPENDS)

$(DEPENDS_DIR)/%.depends: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CXXFLAGS) -x c++ $< \
	| sed 's:\($(notdir $(@:.depends=.o))\):$(OBJ_DIR)/\1:' > $@
	@[ -s $@ ] || rm -f $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -x c++ $< -o $@

$(TARGET): $(OBJS)
	$(CXX) -dynamiclib -install_name @rpath/$@ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-rm -rf $(OBJ_DIR) $(TARGET) 2> /dev/null

clean_dep:
	-rm -rf $(DEPENDS_DIR)

clean_all: clean_dep clean

-include $(DEPENDS)


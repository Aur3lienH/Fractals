CC = g++

TARGET_EXEC ?= fractals
BUILD_DIR ?= ./build
SRC_DIR ?= ./src

INCLUDES = -I$(glfw_inc) -I$(glad_inc)
LIBRARIES = -L$(glfw_lib) 

CXXFLAGS = -Wall -ggdb -O3 $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lSDL2 -lGLEW -lglut

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all : $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@ -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lSDL2 -lGLEW -lglut

$(BUILD_DIR)/%.cpp.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< 


.PHONY: clean

clean:
	rm -f ${BUILD_DIR}/${TARGET_EXEC}
	rm -d -r ${BUILD_DIR}/src

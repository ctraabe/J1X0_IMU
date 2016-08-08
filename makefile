TARGET := $(notdir $(shell pwd))

CCFLAGS    := -std=gnu99
CPPFLAGS   := -std=c++11
LDLIBS     := -lm
LDFLAGS    := -g

CC         := gcc
CPP        := g++

# If the environment variable DEV_BUILD_PATH is set, then the build files will
# be placed there in a named sub-folder, otherwise a build directory will be
# created in the current directory
ifneq ($(DEV_BUILD_PATH),)
  BUILD_PATH := $(DEV_BUILD_PATH)/build/$(TARGET)
  BIN_PATH   := $(DEV_BUILD_PATH)/bin
else
  BUILD_PATH := build
  BIN_PATH   := bin
endif
INSTALL_PATH ?= /usr/local

SOURCES   = $(wildcard *.c)
SOURCES  += $(wildcard *.cpp)
DEPENDS   = $(addsuffix .d, $(addprefix $(BUILD_PATH)/, $(SOURCES)))
OBJECTS   = $(addsuffix .o, $(addprefix $(BUILD_PATH)/, $(SOURCES)))


# Rule to make dependency "makefiles"
$(BUILD_PATH)/%.c.d: %.c
	mkdir -p $(BUILD_PATH)
	$(CC) $(CCFLAGS) -MM -MT '$(addprefix $(BUILD_PATH)/, $(<:.c=.o)) $@' $< -MF $@

# Rule to make dependency "makefiles"
$(BUILD_PATH)/%.cpp.d: %.cpp
	mkdir -p $(BUILD_PATH)
	$(CPP) $(CPPFLAGS) -MM -MT '$(addprefix $(BUILD_PATH)/, $(<:.cpp=.o)) $@' $< -MF $@

# Rule to make the compiled objects
$(BUILD_PATH)/%.c.o: %.c $(BUILD_PATH)/%.c.d
	$(CC) -c $(CCFLAGS) -o $@ $<

# Rule to make the compiled objects
$(BUILD_PATH)/%.cpp.o: %.cpp $(BUILD_PATH)/%.cpp.d
	$(CPP) -c $(CPPFLAGS) -o $@ $<

# Declare targets that are not files
.PHONY: install clean uninstall


# Note that without an argument, make simply tries to build the first target
# (not rule), which in this case is $(TARGET)
$(BIN_PATH)/$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_PATH)
	g++ $(LDFLAGS) -o $(BIN_PATH)/$(TARGET) $(OBJECTS) $(LDLIBS)

install: $(INSTALL_PATH)
	mkdir -p $(INSTALL_PATH)/$(TARGET)
	cp $(BIN_PATH)/$(TARGET) $(INSTALL_PATH)/.

clean: $(BUILD_PATH)
	rm -f $(OBJECTS) $(DEPENDS) $(BIN_PATH)/$(TARGET)
	rmdir $(BUILD_PATH)
ifeq ($(DEV_BUILD_PATH),)
	rmdir $(BIN_PATH)
endif

uninstall:
	rm -rf $(INSTALL_PATH)/$(TARGET)

# Include the dependency "makefiles"
ifneq ($(MAKECMDGOALS), clean)
  -include $(DEPENDS)
endif

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH)
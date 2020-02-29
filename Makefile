# vim 설정
# vim:ts=4:noexpandtab:shiftwidth=4:fileencoding=utf-8:

.PHONY: clean extclean test check run pkg

include func.mk

EXT_DIR    := externals
DEP_DIR    := dep
BUILD_DIR  := build

JSON_DIR     := $(EXT_DIR)/json

EXT_LIB_DIRS :=

INC_DIRS  := include \
             $(JSON_DIR)/include

SYS_INC_DIRS  := $(JSON_DIR)/include

INC_DIR_OPTION := $(addprefix -I, $(INC_DIRS)) $(addprefix -isystem, $(SYS_INC_DIRS))

CXXFLAGS := --std=c++17 -O0 -g -Wall -Werror -fpic -Wno-non-virtual-dtor $(INC_DIR_OPTION) -pthread

DLIBS := stdc++ \
         dl \
         pthread

LIB_DIRS :=

SLIBS := 

LDFLAGS := $(addprefix -L,$(LIB_DIRS)) $(addprefix -l,$(DLIBS)) $(SLIBS) -pthread

SRC_CPP := $(sort $(shell find src -name *.cpp))
SRC_OBJS := $(SRC_CPP:.cpp=.o)

OBJS := $(addprefix $(BUILD_DIR)/,$(SRC_OBJS))
DEPS := $(addprefix $(DEP_DIR)/,$(SRC_OBJS:.o=.d))

TARGET := $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(call LINK_IT, -o $@ $^ $(LDFLAGS))

# Order only prerequisite to make sure modelgen runs before generating dependency rules.
# Be careful not to use pattern rule or static pattern rule. Only static rule works for the purpose.
$(DEPS): | $(EXT_LIB_DIRS)
$(DEPS): $(DEP_DIR)/%.d: %.cpp
	$(call DEP_IT, -MM -MT $(@:$(DEP_DIR)%.d=$(BUILD_DIR)%.o) $(CXXFLAGS) $< > $@)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), extclean)
-include $(DEPS)
endif
endif

$(BUILD_DIR)/%.o: %.cpp
	$(call LINT_IT, -f $< $(INC_DIR_OPTION) -a src -a include)
	$(call COMPILE_IT, $(CXXFLAGS) -c -o $@ $<)

test: check

check:
	$(MAKE) -C unittest clean
	$(MAKE) -C unittest check

clean:
	$(RM) -r info.txt
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(DEP_DIR)

dist: all
	$(MAKE) -C dist dist

pkg:
	$(MAKE) -C dist dist pkg

run : dist
	./dist/run.sh


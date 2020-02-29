# vim 설정
# vim:ts=4:noexpandtab:shiftwidth=4:fileencoding=utf-8:

.PHONY: all release verbose

# verbose mode 
VERB=$(filter verbose,$(MAKECMDGOALS))
ifeq ($(VERB), verbose)
export VERBOSE_MODE = 1
else
export VERBOSE_MODE = 0
endif

ifeq "$(VERBOSE_MODE)" "1"
	silent =
	Q      =
else
	silent = silent_
	Q      = @
endif

# debug mode 
REL=$(filter release,$(MAKECMDGOALS))

ifeq ($(REL), release)
export DEBUG_MODE = 0
VERSION_OPT_STR = RELEASE
else
export DEBUG_MODE = 1
VERSION_OPT_STR = DEBUG
endif

ifeq ($(DEBUG_MODE), 1)
	OPTIMIZE_LEVEL=0
else
	OPTIMIZE_LEVEL=2
endif

silent_compile_msg          = COMPILING ($(VERSION_OPT_STR) MODE) $< 
silent_dependency_check_msg = GEN DEP FOR $<
silent_link_msg             = LINKING $@
silent_gen_model_msg        = GEN MODEL FOR $<
silent_lint_msg        	    = NMLINTING $<

define DEP_IT
    $(Q) mkdir -p $(abspath $(DEP_DIR)/$(dir $<))
	$(Q) $(if $(silent), echo ' $(silent_dependency_check_msg)')
	$(Q) $(CXX) $(1)
endef

define COMPILE_IT
	$(Q) mkdir -p $(abspath $(BUILD_DIR)/$(dir $<))
	$(Q) $(if $(silent), echo ' $(silent_compile_msg)')
	$(Q) $(CXX) $(1)
endef

define LINK_IT
	$(Q) $(if $(silent),echo ' $(silent_link_msg)')
	$(Q) $(CXX) $(1)
endef

define GEN_MODEL
	$(Q) $(if $(silent),echo ' $(silent_gen_model_msg)')
	$(Q) $(MODEL_GEN) $(1)
endef

DO_LINTING = 0

ifeq "$(DO_LINTING)" "1"
define LINT_IT
	$(Q) $(if $(silent),echo ' $(silent_lint_msg)')
	$(Q) nmlint/conf/smtpdev.py $(1)
endef
else
define LINT_IT
endef
endif

# make 의 command line argument 에서 verbose, debug 를 추출해 내기 위해 꼼수를 좀 부렸다.
# make 는 shell 환경변수에 있는 값으로 Makefile 의 변수를 override 하므로
# export 를 하게 되면 각각의 make command 로 변수를 내려 줄 필요 없이 한방에 다 내려간다
release debug verbose: all


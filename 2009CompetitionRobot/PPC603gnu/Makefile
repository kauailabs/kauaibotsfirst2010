# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
PRJ_ROOT_DIR := C:/WindRiver/workspace/DashboardDataExample
WS_ROOT_DIR := C:/WindRiver/workspace



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp  -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip -I. 

IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = DashboardDataExample/$(MODE_DIR)/DashboardDataExample.out \
	 DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage.o


#Rules

# DashboardDataExample
ifeq ($(DEBUG_MODE),1)
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
DashboardDataExample/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
DashboardDataExample/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip -I. 
DashboardDataExample/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
DashboardDataExample/$(MODE_DIR)/% : PROJECT_TYPE = DKM
DashboardDataExample/$(MODE_DIR)/% : DEFINES = 
DashboardDataExample/$(MODE_DIR)/% : EXPAND_DBG = 0
DashboardDataExample/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
DashboardDataExample/$(MODE_DIR)/% : CPU = PPC603
DashboardDataExample/$(MODE_DIR)/% : TOOL_FAMILY = gnu
DashboardDataExample/$(MODE_DIR)/% : TOOL = gnu
DashboardDataExample/$(MODE_DIR)/% : TOOL_PATH = 
DashboardDataExample/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp  -mlongcall
DashboardDataExample/$(MODE_DIR)/% : LIBPATH = 
DashboardDataExample/$(MODE_DIR)/% : LIBS = 
DashboardDataExample/$(MODE_DIR)/% : OBJ_DIR := DashboardDataExample/$(MODE_DIR)

OBJECTS_DashboardDataExample = DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage.o

DashboardDataExample/$(MODE_DIR)/DashboardDataExample.out : $(OBJECTS_DashboardDataExample)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_DashboardDataExample) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_DashboardDataExample) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

DashboardDataExample/$(MODE_DIR)/DashboardDataExample_compile_file : $(FILE) ;

_clean :: DashboardDataExample/$(MODE_DIR)/DashboardDataExample_clean

DashboardDataExample/$(MODE_DIR)/DashboardDataExample_clean : 
	$(TRACE_FLAG)if [ -d "DashboardDataExample" ]; then cd "DashboardDataExample"; rm -rf $(MODE_DIR); fi


# DashboardDataExample_partialImage
ifeq ($(DEBUG_MODE),1)
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
DashboardDataExample_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip -I. 
DashboardDataExample_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
DashboardDataExample_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
DashboardDataExample_partialImage/$(MODE_DIR)/% : DEFINES = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
DashboardDataExample_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
DashboardDataExample_partialImage/$(MODE_DIR)/% : CPU = PPC603
DashboardDataExample_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
DashboardDataExample_partialImage/$(MODE_DIR)/% : TOOL = gnu
DashboardDataExample_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp  -mlongcall
DashboardDataExample_partialImage/$(MODE_DIR)/% : LIBPATH = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : LIBS = 
DashboardDataExample_partialImage/$(MODE_DIR)/% : OBJ_DIR := DashboardDataExample_partialImage/$(MODE_DIR)

DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataExample.o : C:/WindRiver/workspace/DashboardDataExample/DashboardDataExample.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataFormat.o : C:/WindRiver/workspace/DashboardDataExample/DashboardDataFormat.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/GameDrive.o : C:/WindRiver/workspace/DashboardDataExample/GameDrive.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/LogitechGamepad.o : C:/WindRiver/workspace/DashboardDataExample/LogitechGamepad.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Potentiometer.o : C:/WindRiver/workspace/DashboardDataExample/Potentiometer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Speedmotor.o : C:/WindRiver/workspace/DashboardDataExample/Speedmotor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Target.o : C:/WindRiver/workspace/DashboardDataExample/Target.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/TargetDetector.o : C:/WindRiver/workspace/DashboardDataExample/TargetDetector.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_DashboardDataExample_partialImage = DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataExample.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataFormat.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/GameDrive.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/LogitechGamepad.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Potentiometer.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Speedmotor.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Target.o \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/TargetDetector.o

DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage.o : $(OBJECTS_DashboardDataExample_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_DashboardDataExample_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage_compile_file : $(FILE) ;

_clean :: DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage_clean

DashboardDataExample_partialImage/$(MODE_DIR)/DashboardDataExample_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "DashboardDataExample_partialImage" ]; then cd "DashboardDataExample_partialImage"; rm -rf $(MODE_DIR); fi

DEP_FILES := DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataExample.d DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/DashboardDataFormat.d DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/GameDrive.d \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/LogitechGamepad.d DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Potentiometer.d DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Speedmotor.d \
	 DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/Target.d DashboardDataExample_partialImage/$(MODE_DIR)/Objects/DashboardDataExample/TargetDetector.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""

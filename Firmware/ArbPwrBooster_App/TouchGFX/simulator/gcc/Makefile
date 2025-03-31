# Helper macros to convert spaces into question marks and back again
e := 
sp := $(e) $(e)
qs = $(subst ?,$(sp),$1)
sq = $(subst $(sp),?,$1)

# Get name of this Makefile (avoid getting word 0 and a starting space)
makefile_name := $(wordlist 1,1000,$(MAKEFILE_LIST))

# Get path of this Makefile
makefile_path := $(call qs,$(dir $(call sq,$(abspath $(call sq,$(makefile_name))))))

# Get path where the Application is
application_path := $(call qs,$(abspath $(call sq,$(makefile_path)../..)))

.PHONY: clean assets all

ifneq ($(words $(makefile_path))$(words $(MAKEFILE_LIST)),11)
all clean assets:
$(error Spaces not allowed in path)
else

ADDITIONAL_SOURCES_A := ../STM32CubeIDE/Application/User/App_Support/Src/IO_Support.c
ADDITIONAL_SOURCES_B := ../STM32CubeIDE/Application/User/App_Support/Src/ADC_Support.c
ADDITIONAL_SOURCES += ../STM32CubeIDE/Application/User/App_Support/Src/Main_Support.c
ADDITIONAL_INCLUDE_PATHS_A := ../Drivers/STM32F7xx_HAL_Driver/Inc
ADDITIONAL_INCLUDE_PATHS += ../STM32CubeIDE/Application/User/App_Support/Inc
ADDITIONAL_LIBRARY_PATHS := 
ADDITIONAL_LIBRARIES :=
export ADDITIONAL_SOURCES ADDITIONAL_INCLUDE_PATHS ADDITIONAL_LIBRARY_PATHS ADDITIONAL_LIBRARIES ADDITIONAL_SOURCES_A ADDITIONAL_SOURCES_B ADDITIONAL_INCLUDE_PATHS_A

# Add the additional include paths to the CFLAGS
CFLAGS += -I$(ADDITIONAL_INCLUDE_PATHS)

all: $(filter assets,$(MAKECMDGOALS))
all assets: $(filter clean,$(MAKECMDGOALS))
all clean assets:
	@$(MAKE) -r -f generated/simulator/gcc/Makefile -s $(MFLAGS) $@ -C "$(application_path)"
endif


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include libini/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: slam-gateway

# Tool invocations
slam-gateway: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++-4.8 -L"/home/xd/work/ruiwei/SlamGateway/gateway/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-debug/lib" -static -fPIC -o "slam-gateway" $(OBJS) $(USER_OBJS) $(LIBS)  -Xlinker "-(" -lstdc++ -ldl -lrt -lm -lpthread -lrpos_robotplatforms_rpslamware -lrpos_framework -lboost_atomic -lboost_chrono -lboost_date_time -lboost_regex -lboost_system -lboost_filesystem -lboost_thread -lbase64 -ljsoncpp -lrlelib -lcrypto -lcurl -lssl -Xlinker "-)"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) slam-gateway
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets

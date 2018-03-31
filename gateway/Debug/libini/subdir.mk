################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libini/dictionary.cpp \
../libini/iniparser.cpp \
../libini/strlib.cpp 

OBJS += \
./libini/dictionary.o \
./libini/iniparser.o \
./libini/strlib.o 

CPP_DEPS += \
./libini/dictionary.d \
./libini/iniparser.d \
./libini/strlib.d 


# Each subdirectory must supply rules for building sources it contributes
libini/%.o: ../libini/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.8 -std=c++0x -I"/home/hou/SlamGateway/gateway/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-debug/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HxConfig.cpp \
../src/main.cpp \
../src/mqtt_wrap.cpp \
../src/utils.cpp 

C_SRCS += \
../src/cJSON.c 

OBJS += \
./src/HxConfig.o \
./src/cJSON.o \
./src/main.o \
./src/mqtt_wrap.o \
./src/utils.o 

CPP_DEPS += \
./src/HxConfig.d \
./src/main.d \
./src/mqtt_wrap.d \
./src/utils.d 

C_DEPS += \
./src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/home/xd/work/ruiwei/SlamGateway/gateway/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-debug/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



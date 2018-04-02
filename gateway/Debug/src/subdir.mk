################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HxConfig.cpp \
../src/Message.cpp \
../src/Slam.cpp \
../src/main.cpp \
../src/mqtt_wrap.cpp \
../src/utils.cpp 

C_SRCS += \
../src/cJSON.c 

OBJS += \
./src/HxConfig.o \
./src/Message.o \
./src/Slam.o \
./src/cJSON.o \
./src/main.o \
./src/mqtt_wrap.o \
./src/utils.o 

CPP_DEPS += \
./src/HxConfig.d \
./src/Message.d \
./src/Slam.d \
./src/main.d \
./src/mqtt_wrap.d \
./src/utils.d 

C_DEPS += \
./src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.8 -std=c++0x -I"/home/xd/work/ruiwei/SlamGateway/gateway/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-debug/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc-4.8 -std=gnu99 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



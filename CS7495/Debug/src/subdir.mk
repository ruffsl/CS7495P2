################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Extractor.cpp \
../src/Image.cpp \
../src/gmm.cpp \
../src/main.cpp \
../src/numerics.cpp \
../src/utils.cpp 

OBJS += \
./src/Extractor.o \
./src/Image.o \
./src/gmm.o \
./src/main.o \
./src/numerics.o \
./src/utils.o 

CPP_DEPS += \
./src/Extractor.d \
./src/Image.d \
./src/gmm.d \
./src/main.d \
./src/numerics.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



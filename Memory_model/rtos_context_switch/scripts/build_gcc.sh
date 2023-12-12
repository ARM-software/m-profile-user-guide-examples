cd projects/m-profile/memory_model/rtos_context_switch
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMv8MML/Include -I $CMSIS/CMSIS/Core/Include -I ."

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c main.c -o logs/main.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/main.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c mpu_configs.c -o logs/mpu_configs.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/mpu_configs.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c mpu_prog.c -o logs/mpu_prog.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/mpu_prog.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c mpu_reprog.c -o logs/mpu_reprog.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/mpu_reprog.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c myRTOS.c -o logs/myRTOS.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/myRTOS.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c threadA.c -o logs/threadA.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/threadA.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mlittle-endian -O1 -c threadB.c -o logs/threadB.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/threadB.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc  -march=armv8-m.main -mlittle-endian -O1  -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc  -march=armv8-m.main -mlittle-endian -O1  -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections -march=armv8-m.main --specs=rdimon.specs logs/startup.o logs/system.o logs/main.o logs/mpu_configs.o logs/mpu_prog.o logs/mpu_reprog.o logs/myRTOS.o logs/threadA.o logs/threadB.o  -o logs/rtos_context_switch.axf
echo "Image built. Use \"make run search=rtos_context_switch\" to run the example on an FVP."
cd projects/m-profile/exception_model/context-switch-fp
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
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c main.c -o logs/main.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/main.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c scheduler.c -o logs/scheduler.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/scheduler.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c task1.c -o logs/task1.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/task1.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c task2.c -o logs/task2.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/task2.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c RTE/Device/ARMv8MML_DP/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp.dp -mlittle-endian -mfloat-abi=hard -O1 -c RTE/Device/ARMv8MML_DP/system_ARMv8MML.c -o logs/system.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections -march=armv8-m.main+fp.dp -mfloat-abi=hard --specs=rdimon.specs logs/startup.o logs/system.o logs/main.o logs/scheduler.o logs/task1.o logs/task2.o  -lm -o logs/context-switch-fp.axf
echo "Image built. Use \"make run search=context-switch-fp\" to run the example on an FVP."
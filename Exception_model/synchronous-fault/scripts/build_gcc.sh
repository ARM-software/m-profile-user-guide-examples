cd projects/m-profile/exception_model/synchronous-fault
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
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mlittle-endian -mfloat-abi=hard -O1 -mfpu=fpv4-sp-d16 -c main.c -o logs/main.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/main.d
echo "Compiling  funcs.s..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mlittle-endian -mfloat-abi=hard -O1 -mfpu=fpv4-sp-d16 -c funcs.s -o logs/funcs.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/funcs.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mlittle-endian -mfloat-abi=hard -O1 -mfpu=fpv4-sp-d16 -c RTE/Device/ARMv8MML_DP/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mlittle-endian -mfloat-abi=hard -O1 -mfpu=fpv4-sp-d16 -c RTE/Device/ARMv8MML_DP/system_ARMv8MML.c -o logs/system.o -DARMv8MML_DP $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mfloat-abi=hard -mfpu=fpv4-sp-d16 --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections --specs=rdimon.specs logs/startup.o logs/system.o logs/funcs.o logs/main.o -o logs/synchronous-fault.axf 
echo "Image built. Use \"make run search=synchronous-fault\" to run the example on an FVP."
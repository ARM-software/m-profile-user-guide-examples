cd projects/m-profile/exception_model/irq-priority-basic
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
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mfloat-abi=soft -mlittle-endian -O1 -c main.c -o logs/main.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/main.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mfloat-abi=soft -mlittle-endian -O1 -c IRQConfig.c -o logs/IRQConfig.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/IRQConfig.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mfloat-abi=soft -mlittle-endian -O1 -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main -mfloat-abi=soft -mlittle-endian -O1 -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections -march=armv8-m.main --specs=rdimon.specs logs/startup.o logs/system.o logs/main.o logs/IRQConfig.o -o logs/irq-priority-basic.axf 
echo "Image built. Use \"make run search=irq-priority-basic \" to run the example on an FVP."
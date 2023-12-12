cd projects/m-profile/memory_model/rtos_context_switch
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMv8MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
SCAT_FILE="RTE/Device/ARMv8MML/ARMv8MML_ac6.sct"

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c main.c -o logs/main.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c mpu_configs.c -o logs/mpu_configs.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c mpu_prog.c -o logs/mpu_prog.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c mpu_reprog.c -o logs/mpu_reprog.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c myRTOS.c -o logs/myRTOS.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c threadA.c -o logs/threadA.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c threadB.c -o logs/threadB.o -DARMv8MML $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/main.o logs/mpu_configs.o logs/mpu_prog.o logs/mpu_reprog.o logs/myRTOS.o logs/threadA.o logs/threadB.o  -o logs/rtos_context_switch.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/rtos_context_switch.axf -o logs/rtos_context_switch.bin
$COMPILER/fromelf -c logs/rtos_context_switch.axf -o logs/rtos_context_switch.disass
echo "Image built. Use \"make run search=rtos_context_switch\" to run the example on an FVP."

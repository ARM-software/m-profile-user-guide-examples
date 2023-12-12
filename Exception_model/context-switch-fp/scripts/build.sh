cd projects/m-profile/exception_model/context-switch-fp
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMv8MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
SCAT_FILE="RTE/Device/ARMv8MML_DP/ARMv8MML_ac6.sct"

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c main.c -o logs/main.o -DARMv8MML_DP $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c scheduler.c -o logs/scheduler.o -DARMv8MML_DP $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c task1.c -o logs/task1.o -DARMv8MML_DP $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c task2.c -o logs/task2.o -DARMv8MML_DP $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv8MML_DP/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML_DP $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv8MML_DP/system_ARMv8MML.c -o logs/system.o -DARMv8MML_DP $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler --info=sizes logs/startup.o logs/system.o logs/main.o logs/scheduler.o logs/task1.o logs/task2.o -o logs/context-switch-fp.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/context-switch-fp.axf -o logs/context-switch-fp.bin
$COMPILER/fromelf -c logs/context-switch-fp.axf -o logs/context-switch-fp.disass
echo "Image built. Use \"make run search=context-switch-fp\" to run the example on an FVP."
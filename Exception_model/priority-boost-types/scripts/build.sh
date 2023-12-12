cd projects/m-profile/exception_model/priority-boost-types
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -mlittle-endian -c IRQSet.c -o logs/IRQSet.o -DARMv8MML $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/main.o logs/IRQSet.o -o logs/priority-boost-types.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/priority-boost-types.axf -o logs/priority-boost-types.bin
$COMPILER/fromelf -c logs/priority-boost-types.axf -o logs/priority-boost-types.disass
echo "Image built. Use \"make run search=priority-boost-types\" to run the example on an FVP."

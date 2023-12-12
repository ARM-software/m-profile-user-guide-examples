cd projects/m-profile/exception_model/synchronous-fault
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main+fp -O1 -g -mfloat-abi=hard -mlittle-endian -c main.c -o logs/main.o -DARMv8MML_DP $INCLUDES
echo "Compiling  funcs.s..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main+fp -O1 -g -mfloat-abi=hard -mlittle-endian -c funcs.s -o logs/funcs.o -DARMv8MML_DP $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main+fp -O1 -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv8MML_DP/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML_DP $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main+fp -O1 -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv8MML_DP/system_ARMv8MML.c -o logs/system.o -DARMv8MML_DP $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/funcs.o logs/main.o -o logs/synchronous-fault.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/synchronous-fault.axf -o logs/synchronous-fault.bin
$COMPILER/fromelf -c logs/synchronous-fault.axf -o logs/synchronous-fault.disass
echo "Image built. Use \"make run search=synchronous-fault\" to run the example on an FVP."

cd projects/m-profile/memory_model/trap_access
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c main.c -o logs/main.o -DARMv8MML $INCLUDES
echo "Compiling  library function mpu_prog.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c mpu_prog.c -o logs/mpu_prog.o -DARMv8MML $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -g -mfpu=none -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/main.o logs/mpu_prog.o -o logs/trap_access.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/trap_access.axf -o logs/trap_access.bin
$COMPILER/fromelf -c logs/trap_access.axf -o logs/trap_access.disass
echo "Image built. Use \"make run search=trap_access\" to run the example on an FVP."

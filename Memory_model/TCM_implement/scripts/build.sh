cd projects/m-profile/memory_model/TCM_implement
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMCM55/Include -I $CMSIS/CMSIS/Core/Include -I ."
SCAT_FILE="RTE/Device/ARMCM55/ARMCM55_ac6.sct"

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c main.c -o logs/main.o -DARMCM55 $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c mpu_prog.c -o logs/mpu_prog.o -DARMCM55 $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c Excep_prog.c -o logs/Excep_prog.o -DARMCM55 $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c check_is_full.c -o logs/check_is_full.o -DARMCM55 $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMCM55/startup_ARMCM55.c -o logs/startup.o -DARMCM55 $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -mcpu=cortex-m55 -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMCM55/system_ARMCM55.c -o logs/system.o -DARMCM55 $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/main.o logs/mpu_prog.o logs/Excep_prog.o logs/check_is_full.o -o logs/TCM_implement.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/TCM_implement.axf -o logs/TCM_implement.bin
$COMPILER/fromelf -c logs/TCM_implement.axf -o logs/TCM_implement.disass
echo "Image built. Use \"make run search=TCM_implement\" to run the example on an FVP."

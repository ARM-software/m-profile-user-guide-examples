cd projects/m-profile/exception_model/svc-number-as-parameter
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -mfpu=none -mlittle-endian -c main.c -o logs/main.o -DARMv8MML $INCLUDES
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -mfpu=none -mlittle-endian -c SelectSVCNumber.c -o logs/SelectSVCNumber.o -DARMv8MML $INCLUDES
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -mfpu=none -c RTE/Device/ARMv8MML/startup_ARMv8MML.c -o logs/startup.o -DARMv8MML $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8-m.main -O1 -mfpu=none -c RTE/Device/ARMv8MML/system_ARMv8MML.c -o logs/system.o -DARMv8MML $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler logs/startup.o logs/system.o logs/main.o logs/SelectSVCNumber.o -o logs/svc-number-as-parameter.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/svc-number-as-parameter.axf -o logs/svc-number-as-parameter.bin
$COMPILER/fromelf -c logs/svc-number-as-parameter.axf -o logs/svc-number-as-parameter.disass
echo "Image built. Use \"make run search=svc-number-as-parameter\" to run the example on an FVP."

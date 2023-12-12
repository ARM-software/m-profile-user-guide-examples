cd projects/m-profile/exception_model/interrupt-deprivileging
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
SCAT_FILE="RTE/Device/ARMv81MML_DSP_DP_MVE_FP/ARMv81MML_ac6.sct"

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c main.c -o logs/main.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -Wmacro-redefined
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c PrivilegedFuncs.c -o logs/PrivilegedFuncs.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -Wmacro-redefined
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c UnprivilegedFuncs.c -o logs/UnprivilegedFuncs.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -Wmacro-redefined
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c mpu_prog.c -o logs/mpu_prog.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -Wmacro-redefined
echo "Compiling  startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv81MML_DSP_DP_MVE_FP/startup_ARMv81MML.c -o logs/startup.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES
echo "Compiling  system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c RTE/Device/ARMv81MML_DSP_DP_MVE_FP/system_ARMv81MML.c -o logs/system.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES
echo "Linking  image..."
$COMPILER/armlink --scatter=$SCAT_FILE --entry=Reset_Handler --info=sizes logs/startup.o logs/system.o logs/main.o logs/PrivilegedFuncs.o logs/UnprivilegedFuncs.o logs/mpu_prog.o -o logs/interrupt-deprivileging.axf --list=logs/image.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/interrupt-deprivileging.axf -o logs/interrupt-deprivileging.bin
$COMPILER/fromelf -c logs/interrupt-deprivileging.axf -o logs/interrupt-deprivileging.disass
echo "Image built. Use \"make run search=interrupt-deprivileging\" to run the example on an FVP."
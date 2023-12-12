cd projects/m-profile/exception_model/interrupt-deprivileging
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c main.c -o logs/main.o -D ARMv81MML_DSP_DP_MVE_FP -Wbuiltin-macro-redefined $INCLUDES -g -MMD -MP -MF logs/main.d
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c PrivilegedFuncs.c -o logs/PrivilegedFuncs.o -D ARMv81MML_DSP_DP_MVE_FP -Wbuiltin-macro-redefined $INCLUDES -g -MMD -MP -MF logs/PrivilegedFuncs.d
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c UnprivilegedFuncs.c -o logs/UnprivilegedFuncs.o -D ARMv81MML_DSP_DP_MVE_FP -Wbuiltin-macro-redefined $INCLUDES -g -MMD -MP -MF logs/UnprivilegedFuncs.d
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c mpu_prog.c -o logs/mpu_prog.o -D ARMv81MML_DSP_DP_MVE_FP -Wbuiltin-macro-redefined $INCLUDES -g -MMD -MP -MF logs/mpu_prog.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c RTE/Device/ARMv81MML_DSP_DP_MVE_FP/startup_ARMv81MML.c -o logs/startup.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mlittle-endian -mfloat-abi=hard -O1  -c RTE/Device/ARMv81MML_DSP_DP_MVE_FP/system_ARMv81MML.c -o logs/system.o -D ARMv81MML_DSP_DP_MVE_FP $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve+fp.dp -mfloat-abi=hard --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections --specs=rdimon.specs logs/startup.o logs/system.o logs/main.o logs/PrivilegedFuncs.o logs/UnprivilegedFuncs.o logs/mpu_prog.o -o logs/interrupt-deprivileging.axf
echo "Image built. Use \"make run search=interrupt-deprivileging\" to run the example on an FVP."
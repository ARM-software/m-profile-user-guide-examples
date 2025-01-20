cd projects/m-profile/security/security-func-call-params-passing
CMSIS=../../../../CMSIS_5
RTE_S=security-func-call-params-passing_s/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
RTE_NS=security-func-call-params-passing_ns/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
INCLUDES_S="-I security-func-call-params-passing_s -I $RTE_S -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
INCLUDES_NS="-I security-func-call-params-passing_ns -I $RTE_NS -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."


while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling secure main_s.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -mfloat-abi=hard -mthumb -mcmse -O1 -g -mfpu=fpv5-sp-d16 -c security-func-call-params-passing_s/main_s.c -o logs/main_s.o $INCLUDES_S
echo "Compiling secure initialization init.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -mfloat-abi=hard -mthumb -mcmse -O1 -g -mfpu=fpv5-sp-d16 -c security-func-call-params-passing_s/init.c -o logs/init_s.o $INCLUDES_S
echo "Compiling secure interface.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -mfloat-abi=hard -mthumb -mcmse -O1 -g -mfpu=fpv5-sp-d16 -c security-func-call-params-passing_s/interface.c -o logs/interface_s.o $INCLUDES_S
echo "Compiling secure startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -O1 -g -mfloat-abi=hard -mthumb -mcmse -c $RTE_S/startup_ARMv81MML.c -o logs/startup_s.o $INCLUDES_S
echo "Compiling secure system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -O1 -g -mfloat-abi=hard -mthumb -mcmse -c $RTE_S/system_ARMv81MML.c -o logs/system_s.o $INCLUDES_S
echo "Compiling non-secure main_ns.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -mfloat-abi=hard -O1 -g -mfpu=fpv5-sp-d16 -c security-func-call-params-passing_ns/main_ns.c -o logs/main_ns.o $INCLUDES_NS
echo "Compiling  non-secure callback_ns.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -mfloat-abi=hard -O1 -g -mfpu=fpv5-sp-d16 -c security-func-call-params-passing_ns/callback_ns.c -o logs/callback_ns.o $INCLUDES_NS
echo "Compiling non-secure startup file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -O1 -g -mfloat-abi=hard -c $RTE_NS/startup_ARMv81MML.c -o logs/startup_ns.o $INCLUDES_NS
echo "Compiling non-secure system file..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -DARMv81MML_DSP_DP_MVE_FP -O1 -g -mfloat-abi=hard -c $RTE_NS/system_ARMv81MML.c -o logs/system_ns.o $INCLUDES_NS
echo "Linking secure image..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mfpu=fpv5-sp-d16 --entry=Reset_Handler -T scripts/gcc_arm_s.ld -Wl,--gc-sections --specs=rdimon.specs logs/startup_s.o logs/init_s.o logs/system_s.o logs/main_s.o logs/interface_s.o -o logs/security-func-call-params-passing_s.axf -Xlinker --wrap=main -Xlinker --cmse-implib -Xlinker --out-implib=logs/security-func-call-params-passing-CMSE-Lib.o
echo "Linking non-secure image..."
$COMPILER/arm-none-eabi-gcc -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mfpu=fpv5-sp-d16 --entry=Reset_Handler -T scripts/gcc_arm_ns.ld -Wl,--gc-sections --specs=rdimon.specs logs/startup_ns.o logs/system_ns.o logs/main_ns.o logs/callback_ns.o logs/security-func-call-params-passing-CMSE-Lib.o -o logs/security-func-call-params-passing_ns.axf
echo "Images built. Use \"make run search=security-func-call-params-passing\" to run the example on an FVP."

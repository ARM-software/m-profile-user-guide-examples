cd projects/m-profile/security/security-func-call-params-passing
CMSIS=../../../../CMSIS_5
RTE_S=security-func-call-params-passing_s/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
RTE_NS=security-func-call-params-passing_ns/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
INCLUDES_S="-I security-func-call-params-passing_s -I $RTE_S -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
INCLUDES_NS="-I security-func-call-params-passing_ns -I $RTE_NS -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
SCAT_FILE_S=$RTE_S/ARMv81MML_ac6_s.sct
SCAT_FILE_NS=$RTE_NS/ARMv81MML_ac6.sct

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling secure main_s.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c security-func-call-params-passing_s/main_s.c -o logs/main_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure initialization init.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c security-func-call-params-passing_s/init.c -o logs/init.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure interface.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c security-func-call-params-passing_s/interface.c -o logs/interface_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mcmse -mlittle-endian -c $RTE_S/startup_ARMv81MML.c -o logs/startup_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mcmse -mlittle-endian -c $RTE_S/system_ARMv81MML.c -o logs/system_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling non-secure main_ns.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mlittle-endian -O1 -g -mthumb -c security-func-call-params-passing_ns/main_ns.c -o logs/main_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling  non-secure callback_ns.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mlittle-endian -O1 -g -mthumb -c security-func-call-params-passing_ns/callback_ns.c -o logs/callback_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mlittle-endian -c $RTE_NS/startup_ARMv81MML.c -o logs/startup_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mlittle-endian -c $RTE_NS/system_ARMv81MML.c -o logs/system_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Linking secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_S --entry=Reset_Handler --info=sizes logs/startup_s.o logs/system_s.o logs/main_s.o logs/init.o logs/interface_s.o -o logs/security-func-call-params-passing_s.axf --list=logs/image_s.lst --map --import-cmse-lib-out=logs/security-func-call-params-passing-CMSE-Lib.o
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/security-func-call-params-passing_s.axf -o logs/security-func-call-params-passing_s.bin
$COMPILER/fromelf -c logs/security-func-call-params-passing_s.axf -o logs/security-func-call-params-passing_s.disass
echo "Linking non-secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_NS --entry=Reset_Handler --info=sizes logs/startup_ns.o logs/system_ns.o logs/main_ns.o logs/callback_ns.o logs/security-func-call-params-passing-CMSE-Lib.o -o logs/security-func-call-params-passing_ns.axf --list=logs/image_ns.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/security-func-call-params-passing_ns.axf -o logs/security-func-call-params-passing_ns.bin
$COMPILER/fromelf -c logs/security-func-call-params-passing_ns.axf -o logs/security-func-call-params-passing_ns.disass 
echo "Images built. Use \"make run search=security-func-call-params-passing\" to run the example on an FVP."

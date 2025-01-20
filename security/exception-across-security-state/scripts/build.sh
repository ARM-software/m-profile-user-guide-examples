cd projects/m-profile/security/exception-across-security-state
CMSIS=../../../../CMSIS_5
RTE_S=exception-across-security-state_s/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
RTE_NS=exception-across-security-state_ns/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
INCLUDES_S="-I exception-across-security-state_s -I $RTE_S -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
INCLUDES_NS="-I exception-across-security-state_ns -I $RTE_NS -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c exception-across-security-state_s/main_s.c -o logs/main_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure init.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c exception-across-security-state_s/init.c -o logs/init.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure interface.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c exception-across-security-state_s/interface.c -o logs/interface.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c $RTE_S/startup_ARMv81MML.c -o logs/startup_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -c $RTE_S/system_ARMv81MML.c -o logs/system_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling non-secure main_ns.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -O1 -g -c exception-across-security-state_ns/main_ns.c -o logs/main_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure IRQconfig_ns.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -O1 -g -c exception-across-security-state_ns/IRQconfig_ns.c -o logs/IRQconfig_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -O1 -g -c $RTE_NS/startup_ARMv81MML.c -o logs/startup_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -O1 -g -c $RTE_NS/system_ARMv81MML.c -o logs/system_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Linking secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_S --entry=Reset_Handler --info=sizes logs/startup_s.o logs/system_s.o logs/main_s.o logs/init.o logs/interface.o -o logs/exception-across-security-state_s.axf --list=logs/image_s.lst --map --import-cmse-lib-out=logs/exception-across-security-state-CMSE-Lib.o
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/exception-across-security-state_s.axf -o logs/exception-across-security-state_s.bin
$COMPILER/fromelf -c logs/exception-across-security-state_s.axf -o logs/exception-across-security-state_s.disass
echo "Linking non-secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_NS --entry=Reset_Handler --info=sizes logs/startup_ns.o logs/system_ns.o logs/main_ns.o logs/IRQconfig_ns.o logs/exception-across-security-state-CMSE-Lib.o -o logs/exception-across-security-state_ns.axf --list=logs/image_ns.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/exception-across-security-state_ns.axf -o logs/exception-across-security-state_ns.bin
$COMPILER/fromelf -c logs/exception-across-security-state_ns.axf -o logs/exception-across-security-state_ns.disass
echo "Images built. Use \"make run search=exception-across-security-state\" to run the example on an FVP."



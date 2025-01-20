cd projects/m-profile/security/hello-world-in-security-states
CMSIS=../../../../CMSIS_5
RTE_S=hello-world-in-security-states_s/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
RTE_NS=hello-world-in-security-states_ns/RTE/Device/ARMv81MML_DSP_DP_MVE_FP
INCLUDES_S="-I hello-world-in-security-states_s -I $RTE_S -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
INCLUDES_NS="-I hello-world-in-security-states_ns -I $RTE_NS -I $CMSIS/Device/ARM/ARMv81MML/Include -I $CMSIS/CMSIS/Core/Include -I ."
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
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -mfpu=none -c hello-world-in-security-states_s/main_s.c -o logs/main_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure initialization init.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mthumb -mlittle-endian -mcmse -O1 -g -mfpu=none -c hello-world-in-security-states_s/init.c -o logs/init_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mcmse -mlittle-endian -c $RTE_S/startup_ARMv81MML.c -o logs/startup_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mthumb -mcmse -mlittle-endian -c $RTE_S/system_ARMv81MML.c -o logs/system_s.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_S
echo "Compiling non-secure main_ns.c..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mlittle-endian -O1 -g -mfpu=none -c hello-world-in-security-states_ns/main_ns.c -o logs/main_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
#echo "Compiling  non-secure initialization Init.c..."
#$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard -mlittle-endian -O1 -g -mfpu=none -c hello-world-in-security-states_ns/Init.c -o logs/Init_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure startup file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c $RTE_NS/startup_ARMv81MML.c -o logs/startup_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Compiling non-secure system file..."
$COMPILER/armclang --target=arm-arm-none-eabi -march=armv8.1-m.main+mve.fp+fp.dp -O1 -g -mfloat-abi=hard -mlittle-endian -c $RTE_NS/system_ARMv81MML.c -o logs/system_ns.o -DARMv81MML_DSP_DP_MVE_FP $INCLUDES_NS
echo "Linking secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_S --entry=Reset_Handler --info=sizes logs/startup_s.o logs/system_s.o logs/main_s.o logs/init_s.o -o logs/hello-world-in-security-states_s.axf --import_cmse_lib_out=logs/hello-world-in-security-states-CMSE-Lib.o --list=logs/image_s.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/hello-world-in-security-states_s.axf -o logs/hello-world-in-security-states_s.bin
$COMPILER/fromelf -c logs/hello-world-in-security-states_s.axf -o logs/hello-world-in-security-states_s.disass
echo "Linking non-secure image..."
$COMPILER/armlink --scatter=$SCAT_FILE_NS --entry=Reset_Handler --info=sizes logs/startup_ns.o logs/system_ns.o logs/main_ns.o logs/hello-world-in-security-states-CMSE-Lib.o -o logs/hello-world-in-security-states_ns.axf --list=logs/image_ns.lst --map
echo "Generating secure binary file..."
$COMPILER/fromelf --bincombined logs/hello-world-in-security-states_ns.axf -o logs/hello-world-in-security-states_ns.bin
$COMPILER/fromelf -c logs/hello-world-in-security-states_ns.axf -o logs/hello-world-in-security-states_ns.disass
echo "Images built. Use \"make run search=hello-world-in-security-states\" to run the example on an FVP."

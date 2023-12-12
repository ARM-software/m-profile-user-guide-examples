cd projects/m-profile/memory_model/TCM_implement
CMSIS=../../../../CMSIS_5
INCLUDES="-I $CMSIS/Device/ARM/ARMCM55/Include -I $CMSIS/CMSIS/Core/Include -I ."

while getopts "c:" opt; do
    case "$opt" in
        c) COMPILER="$OPTARG";;
    esac
done

echo "Cleaning old files..."
rm -f -v logs/*
echo "Compiling  main.c..."
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1 -c main.c -o logs/main.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/main.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1 -c mpu_prog.c -o logs/mpu_prog.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/mpu_prog.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1 -c Excep_prog.c -o logs/Excep_prog.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/Excep_prog.d
$COMPILER/arm-none-eabi-gcc -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1 -c check_is_full.c -o logs/check_is_full.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/check_is_full.d
echo "Compiling  startup file..."
$COMPILER/arm-none-eabi-gcc  -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1  -c RTE/Device/ARMCM55/startup_ARMCM55.c -o logs/startup.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/startup.d
echo "Compiling  system file..."
$COMPILER/arm-none-eabi-gcc  -march=armv8-m.main+fp -mfloat-abi=hard -mlittle-endian -O1  -c RTE/Device/ARMCM55/system_ARMCM55.c -o logs/system.o -D ARMCM55 $INCLUDES -g -MMD -MP -MF logs/system.d
echo "Linking  image..."
$COMPILER/arm-none-eabi-gcc --entry=Reset_Handler -T scripts/gcc_arm.ld -Wl,--gc-sections -march=armv8-m.main+fp -mfloat-abi=hard --specs=rdimon.specs logs/startup.o logs/system.o logs/main.o logs/mpu_prog.o logs/Excep_prog.o logs/check_is_full.o -o logs/TCM_implement.axf
echo "Image built. Use \"make run search=TCM_implement fvp_path=gcc\" to run the example on an FVP."
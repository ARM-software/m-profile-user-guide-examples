cd projects/m-profile/memory_model/rtos_context_switch

while getopts "f:t:k:" opt; do
    case "$opt" in
        f) FVP="$OPTARG";;
        t) TARMAC="$OPTARG";;
        k) KILLTIME="$OPTARG";;
    esac
done

$FVP/FVP_MPS2_Cortex-M33 --stat -C fvp_mps2.DISABLE_GATING=1 -C cpu0.semihosting-heap_base=0x0 -C cpu0.semihosting-heap_limit=0x0 -C cpu0.semihosting-stack_base=0x0 -C cpu0.semihosting-stack_limit=0x0 -a logs/rtos_context_switch.axf --plugin $TARMAC -C TRACE.TarmacTrace.trace-file=logs/tarmac.log -T $KILLTIME #--data logs/MPU_and_my_RTOS_armv8m.bin@0x10000000
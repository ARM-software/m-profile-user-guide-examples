cd projects/m-profile/memory_model/TCM_implement

while getopts "f:k:" opt; do
    case "$opt" in
        f) FVP="$OPTARG";;
        k) KILLTIME="$OPTARG";;
    esac
done

$FVP/FVP_MPS2_Cortex-M55 --stat -C fvp_mps2.DISABLE_GATING=1 -C cpu0.INITSVTOR=0x10000000  -C cpu0.semihosting-heap_base=0x0 -C cpu0.semihosting-heap_limit=0x0 -C cpu0.semihosting-stack_base=0x0 -C cpu0.semihosting-stack_limit=0x0 -a logs/TCM_implement.axf -T $KILLTIME
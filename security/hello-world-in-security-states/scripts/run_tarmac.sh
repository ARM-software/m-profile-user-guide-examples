cd projects/m-profile/security/hello-world-in-security-states

while getopts "f:t:k:" opt; do
    case "$opt" in
        f) FVP="$OPTARG";;
        t) TARMAC="$OPTARG";;
        k) KILLTIME="$OPTARG";;
    esac
done

$FVP/FVP_MPS2_Cortex-M55 -C fvp_mps2.DISABLE_GATING=1 -C NSC_CFG_0=1 -C cpu0.INITNSVTOR=0x00200000 -a logs/hello-world-in-security-states_ns.axf -a logs/hello-world-in-security-states_s.axf --stat --plugin $TARMAC -C TRACE.TarmacTrace.trace-file=logs/tarmac.log -T $KILLTIME

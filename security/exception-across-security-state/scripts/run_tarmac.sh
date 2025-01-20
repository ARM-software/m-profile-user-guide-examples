cd projects/m-profile/security/exception-across-security-state
while getopts "f:k:" opt; do
    case "$opt" in
        f) FVP="$OPTARG";;
        k) KILLTIME="$OPTARG";;
    esac
done

$FVP/FVP_MPS2_Cortex-M55 -C fvp_mps2.DISABLE_GATING=1 -C NSC_CFG_0=1 -a logs/exception-across-security-state_ns.axf -a logs/exception-across-security-state_s.axf --stat -T $KILLTIME


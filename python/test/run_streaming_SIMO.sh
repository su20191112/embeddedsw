#!/bin/bash


test="test_streaming_SIMO.py"
export py=python

. config.sh

if [ $# -lt 1 ]; then
    echo "Usage $0 <config.json> [clk_config.json]"
    echo "Example: sudo ./run_streaming_SIMO.sh configs/streaming_large.json ../rfdc/configs/300MHz_RefClk_58G.json"
    exit 1
fi

test_config=$1
clk_config=$2

lmk_config=$(get_config $clk_config lmk "")
lmx2820_config=$(get_config $clk_config lmx2820 "")
rf_pwr_config="../hmc/configs/rf_power.json"
rf_pwr_config_pre="../hmc/configs/rf_power_pre.json"

[[ $lmk_config != "" ]] && lmk_config="--lmk ../rfdc/configs/$lmk_config"
[[ $lmx2820_config != "" ]] && lmx2820_config="--lmx2820 ../lmx/$lmx2820_config"

num_iterations=$(get_config $test_config num_iterations 10)
adc_dac_hw_loppback=$(get_config $test_config adc_dac_hw_loppback False)
adc_dac_sw_loppback=$(get_config $test_config adc_dac_sw_loppback False)

if [ $adc_dac_hw_loppback == "False" ] && [ $adc_dac_sw_loppback == "False" ]; then
    mode="RF"
elif [ $adc_dac_hw_loppback == "True" ] && [ $adc_dac_sw_loppback == "False" ]; then
    mode="hw_loopback"
else
    mode="sw_loopback"
fi

echo "Initialization mode: $mode"

#Init CLK 104
$py ../rfdc/rfdc_clk.py --clk_104 $lmk_config
[[ $? != 0 ]] && exit 1

case $mode in
    "RF")
        # Setup RF scenario:
        # Init LO
        # Init RF RX/TX
        # Shut down RX outputs
        # Init RFDC
        # Enable RF RX/TX

        #Init LO lmx2820
        $py ../lmx/lmx.py $lmx2820_config

        #Init HMC (Pre-init stage, to let rfdc calibrate)
        $py ../hmc/hmc.py $test_config $rf_pwr_config_pre
        [[ $? != 0 ]] && exit 1

        #Init RFDC (Calibration)
        $py ../rfdc/rfdc_clk.py --rfdc
        [[ $? != 0 ]] && exit 1

        #Init HMC (Post-init stage)
        $py ../hmc/hmc.py $test_config $rf_pwr_config
        [[ $? != 0 ]] && exit 1
        ;;
    "hw_loopback")
        # Setup HW loopback scenario:
        # Export BRAM content
        # Zero BRAM, so DAC outputs near zero
        # Init RFDC
        # Load BRAM

        # Data will be stored as ./bram0.npy, ./bram1.npy
        export_path="."
        $py ../dac/player.py --export $export_path
        $py ../dac/player.py --zero

        #Init RFDC (Calibration)
        $py ../rfdc/rfdc_clk.py --rfdc
        [[ $? != 0 ]] && exit 1

        #Load exported BRAM back
        $py ../dac/player.py --bram0 "$export_path/bram0.npy" --bram1 "$export_path/bram1.npy"

        rm "./bram0.npy" "$export_path/bram1.npy"
        ;;
    "sw_loopback")
        ;;
esac

sn=0

while true; do
    $py $test $sn $test_config
    [[ $? != 0 ]] && exit 1

    sn=$(($sn + $num_iterations))
done

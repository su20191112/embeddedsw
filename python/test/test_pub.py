
import sys
from test import TestSuite
import numpy as np
from time import sleep, time
import zmq
import json
import os

from inet import Inet

sys.path.append('../misc')

from widebuf import WideBuf

class Test_1x8_Sweep(TestSuite):
    def __init__(self, port):
        TestSuite.__init__(self)

        context = zmq.Context()
        self.publisher = context.socket(zmq.PUB)
        self.publisher.bind("tcp://0.0.0.0:%s" % port)

    def load_ext_bram(self, Ipath, Qpath, dtype):
        sampleSize = np.dtype(dtype).itemsize
        buffersCount = self.hw.BUFFERS_IN_BRAM
        numBytes = int(self.getBramSize() / buffersCount)
        numSamples = int(numBytes / sampleSize)
        samplesPerFLit = self.hw.SAMPLES_PER_FLIT

        buffer = np.empty(buffersCount * numSamples, dtype=dtype)

        for i in range(0, buffersCount, 2):
            I = np.fromfile(Ipath, dtype)
            Q = np.fromfile(Qpath, dtype)
            WideBuf().make(buffer, I, i, buffersCount, samplesPerFLit)
            WideBuf().make(buffer, Q, i+1, buffersCount, samplesPerFLit)

        return buffer, int(0)

    @TestSuite.Test
    def run_test(self):
        samplingFreq = self.rfdc.getSamplingFrequency()

        print('\n\n\n=== Running test ===')
        print('RFDC Sampling Rate = {}'.format(samplingFreq))
        print('Using Max Gain = {}'.format(self.max_gain))

        dtype = np.uint16

        if load_bram:
            if self.ext_bram_path is not None:
                Ipath = self.ext_bram_path + os.sep + 'Ichannel.npy'
                Qpath = self.ext_bram_path + os.sep + 'Qchannel.npy'
                bram0_data = self.load_ext_bram(Ipath, Qpath, dtype)
                bram1_data = np.copy(bram0_data)
            else:
                print('=== Generating tones ===')
                bram0_data, _ = self.make_sweep_tone_bram(samplingFreq, self.freq, self.dBFS, self.freqStep, dtype)
                bram1_data, _ = self.make_sweep_tone_bram(samplingFreq, self.freq, self.dBFS, self.freqStep, dtype)

            self.load_dac_player(bram0_data, bram1_data)

        self.setup_RF_Clk(self.ticsFilePath, self.restart_rfdc)
        #Turn on all the dac player outputs
        self.dac_gate(0xffff)
        sn = 0x0
        for txi in self.tx:

            print('*** Running Iteration : rx={}, tx={}'.format(self.rx, txi))
            self.setup_RF([txi], self.rx)
            sleep(1)

            #input("Press Enter to continue...")

            self.adc_dac_sync(False)

            #Let the RF to settle the configuration
            ids0 = [i for i in range(8)]
            ids1 = [i for i in range(8, 16, 1)]
            offset = 0x0

            area = []
            for ids, ddr in [(ids0, self.ddr0), (ids1, self.ddr1)]:
                a = self.start_dma(ddr, ids, offset, self.captureSize)
                area.append(a)

            self.adc_dac_sync(True)

            t = self.calc_capture_time(self.captureSize)
            print('Waiting %fs for capture to complete ' % t)
            sleep(t)
            
            self.publish(area, txi)
            sn += 1
            self.shutdown_RF()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('{}: Usage'.format(sys.argv[0]))
        exit()

    ticsFilePath = sys.argv[1]

    freq = 75_000_000
    freqStep = 0
    dBFS = int(-9)
    captureSize = 128 * 4096 * 2
    restart_rfdc = False
    load_bram = True
    ext_bram_path = './'
    capture_data = True
    #Which radios to use:
    tx = [i for i in range(8)]
    rx = [i for i in range(8)]
    max_gain = True

    test = Test_1x8_Sweep(Inet.PORT)

    test.run_test(ticsFilePath  =ticsFilePath,
                freq            =freq,
                dBFS            =dBFS,
                freqStep        =freqStep,
                captureSize     =captureSize,
                restart_rfdc    =restart_rfdc,
                load_bram       =load_bram,
                capture_data    =capture_data,
                tx              =tx,
                rx              =rx,
                max_gain        =max_gain,
                ext_bram_path   =ext_bram_path)
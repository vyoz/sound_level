import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt


def load_wav(wave_input_path):
    wf = wave.open(wave_input_path, 'rb') 
    fs = wf.getframerate()
    nframes = wf.getnframes()
    str_data = wf.readframes(nframes)
    print("frame num:", nframes)
    wf.close()
    wave_data = np.fromstring(str_data, dtype=np.short)
    return wave_data.astype(np.float64), fs

def SPLCal(x):
    Leng = len(x)
    pa = np.sqrt(np.sum(np.power(x, 2))/Leng)
    #p0 = 2e-5
    p0 = 1
    spl = 20 * np.log10(pa / p0)
    return spl

if __name__ == '__main__':
    x, fs = load_wav('test.wav')
    Leng = len(x)
    frameTime = 100
    frameLen = fs * frameTime // 1000
    m = np.mod(Leng, frameLen)
    print("len:", Leng)
    print("frame len:", frameLen)
    print("frame time:", frameTime)
    print("mod:", m)
    if m>=frameLen/2:
        x = np.append(x, np.zeros(int(frameLen-m)))
        Leng = len(x)
    else:
        print("Leng:", Leng)
        nframe = int(np.floor(Leng/frameLen))
        print("nframe:", nframe)
        x = x[0:nframe * frameLen + 1]
        Leng = len(x)

    N = Leng // frameLen
    print("wave len:", N)
    spl = np.array([])
    for k in range(N):
        s = x[k*frameLen: (k+1)*frameLen]
        spl = np.append(spl, SPLCal(s))

    spl_rep = np.repeat(spl, frameLen)
    plt.figure()
    plt.subplot(211)
    plt.plot(x)
    plt.subplot(212)
    plt.plot(spl_rep)
    #plt.show()
    plt.savefig("test.png")

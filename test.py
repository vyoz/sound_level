from scipy.io.wavfile import read
samprate, wavdata = read('audio.wav')
import numpy as np
import math
import statistics 
# basically taking a reading every half a second - the size of the data 
# divided by the sample rate gives us 1 second chunks so I chop 
# sample rate in half for half second chunks
chunks = np.array_split(wavdata, wavdata.size/(samprate/2))
dbs = [20*math.log10( math.sqrt(statistics.mean(chunk**2)) ) for chunk in chunks]
print(dbs)

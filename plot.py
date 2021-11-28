# imports
import matplotlib.pyplot as plt
import numpy as np
import wave, sys

# shows the sound waves
def visualize(path: str):
	
	# reading the audio file
	raw1 = wave.open(path)
	raw = wave.open("sample.wav")
	
	# reads all the frames
	# -1 indicates all or max frames
	signal = raw.readframes(-1)
	signal = np.frombuffer(signal, dtype ="int16")
	signal1 = raw1.readframes(-1)
	signal1 = np.frombuffer(signal1, dtype ="int16")
	
	# gets the frame rate
	f_rate = raw.getframerate()

	# to Plot the x-axis in seconds
	# you need get the frame rate
	# and divide by size of your signal
	# to create a Time Vector
	# spaced linearly with the size
	# of the audio file
	time1 = np.linspace(
		0, # start
		len(signal1) / f_rate,
		num = len(signal1)
	)
	time = np.linspace(
		0, # start
		len(signal) / f_rate,
		num = len(signal)
	)

	# using matlplotlib to plot
	# creates a new figure
	plt.figure(1)
	
	# title of the plot
	plt.title("Sound Wave r -> Original .wav file")
	
	# label of x-axis
	plt.xlabel("Time")
	
	# actual ploting
	plt.plot(time1, signal1, 'b')
	plt.plot(time, signal, 'r')
	
	# shows the plot
	# in new window
	plt.show()

	# you can also save
	# the plot using
	# plt.savefig('filename')


if __name__ == "__main__":
	
	# gets the command line Value
	path = sys.argv[1]

	visualize(path)


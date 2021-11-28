Steps
> Create the sample.wav file.
    $ arecord -r 16000 -f S16_LE -t wav sample.wav
> Compile the amplify-signal.c file.
    $ gcc amplify-signal.c
> Execute the build file
    $ ./a.out
> Plot the files
    $ python3 plot.py sample-modified.wav
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "wav.h"

#define WAV_FILE "tmp.wav"

#define WAV_VOLUME_PERCENT (0.50)

#define MAX_FILE_SIZE (100000)

void wav_parse_header(void *buf)
{
	struct wav *wav = (struct wav *)buf;

	printf("%.4s\n", wav->riff);

	printf("Overall File Size: %d\n", wav->overall_size);

	printf("File Type: %.4s\n", wav->wave);
	
	printf("Format Chunk marker: %.4s\n", wav->fmt_chunk_marker);

	printf("size per data: %d\n", wav->length_of_fmt);

	printf("Data Format: ");

	if (wav->format_type == 1) {
		printf("\"PCM\"\n");
	} else {
		printf("Unknown value: %d\n", wav->format_type);
	}

	printf("Number of Channels: %d\n", wav->channels);

	printf("Sample Rate: %d Samples per Second\n", wav->sample_rate);

	printf("Byte Rate: %d Bps or %d kbps\n", wav->byterate, (wav->byterate * 8) / 1000);

	printf("Audio Type: ");

	if (wav->block_align == 1) {
		printf("8-bit Mono\n");
	} else if (wav->block_align == 2) {
		printf("8-bit Stereo or 16-bit Mono\n");
	} else if (wav->block_align == 4) {
		printf("16-bit stereo\n");
	} else {
		printf("Unknown value: %d\n", wav->block_align);
	}

	printf("Bits Per Sample: %d\n", wav->bits_per_sample);

	printf("Data marker: %.4s\n", wav->data_chunk_header);

	printf("Data Size: %d\n", wav->data_size);

	printf("Signal time: %fs\n", ((double)wav->data_size / wav->byterate));
}

void wav_dump_data(int16_t *buf, int size)
{
	int i, count = 0;

	char ch;

	printf("size = %d\n", size);

	for (i = 0; i < size / 2; i++) {
		if (ch == buf[i]) {
			count++;
			continue;
		} else {
			if (count > 0) {
				//printf("[%d] ", count);
			}

			count = 0;
		}

		printf("%d ", (int)(int16_t)buf[i]);

		if (i % 10 == 0) {
			printf("\n");
		}

		ch = buf[i];
	}

	printf("size = %d\n", size);
}

void wav_modify_volume(struct wav *wav, int16_t *buf, int size)
{
	int fd, i;

	char ch;

	int16_t data, raw_buf[MAX_FILE_SIZE];

	struct wav wav_buf;

	fd = open("Wav-modify-volume.wav", O_RDWR | O_CREAT, 0664);

	if (fd < 0) {
		perror("open error: ");
		exit(-1);
	}

	write(fd, wav, sizeof(struct wav));

	printf("size = %d\n", size);
	
	for (i = 0; i < size / 2; i++) {
		data = buf[i] - buf[i] * WAV_VOLUME_PERCENT;

		write(fd, &data, 2);
	}

	close(fd);

	fd = open("Wav-modify-volume.wav", O_RDONLY);

	if (fd < 0) {
		perror("open error: ");
		exit(-1);
	}

	read(fd, &wav_buf, sizeof(struct wav));

	printf("Parsing Modified File\n");

	wav_parse_header(&wav_buf);

	printf("Want data to be dumped Y/N: ");

	scanf(" %c", &ch);

	if (ch == 'Y' || ch == 'y') {
		read(fd, raw_buf, wav_buf.data_size * (wav_buf.bits_per_sample / 8));
		printf("size = %d\n", size);
		wav_dump_data(raw_buf, size);
	}

	close(fd);
}

int main(void)
{
	int fd, ret, i, count = 0;

	struct wav wav;

	char ch, data;

	int16_t buf[MAX_FILE_SIZE];

	fd = open(WAV_FILE, O_RDONLY);

	if (fd < 0) {
		perror("open error: ");
		return -1;
	}

	ret = read(fd, &wav, sizeof(struct wav));

	wav_parse_header(&wav);

	ret = read(fd, buf, wav.data_size * (wav.bits_per_sample / 8));

	printf("ret = %d\n", ret);

	ch = '\0';

	printf("Want data to be dumped Y/N: ");

	scanf("%c", &ch);

	if (ch == 'Y' || ch == 'y') {
		wav_dump_data(buf, ret);
	}

	printf("Want to Modify the Wav file Y/N: ");

	scanf(" %c", &ch);

	if (ch == 'Y' || ch == 'y') {
		wav_modify_volume(&wav, buf, ret);
	}

	return 0;
}

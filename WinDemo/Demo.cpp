/*!

License Public Domain (CC0) 2016 Julio Tentor

To the extent possible under law, Julio Tentor has waived all copyright and related
or neighboring rights. This work is published from: Jujuy - Argentina.

http://creativecommons.org/publicdomain/zero/1.0/

*/

#include "AudioWav.h"
#include "ofa1/ofa.h"


int main(int argc, char **argv) {

	AudioWav *wav;
	char *fileName;

	for (int i = 1; i < argc; ++i) {
		fileName = argv[i];

		wav = new AudioWav(fileName);

		wav->Load();

		printf("File            : %s\n", fileName);
		printf("  WavFormat     : %d\n", (int16_t)wav->getWavFormat());
		printf("  Channels      : %d\n", (int16_t)wav->getChannels());
		printf("  SampleRate    : %d\n", (int32_t)wav->getSampleRate());
		printf("  WavDataLength : %d\n", (int32_t)wav->getWavDataLength());

		if (wav->getWavFormat() != 1) {
			printf("  Solo para formato PCM\n");
			continue;
		}
		if (wav->getSampleRate() != 44100) {
			printf("  Solo para sample rate 44100 Khz\n");
			continue;
		}

		unsigned char *samples = wav->getWavData();
		const char *fingerPrint = NULL;
		fingerPrint = ofa_create_print(samples,
			OFA_LITTLE_ENDIAN,
			(wav->getWavDataLength() / 2),
			wav->getSampleRate(),
			((wav->getChannels() == 2) ? 1 : 0));

		if (!fingerPrint) {
			printf("  Error al generar fingerPrint\n");
			continue;
		}

		printf("Fingerprint     : %s (%d bytes)\n", fingerPrint, strlen(fingerPrint));
		printf("\n");

		delete wav;
	}
	return 0;
}


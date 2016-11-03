/*!

License Public Domain (CC0) 2016 Julio Tentor

To the extent possible under law, Julio Tentor has waived all copyright and related
or neighboring rights. This work is published from: Jujuy - Argentina.

http://creativecommons.org/publicdomain/zero/1.0/

*/
#pragma once
#ifndef _AUDIOWAV_H_
#define _AUDIOWAV_H_


#ifdef WIN32
#include "io.h"
#endif

#include <fcntl.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Endianness.h"

#include <malloc.h>

#include <stdexcept>

/*!
	Audio File Exception header only implementation
*/
class AudioFileException : public std::exception {
public:
	AudioFileException() {
		sprintf_s(_errorMessage,
			"Exception in file access");
	};
	AudioFileException(const char *fileName, const errno_t error) {
		sprintf_s(_errorMessage,
			"File: %s - Error: %d", fileName, error);
	};
	AudioFileException(const char *fileName, const char *message) {
		sprintf_s(_errorMessage,
			"File: %s - Error: %s", fileName, message);
	};

	~AudioFileException() {};

	virtual const char *what() const { return _errorMessage; };
private:
	char _errorMessage[1024];
};

/*!
	This values are 32 bits little endian 
	Found at http://stackoverflow.com/a/16075935/4718266
*/
enum class WavChunks {					// little Endian	<- chars
	RiffHeader			= 0x46464952,	// FFIR				<- RIFF
	WavRiff				= 0x54651475,	// Te.u				<- u.eT
	Format				= 0x020746d66,	//  tmf				<- fmt 
	LabeledText			= 0x478747C6,	// G.G.				<- .G.G
	Instrumentation		= 0x478747C6,	// G.G.				<- .G.G
	Sample				= 0x6C706D73,	// lpms				<- smpl
	Fact				= 0x47361666,	// G6.f				<- f.6G
	Data				= 0x61746164,	// atad				<- data
	Junk				= 0x4b4e554a	// KNUJ				<- JUNK
};

enum class WavFormat {
	PulseCodeModulation	= 0x01,
	IEEEFloatingPoint	= 0x03,
	ALaw				= 0x06,
	MuLaw				= 0x07,
	IMAADPCM			= 0x11,
	YamahaITUG723ADPCM	= 0x16,
	GSM610				= 0x31,
	ITUG721ADPCM		= 0x40,
	MPEG				= 0x50,
	Extensible			= 0xFFFE
};

struct WavHeader {
	unsigned char		riffChunk[4];
	Endian32			overallLength;
	unsigned char		waveChunk[4];
	unsigned char		fmtChunk[4];
	Endian32			fmtChunkLength;
	Endian16			format;
	Endian16			chanels;
	Endian32			sampleRate;
	Endian32			bitsPerSecond;
	Endian16			bytesPerSample;
	Endian16			bitsPerSimpleSample;
};

struct WavDataBuffer {
	//struct WavHeader	header;
	unsigned char		dataChunk[4];
	Endian32			dataChunkLength;
};

/*!
	AudioWav class header only implementation
*/
class AudioWav {	
public:
	AudioWav(const char *fileName) {
		strncpy_s(_fileName, sizeof(_fileName) - 1, fileName, strlen(fileName));
		errno_t err = _sopen_s(&_fileHandle, _fileName, _O_BINARY | _O_RDONLY, _SH_DENYRW, 0);
		if (err != 0) {
			throw AudioFileException(_fileName, err);
		}
	}

	~AudioWav() {
		if (_fileHandle > 0) {
			_close(_fileHandle);
		}
		if (_wavData != NULL) {
			free(_wavData);
		}
		//if (_wavData != NULL) {
		//	delete []_wavData;
		//}
	}

	// File member functions ----------------------------------------------------
	void Close() {
		if (_fileHandle <= 0) {
			throw AudioFileException(_fileName, "Fail seek, file not open");
		}
		if (_close(_fileHandle) < 0) {
			throw AudioFileException(_fileName, "Fail close");
		}
		_fileHandle = 0;
	}

	int ReadChar(unsigned char *buffer, const int count) {
		if (_fileHandle <= 0) {
			throw AudioFileException(_fileName, "Fail seek, file not open");
		}
		int bytesRead;
		if ((bytesRead = _read(_fileHandle, buffer, count)) <= 0) {
			throw AudioFileException(_fileName, "Fail read, can not read chars");
		}
		return bytesRead;
	}

	int16_t ReadInt16(bool littleEndian = true) {
		if (_fileHandle <= 0) {
			throw AudioFileException(_fileName, "Fail seek, file not open");
		}
		unsigned char buffer[4];
		if (_read(_fileHandle, buffer, 2) != 2) {
			throw AudioFileException(_fileName, "Fail read, can not read int16");
		}
		if (littleEndian) {
			return littleEndian16(buffer);
		}
		else {
			return bigEndian16(buffer);
		}
	}

	int32_t ReadInt32(bool littleEndian = true) {
		if (_fileHandle <= 0) {
			throw AudioFileException(_fileName, "Fail seek, file not open");
		}
		unsigned char buffer[8];
		if (_read(_fileHandle, buffer, 4) != 4) {
			throw AudioFileException(_fileName, "Fail read, can not read int32");
		}
		if (littleEndian) {
			return littleEndian32(buffer);
		}
		else {
			return bigEndian32(buffer);
		}
	}

	long Seek(const long offset, int origin = SEEK_CUR) {
		if (_fileHandle <= 0) {
			throw AudioFileException(_fileName, "Fail seek, file not open");
		}
		long ret;
		if ((ret = _lseek(_fileHandle, offset, origin)) == -1L) {
			throw AudioFileException(_fileName, "Fail seek");
		}
		return ret;
	}



	void Load() {
		bool findData = false;
		WavChunks chunkId;
		int32_t memSize, riffStyle, skipSize;

		while (!findData) {
			chunkId = (WavChunks)this->ReadInt32(true);
			switch (chunkId) {
			case WavChunks::Format:
				_formatSize = this->ReadInt32(true);
				_format = (WavFormat)this->ReadInt16(true);
				_channels = this->ReadInt16(true);
				_sampleRate = this->ReadInt32(true);
				_bitsPerSecond = this->ReadInt32(true);
				_bytesPerSample = this->ReadInt16(true);
				_bitsPerSimpleSample = this->ReadInt16(true);
				if (_formatSize == 18) {
					int32_t extraData = this->ReadInt16(true);
					this->Seek(extraData, SEEK_CUR);
				}
				break;

			case WavChunks::RiffHeader:
				memSize = this->ReadInt32(true);
				riffStyle = this->ReadInt32(true);
				break;

			case WavChunks::Data:
				/*!
				Found data chunk, get chunk length
				*/
				_dataChunkLength = this->ReadInt32(true);
				findData = true;
				break;

			default:
				skipSize = this->ReadInt32(true);
				this->Seek(skipSize, SEEK_CUR);
				break;
			}
		}

		//_wavData = new unsigned char[_dataChunkLength];
		_wavData = (unsigned char *)malloc(_dataChunkLength * sizeof(unsigned char));
		if (_wavData == NULL) {
			throw std::runtime_error("Fail memory allocation");
		}
		this->ReadChar(_wavData, _dataChunkLength);
	}

public:
	const int16_t getWavFormat()			{ return (int16_t)_format; }
	const int32_t getChannels()				{ return _channels;  }
	const int32_t getSampleRate()			{ return _sampleRate; }
	const int32_t getWavDataLength()		{ return _dataChunkLength; }
	unsigned char * getWavData()			{ return _wavData; }

private:
	char _fileName[1024];
	int _fileHandle;

private:
	int32_t			_formatSize;
	WavFormat		_format;
	int16_t			_channels;
	int32_t			_sampleRate;
	int32_t			_bitsPerSecond;
	int16_t			_bytesPerSample;
	int16_t			_bitsPerSimpleSample;

	//unsigned char	_dataChunk[4];
	int32_t			_dataChunkLength;

	unsigned char	*_wavData;
};




#endif	// _AUDIOWAV_H_


#include "TargaLoader.h"

unsigned char* TargaLoader::LoadTarga(char* filename, int& height, int& width)
{
	FILE* filePointer = OpenFile(filename);

	TargaHeader targaFileHeader = GetFileHeaderInformation(filePointer, filename);

	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);

	if (CheckTarga32BitsPerPixel(targaFileHeader) == false)
		throw Exception("'" + string(filename) + "' is not a 32bit Targa image.");

	int imageSize = width * height * 4;

	unsigned char* targaImage = ReadTargaImageData(filePointer, imageSize, filename);

	CloseFile(filePointer, filename);

	return ReverseTargaData(targaImage, imageSize, width, height);
}

FILE* TargaLoader::OpenFile(char* filename)
{
	FILE* filePointer;

	int error = fopen_s(&filePointer, filename, "rb");
	if (error != 0)
		throw Exception("Errors occured opening file: '" + string(filename) + "'");

	return filePointer;
}

void TargaLoader::CloseFile(FILE* filePointer, char* filename)
{
	int error = fclose(filePointer);
	if (error != 0)  throw Exception("Failed to close the file: '" + string(filename) + "'");
}

TargaHeader TargaLoader::GetFileHeaderInformation(FILE* filePointer, char* filename)
{
	TargaHeader targaFileHeader;

	int count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePointer));
	if (count != 1) 
		throw Exception("Failed to read Targa header information for: '" + string(filename) + "'");

	return targaFileHeader;
}

bool TargaLoader::CheckTarga32BitsPerPixel(TargaHeader headerInfo)
{
	return static_cast<int>(headerInfo.bpp) == 32;
}

unsigned char* TargaLoader::ReadTargaImageData(FILE* filePointer, int imageSize, char* filename)
{
	unsigned char* targaImage = new unsigned char[imageSize];
	if (!targaImage) 
		throw Exception("Failed to allocate memory for Targa image: '" + string(filename) + "'");

	int count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, filePointer));
	if (count != imageSize) 
		throw Exception("Failed to read the data from: '" + string(filename) + "'");

	return targaImage;
}

unsigned char* TargaLoader::ReverseTargaData(unsigned char* rawTargaData, int imageSize, int width, int height)
{
	unsigned char* targaData = new unsigned char[imageSize];
	if (!targaData) 
		throw Exception("Failed to allocate memory for Targa data");

	int index = 0;
	int i = 0;
	int j = 0;
	int k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			targaData[index] = rawTargaData[k + 2];
			targaData[index + 1] = rawTargaData[k + 1];
			targaData[index + 2] = rawTargaData[k];
			targaData[index + 3] = rawTargaData[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] rawTargaData;
	rawTargaData = nullptr;

	return targaData;
}
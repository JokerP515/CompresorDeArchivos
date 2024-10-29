// CompresorDeArchivos.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>
using namespace std;

string readFile(const string& filename)
{
	ifstream file(filename, ios::binary);
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	return content;
}

void writeCompressedFile(const string& filename, const string& content)
{
	uLong sourceLength = (uLong)content.size();
	uLong destLength = compressBound(sourceLength); //Estima el tamaño del buffer de salida

	vector<char> compressedData(destLength);

	int result = compress(reinterpret_cast<Bytef*>(compressedData.data()), &destLength, 
		reinterpret_cast<const Bytef*>(content.data()), sourceLength);

	if(result==Z_OK)
	{
		ofstream outputFile(filename, ios::binary);
		outputFile.write(compressedData.data(), destLength);
		cout<<"Archivo comprimido como: "<<filename<<endl;
		//compressedData.resize(destLength);
	}
	else
	{
		throw runtime_error("Error comprimiendo el archivo");
	}
}

string readCompressedFile(const string& filename)
{
	ifstream file(filename, ios::binary);
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	return content;
}

void decompressFile(const string& compressedFilename, const string& outputFilename)
{
	string compressedData = readCompressedFile(compressedFilename);
	
	uLongf uncompressedLength = (uLongf)compressedData.size()*10; //Estima el tamaño del buffer de salida
	vector<char> uncompressedData(uncompressedLength);

	int result = uncompress(reinterpret_cast<Bytef*>(uncompressedData.data()), &uncompressedLength, 
		reinterpret_cast<const Bytef*>(compressedData.data()), (uLong)compressedData.size());
	if(result==Z_OK)
	{
		ofstream outputFile(outputFilename, ios::binary);
		outputFile.write(uncompressedData.data(), uncompressedLength);
		cout<<"Archivo descomprimido como: "<<outputFilename << endl;
	}
	else
	{
		throw runtime_error("Error descomprimiendo el archivo");
	}
}

int main()
{
	string inputFilename = "Arlecchino.mp4";
	string outputFilename = "output3.gzip";
	string decompressedFilename = "decompressed3.mp4";

	string fileData = readFile(inputFilename);
	writeCompressedFile(outputFilename, fileData);
    decompressFile(outputFilename, decompressedFilename);

	return 0;
}

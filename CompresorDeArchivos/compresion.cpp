// THis version is NOT tested yet, it will write some metadata stuff to get the original file, just like a normal ZIP file
#include<iostream>
#include<fstream>
#include<vector>
#include<zlib.h>
using namespace std;

string readFile(const string& filename){
    ifstream file(filename, ios::binary);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    
    return content;
}

void writeCompressedFile(const string& filename, const string& content, int compressionLevel = Z_BEST_COMPRESSION){
    uLong sourceLength = (uLong)content.size();
    uLong destLength = compressBound(sourceLength);

    vector<char> compressedData(destLength);
    int result = compress2(reinterpret_cast<Bytef*>(compressedData.data()), &destLength,
                           reinterpret_cast<const Bytef*>(content.data()), sourceLength, compressionLevel);

    if (result == Z_OK){
        ofstream outputFile(filename, ios::binary);

        // Agregar metadatos del nombre del archivo original
        uint32_t nameLength = inputFilename.size();
        outputFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));  // Longitud del nombre
        outputFile.write(inputFilename.c_str(), nameLength);                          // Nombre del archivo

        // Escribir datos comprimidos
        outputFile.write(compressedData.data(), destLength);

        cout << "Archivo comprimido como: " << filename << endl;
    }
    else
    {
        throw runtime_error("Error comprimiendo el archivo");
    }
}

void decompressFile(const string& compressedFilename){
    ifstream inputFile(compressedFilename, ios::binary);

    uint32_t nameLength; // Se lee la longitud del nombre del archivo original
    inputFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    string originalFilename(nameLength, '\0');
    inputFile.read(&originalFilename[0], nameLength);

    // Leer el contenido comprimido
    string compressedData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    uLongf uncompressedLength = compressedData.size() * 10;
    vector<char> uncompressedData(uncompressedLength);

    int result = uncompress(reinterpret_cast<Bytef*>(uncompressedData.data()), &uncompressedLength,
                            reinterpret_cast<const Bytef*>(compressedData.data()), (uLong)compressedData.size());

    if (result == Z_OK)
    {
        ofstream outputFile(originalFilename, ios::binary);
        outputFile.write(uncompressedData.data(), uncompressedLength);
        cout << "Archivo descomprimido como: " << originalFilename << endl;
    }
    else
    {
        throw runtime_error("Error descomprimiendo el archivo");
    }
}

int main(){
    
    string inputFilename = "input2.txt";
    string outputFilename = "outputTest.gzip";
    
    string fileData = readFile(inputFilename);
    
    writeCompressedFile(outputFilename, fileData);
    
    decompressFile(outputFilename);
    
    return 0;
}

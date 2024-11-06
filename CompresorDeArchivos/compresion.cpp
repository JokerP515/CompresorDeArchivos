#include<iostream>
#include<fstream>
#include<vector>
#include<zlib.h>
using namespace std;

string readFile(const string& filename){ // Lee el archivo sin comprimir y lo guarda en un string
    ifstream file(filename, ios::binary);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    
    return content;
}

// Se lee el archivo comprimido y se guarda en un string
string readCompressedFile(const string& filename, string& originalFilename){
    ifstream inputFile(filename, ios::binary);

    // Leer la longitud del nombre del archivo original
    uint32_t nameLength = 0;
    inputFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    // Leer el nombre del archivo original
    originalFilename.resize(nameLength);
    inputFile.read(&originalFilename[0], nameLength);

    // Leer el contenido comprimido
    string compressedData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    return compressedData;
}


void writeCompressedFile(const string& inputFilename,const string& filename, const string& content, int compressionLevel = Z_BEST_COMPRESSION){
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
    string originalFilename;
    string compressedData = readCompressedFile(compressedFilename, originalFilename);

    uLongf uncompressedLength = compressedData.size() * 10; // Estima el tamaño del buffer de salida
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
    
    writeCompressedFile(inputFilename,outputFilename, fileData);

    system("pause");
    
    decompressFile(outputFilename);
    
    return 0;
}

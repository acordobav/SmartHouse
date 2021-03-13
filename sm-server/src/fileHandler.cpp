#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileHandler
{
public:
    FileHandler() {}

    /**
     * Metodo que escribe en un archivo, si el archivo existe
     * este es sobreescritod
     * filename: ruta y nombre del archivo a escribir
     * content: contenido que debe ser almacenado
     * return: 0 -> escritura exitosa
     *         1 -> error
    **/
    int static writeFile(string filename, string content)
    {
        ofstream file;
        file.open(filename);

        if (file.is_open())
        {
            file << content;
            file.close();
            return 0;
        }

        return 1;
    }

    /**
     * Metodo que lee el contenido de un archivo
     * filename: ruta y nombre del archivo a leer
     * return: string con el contenido del archivo
    **/
    string static readFile(string filename)
    {
        ifstream file(filename);
        string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return content;
    }
};
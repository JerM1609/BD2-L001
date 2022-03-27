/**
 * ! Implement all operations for binary file
 * ! Missing binary file
 * ! Use FreeList strategy in bool delete(int pos)
*/

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
using namespace std;

struct Alumno
{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];

    int ciclo;
    float mensualidad;

    int nextDel = 0;
};

ostream& operator<<(ostream& stream, Alumno& record)
{
    stream.write(record.codigo, 5);
    stream.write(record.nombre, 11);
    stream.write(record.apellidos, 20);
    stream.write(record.carrera, 15);
    stream << record.ciclo << record.mensualidad;
    stream << '\n';
    stream << flush;
    return stream;
}


class FixedRecord
{
    string fName;
public:
    FixedRecord(string str) : fName{str}{};
    
    vector<Alumno> load() 
    {
        /**
        * 
        */
        vector<Alumno> vec;
        return vec;
    }

    void add(Alumno record)
    {
        ofstream file(this->fName, ios::app);
        if (file.is_open())
        {
            file << '\n' << record;
            file.close();
        }
        else throw std::runtime_error("file open error\n");
    }

    Alumno readRecord(int pos)
    {
        /**
         * ? pos = i * bytes || pos is an index
        */
        int rel = sizeof(Alumno);
    }

    bool deleteRecord(int pos)
    {
        /**
         * 
        */
    }
};

int main()
{
    return 0;
}

/**
 * * difference between binary files and text files c++
 *  ? https://www.codingeek.com/tutorials/c-programming/text-files-vs-binary-files-in-c-programming-language/#:~:text=The%20major%20difference%20between%20these,version%20of%20a%20text%20file.
 * 
 **/

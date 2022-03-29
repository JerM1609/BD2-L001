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
#include <cstring>
using namespace std;

void parseString(char arr[], int sz, const string& str)
{
    int mn = min((int)str.size(), sz);
    for (int i = 0; i < mn; i++)
        *(arr+i) = str[i];
    for (int i = mn; i < sz; i++)
        *(arr+i) = ' ';
}

struct Alumno
{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
                        // 51
    int ciclo;          // 4
    float mensualidad;  // 4

    int nextDel = 0;    // 4

    Alumno() = default;
    Alumno(string code, string name, string last, string career, int ciclo, float m)
    {
        parseString(codigo, 5, code);
        parseString(nombre, 11, name);
        parseString(apellidos, 20, last);
        parseString(carrera, 15, career);
        
        this->ciclo = ciclo;
        this->mensualidad = m;        
    }
};

ostream& operator<<(ostream& stream, Alumno& record)
{
    stream.write(record.codigo, 5);
    stream.write(record.nombre, 11);
    stream.write(record.apellidos, 20);
    stream.write(record.carrera, 15);
    stream << record.ciclo << ' ' << record.mensualidad << ' ' << record.nextDel;
    stream << '\n';
    stream << flush;
    return stream;
}

class FixedRecord
{
    string fName;
public:
    FixedRecord(string str) : fName{str}
    {
        /**
         * Verificando si el archivo es vacio para inicializarlo con centinela
         **/ 
        if (this->sizeBin() == 0)
        {   // add centinel node to FreeList
            ofstream outFile(this->fName, ios::binary);
            
            Alumno al; al.nextDel = -1;
            outFile.write((char*) &al, sizeof(Alumno));

            outFile.close();
        }
    };
    
    int sizeBin()
    {
        /**
         * Tamaño del file sin registro centinela
         **/

        streampos begin, end;
        ifstream file(fName, ios::binary);
        
        size_t n = sizeof(Alumno);

        file.seekg(n, ios::beg);    begin = file.tellg();
        file.seekg(0, ios::end);    end = file.tellg();
        file.close();

        return end-begin;
    }

    vector<Alumno> load() 
    {
        /**
         * Cargar solo los que tengan el flag nextDel == 0 
         **/
        ifstream file(this->fName, ios::binary);
        vector<Alumno> vec;

        if (!file)
            throw std::runtime_error("cannot open file!\n");

        size_t n = sizeof(Alumno);

        // Get start[ignore first(centinel) line] and end cursor
        file.seekg(n, ios::beg);    
        streampos start = file.tellg(); file.seekg (0, ios::end);
        streampos end = file.tellg();   file.seekg(n, ios::beg);

        while (start < end)
        {
            Alumno al;
            file.read((char*)&al, n);

            if (al.nextDel == 0)
                vec.push_back(al);  // is non-delete record
            start += n;
        }

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");

        return vec;
    }

    void add(Alumno record)
    {
        /**
         * Añadir al final si no hay eliminados
         * Chancar el ultimo eliminado y cambiar el nextDel del anterior por -1
         **/
        fstream file;
        file.open(this->fName, ios::in | ios::out | ios::app | ios::binary);

        if (!file)
            throw std::runtime_error("cannot open file!\n");

        size_t n = sizeof(Alumno);

        Alumno header; 
        file.read((char*) &header, n);
        if (header.nextDel != -1)
        {   // ? FreeList Strategy
            throw std::runtime_error("FreeList\n");
        }
        else file.write((char*) &record, n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");
    }

    Alumno readRecord(int pos)
    {
        /**
         * Iterar sobre todos los elementos verificando que no sean eliminados
         **/
        ifstream file(this->fName, ios::binary);
        Alumno record;

        if (!file)
            throw std::runtime_error("cannot open file!\n");
        
        size_t n = sizeof(Alumno);

        streampos goal = (pos+1)*n;     // goal cursor position
        file.seekg(n, ios::beg);        // initial position
        streampos start = file.tellg(); 

        cout << "s: " << start << " goal: " << goal << endl;

        while (start != goal)
        {
            Alumno aux;
            file.read((char*) &aux, n);
            if (record.nextDel == 0)            
                start += n;     // file is valid
        }
        file.read((char*) &record, n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at reading time\n");
        return record;
    }

    bool deleteRecord(int pos)
    {
        /**
         * Busqueda 
         * Cambio de flag al eliminado previo y al eliminado siguiente
         **/
        return true;
    }
};

void test()
{
    FixedRecord fr("datos2.bin");

}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif 
    FixedRecord fr("datos2.bin");
    
    // cout << sizeof(Alumno) << endl;
    Alumno al("202010311", "Jeremy Jeffrey", "Matos Cangalaya", "Computer Science", 1, 2200.5);
    // cout << al << endl;

    fr.add(al);
    cout << fr.sizeBin() << endl;

    auto alumn = fr.readRecord(0);
    cout << alumn << endl;

    // auto vec = fr.load();

    return 0;
}

/**
 * * difference between binary files and text files c++
 *  ? https://www.codingeek.com/tutorials/c-programming/text-files-vs-binary-files-in-c-programming-language/#:~:text=The%20major%20difference%20between%20these,version%20of%20a%20text%20file.
 * 
 **/

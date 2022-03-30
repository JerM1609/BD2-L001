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
#include <algorithm>
#include <iterator>
#include <random>
using namespace std;

int RNG(int first , int last){
    random_device rd ;
    default_random_engine dre(rd()) ;
    uniform_int_distribution<int> limits(first , last) ;
    return limits(dre) ;
}

void parseString(char arr[], int sz, const string& str)
{
    int mn = min((int)str.size(), sz);
    for (int i = 0; i < mn; i++)
        *(arr+i) = str[i];
    for (int i = mn; i < sz; i++)
        *(arr+i) = ' ';
    *(arr+sz) = '\0';
}

struct Alumno
{
    char codigo [6];
    char nombre [12];
    char apellidos [21];
    char carrera [16];
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

ostream& operator<<(ostream& stream, Alumno record)
{
    // stream.write(record.codigo, 5);
    stream.write(record.nombre, 11);
    // stream.write(record.apellidos, 20);
    // stream.write(record.carrera, 15);
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
    {   // initialize file with centinel line for FreeList
        /**
         * Verificando si el archivo es vacio para inicializarlo con centinela
         **/ 
        if (this->sizeBin() == 0)
        {   // add centinel node to FreeList
            ofstream outFile(this->fName, ios::binary);
            
            Alumno centinel; centinel.nextDel = -1;
            outFile.write((char*) &centinel, sizeof(Alumno));

            outFile.close();
        }
    };
    
    int sizeBin()
    {   // get size of file in bytes  
        /**
         * Tamaño del file contando registro centinela
         **/

        streampos begin, end;
        ifstream file(fName, ios::binary);
        
        if (!file)
            throw std::runtime_error("cannot open file!\n");

        begin = file.tellg();
        file.seekg(0, ios::end);    
        end = file.tellg();

        file.close();
        return end-begin;
    }

    int numRecords()
    {   // get number of records
        /**
         * * IDEA: Count deleted elements and apply complement law
         **/
        int len = 0;
        ifstream file(this->fName, ios::binary);

        if (!file)
            throw std::runtime_error("cannot open file!\n");

        size_t n = sizeof(Alumno);
        streampos start, end;

        file.seekg(n, ios::beg);    start = file.tellg();
        file.seekg(0, ios::end);    end = file.tellg();
        file.seekg(n, ios::beg);

        Alumno curr;
        while (start < end)
        {
            file.read((char*) &curr, n);
            if (curr.nextDel == 0)
                len++;
            start += n;
        }

        if (!file.good())
            throw std::runtime_error("error occurred at reading time\n");
        return len;
    }

    /*Alumno readLine(int pos)
    {   // read n-th token from binary FreeList file
        ifstream file(this->fName, ios::binary);
        Alumno record;

        if (!file)
            throw std::runtime_error("cannot open file!\n");

        size_t n = sizeof(Alumno);

        file.seekg((pos+1)*n, ios::beg);
        file.read((char*)&record, n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at reading time\n");
        return record;   
    }

    void rewriteLine(Alumno record, int pos)
    {   // update n-th token from binary FreeList file
        ofstream file(this->fName, ios::binary);
        
        if (!file)
            throw std::runtime_error("cannot open file!\n");
        size_t n = sizeof(Alumno);
        
        file.seekp((pos+1)*n, ios::beg);
        file.write((char*) &record, n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");
    }*/

    vector<Alumno> load() 
    {   // upload records to std::vector
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
            // int prev, curr = header.nextDel;

            // // get index of last element deleted
            // Alumno traverse = this->readRecord(curr);
            // while(traverse.nextDel != -1)
            // {   
            //     prev = curr;
            //     curr = traverse.nextDel;
            //     traverse = this->readRecord(curr);
            // }
            // // hit last deleted
            // this->rewriteLine(record, curr);
            // // 
            // auto prevRecord = this->readLine(prev);
            // prevRecord.nextDel = -1;
            // this->rewriteLine(prevRecord, prev);
        }
        else file.write((char*) &record, n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");
    }

    Alumno readRecord(int pos)
    {   // read n-th record of binary file | first index = 1
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

void loadPrint(FixedRecord& fixRec)
{
    auto al = fr.load();    
    copy(al.begin(), al.end(), ostream_iterator<Alumno>(cout));
    cout << '\n';
}

void test()
{
    FixedRecord fr("datos2.dat");
    Alumno a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
    vector<Alumno> alumnos;
    int num;

    a1 = Alumno("202010311", "Jeremy Jeffrey", "Matos", "Computer Science", 4, 2200.5);
    a2 = Alumno("201715","Rafael","Galloso","Ing Mecanica", 5, 1900);
    a3 = Alumno("3274128","Angel Romario","Vega","Admin y ND", 6, 2400);
    a4 = Alumno("12341","Fabrizio","Natal Fuentes Campos","Ing Ambiental", 8, 3250.78);
    a5 = Alumno("124","Camila Alejandra","Turin Alcedo","Bioingenieria", 2, 1235.8);
    a6 = Alumno("2021231","Marcos","Ayala Pineda","CS", 7, 2341.89);
    a7 = Alumno("890324","Ariana","Ponce Bohorquez","Bioingenieria", 9, 2312.7);
    a8 = Alumno("432512","Cecilia Natali","Pilco","Ciencia de la Computacion", 5, 2134.890);
    a9 = Alumno("63543","Andre Ruben","Quiroga","Derecho", 4, 1231.9);
    a10 = Alumno("123412","Nicolas","Uman Alvarado","Literatura", 2, 2131.78);
    
    cout << "\tsizeof(Alumno): " << sizeof(Alumno) << endl;
    cout << "\tinitial size of file: " << fr.sizeBin() << endl;

    // ADD & LOAD
    fr.add(a1); fr.add(a2); fr.add(a3); fr.add(a4); fr.add(a5);
    alumnos = fr.load();
    for (const auto& a : alumnos)
        cout << a.codigo << " " << a.nombre << endl;

    num = fr.numRecords();
    cout << "\t numRecords: "<< num << endl;
    cout << "\tmetadata: (" << alumnos.size() << " + 1) * " << sizeof(Alumno) << " = " << fr.sizeBin() << endl;
        
    // READ
    int iterations = RNG(1, num);
    while (iterations--)
    {
        auto aux = fr.readRecord(RNG(1, num));
        cout << aux;
    }

    // DELETE
    fr.deleteRecord(3); fr.deleteRecord(5); 
    loadPrint(fr);
    
    fr.deleteRecord(1); fr.deleteRecord(2);
    loadPrint(fr);

    // ADD AFTER DELETE
    fr.add(a6); fr.add(a7); 
    loadPrint(fr);

    fr.add(a8); 
    loadPrint(fr);

    fr.add(a9); fr.add(a10);
    loadPrint(fr);    

    num = fr.numRecords();
    cout << "\t numRecords: "<< num << endl;
    cout << "\tmetadata: (" << alumnos.size() << " + 1) * " << sizeof(Alumno) << " = " << fr.sizeBin() << endl;
}   

int main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    test();
    return 0;
}

/**
 * https://stackoverflow.com/questions/9244563/writing-integer-to-binary-file-using-c
 **/

/**
 * ! archivo binario usando el tamaño del dato como separador
 * ? implementar metodos sobre una clase
 * ? Matricula readRecord(int pos)
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

struct EntryMeta
{
    int position;   // posición inicial de registro
    int size;       // tamaño en bytes de registro
    EntryMeta() = default;
    EntryMeta(int p, int sz): position{p}, size{sz}{}

    void print()
    {
        cout << this->position << " " << this->size << endl;
    }
};

struct Alumno
{
    string codigo;          // variable
    int ciclo;          // 4
    float mensualidad;  // 4
    string observaciones;   // variable

    Alumno() = default;
    Alumno(string c, int ci, float men, string obs): codigo{c}, ciclo{ci}, mensualidad{men}, observaciones{obs}{}
    Alumno(char* buffer)
    {
        // desempaquetar
    }

    size_t sizeBytes()
    {
        size_t sz = 0;
        sz += sizeof(ciclo);
        sz += codigo.size();
        sz += sizeof(mensualidad);
        sz += observaciones.size();
        return sz;
    }

    size_t sizeRecord()
    {
        int n = this->sizeBytes() + 4;
        return n;
    }


    char* pack()
    {
        auto szRecord = this->sizeRecord();
        char* buffer = new char[szRecord];
        memset(buffer, 0, szRecord);
        buffer[0] = ':';
        strcat(buffer, codigo.c_str());
        buffer[codigo.size()+1] = ':';
        // cout << (char*) &ciclo << endl;
        cout << reinterpret_cast<const char*>(&mensualidad) << endl;
        // strcat(buffer, (char*) &ciclo);
        // strcat(buffer, (char*) &mensualidad);
        cout << sizeof((char*) &ciclo) << endl;
        cout << sizeof((char*) &mensualidad) << endl;

        buffer[codigo.size()+1] = ':';
        strcat(buffer, observaciones.c_str());
        return buffer;
    }

};

ostream& operator<<(ostream& os, Alumno& al)
{
    os << al.codigo << " " << al.ciclo << " " << al.mensualidad << " " << al.observaciones << endl;
    return os;
}

class VariableRecord
{
    string dName, mName ;
public:
    VariableRecord(string file): dName{file + "_d.dat"}, mName{file + "_m.dat"}
    {
        cout << this->dName << " " << this->mName << endl;
    }

    vector<Alumno> load()
    {
        vector<Alumno> vec;
        return vec;
    }

    void add(Alumno record)
    {
        ofstream file(dName, ios::binary | ios::app);
        if (!file)
            throw std::runtime_error("cannot open file!\n");
        int n = record.sizeRecord();
        // char* buffer = record.pack();
        // file.write(buffer, n);

        
        file.write(record.codigo.c_str(), record.codigo.size());
        file.write((char*) &record.ciclo, sizeof(int));
        file.write((char*) &record.mensualidad, sizeof(float));
        file.write(record.observaciones.c_str(), record.observaciones.size());

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");
    }

    Alumno readRecord(int pos)
    {

        // EntryMeta eM;
        // ifstream metaData(this->mName, ios::binary);
        // if (!metaData)
        //     throw std::runtime_error("cannot open file!\n");

        // metaData.seekg(pos*sizeof(EntryMeta), ios::beg);
        // metaData.read((char*)&eM, sizeof(EntryMeta));
        // eM.print();

        // metaData.close();
        cout << '\t' << __PRETTY_FUNCTION__  << endl;
        ifstream binData(this->dName, ios::binary);
        if (!binData)
            throw std::runtime_error("cannot open file!\n");

        Alumno al;


        char* field = nullptr;

        // * CODIGO     cout << al.codigo << endl;
        field = new char[9];   memset(field, 0, 10);
        binData.read(field, 9);     al.codigo = string(field);

        // * CICLO      cout << al.ciclo << endl;
        binData.read((char*) &al.ciclo, sizeof(int));

        // * MENSUALIDAD    cout << al.mensualidad << endl;
        binData.read((char*) &al.mensualidad, sizeof(float));

        // * OBSERVACIONES  cout << al.observaciones << endl;
        field = new char[15];   memset(field, 0, 15);
        binData.read(field, 15);
        al.observaciones = string(field);


        // binData.read((char*) &al.ciclo, 4);
        // char arr[32];
        // binData.read(arr, 32);
        // string str(arr);
        // stringstream strm(str);
        // cout << arr << endl << str << endl;

        // Alumno al;
        // strm >> al.codigo >> al.ciclo;
        // cout << al.codigo << " " << al.ciclo << endl;

        // binData.seekg(eM.position, ios::beg);
        // char* buff = new char[eM.size];
        // binData.read(buff, eM.size);
        // Alumno al(buff);
        // binData.close();

        binData.close();
        if (!binData.good())
            throw std::runtime_error("error occurred at reading time\n");
        return al;
    }

};

void test()
{
    // 9 + 4 + 4 + 15
    Alumno al("202010311", 5, 2400.5, "decimo superior");
    cout << al.sizeBytes() << endl;
    VariableRecord vR("datos4");
    vR.add(al);

    auto record = vR.readRecord(0);
    cout << record << endl;
}

int main()
{
    test();
}

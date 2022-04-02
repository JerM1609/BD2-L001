#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <random>
using namespace std;

int RNG(int first , int last){
    random_device rd ;
    default_random_engine dre(rd()) ;
    uniform_int_distribution<int> limits(first , last) ;
    return limits(dre) ;
}

struct EntryMeta
{
    int position;   // posición inicial de registro
    int size;       // tamaño en bytes de registro
    EntryMeta() = default;
    EntryMeta(int p, int sz): position{p}, size{sz}{}
};

ostream& operator<<(ostream& os, EntryMeta& em)
{
    os << &em << " -> position: " << em.position << " size: " << em.size;
    return os;
}

struct Alumno
{
    string codigo;          // variable
    int ciclo;          // 4
    float mensualidad;  //
    string observaciones;   // variable

    Alumno() = default;
    Alumno(string c, int ci, float men, string obs): codigo{c}, ciclo{ci}, mensualidad{men}, observaciones{obs}{}

    size_t sizeBytes()
    {
        size_t sz = 0;
        sz += sizeof(ciclo);
        sz += codigo.size();
        sz += sizeof(mensualidad);
        sz += observaciones.size();
        return sz;
    }
};

ostream& operator<<(ostream& os, Alumno al)
{
    os << al.codigo << " " << al.ciclo << " " << al.mensualidad << " " << al.observaciones;
    return os;
}

class VariableRecord
{
    string dName, mName ;
public:
    VariableRecord(string file): dName{file + "_d.dat"}, mName{file + "_cabecera.dat"}
    {
        //cout << "\n\t" << this->dName << " " << this->mName << endl;
        // Initialize files
        ofstream f;
        f.open(this->dName);    f.close();
        f.open(this->mName);    f.close();
    }

    vector<Alumno> load()
    {
        streampos curr, end;
        vector<Alumno> vec;

        fstream file;
        if (!file)
            throw std::runtime_error("cannot open file!\n");
        file.open(this->dName, ios::binary | ios::in);

        curr = file.tellg();    file.seekg(0, ios::end);
        end = file.tellg();     file.seekg(0, ios::beg);

        while (curr < end)
        {
            Alumno aux;
            int idx;
            char* field = nullptr;

            // * CODIGO     cout << al.codigo << endl;
            file.read((char*) &idx, sizeof(int));    // cout << idx << " ";
            field = new char[idx];   memset(field, 0, idx+1);
            file.read(field, idx);     aux.codigo = string(field);

            // * CICLO      cout << al.ciclo << endl;
            file.read((char*) &idx, sizeof(int));    // cout << idx << " ";
            file.read((char*) &aux.ciclo, idx);

            // * MENSUALIDAD    cout << al.mensualidad << endl;
            file.read((char*) &idx, sizeof(int));    // cout << idx << " ";
            file.read((char*) &aux.mensualidad, idx);

            // * OBSERVACIONES  cout << al.observaciones << endl;
            file.read((char*) &idx, sizeof(int));    // cout << idx << "\n";
            field = new char[idx];   memset(field, 0, idx+1);
            file.read(field, idx);
            aux.observaciones = string(field);

            //cout << aux << endl;
            curr += aux.sizeBytes() + 16;

            vec.push_back(aux);
        }

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at reading time\n");
        return vec;
    }

    void add(Alumno record)
    {
        fstream file;
        file.open(dName, ios::binary | ios::app | ios::out | ios::in);
        if (!file)
            throw std::runtime_error("cannot open file!\n");

        // SAVE RECORD
        int n;      // RECORD LINE SIZE = BYTES OF OBJECT + 16 (4 ints de separador)
        n = record.codigo.size();   file.write((char*) &n, sizeof(int));
        file.write(record.codigo.c_str(), n);

        n = sizeof(int);    file.write((char*) &n, n);
        file.write((char*) &record.ciclo, n);

        n = sizeof(float);  file.write((char*) &n, n);
        file.write((char*) &record.mensualidad, n);

        n = record.observaciones.size();    file.write((char*) &n, sizeof(int));
        file.write(record.observaciones.c_str(), n);

        file.close();
        if (!file.good())
            throw std::runtime_error("error occurred at writing time\n");

        // SAVE METADATA
        streampos start, end;
        EntryMeta last, final;
        int emSize = sizeof(EntryMeta);

        fstream meta;
        meta.open(mName, ios::binary | ios::in | ios::out);
        if (!meta)
            throw std::runtime_error("cannot open file\n");

        meta.seekg(0, ios::beg);    start = meta.tellg();
        meta.seekg(0, ios::end);    end = meta.tellg();
        int sz = (end - start)/emSize;  // cout << sz << endl;

        // read last EntryMeta record
        if (sz > 0) {
            meta.seekg((sz-1)*emSize, ios::beg);
            meta.read((char*)&last, emSize);
        } else {
            last = EntryMeta(0, 0);
        }   // cout << last << endl;

        // write new last EntryMeta in file
        final = EntryMeta(last.position + last.size, record.sizeBytes() + 16);
        meta.seekp(sz*emSize, ios::beg);
        meta.write((char*) &final, emSize);
        meta.close();
    }

    EntryMeta getMeta(int pos)
    {
        //cout << '\t' << __PRETTY_FUNCTION__  << endl;
        EntryMeta em;
        int emSize = sizeof(EntryMeta);
        ifstream meta(mName, ios::binary);
        if (!meta)
            throw std::runtime_error("cannot open file!\n");
        meta.seekg((pos-1)*emSize, ios::beg);
        meta.read((char*)& em, emSize);
        meta.close();
        if (!meta.good())
            throw std::runtime_error("error occurred at reading time\n");
        return em;
    }

    Alumno readRecord(int pos)
    {
        //cout << '\t' << __PRETTY_FUNCTION__  << endl;

        auto metaObj = this->getMeta(pos);
        // cout << metaObj << endl;

        fstream binData;
        binData.open(this->dName, ios::binary | ios::in | ios::out);
        if (!binData)
            throw std::runtime_error("cannot open file!\n");
        binData.seekg(metaObj.position, ios::beg);
        Alumno al;

        int idx;
        char* field = nullptr;

        // * CODIGO     cout << al.codigo << endl;
        binData.read((char*) &idx, sizeof(int));    // cout << idx << " ";
        field = new char[idx];   memset(field, 0, idx+1);
        binData.read(field, idx);     al.codigo = string(field);

        // * CICLO      cout << al.ciclo << endl;
        binData.read((char*) &idx, sizeof(int));    // cout << idx << " ";
        binData.read((char*) &al.ciclo, idx);

        // * MENSUALIDAD    cout << al.mensualidad << endl;
        binData.read((char*) &idx, sizeof(int));    // cout << idx << " ";
        binData.read((char*) &al.mensualidad, idx);

        // * OBSERVACIONES  cout << al.observaciones << endl;
        binData.read((char*) &idx, sizeof(int));    // cout << idx << "\n";
        field = new char[idx];   memset(field, 0, idx+1);
        binData.read(field, idx);
        al.observaciones = string(field);

        binData.close();
        if (!binData.good())
            throw std::runtime_error("error occurred at reading time\n");
        return al;
    }

};

void test()
{
    // cout << sizeof(EntryMeta) << '\n';
    Alumno a1, a2, a3, a4, a5, a6;
    a1 = Alumno("202010311", 5, 2400.5, "Jeremy Jeffrey Matos Cangalaya");
    a2 = Alumno("201715", 4, 1900, "Rafael Andre");
    a3 = Alumno("32741213428", 6, 2400, "Angel Romario");
    a4 = Alumno("C-1240", 8, 3250, "Natal Fuentes Campos");
    a5 = Alumno("", 2, 2235, "Camila Alejandra");
    a6 = Alumno("2021231", 7, 2341, "Ciencia de la Computacion");

    // cout << a1.sizeBytes() << endl;
    VariableRecord vR("datos4");
    cout << "\tADD\n";
    vR.add(a1); vR.add(a2);
    vR.add(a3); vR.add(a4);
    vR.add(a5); vR.add(a6);

    cout << "\tLOAD\n";
    auto vec = vR.load();
    for (auto v : vec)
        cout << " -> " << v << endl;

    cout << "\tREAD RECORD\n";
    int it = RNG(1, 10);    cout << "it ->" << it << endl;
    while (it--)
    {
        int idx = RNG(1, vec.size());
        cout << idx << " -> " <<  vR.readRecord(idx) << endl;
    }
}

int main()
{
    test();
}

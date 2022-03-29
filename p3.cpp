#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
using namespace std;

int RNG(int first , int last){
    random_device rd ;
    default_random_engine dre(rd()) ;
    uniform_int_distribution<int> limits(first , last) ;
    return limits(dre) ;
}

struct Alumno 
{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;

    Alumno() = default;
    Alumno(string n, string a, string c, float m):Nombre{n}, Apellidos{a}, Carrera{c}, mensualidad{m}{}
    Alumno(string attributes, char sep)
    {
        string var;
        vector<string> feat;
        stringstream fields(attributes);

        while(getline(fields, var, sep))
            feat.push_back(var);

        this->Nombre = feat[0]; 
        this->Apellidos = feat[1]; 
        this->Carrera = feat[2];
        this->mensualidad = stof(feat[3]);
    }
};

ostream& operator<<(ostream& os, Alumno a)
{
    os << a.Nombre << " || " << a.Apellidos << " || " << a.Carrera << " || " << a.mensualidad << '\n';
    return os;
}

class VariableRecord
{
    string fName;
public:
    VariableRecord(string str) : fName{str}{};

    vector<Alumno> load()
    {
        vector<Alumno> vec;
        ifstream file(this->fName);
        string line;

        if (file.is_open())
        {
            getline(file, line);
            while(getline(file, line))
                vec.push_back(Alumno(line, '|'));
            
            file.close();
        }
        else throw std::runtime_error("file open error\n");

        return vec;
    }

    void add(Alumno record)
    {
        ofstream file(this->fName, ios::app);
        if (file.is_open())
        {
            file << '\n' << record.Nombre << "|" << record.Apellidos << "|" << record.Carrera << "|" << record.mensualidad;       
            file.close();            
        }
        else throw std::runtime_error("file open error\n");
    }

    Alumno readRecord(int pos)
    {
        ifstream file(this->fName);
        string line; 

        if (file.is_open())
        {
            getline(file, line);

            while(pos--)
                getline(file, line);
            
            file.close();          //cout << line << endl;
        }
        else throw std::runtime_error("file open error\n");
        
        return Alumno(line, '|');
    }
};

int main()
{
    string fileName = "datos3.txt";
    VariableRecord vr(fileName);
    
    // ADD
    vr.add(Alumno("Jeremy", "Matos Cangalaya", "Computer Science", 2200.50));
    cout << '\n';

    // LOAD
    auto alumnos = vr.load();
    for (const auto& al : alumnos)
        cout << al;
    cout << '\n';

    // READ
    int sz = alumnos.size(), test = 5;
    while(test--)
    {
        auto rN = RNG(1, sz);
        cout << rN << " -> " << vr.readRecord(rN);
    }
}

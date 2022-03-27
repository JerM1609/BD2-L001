/**
* ? pos = i * bytes || pos is an index
*/
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void parseString(char arr[], int sz, const string& str)
{
    int mn = min((int)str.size(), sz);
    for (int i = 0; i < mn; i++)
        *(arr+i) = str[i];
    for (int i = mn; i < sz; i++)
        *(arr+i) = ' ';
}

template<typename T>
ostream& operator<<(ostream& os, pair<T*, int> pr)
{
    for (int i = 0; i < pr.second; i++)
        os << *(pr.first + i);
    return os;
}

struct Alumno
{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];

    Alumno() = default;

    Alumno(string code, string name, string last, string career)
    {
        parseString(codigo, 5, code);
        parseString(nombre, 11, name);
        parseString(apellidos, 20, last);
        parseString(carrera, 15, career);


        // cout << make_pair(codigo, 5) << '\n';
        // cout << make_pair(nombre, 11) << '\n';
        // cout << make_pair(apellidos, 20) << '\n';
        // cout << make_pair(carrera, 15) << '\n';
    }
};

istream& operator>>(istream& stream, Alumno& record)
{
    stream.read(record.codigo, 5);
    stream.read(record.nombre, 11);
    stream.read(record.apellidos, 20);
    stream.read(record.carrera, 15);
    stream.get();       // read \n
    return stream;
}

ostream& operator<<(ostream& stream, Alumno& record)
{
    stream.write(record.codigo, 5);     
    stream.write(record.nombre, 11);
    stream.write(record.apellidos, 20);
    stream.write(record.carrera, 15);
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
        vector<Alumno> vec;
        ifstream file(this->fName);

        if (file.is_open())
        {
            stringstream strL;
            string line;
            while(getline(file, line))
            {
                strL = stringstream(line);
                Alumno al;
                strL >> al;     //cout << al;
                vec.push_back(al);
            }
        }
        else throw std::runtime_error("file open error\n");

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
        Alumno al;
        ifstream file(this->fName);
        if (file.is_open())
        {
            string line;
            
            while (pos--)
                getline(file, line);

            stringstream str(line);
            str >> al;
        } 
        else throw std::runtime_error("file open error\n");

        return al;
    }
};

int main()
{
    string fileName = "datos1.txt";
    FixedRecord fr(fileName);

    auto al = fr.readRecord(5);
    cout << al << endl;
    
    // fr.add(Alumno("202010311", "Jeremy Jeffrey", "Matos Cangalaya", "Computer Science"));
    // fr.add(Alumno("2020", "Jeremy", "Matos", "CS"));
 
    // auto alumnos = fr.load();
    // cout << "ALUMNOS\n";
    // for (auto a : alumnos)
    //     cout << a << '\n';
    
    return 0;
}

/**
 * ? while !file.eof() c++ possible bugs
 * https://stackoverflow.com/questions/5605125/why-is-iostreameof-inside-a-loop-condition-i-e-while-stream-eof-cons
 * https://hownot2code.com/2016/06/08/the-end-of-file-eof-check-may-not-be-enough/
 * https://www.daniweb.com/programming/software-development/threads/38426/while-loop-not-ending-when-reading-from-file
*/

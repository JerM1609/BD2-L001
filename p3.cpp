#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <iomanip>
#define endll '\n'
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

    Alumno()=default;
    Alumno(const string& Nombre, const string& Apellidos, const string& Carrera, float mensualidad): Nombre(Nombre), Apellidos(Apellidos), Carrera(Carrera), mensualidad(mensualidad) {}
    void building(){
        getline(cin,Nombre);
        getline(cin,Apellidos);
        getline(cin,Carrera);
        cin>>mensualidad;
    }
    string print(){
        return Nombre+" "+Apellidos+" "+Carrera+" "+to_string(mensualidad);
    }
    

};
void saving_alumno(Alumno& nuevo, string& leido){
    vector<string> cont; 
    stringstream led(leido);
    while(getline(led, leido, '|')){
        cont.push_back(leido);
    }
    nuevo.Nombre = cont[0];
    nuevo.Apellidos = cont[1];
    nuevo.Carrera = cont[2];
    nuevo.mensualidad = stof(cont[3]);
}

class VariableRecord{
    string archivo; 
public:
    VariableRecord(string archivo): archivo(archivo){}
    
    Alumno readRecord(int pos){
        ifstream stream(archivo); 
        Alumno nuevo; 
        string leido; 
        int i  = 1; 

        while(getline(stream, leido)){             
            if(i == pos){
                saving_alumno(nuevo, leido);
                break; 
            }
            i++; 
        }
        stream.close();
        return nuevo; 
    }
    vector<Alumno> load(){
        vector<Alumno> out; 
        ifstream stream(archivo);
        string leido; 
        Alumno nuevo; 
        while(getline(stream, leido)){
                       
            saving_alumno(nuevo, leido);
            out.push_back(nuevo);
            
        }
        return out;
    }

    void add(const Alumno& record){
        ofstream stream(archivo, ios::app);
        
        stream << record.Nombre+'|'+record.Apellidos+'|'+record.Carrera+'|';
        stream << fixed<< setprecision(2)<<record.mensualidad;
        stream <<'\n';
    }

};

int main()
{
    string fileName = "datos3.txt";
    VariableRecord vr(fileName);
    
    // ADD
    vr.add(Alumno("Jeremy", "Matos Cangalaya", "Computer Science", 2200.50));
    vr.add(Alumno("Marcos Daniel", "Ayala Pineda", "Computer Science", 2100));
    cout << '\n';

    // LOAD
    auto alumnos = vr.load();
    vector<Alumno> s1 = vr.load();
    for(int i = 0 ; i <s1.size(); i++){
        cout<<s1[i].Nombre<<" "<<s1[i].Apellidos<<" "<<s1[i].Carrera<<" "<<s1[i].mensualidad<<endll; 
    }

    // READ
    int sz = alumnos.size(), test = 5;

    while(test--)
    {
        auto rN = RNG(1, sz);
        cout << rN << " -> "<< vr.readRecord(rN).print()<<endll;
    }

}

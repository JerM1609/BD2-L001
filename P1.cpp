#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include <string>
#include <sstream>
using namespace std;
#define endll '\n'


class Alumno
{
public:
    char codigo [6];
    char nombre [12];
    char apellidos [21];
    char carrera [16];
    Alumno()=default;
    Alumno(string scodigo, string snombre, string sapellidos, string scarrera){
        for(int i = 0  ; i<5; i++) {
            codigo[i] = scodigo.size() <= i ?  ' ' : scodigo[i]; 
        }            
        for(int i = 0  ; i<11; i++) {
            nombre[i] = snombre.size() <=i ?  ' ' : snombre[i]; 
        }            
        for(int i = 0  ; i<20; i++) {
            apellidos[i] = sapellidos.size() <=i ?  ' ' : sapellidos[i];
        }        
        for(int i = 0  ; i<15; i++) {
            carrera[i] = scarrera.size() <=i ?  ' ' : scarrera[i];
        }
    }
    void print(){cout<<codigo<<" "<<nombre<<" "<<apellidos<<" "<<carrera<<endl;}
};


Alumno initRecord(){
    Alumno nuevo; 
    string axN;
    getline(cin, axN);
    for(int i = 0  ; i<5; i++) {
        nuevo.codigo[i] = axN.size()-1 <i ?  ' ' : axN[i]; 
    }    
    getline(cin, axN);
    for(int i = 0  ; i<11; i++) {
        nuevo.nombre[i] = axN.size()-1 <i ?  ' ' : axN[i]; 
    }    
    getline(cin, axN);
    for(int i = 0  ; i<20; i++) {
        nuevo.apellidos[i] = axN.size()-1 <i ?  ' ' : axN[i];
    }
    getline(cin, axN);
    for(int i = 0  ; i<15; i++) {
        nuevo.carrera[i] = axN.size()-1 <i ?  ' ' : axN[i];
    }

    return nuevo;
}
ostream & operator << (ostream & stream, Alumno & record)
{  
    stream.write(record.codigo, 5); 
    stream.write(record.nombre, 11); 
    stream.write(record.apellidos, 20); 
    stream.write(record.carrera, 15); 

    stream << "\n";
    stream << flush;
    return stream;
} 
istream& operator>>(istream& stream, Alumno& record)
{
    stream.read(record.codigo, 5);      record.codigo[5] = '\0';
    stream.read(record.nombre, 11);     record.nombre[11] = '\0';
    stream.read(record.apellidos, 20);  record.apellidos[20] = '\0';
    stream.read(record.carrera, 15);    record.carrera[15] = '\0';
    stream.get();       // read \n
    return stream;
}
class FixedRecord{
    string archivo; 
public:
    FixedRecord(string archivo): archivo(archivo) {}


    vector<Alumno> load(){
        ifstream stream; 
        stream.open(archivo);
        Alumno nuevo; 

        stream.seekg (0, stream.end);
        int length = stream.tellg()/51;
        stream.seekg (0, stream.beg);
        vector<Alumno> out(length);

        for(int i = 1 ; i<length ; i++)
        {            
            out[i] = readRecord(i);          
        }                        

        return out;
    }

    Alumno readRecord(int pos){ 
        Alumno al;
        ifstream file(this->archivo);
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
   void add(Alumno record){
        ofstream loading_file; 
        loading_file.open(archivo,ios::app);    
        loading_file<<record;
    }
};



int main(){
    FixedRecord work("datos1.txt"); 
    
    work.readRecord(5).print();
    work.readRecord(1).print();
    work.readRecord(7).print();
    cout<<"-------------"<<endll;
    Alumno a("202010311", "Jeremy Jeffrey", "Matos Cangalaya", "Computer Science");
    a.print();
    work.add(a);
    
    vector<Alumno> ext = work.load();     
    for(int i  = 0 ; i<ext.size();i++){
        ext[i].print();
    }

}
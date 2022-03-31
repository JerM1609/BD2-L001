/**
 * ! archivo binario usando el tamaño del dato como separador
 * ? implementar metodos sobre una clase
 * ? Matricula readRecord(int pos)
*/
#include <iostream>
using namespace std;

struct EntryMeta
{
    int position, size;
    EntryMeta() = default;
    EntryMeta(int p, int sz): position{p}, size{sz}{}
}

struct Alumno
{
    string codigo;
    int ciclo; 
    float mensualidad;
    string observaciones;

    Alumno() = default;
    Alumno(string c, int ci, float men, string obs): codigo{c}, ciclo{ci}, mensualidad{men}, observaciones{ob}{}
};

void test()
{

}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    test();
}

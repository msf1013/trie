#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include<ctype.h>

using namespace std;

// Archivo de salida
ofstream archDiccSalida;

// Metodo que convierte caracteres de minusculas a mayusculas
string minusculas(string p){
    transform(p.begin(),p.end(), p.begin(), ::tolower);
    return p;
}

// Metodo que evalua si un string se conforma unicamente por digitos numericos
bool todosDigitos(string p){
    for(int i=0; i<p.length(); i++){
        if(!isdigit(p[i])) return false;
    }
    return true;
}

// Metodo que evalua si un string se conforma unicamente por caracteres alfabeticos
bool todosAlfa(string p){
    for(int i=0; i<p.length(); i++){
        if(!isalpha(p[i])) return false;
    }
    return true;
}

// Metodo que elimina caracteres que no representan letras del alfabeto
string eliminaNoLetras(string p){

    int pos=0;
    while(pos<p.length()){
        if(!isalpha(p[pos])){
            p.erase(pos,1);
        } else {
            pos++;
        }
    }
    return p;
}

class Nodo{
    public:
        char val; // Caracter almacenado por el nodo
        bool palabraCompleta; // Valor booleano que identifica si el string que termina con la letra del nodo, es una palabra completa
        Nodo * arr[26]; // Arreglo de apuntadores a los siguientes nodos/palabras
        Nodo(){
            val=' ';
            palabraCompleta=0;
            for(int i=0; i<26; i++) arr[i]=NULL;
        }
        Nodo(char v){
            val=v;
            palabraCompleta=0;
            for(int i=0; i<26; i++) arr[i]=NULL;
        }
};

class Arbol{
    private:
        Nodo * raiz;
    public:
        // Metodo para la insercion de una nueva palabra en el arbol
        void inserta(string pal){
            pal+="z"; // Letra "extra" agregada a la palabra a insertar, para facilitar ciclo de insercion
            Nodo * ant, * act;
            ant=raiz;
            act=raiz->arr[ (int)pal[0]-97 ];

            for(int i=0; i<pal.size()-1; i++){
                if(act==NULL){
                    ant->arr[ (int)pal[i]-97 ]=new Nodo(pal[i]);
                    act=ant->arr[ (int)pal[i]-97 ];
                }
                ant=act;
                act=act->arr[ (int)pal[i+1]-97 ];
            }
            ant->palabraCompleta=1;
        }

        // Metodo booleano para evaluar la existencia de una palabra en el arbol
        bool existe(string pal){
            pal=minusculas(pal); // La palabra a buscar se convierte a minusculas
            Nodo * aux = raiz->arr[ (int)pal[0]-97 ];
            for(int i=0; i<pal.length()-1; i++){
                if(aux==NULL){
                    return false;
                }
                aux=aux->arr[ (int)pal[i+1]-97 ];
            }
            if(aux==NULL){
                return false;
            }
            if(aux->palabraCompleta)
                return true;
            else
                return false;
        }
        // Metodo que devuelve un string con la rama mas profunda del arbol que coincide con el string recibido como parametro
        string raizPal(string pal){
            string raizComun="";
            pal=minusculas(pal);
            pal+="z";
            if(pal.length()==1) return raizComun;
            Nodo * aux = raiz->arr[ (int)pal[0]-97 ];
            for(int i=0; i<pal.length(); i++){
                if(aux==NULL){
                    return raizComun;
                }
                raizComun=raizComun+pal[i];
                aux=aux->arr[ (int)pal[i+1]-97 ];
            }
            return raizComun;
        }

        // Metodo recursivo para encontrar las palabras del arbol cuya raiz coincide con el string recibido como parametro
        // Recibe como parametro el string que representa la raiz a buscar, la referencia al vector en el que se almacenan las coincidencias
        // y el apuntador al nodo que actualmente se visita
        void busqueda(string pal, vector<string> &prueba, Nodo * r){
            if(r!=NULL){
                bool fin=1;
                for(int i=0; i<26; i++){
                    if(r->arr[i]!=NULL){
                        fin=0;
                        pal.push_back(r->arr[i]->val);
                        busqueda(pal,prueba,r->arr[i]);
                        pal=pal.substr(0, pal.size()-1);
                    }
                }
                if(fin||r->palabraCompleta)
                    prueba.push_back(pal);
            }
        }

        // Metodo recursivo para encontrar las palabras del arbol cuya raiz coincide con el string recibido como parametro
        // Recibe como parametro el string que representa la raiz a buscar, la referencia al vector en el que se almacenan las coincidencias
        // y el apuntador al nodo que actualmente se visita
        void busqueda2(string pal, vector<string> &prueba, Nodo * r){
            if(r!=NULL){
                if(r->palabraCompleta){
                    prueba.push_back(pal);
                }
                for(int i=0; i<26; i++){
                    busqueda2(pal+(char)(i+97),prueba,r->arr[i]);
                }
            }
        }

        // Metodo que invoca al metodo recursivo "busqueda(...)" para encontrar las palabras cuya raiz coincide con el string recibido como parametro
        void busca(string pal, vector<string> &prueba){
            pal=minusculas(pal);
            Nodo * sig = raiz->arr[ (int)pal[0]-97 ];
            for(int i=0; i<pal.size()-1; i++){
                if(sig==NULL){
                    return;
                }
                sig=sig->arr[ (int)pal[i+1]-97 ];
            }
            if(sig==NULL){
                return;
            }
            busqueda2(pal, prueba, sig);
        }

        // Metodo que elimina del arbol el string recibido como parametro
        // Precondicion: La palabra existe en el arbol y el arbol no esta vacio
        // Postcondicion: Palabra eliminada del arbol
        void elimina(string pal){
            if(eliminaRec(raiz->arr[(int)pal[0]-97],0,pal)){
                delete raiz->arr[(int)pal[0]-97];
                raiz->arr[(int)pal[0]-97]=NULL;
            }
        }

        // Metodo que elimina del arbol el nodo apuntado por el apuntador recibido como parametro
        bool eliminaRec(Nodo * r, int pos, string pal){

            if(pos!=pal.length()-1){
                bool sigEliminar = eliminaRec(r->arr[(int)pal[pos+1]-97],pos+1,pal);
                if(sigEliminar==1){
                    delete r->arr[(int)pal[pos+1]-97];
                    r->arr[(int)pal[pos+1]-97]=NULL;
                    for(int i=0; i<26; i++){
                        if(r->arr[i]!=NULL){
                            return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
            } else {
                r->palabraCompleta=0;
                for(int i=0; i<26; i++){
                    if(r->arr[i]!=NULL) return false;
                }
                return true;
            }

        }

        Arbol(){
            raiz=new Nodo();
        }
        ~Arbol(){
            libera(raiz);
        }

        void libera (Nodo * r)
        {
            if (r != NULL)
            {
                for(int i=0; i<26; i++){
                    libera(r->arr[i]);
                }
                delete(r);
            }
        }

        // Metodo para guardar el archivo de diccionario con los cambios realizados tras ejecutar el programa
        void guardaArchivo(){
            guarda(raiz,"");
        }

        // Metodo recursivo para imprimir en archivo las palabras del diccionario
        void guarda(Nodo * aux, string pal){
            if(aux!=NULL){
                if(aux->palabraCompleta){
                    archDiccSalida << pal << endl;
                }
                for(int i=0; i<26; i++){
                    guarda(aux->arr[i],pal+(char)(i+97));
                }
            }
        }

};

int main(){
    string menu="1) Corregir texto\n2) Agregar palabra al diccionario\n3) Eliminar palabra del diccionario\n4) Buscar palabra en la lista\n5) Salir\n";
    Arbol a;
    string k, nombreArchEntrada, nombreArchSalida;
    ifstream archDicc,archEntrada;
    ofstream archSalida;
    int contador=0;
    archDicc.open("google-10000-english.txt");
    while(archDicc>>k){
            contador++;
        a.inserta(k);
    }
    cout << contador << endl;
    archDicc.close();

    int opcion=1;

    while(opcion!=5){
        cout << menu;
        cin >> opcion;
        // Corregir texto
        if(opcion==1){
            cout << "Ingrese el nombre del archivo de entrada a corregir:" << endl;
            cin>>nombreArchEntrada;
            cout << "Ingrese el nombre del archivo de salida:" << endl;
            cin >> nombreArchSalida;

            archEntrada.open(nombreArchEntrada.c_str());
            archSalida.open(nombreArchSalida.c_str());
            bool valido,mayuscula;
            char charIni, charFin, charFin2, charFin3;
            string copia,respuesta,resAgregar;
            while(archEntrada>>k){
                mayuscula=0;
                charIni=k[0];
                charFin=k[k.length()-1];

                if(!isalpha(charIni)){
                    k.erase(0,1);
                } else {
                    charIni='\0';
                }

                // Ultimo caracter
                if(k.length()!=0 && !isalpha(charFin)){
                    k.erase(k.length()-1,1);
                } else {
                    charFin='\0';
                }
                // Penultimo caracter
                if(k.length()!=0 && charFin){
                    charFin2=k[k.length()-1];
                    if(!isalpha(charFin2)){
                        k.erase(k.length()-1,1);
                    } else {
                        charFin2='\0';
                    }
                } else {
                    charFin2='\0';
                }
                // Antepenultimo caracter
                if(k.length()!=0 && charFin2){
                    charFin3=k[k.length()-1];
                    if(!isalpha(charFin3)){
                        k.erase(k.length()-1,1);
                    } else {
                        charFin3='\0';
                    }
                } else {
                    charFin3='\0';
                }
                int tamOriginal=k.length();
                copia=k;
                k=eliminaNoLetras(k);
                if( tamOriginal==k.length()&&a.existe(k) ){
                    if(charIni) k=charIni+k;
                    if(charFin3) k=k+charFin3;
                    if(charFin2) k=k+charFin2;
                    if(charFin) k=k+charFin;
                    archSalida<< k << " ";
                } else if( todosDigitos(copia) ){
                    if(charIni) copia=charIni+copia;
                    if(charFin3) copia=copia+charFin3;
                    if(charFin2) copia=copia+charFin2;
                    if(charFin) copia=copia+charFin;
                    archSalida<< copia << " ";
                } else {
                    cout << "La palabra \"" << k << "\" no se encuentra en el diccionario, ¿desea agregarla?" << endl;
                    cout << "(s/n)" << endl;
                    cin >> resAgregar;
                    if(resAgregar=="S"||resAgregar=="s"){
                        a.inserta(k);
                        if(charIni) k=charIni+k;
                        if(charFin3) k=k+charFin3;
                        if(charFin2) k=k+charFin2;
                        if(charFin) k=k+charFin;
                        archSalida<< k << " ";
                    } else {
                        vector<string> opciones;
                        if(k.length()!=0&&isupper(k[0])) mayuscula=1;
                        a.busca(a.raizPal(k),opciones);
                        respuesta="";
                        if(opciones.size()==0){
                            if(charIni) respuesta=charIni;
                            respuesta+="[Palabra no encontrada]";
                            if(charFin3) respuesta=respuesta+charFin3;
                            if(charFin2) respuesta=respuesta+charFin2;
                            if(charFin) respuesta=respuesta+charFin;
                        } else {
                            if(charIni) respuesta=charIni;
                            if(mayuscula) opciones[0][0]=toupper(opciones[0][0]);
                            respuesta+="["+opciones[0];
                            for(int y=1; y<opciones.size()&&y<10; y++){
                                if(mayuscula) opciones[y][0]=toupper(opciones[y][0]);
                                respuesta+=","+opciones[y];
                            }
                            respuesta+="]";
                            if(charFin3) respuesta=respuesta+charFin3;
                            if(charFin2) respuesta=respuesta+charFin2;
                            if(charFin) respuesta=respuesta+charFin;
                        }
                        archSalida<< respuesta << " ";
                    }
                }

            }

            archEntrada.close();
            archSalida.close();

        // Agregar palabra al diccionario
        } else if(opcion==2){
            cout << "Ingrese el nombre la palabra que desea agregar al diccionario:" << endl;
            cin>>k;
            if(todosAlfa(k)&&!a.existe(k)){
                a.inserta(k);
                cout << "La palabra se agrego con exito al diccionario" << endl;
            } else {
                cout << "La palabra no se pudo agregar porque contiene caracteres invalidos o ya se encuentra dentro del diccionario" << endl;
            }
        // Eliminar palabra del diccionario
        } else if(opcion==3){
            cout << "Ingrese el nombre la palabra que desea eliminar del diccionario:" << endl;
            cin>>k;
            if(todosAlfa(k)&&a.existe(k)){
                a.elimina(k);
                cout << "La palabra se elimino con exito del diccionario" << endl;
            } else {
                cout << "La palabra no se pudo eliminar porque contiene caracteres invalidos o no forma parte del diccionario" << endl;
            }
        // Buscar palabra en la lista
        } else if(opcion==4){
            vector<string> resultados;
            cout << "Ingrese el nombre la palabra que desea buscar:" << endl;
            cin>>k;
            if(todosAlfa(k)){
                a.busca(k,resultados);
                if(resultados.size()){
                    cout << "Lista de resultados que coinciden con el criterio de busqueda:" << endl;
                    for(int i=0; i<resultados.size(); i++){
                        cout << resultados[i] << endl;
                    }
                } else {
                    cout << "No se encontraron resultados" << endl;
                }
                cout << endl;
            } else {
                cout << "La palabra no se pudo buscar porque contiene caracteres invalidos" << endl;
            }
        }
    }

    archDiccSalida.open("google-10000-english.txt");
    a.guardaArchivo();
    archDiccSalida.close();

    return 0;
}

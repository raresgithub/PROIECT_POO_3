#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

const int NR_TRANZACTII=2;
const int BONUS_CONT=10;
const int BONUS_CONT_DE_ECONOMII=5;
const double COST_RETRGERE=3.45;
const int DIMENSIUNE_NUME_CONT=50;
const int DIMENSIUNE_ISTORIC=250;

class Cont
{
    static int nr_conturi;
    string detinator;
    string data;
    double sold;
protected:
    int nr_tranzactii_gratuite;
public:
    Cont();
    Cont(string,string,double);
    Cont(const Cont&c2);
    virtual ~Cont();
    static int get_numar_conturi();
    string get_detinator()const;
    double get_sold()const;
    void set_sold(double);
    void aduna_bonus_cont(int bon);
    virtual void retragere(double);
    virtual void depunere(double);
    virtual void citire(istream&is);
    virtual void afisare(ostream&os);
    friend istream&operator>>(istream&is,Cont&c);
    friend ostream&operator<<(ostream&os,const Cont&c);
    Cont operator=(const Cont&c2);
};

int Cont::nr_conturi=0;

Cont::Cont()
{
    nr_conturi++;
    sold=0;
    nr_tranzactii_gratuite=NR_TRANZACTII;
}

Cont::Cont(string detinator2,string data2,double s)
{
    nr_conturi++;
    detinator=detinator2;
    data=data2;
    sold=s;
    nr_tranzactii_gratuite=NR_TRANZACTII;
}

Cont::Cont(const Cont&c2)
{
    nr_conturi++;
    detinator=c2.detinator;
    data=c2.data;
    sold=c2.sold;
    nr_tranzactii_gratuite=c2.nr_tranzactii_gratuite;
}

Cont::~Cont()
{
    nr_conturi--;
}

int Cont::get_numar_conturi()
{
    return nr_conturi;
}

double Cont::get_sold()const
{
    return sold;
}

void Cont::set_sold(double sold_nou)
{
    sold=sold_nou;
}

string Cont::get_detinator()const
{
    return detinator;
}

void Cont::aduna_bonus_cont(int bon)
{
    nr_tranzactii_gratuite=nr_tranzactii_gratuite+bon;
}

void Cont::depunere(double suma)
{
    sold=sold+suma;
}

void Cont::retragere(double suma)
{
    if(nr_tranzactii_gratuite)
        try
        {
            if(suma>sold)
                throw 0;
            nr_tranzactii_gratuite--;
            sold=sold-suma;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
    else
        try
        {
            if(suma+COST_RETRGERE>sold)
                throw 0;
            sold=sold-suma-COST_RETRGERE;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }

}

istream&operator>>(istream&is,Cont&c)
{
    cout<<"Detinator: ";
    getline(is,c.detinator);
    cout<<"Data: ";
    getline(is,c.data);
    cout<<"Sold: ";
    is>>c.sold;

    string temp;
    getline(is,temp);
    return is;
}

void Cont::citire(istream&is)
{
    is>>(*this);
}

ostream&operator<<(ostream&os,const Cont&c)
{
    os<<"Detinator: "<<c.detinator<<"\n";
    os<<"Data deschiderii: "<<c.data<<"\n";
    os<<"Sold: "<<c.sold<<"\n";
    os<<"Nr. tranzactii: "<<c.nr_tranzactii_gratuite<<"\n";
    return os;
}

void Cont::afisare(ostream&os)
{
    os<<(*this);
}

Cont Cont::operator=(const Cont&c2)
{
    if(this!=&c2)
    {
        detinator=c2.detinator;
        data=c2.data;
        sold=c2.sold;
        nr_tranzactii_gratuite=c2.nr_tranzactii_gratuite;
    }
    return (*this);
}


/********************************************/
class Cont_de_Economii:public Cont
{
    double rata_dobanda;
    int interval;
    string istoric;
public:
    Cont_de_Economii();
    Cont_de_Economii(string,string,double,double,int);
    Cont_de_Economii(const Cont_de_Economii&);
    ~Cont_de_Economii();
    void retragere(double);
    void depunere(double);
    void citire(istream&is);
    void afisare(ostream&os);
    Cont_de_Economii operator=(const Cont_de_Economii&c2);
};

Cont_de_Economii::Cont_de_Economii():Cont()
{
    rata_dobanda=0;
    interval=0;
    string temp=to_string(get_sold());
    istoric+=temp;
}

Cont_de_Economii::Cont_de_Economii(string det,string data,double sold,double rata,int interv):Cont(det,data,sold)
{
    rata_dobanda=rata;
    interval=interv;
    string temp=to_string(get_sold());
    istoric+=temp;
}

Cont_de_Economii::Cont_de_Economii(const Cont_de_Economii&c2):Cont(c2)
{
    rata_dobanda=c2.rata_dobanda;
    interval=c2.interval;
    string temp;
    istoric+=" ";
    temp=to_string(get_sold());
    istoric+= temp;
}

Cont_de_Economii::~Cont_de_Economii() {}

void Cont_de_Economii::depunere(double suma)
{
    set_sold(get_sold()+suma);
    string temp;
    istoric+=" ";
    temp=to_string(get_sold());
    istoric+= temp;
}

void Cont_de_Economii::retragere(double suma)
{
    if(nr_tranzactii_gratuite)
        try
        {
            if(suma>(this->get_sold()))
                throw 0;

            nr_tranzactii_gratuite--;
            set_sold(get_sold()-suma);
            string temp;
            istoric+=" ";
            temp=to_string(get_sold());
            istoric+= temp;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
    else
        try
        {
            if(suma+COST_RETRGERE>(this->get_sold()))
                throw 0;
            set_sold(get_sold()-suma-COST_RETRGERE);
            string temp;
            istoric+=" ";
            temp=to_string(get_sold());
            istoric+= temp;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
}

void Cont_de_Economii::citire(istream&is)
{
    this->Cont::citire(is);
    cout<<"Rata dobanda: ";
    is>>rata_dobanda;
    cout<<"Interval: ";
    is>>interval;
    is.get();
    string temp;
    istoric+=" ";
    temp=to_string(get_sold());
    istoric+= temp;
    try
    {
        if(interval<0)
            throw -1;
        if(rata_dobanda<0)
            throw 'a';
    }
    catch (int x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" intervalul este invalid!\n";
    }
    catch (char x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" rata dobanzii este invalida!\n";
    }
}

void Cont_de_Economii::afisare(ostream&os)
{
    this->Cont::afisare(os);
    os<<"Rata dobanzii: "<<rata_dobanda<<"% \n";
    os<<"Interval: "<<interval<<" luni\n";
    os<<"Istoric: "<<istoric<<"\n";
}

Cont_de_Economii Cont_de_Economii::operator=(const Cont_de_Economii&c2)
{
    if(this!=&c2)
    {
        this->Cont::operator=(c2);
        rata_dobanda=c2.rata_dobanda;
        interval=c2.interval;
        istoric=c2.istoric;
    }
    return (*this);
}


/********************************************/
template <class T> class GestionareConturi
{
    vector<T*>vectConturi;
    static int index;
    int nr_conturi;
public:
    GestionareConturi();
    GestionareConturi(const GestionareConturi<T>&c2);
    ~GestionareConturi();
    void get_tip_cont(int,ostream&)const;
    void get_nr_tip_conturi(ostream&);
    void adaugare_cont(istream&);
    void afisare_cont(ostream&os,int nr_cont);
    void afisare_conturi_banca(ostream&os);
    GestionareConturi<T> operator=(const GestionareConturi<T>&c2);
    GestionareConturi<T> operator++();
};

template<class T> int GestionareConturi<T>::index=0;

template<class T> GestionareConturi<T>::GestionareConturi()
{
    index++;
};

template<class T> GestionareConturi<T>::~GestionareConturi()
{
    index--;
};

template<class T> GestionareConturi<T>::GestionareConturi(const GestionareConturi<T>&c2)
{
    index=c2.index;
    nr_conturi=c2.nr_conturi;
    for(int i=0; i<c2.nr_conturi; i++)
        if(typeid(*c2.vectConturi[i])==typeid(Cont_de_Economii))
        {
            vectConturi.push_back(new Cont_de_Economii());
            (*vectConturi[i])=(*c2.vectConturi[i]);
        }
        else if(typeid(*c2.vectConturi[i])==typeid(T))
        {
            vectConturi.push_back(new T());
            (*vectConturi[i])=(*c2.vectConturi[i]);
        }
        else
            cout<<"\n\nEROARE\n\n";
}

template<class T> void GestionareConturi<T>::get_tip_cont(int nr_cont,ostream&os)const
{
    if(typeid(*vectConturi[nr_cont-1])==typeid(Cont_de_Economii))
        os<<"Tipul contului "<<nr_cont<<": Cont de economii.\n";
    else if(typeid(*vectConturi[nr_cont-1])==typeid(T))
        os<<"Tipul contului "<<nr_cont<<": Cont curent.\n";
    else
        os<<"Tipul contului "<<nr_cont<<": EROARE.\n";
}

template<class T> void GestionareConturi<T>::get_nr_tip_conturi(ostream&os)
{
    int nrce=0,nrc=0;
    for(int i=0; i<nr_conturi; i++)
    {
        Cont_de_Economii*pce;
        T*pc;

        pce=dynamic_cast<Cont_de_Economii*>(vectConturi[i]);
        if(pce)
            nrce++;
        else
        {
            pc=dynamic_cast<T*>(vectConturi[i]);
            if(pc)
                nrc++;
        }
    }
    os<<"Numar de conturi curente: "<<nrc<<"\n";
    os<<"Numar de conturi de economii: "<<nrce<<"\n\n";
}

template<class T>void GestionareConturi<T>::adaugare_cont(istream&is)
{
    cout<<"Tipuri de cont: 1. Cont curent 2. Cont de economii\n";
    cout<<"Tipul de cont dorit: ";
    string tip;
    getline(is,tip);
    if(tip=="Cont de economii")
    {
        nr_conturi++;
        vectConturi.push_back(new Cont_de_Economii());
        (*vectConturi[nr_conturi-1]).citire(is);
    }
    else if(tip=="Cont curent")
    {
        nr_conturi++;
        vectConturi.push_back(new T());
        (*vectConturi[nr_conturi-1]).citire(is);
    }
    else
        cout<<"Tip de cont invalid.\n";
}

template<class T> void GestionareConturi<T>::afisare_cont(ostream&os,int nr_cont)
{
    (*vectConturi[nr_cont-1]).afisare(os);
    os<<"\n";
}

template<class T> void GestionareConturi<T>::afisare_conturi_banca(ostream&os)
{
    for(int i=0; i<nr_conturi; i++)
    {
        (*vectConturi[i]).afisare(os);
        os<<"\n";
    }
}

template<class T> GestionareConturi<T> GestionareConturi<T>::operator=(const GestionareConturi<T>&c2)
{
    index=c2.index;
    nr_conturi=c2.nr_conturi;
    vectConturi.clear();
    for(int i=0; i<c2.nr_conturi; i++)
        if(typeid(*c2.vectConturi[i])==typeid(Cont_de_Economii))
        {
            Cont_de_Economii *pce=new Cont_de_Economii();
            Cont_de_Economii *p=dynamic_cast<Cont_de_Economii*>(c2.vectConturi[i]);
            (*pce)=(*p);
            vectConturi.push_back(pce);
        }
        else if(typeid(*c2.vectConturi[i])==typeid(T))
        {
            vectConturi.push_back(new T());
            (*vectConturi[i])=(*c2.vectConturi[i]);
        }
        else
            cout<<"\n\nEROARE\n\n";

    return (*this);
}

template<class T> GestionareConturi<T> GestionareConturi<T>::operator++(){return (*this);}

template<> GestionareConturi<Cont> GestionareConturi<Cont>::operator++()
{
    for(int i=0;i<nr_conturi;i++)
        (*vectConturi[i]).aduna_bonus_cont(BONUS_CONT);
    return (*this);
}

template<> GestionareConturi<Cont_de_Economii> GestionareConturi<Cont_de_Economii>::operator++()
{
    for(int i=0;i<nr_conturi;i++)
        (*vectConturi[i]).aduna_bonus_cont(BONUS_CONT_DE_ECONOMII);
    return (*this);
}

template<class T> T sold_maxim(const T&ob1,const T&ob2)
{
    if(ob1>ob2)
        return ob1;
    else
        return ob2;
}

template<> Cont sold_maxim(const Cont&ob1,const Cont&ob2)
{
    if(ob1.get_sold()>ob2.get_sold())
        return ob1;
    else
        return ob2;
}


int main()
{
    ifstream f;
    ofstream g;

    f.open("date.in");
    g.open("date.out");
    if(f.is_open())
    {
        cout<<"Banca are urmatoarele tipuri de conturi: \n"<<"1. Cont Bancar;\n2. Cont de economii.\n\n";
        cout<<"Pentru orice tip de cont avem urmatoarele proprietati:\n";
        cout<<"-orice depunere este gratuita;\n-este permis un numar de "<<NR_TRANZACTII;
        cout<<" retrageri gratuite. Pentru orice alta retragere suplimentara se va aplica un cost de "<<COST_RETRGERE<<"lei/retragere.\n\n";

        GestionareConturi<Cont> *Banca1;
        Banca1=new GestionareConturi<Cont>();
        Banca1->adaugare_cont(f);
        Banca1->adaugare_cont(f);
        Banca1->adaugare_cont(f);
        g<<"----------------------------------------------------------\n";
        g<<"Afisare cont 1 din Banca1:\n";
        Banca1->afisare_cont(g,1);


        GestionareConturi<Cont> *Banca2(Banca1);
        g<<"----------------------------------------------------------\n";
        g<<"Afisare conturi Banca2 - constructor de copiere Banca1 -:\n";
        Banca2->afisare_conturi_banca(g);


        GestionareConturi<Cont> *Banca3;
        Banca3=new GestionareConturi<Cont>();
        (*Banca3)=(*Banca1);
        g<<"----------------------------------------------------------\n";
        g<<"Afisare conturi Banca3 - operator= Banca1 -:\n";
        Banca3->afisare_conturi_banca(g);

        g<<"----------------------------------------------------------\n";
        g<<"Afisarea tipului contului 1 si 2 ale Banca3 - get_tip_cont() -:\n";
        Banca3->get_tip_cont(1,g);
        Banca3->get_tip_cont(2,g);

        g<<"----------------------------------------------------------\n";
        g<<"Afisarea numarului tipurilor de conturi ale Banca3 - get_nr_tip_conturi(ostream&) - => dynamic_cast:\n";
        Banca3->get_nr_tip_conturi(g);


        Cont_de_Economii*c;
        g<<"----------------------------------------------------------\n";
        g<<"Afisarea numarului de conturi de oricare fel create pana in prezent - get_numar_conturi() - => static function:\n";
        g<<c->get_numar_conturi()<<"\n";

        c=new Cont_de_Economii();
        c->citire(f);
        g<<"----------------------------------------------------------\n";
        g<<"Din contul:\n\n";
        c->afisare(g);
        g<<"\nse retrag 10.2 lei.\n\nDupa efectuarea retragerii:\n";
        c->retragere(10.2);
        c->afisare(g);

        g<<"----------------------------------------------------------\n";
        g<<"In contul:\n\n";
        c->afisare(g);
        g<<"\nse depun 23.2 lei.\n\nDupa efectuarea depunerii:\n";
        c->depunere(23.2);
        c->afisare(g);

        g<<"----------------------------------------------------------\n";
        g<<"Aruncarea/tratarea exceptiilor este evidentiata in functia:\n";
        g<<"void Cont_de_Economii::citire(istream&is)\n";


        g<<"----------------------------------------------------------\n";
        g<<"Exemplu specializare explicita pentru functia template<class T> void sold_maxim(T ob1,T ob2):\n\n";
        g<<"Sold maxim intre 12.5 si 36.5:\n";
        g<<sold_maxim(12.5,36.5)<<"\n\n";

        Cont *ob1,*ob2;
        ob1=new Cont("IONICA","01.11.1965",23.5);
        ob2=new Cont("POPICA","29.07.1985",100.3);
        g<<"ob1:\n";
        ob1->afisare(g);
        g<<"\n";
        g<<"ob2:\n";
        ob2->afisare(g);
        g<<"\n";
        g<<"Sold maxim intre Cont* ob1 si Cont* ob2:\n";
        sold_maxim(*ob1,*ob2).afisare(g);

        delete Banca1;
        delete Banca2;
        delete Banca3;
        delete c;
        delete ob1;
        delete ob2;


        g<<"\n----------------------------------------------------------\n";
        g<<"Exemplu specializare explicita pentru functia GestionareConturi<T> operator++() => creste numarul tranzactiilor gratuite:\n\n";
        g<<"\nPentru Banci care au la baza tipul Cont.\n\n";
        GestionareConturi<Cont> *B1;
        B1=new GestionareConturi<Cont>();
        B1->adaugare_cont(f);
        B1->afisare_conturi_banca(g);
        ++(*B1);
        B1->afisare_conturi_banca(g);

        g<<"\nPentru Banci care au la baza tipul Cont de economii.\n\n";
        GestionareConturi<Cont_de_Economii> *B2;
        B2=new GestionareConturi<Cont_de_Economii>();
        B2->adaugare_cont(f);
        B2->afisare_conturi_banca(g);
        ++(*B2);
        B2->afisare_conturi_banca(g);
    }

    f.close();
    g.close();
}

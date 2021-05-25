// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <fstream>
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return TBitField(BitField);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(s == *this);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(this->BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// Доп. Задание 1
TSet TSet::Reminders(int K) const {
    TSet temp(this->BitField);
    for(int i=1;i<GetMaxPower(); ++i){
        if(i%K!=0) temp.DelElem(i);
    }
    return temp;
}

// Доп. Здадание 2, запись и чтение из файла
bool TSet::WriteToFile (const std::string& filename) const {
    std::ofstream file(filename.c_str());
    if(!file.is_open()){
        return false;
    }
    file<<*this;
    return true;
}
// Доп. Здадание 2, запись и чтение из файла
bool TSet::ReadFromFile (const std::string& filename){
    std::ifstream file(filename.c_str());
    if(!file.is_open()){
        return false;
    }
    file>>*this;
    return true;
}

// Доп. Задание 3
void TSet::Update(const int index, const int NewElem){
    int k = index;
    for(int i=0;i<GetMaxPower(); ++i){
        if(IsMember(i)) k--;
        if(k<0){
            DelElem(i);
            InsElem(NewElem);
            break;
        }
    }
}
// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int temp;
    char ch;
    do{
        istr >> ch;
    }
    while(ch!='{');
    do{
        istr >> temp;
        s.InsElem(temp);
        do{
            istr >> ch;
        }
        while (ch!=',' && ch!='}');
    }
    while (ch != '}');
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    char ch=' ';
    ostr << '{';
    for(int i=0; i<s.GetMaxPower(); ++i){
        if(s.IsMember(i)!=0){
            ostr << ch << ' ' << i;
            ch = ',';
        }
    }
    ostr << '}';
    return ostr;
}

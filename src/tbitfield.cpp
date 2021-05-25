// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if(len<0) throw std::logic_error("negative size!");
    BitLen = len;
    MemLen = (len + sizeof(TELEM) * 8 - 1 ) >> sizeof(TELEM);
    pMem = new TELEM[MemLen];
    if (pMem != NULL)
    {
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != NULL)
    {
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & (8 * sizeof(TELEM) -1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > -1 && n < BitLen)
    {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    else{
        throw std::logic_error("negative size!");
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > -1 && n < BitLen)
    {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
    else{
        throw std::logic_error("negative size!");
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > -1 && n < BitLen)
    {
        return pMem[GetMemIndex(n)] & GetMemMask(n);
    }
    else{
        throw std::logic_error("negative size!");
    }

}

// битовые операции

TBitField &TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        if (pMem != NULL)
            delete[] pMem;
        pMem = new TELEM[MemLen];
    }
    if (pMem != NULL)
    {
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int res = 1;
    if (BitLen != bf.BitLen)
    {
        res = 0;
    }
    else
    {
        for (int i = 0; i < BitLen; ++i)
        {
            if (GetBit(i) != bf.GetBit(i))
            {
                res = 0;
                break;
            }
        }
    }
    return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(bf == *this);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField temp(std::max(BitLen, bf.BitLen));
    for (int i = 0; i < bf.BitLen; ++i)
    {
        if(bf.GetBit(i)!=0) temp.SetBit(i);
    }
    for (int i = 0; i < BitLen; ++i)
    {
        if(GetBit(i)!=0 || temp.GetBit(i)!=0) temp.SetBit(i);
        else temp.ClrBit(i);
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField temp(std::max(BitLen, bf.BitLen));
    for (int i = 0; i < MemLen; ++i){
        temp.pMem[i]=pMem[i];
    }
    for (int i = 0; i < bf.MemLen; ++i){
        temp.pMem[i]&=bf.pMem[i];
    }
    /*for (int i = 0; i < bf.BitLen; ++i)
    {
        if(bf.GetBit(i)!=0) temp.SetBit(i);
    }
    for (int i = 0; i < BitLen; ++i)
    {
        if(GetBit(i)!=0 && temp.GetBit(i)!=0) temp.SetBit(i);
        else temp.ClrBit(i);
    }
    */
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < BitLen; ++i)
    {
        if(GetBit(i)!=0) temp.ClrBit(i);
        else temp.SetBit(i);
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;
    do
    {
        istr >> ch;
    } while (ch != ' ');
    while (1)
    {
        istr >> ch;
        if (ch == '0')
        {
            bf.ClrBit(i);
        }
        else if (ch == '1')
        {
            bf.SetBit(i);
        }
        else
        {
            break;
        }
        ++i;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.GetLength()-1; i>=0; --i)
    {
        ostr << (bf.GetBit(i)!=0) ? '1' : '0';
    }

    return ostr;
}
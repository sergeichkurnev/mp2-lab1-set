// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) {}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower) {}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()) {}


TSet::operator TBitField()
{
    TBitField BITFIELD(BitField);
    return BITFIELD;
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

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s)
    {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower == s.MaxPower)
    {
        return BitField == s.BitField;
    }
    else
        return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int size;
    if (s.MaxPower >= MaxPower)
        size = s.MaxPower;
    else
        size = MaxPower;
    TSet Set(size);
    for(int i=0;i<MaxPower;i++)
        if (BitField.GetBit(i))
            Set.BitField.SetBit(i);
    for(int i=0;i<s.MaxPower;i++)
        if (s.BitField.GetBit(i))
            Set.BitField.SetBit(i);
    return Set;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    int size;
    if (Elem > MaxPower)
        size = Elem;
    else
        size = MaxPower;
    TSet ANS(size);
    for (int i = 0; i < MaxPower; i++)
        if (this->IsMember(i))
            ANS.InsElem(i);
    ANS.InsElem(Elem);
    return ANS;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet ANS(MaxPower - 1);
    for (int i = 0; i < Elem; i++)
        if (BitField.GetBit(i))
            ANS.BitField.SetBit(i);
    for (int i = 0; i < MaxPower - 1; i++)
        if (BitField.GetBit(i + 1))
            ANS.BitField.SetBit(i);
    return ANS;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int size,min_size;
    if (s.MaxPower > MaxPower)
    {
        size = s.MaxPower;
        min_size = MaxPower;
    }
    else
    {
        size = MaxPower;
        min_size = s.MaxPower;
    }
    TSet ANS(size);
    ANS.BitField = BitField & s.BitField;
    return ANS;
}

TSet TSet::operator~(void) // дополнение
{
    TSet ANS(MaxPower);
    ANS.BitField = ~(BitField);
    return ANS;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << "MaxPower: " << s.MaxPower << endl;
    ostr << "Сharacteristic vector: " << s.BitField << endl;
    return ostr;
}

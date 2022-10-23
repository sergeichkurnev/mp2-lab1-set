#include"tbitfield.h"
// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

//#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw invalid_argument("invalid argument");
    BitLen = len;
    MemLen = len / sizeof(TELEM) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = (TELEM)0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    pMem = new TELEM[bf.MemLen];
    memcpy(pMem, bf.pMem, sizeof(TELEM) * bf.MemLen);
    MemLen = bf.MemLen;
}

TBitField::~TBitField()
{
    delete[] pMem;
}


int TBitField::GetMemIndex(const int n) const noexcept // индекс Мем для бита n
{
    return n >> shiftSize;
}

TELEM TBitField::GetMemMask(const int n) const noexcept// битовая маска для бита n
{
    return 1 << (n % sizeof(TELEM));
}
// доступ к битам битового поля

int TBitField::GetLength(void) const noexcept // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    return (pMem[GetMemIndex(n)] & (GetMemMask(n))) != 0;
}
// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (&bf != this)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        pMem = new TELEM[MemLen];
        memcpy(pMem, bf.pMem, sizeof(TELEM) * bf.MemLen);
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen == bf.BitLen)
    {
        for (int i = 0; i < MemLen - 1; i++)
            if (pMem[i] != bf.pMem[i])
                return false;
        for(int i=BitsInElem*(MemLen-1);i<BitLen;i++)
            if (GetBit(i) != bf.GetBit(i))
                return false;
        return true;
    }
    else return false;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int MaxSize, MinSize;
    if (this->BitLen >= bf.BitLen)
    {
        MaxSize = (this->BitLen);
        MinSize = bf.BitLen;
    }

    else
    {
        MaxSize = (bf.BitLen);
        MinSize = this->BitLen;
    }

    TBitField result(MaxSize);
    for (int i = 0; i < MinSize; i++)
        if (bf.GetBit(i) || GetBit(i))
            result.SetBit(i);
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int MaxSize,MinSize;
    if (this->BitLen >= bf.BitLen)
    {
        MaxSize=(this->BitLen);
        MinSize = bf.BitLen;
    }
        
    else
    {
        MaxSize=(bf.BitLen);
        MinSize = this->BitLen;
    }
        
    TBitField result(MaxSize);
    for (int i = 0; i < MinSize; i++)
        if (bf.GetBit(i) && GetBit(i))
            result.SetBit(i);
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result1(BitLen);
    TBitField result2(BitLen);
    for (int i = 0; i < BitLen; i++) {
        result2.SetBit(i);
    }
    for (int i = 0; i < MemLen; i++)
        result1.pMem[i] = ~pMem[i];
    result1 = result2 & result1;
    return result1;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int n;
    for (int i = 0; i < bf.BitLen; i++)
    {
        cin >> n;
        if (n != 0 && n != 1)
            throw invalid_argument("invalid argument");
        if (n)
            bf.SetBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        cout << bf.GetBit(i);
    return ostr;
}

#pragma once
#pragma once
#include <string>
#include <iostream>
#define END 128
#define FIRST 0

using namespace std;

namespace QInt {
	class QInt {
	private:
		uint32_t a[4] = { 0 };				//Data
	public:
		QInt();
		QInt(string);
		QInt(char*);
		QInt(int);
		QInt(const QInt&);
		QInt& operator=(const QInt&);
		bool operator==(QInt);
		bool operator!=(QInt);
		QInt operator+ (QInt);
		QInt operator- (QInt);
		QInt operator* (QInt);
		QInt operator/ (QInt);
		QInt operator& (QInt);
		QInt operator| (QInt);
		QInt operator^ (QInt);
		QInt operator~ ();
		QInt& operator--();
		QInt& operator++();
		QInt operator--(int);
		QInt operator++(int);

		//Nhập số từ bàn phím, hoặc truyền vào
		void Scan(string = "");
		//Trả về chuỗi là giá trị, nếu tham số là true thì in ra màn hình
		string Print(bool = 0);
		//Set bit từ 0 -> 127 giá trị 
		void SetBitI(char, bool);
		bool GetBitI(char);
		void PrintBit();
		//đổi dấu
		QInt oppositeNumber();
		// chuyển về dương
		QInt returnPosNumber();
		void shiftLeft();
		void shiftRight();
		//Lấy bù 2	
		QInt Complement2();
	};



	void ScanQInt(QInt& x);
	void PrintQInt(QInt x);
	string DecToBin(QInt x);
	QInt BinToDec(string bit);
	string BintoHex(string bit);
	string DecToHex(QInt x);
	string HexToBin(string hex);
	QInt HexToDec(string hex);
}


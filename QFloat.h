#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace Qfloat {
	class Qfloat {
	private:
		uint32_t a[4];
	public:
		Qfloat();
		Qfloat(std::string);
		Qfloat(const Qfloat &x);
		Qfloat& operator=(const Qfloat &x);
		bool Qfloat::operator==(const Qfloat &x);
		void Scan(std::string = "");
		std::string Print(bool = false);
		bool GetBitI(int i);
		void SetBitI(int i, bool val);
		void PrintBit();

		static std::string DecToBin(Qfloat x);
		static Qfloat BinToDec(std::string bit);
		Qfloat operator+(Qfloat x);
		Qfloat operator-(Qfloat x);
		Qfloat operator*(Qfloat x);
		Qfloat operator/(Qfloat x);
	};

	void ScanQfloat(Qfloat &x);
	void PrintQfloat(Qfloat x);
}
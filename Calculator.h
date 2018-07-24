
#pragma once
#include <fstream>
#include "Qint.h"
#include "QFloat.h"

using namespace std;

//Class chứa biểu thức (cơ số, phép tính và toán hạng)
class Expression {
private:
	string operatorr; //Có phép dịch
	string base1, base2;
	string num1, num2;
public:
	Expression();
	Expression(string ope, string bs1, string bs2, string n1, string n2);
	string Answer();
};

//Nhập và xuất trên file
void Compute(ifstream&, ofstream&);
//Kiểm tra xem chuỗi s có phải số nguyên không 
bool isInt(string);
//Tính toán khi không dùng tham số dòng lệnh
void ComputeMenus();
//In ra menu với các option truyền vào
string Menu(vector<string>);
//Xóa số 0 đầu chuỗi
string Simplify(string&);
//Tách chuỗi theo kí tự t, bỏ đi t
vector<string> stringtok(string, char);
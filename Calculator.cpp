#include "Calculator.h"

Expression::Expression() {
	operatorr = base1 = base2 = num1 = num2 = "";
}

//Biểu thức
Expression::Expression(string ope, string bs1, string bs2, string n1, string n2) :
	operatorr(ope), base1(bs1), base2(bs2), num1(n1), num2(n2)
{
}

//Trả về kết quả biểu thức
string Expression::Answer() {
	string output;

	//Cả 2 số là int thì sẽ xử lý theo Qint
	if (isInt(num1) && isInt(num2)) {
		QInt::QInt ans;
		QInt::QInt a, b;

		//Trường hợp đổi giữa các cơ số
		if (base2.size()) {
			if (base1 == "2") {
				if (base2 == "10") {
					ans = QInt::BinToDec(num1);
					output = ans.Print();
				}
				else if (base2 == "16")
					output = QInt::BintoHex(num1);

				else
					output = "Khong ho tro";
			}
			else if (base1 == "10") {
				ans.Scan(num1);
				if (base2 == "2")
					output = QInt::DecToBin(ans);

				else if (base2 == "16")
					output = QInt::DecToHex(ans);

				else
					output = "Khong ho tro";
			}
			else if (base1 == "16") {
				if (base2 == "10") {
					ans = QInt::HexToDec(num1);
					output = ans.Print();
				}
				else if (base2 == "2")
					output = QInt::HexToBin(num1);

				else
					output = "Khong ho tro";
			}
		}

		//Trường hợp là 1 phép tính giữa 2 số
		else {
			//Nhập 2 số vào a b
			if (base1 == "10") {
				a.Scan(num1);
				b.Scan(num2);
			}
			else if (base1 == "2") {
				a = QInt::BinToDec(num1);
				b = QInt::BinToDec(num2);
			}
			else if (base1 == "16") {
				a = QInt::HexToDec(num1);
				b = QInt::HexToDec(num2);
			}


			if (operatorr == "+")
				ans = a + b;
			else if (operatorr == "-")
				ans = a - b;
			else if (operatorr == "*")
				ans = a*b;
			else if (operatorr == "/")
				if (b != ans)
					ans = a / b;
				else
					return "NaN";
			else if (operatorr == ">>") {
				//Lúc đầu ans == 0, lặp tới khi b = 0
				while (b != ans) {
					a.shiftRight();
					b--;
				}
				ans = a;
			}
			else if (operatorr == "<<") {
				while (b != ans) {
					a.shiftLeft();
					b--;
				}
				ans = a;
			}
			else if (operatorr == "~")
				ans = ~a;

			else if (operatorr == "^")
				ans = a^b;

			else if (operatorr == "&")
				ans = a&b;

			else if (operatorr == "|")
				ans = a | b;


			//Chuyển kết quả về đúng hệ
			if (base1 == "10")
				output = ans.Print();
			else if (base1 == "2")
				output = QInt::DecToBin(ans);
			else if (base1 == "16")
				output = QInt::DecToHex(ans);
		}
	}

	//1 trong 2 số là float => tính theo float
	else {
		Qfloat::Qfloat a, b, ans;

		//Chuyển đổi cơ số
		if (base2.size()) {
			//Chuyển từ 2 sang 10
			if (base1 == "2") {
				ans = Qfloat::Qfloat::BinToDec(num1);
				output = ans.Print();
			}
			//Chuyển từ hệ 10 sang 2
			else {
				ans.Scan(num1);
				output = Qfloat::Qfloat::DecToBin(ans);
				return output;
			}
		}

		//Thực hiện phép tính
		else {
			if (base1 == "10") {
				a.Scan(num1);
				b.Scan(num2);
			}
			else {
				a = Qfloat::Qfloat::BinToDec(num1);
				b = Qfloat::Qfloat::BinToDec(num2);
			}

			if (operatorr == "+")
				ans = a + b;
			else if (operatorr == "-")
				ans = a - b;
			else if (operatorr == "*")
				ans = a*b;
			else if (operatorr == "/")
				ans = a / b;

			if (base1 == "10")
				output = ans.Print();
			else
				output = Qfloat::Qfloat::DecToBin(ans);
		}
	}

	return Simplify(output);
}

//Nhập và xuất trên file
void Compute(ifstream &ifs, ofstream &ofs) {
	int i, j, n;
	string buffer;
	string bs1, bs2, num1, num2, ope;
	vector<string> vec;
	bool mode; //True nếu đổi hệ, False nếu cộng trừ nhân chia ...
	getline(ifs, buffer);
	n = stoi(buffer);
	for (i = 0; i < n; i++) {
		//buffer.clear();
		mode = true;
		ope.clear();
		bs1.clear();
		bs2.clear();
		num1.clear();
		num2.clear();
		getline(ifs, buffer);

		vec = stringtok(buffer, ' ');

		//Chuyển đổi cơ số, dòng có 3 thành phần
		if (vec.size() == 3) {
			bs1 = vec[0];
			bs2 = vec[1];
			num1 = vec[2];
		}
		//Tính toán, dòng có 4 thành phần
		else if (vec.size() == 4) {
			bs1 = vec[0];
			num1 = vec[1];
			ope = vec[2];
			num2 = vec[3];
		}
		//Khác, in ra dòng trống
		else {
			ofs << endl;
			continue;
		}
		Expression a(ope, bs1, bs2, num1, num2);
		ofs << Simplify(a.Answer()) << endl;
	}
}

//Kiểm tra xem chuỗi s có phải số nguyên không 
bool isInt(string s) {
	int n = s.size();
	for (int i = 0; i < n; i++)
		if (s[i] == '.')
			return false;
	return true;
}

//In ra menu với các option truyền vào
string Menu(vector<string> option) {
	int n = option.size();
	string ans;
	for (int i = 0; i < n; i++)
		cout << option[i] << endl;
	cout << "Ban chon: ";
	cin >> ans;
	return ans;
}

//Tính toán khi không dùng tham số dòng lệnh
void ComputeMenus() {
	string choice;
	string num1, num2;
	string bs1, bs2;
	string ope;
start: choice = Menu({ "1. Chuyen doi co so","2. Tinh toan" });
	if (choice == "1") {
		cout << "Nhap he so ban nhap va he so can chuyen sang: ";
		cin >> bs1 >> bs2;
		cout << "Nhap so can chuyen: ";
		cin >> num1;
		Expression exp("", bs1, bs2, num1, "");
		cout << "Ket qua la: " << exp.Answer() << endl << endl;
		cout << "Ban muon tinh tiep? (Y/N) ";
		cin >> choice;
		if (choice == "Y" || choice == "y")
			goto start;
	}
	else if (choice == "2") {
		cout << "Nhap he so: ";
		cin >> bs1;
		cout << "Nhap phep tinh: ";
		cin >> num1 >> ope >> num2;
		Expression exp(ope, bs1, "", num1, num2);
		cout << "Ket qua la: " << exp.Answer() << endl << endl;
		cout << "Ban muon tinh tiep? (Y/N) ";
		cin >> choice;
		if (choice == "Y" || choice == "y")
			goto start;
	}

}

//Xóa số 0 đầu chuỗi
string Simplify(string& s) {
	int n = s.size();
	bool neg = false;
	if (s[0] == '-') {
		neg = true;
		s = s.substr(1);
	}
	while (s[0] == '0' && s != "0" && s[1] != '.')
		s = s.substr(1);

	if (neg)
		s.insert(s.begin(), '-');
	return s;
}

//Tách chuỗi theo kí tự t, bỏ đi t
vector<string> stringtok(string s, char t) {
	int n = s.size();
	int i = 0;
	string tmp;
	vector<string> ans;
	s.push_back(t);
	while (i < n) {
		while (s[i] != t)
			tmp.push_back(s[i++]);
		i++;
		if (tmp[0] != 0) {
			ans.push_back(tmp);
			tmp.clear();
		}
	}
	return ans;
}
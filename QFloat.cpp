#include <vector>
#include <string>
#include "QFloat.h"
using namespace std;


namespace Qfloat {
	//Chuẩn hóa chuỗi - bỏ số 0 dư
	std::string NormalizeString(std::string&);
	//Lấy phần nguyên của chuỗi
	std::string SubString1(std::string);
	//Lấy phần thập phân của chuỗi
	std::string SubString2(std::string);
	//Cộng 2 chuỗi số thực
	std::string AddString(std::string, std::string);
	//Trừ 2 chuỗi số thực
	std::string SubtractString(std::string, std::string);
	//std::string NormalizeNum(std::string); //Chuẩn hóa số dạng nhị phân

	//Chia phần nguyên và chia phần thập phân riêng
	std::string DivideString2(std::string);
	//Mod chuỗi cho 2
	bool ModString2(std::string);
	//Nhân chuỗi cho 2
	string MultiplyStr2(string s);
	//Kiểm tra chuỗi thực bằng không
	//True nếu bằng không
	bool isStrZero(std::string);
	//Chuỗi trái lớn hơn hoặc bằng chuỗi phải  -> True
	bool isLargerOrEqual(std::string, std::string);
	//Thêm 0 vào đầu cuối cho 2 chuỗi bằng nhau, đúng vị trí
	void Padding(std::string&, std::string&);

	//Cộng 2 dãy bit
	std::vector <bool> addBit(std::vector <bool>, std::vector <bool>, bool &carry);
	//Trừ 2 dãy bit
	std::vector <bool> subBit(std::vector <bool>, std::vector <bool>, bool &carry);
	//so sánh 2 dãy bit
	bool compBit(std::vector <bool>, std::vector <bool>);



	string MultiplyStr2(string s) {
		string ans;
		int n = s.size();
		int b = 0;
		for (int i = n - 1; i >= 0; i--) {
			int t = (s[i] - '0') * 2 + b;

			if (t / 10 > 0)
				b = 1;
			else
				b = 0;

			t = t % 10;

			ans.push_back(t + '0');
		}

		if (b)
			ans.push_back('1');

		n = ans.size();
		for (int i = 0; i < n / 2; i++) {
			char temp = ans[i];
			ans[i] = ans[n - i - 1];
			ans[n - i - 1] = temp;
		}

		return ans;
	}


	Qfloat::Qfloat() {
		for (int i = 0; i < 4; i++)
			a[i] = 0;
	}

	Qfloat::Qfloat(string s) {
		for (int i = 0; i < 4; i++)
			a[i] = 0;

		NormalizeString(s);
		if (s[0] == '-') {
			SetBitI(0, true);
			s = s.substr(1);
		}
		//Lấy phần nguyên
		string nguyen = SubString1(s);
		vector<bool> bitNguyen;

		while (!isStrZero(nguyen)) {
			bitNguyen.push_back(ModString2(nguyen)); //Thứ tự bị ngược
			nguyen = DivideString2(nguyen);
		}


		//Lấy phần thập phân
		string tphan = SubString2(s);
		string temp = ".5";
		vector<bool> bitTP;
		int i = 114; //Số bit tối đa

		while (i > 0 && !isStrZero(tphan)) {
			if (isLargerOrEqual(tphan, temp)) {
				bitTP.push_back(true); //Thứ tự thuận
				tphan = SubtractString(tphan, temp);
			}
			else
				bitTP.push_back(false);
			temp = DivideString2(temp);
			i--;
		}

		//Tìm exp => mantissa
		int32_t exp;
		//Phần nguyên = 0
		if (bitNguyen.size() == 0)
			//Số 0
			if (bitTP.size() == 0)
				return;
		//Trường hợp 0.000xxx
			else
			{
				int j = 1;
				while (bitTP[j - 1] == 0)
					j++;
				exp = -j;
			}
		//Phần nguyên khác không
		else
			exp = bitNguyen.size() - 1;

		//Điền giá trị exp
		//Trường hợp số quá nhỏ -> số dạng ko chuẩn. Số lớn -> NaN? nên ko quan tâm?

		exp = exp + 0x00004000 - 1; // Cộng với 2^14 (bias) trừ 1
		i = 15;

		if (exp <= 0)
			for (int i = 1; i < 16; i++)
				SetBitI(i, false);

		int exp1 = exp;
		while (exp > 0) {
			SetBitI(i, exp & 1);
			exp = exp >> 1;
			i--;
		}



		//Điền Mantissa
		int n = bitNguyen.size(), m = bitTP.size();

		//Tìm bit số 1 đầu tiên trong dãy nhị phân
		if (n > 0) {
			bitNguyen.pop_back();
			n--;
		}
		else {
			while (m > 0) {
				if (!bitTP[0]) {
					bitTP.erase(bitTP.begin());
					m--;
				}
				else
					break;
			}
			//Tìm thấy số 1 đầu tiên, xóa nó luôn
			bitTP.erase(bitTP.begin());
			m--;
		}

		//Nếu là số dạng ko chuẩn, điền số 1 lại, thêm số 0 lại cho đủ :v
		if (exp1 <= 0) {
			exp1++;
			bitTP.insert(bitTP.begin(), true);
			m++;
			while (exp1 <= 0) {
				exp1++;
				bitTP.insert(bitTP.begin(), false);
				m++;
			}
		}

		//Điền
		i = 16;
		while (n > 0 && i < 128) {
			SetBitI(i, bitNguyen[n - 1]);
			bitNguyen.pop_back();
			n--;
			i++;
		}
		while (m > 0 && i < 128) {
			SetBitI(i, bitTP[0]);
			bitTP.erase(bitTP.begin());
			m--;
			i++;
		}
	}

	Qfloat::Qfloat(const Qfloat &x) {
		for (int i = 0; i < 4; i++)
			a[i] = x.a[i];
	}

	Qfloat& Qfloat::operator=(const Qfloat &x) {
		for (int i = 0; i < 4; i++)
			a[i] = x.a[i];
		return *this;
	}

	bool Qfloat::operator==(const Qfloat &x)
	{
		return (a[0] == x.a[0]) && (a[1] == x.a[1]) && (a[2] == x.a[2]) && (a[3] == x.a[3]);
	}

	void Qfloat::Scan(string s) {
		if (s[0] == 0)
			cin >> s;
		Qfloat tmp(s);
		*this = tmp;
	}

	string Qfloat::Print(bool print_console) {
		//Tính ra phần mũ
		int exp = 0;
		int base = 1;
		int i;
		int spec = 0; //Số 0, NaN, Inf
					  //Spec: 0: số không chuẩn và chuẩn, 1: số không, 2: NaN, 3: Inf
		bool subnormal = false; //Số không chuẩn
		string ans = ".";

		//Kiểm tra số 0, NaN hay Inf
		bool tmp = GetBitI(1);
		for (i = 2; i < 16; i++) {
			if (GetBitI(i) != tmp)
				break;
		}
		if (i == 16) {
			if (tmp) //Toàn bit 1 ở phần exp
			{
				spec = 3; //Inf
				for (; i < 128; i++)
					if (GetBitI(i)) {
						//Nếu có bit ở phần trị khác không thì không phải Inf
						spec = 2;
						break;
					}
			}
			else //Toàn bit 0 
			{
				spec = 1;
				for (; i < 128; i++)
					if (GetBitI(i)) {
						spec = 0;
						break;
					}
			}
		}

		else spec = 0;



		if (spec == 0) { //Khác không


			for (i = 0; i < 15; i++) {
				exp += base * GetBitI(15 - i);
				base *= 2;
			}

			if (exp == 0) //xử lý cho số không chuẩn
				subnormal = true;


			exp += 1 - 0x00004000; //Trừ lại cho bias



								   //Chuyển về dạng nhị phân mà không có nhân 2 mũ
			vector<bool> nguyen, tphan;
			//Luôn có số 1 ở phần nguyên nếu là số chuẩn
			if (!subnormal)
				nguyen.push_back(true);

			i = 16;
			if (exp >= 0) {
				while (exp > 0) {
					nguyen.push_back(GetBitI(i));
					i++;
					exp--;
				}
				while (i < 128) {
					tphan.push_back(GetBitI(i));
					i++;
				}
			}
			else {
				//Đưa số 1 ở phần nguyên vào sau dấu chấm hoặc số 0 nếu là số ko chuẩn
				nguyen.clear();
				tphan.push_back(!subnormal);
				exp++;
				//Thêm các số 0 vào 
				while (exp < 0) {
					tphan.insert(tphan.begin(), false);
					exp++;
				}
				//Đưa các số phần thập phân vào
				while (i < 128) {
					tphan.push_back(GetBitI(i));
					i++;
				}
			}

			string ngn = "1";
			//Phần thập phân, bắt đầu từ 0.5, tìm tới đâu chia đôi tới đó
			string tp = ".5";

			int n = nguyen.size(), m = tphan.size();
			i = 0;

			//Cộng các lũy thừa dương của 2 vào phần nguyên
			while (i < n) {
				if (nguyen[n - 1 - i])
					ans = AddString(ans, ngn);
				ngn = MultiplyStr2(ngn);
				i++;
			}

			i = 0;
			//Cộng các lũy thừa âm của 2 vào phần thập phân
			while (i < m) {
				if (tphan[i])
					ans = AddString(ans, tp);
				tp = DivideString2(tp);
				i++;
			}

			//chèn thêm 0 vào trước các số chưa có phần nguyên
			if (ans[0] == '.')
				ans.insert(ans.begin(), '0');

			//Nếu là số âm
			if (GetBitI(0)) {
				if (print_console)
					cout << '-';
				ans.insert(ans.begin(), '-');
			}
		}
		else {
			switch (spec) {
			case 1: ans = "0.0"; break;
			case 2: ans = "NaN"; break;
			case 3:
				if (GetBitI(0))
					ans.insert(ans.begin(), '-');
				ans.append("Inf"); break;
			}

		}


		if (print_console)
			cout << ans << endl;
		return ans;
	}

	void Qfloat::PrintBit() {
		for (int i = 0; i <= 127; i++) {
			if (i % 8 == 0 && i > 0)
				cout << ' ';
			cout << GetBitI(i);
		}
		cout << endl;
	}

	bool Qfloat::GetBitI(int i) {
		if (i > 127)
			return false;

		int n = i / 32, m = i % 32;
		return (a[n] >> (31 - m)) & 1;
	}

	void Qfloat::SetBitI(int i, bool val) {
		if (i > 127)
			return;

		int n = i / 32, m = i % 32;
		uint32_t b = 1;

		b <<= (31 - m); // Dịch trái số 1 (31-m) bit
		if (!val)// Nếu bit cần sửa là 0 thì phải đảo hết cả chuỗi bit thành 11...0..111
			b = ~b;

		if (val)
			a[n] = a[n] | b;
		else
			a[n] = a[n] & b;

	}

	string Qfloat::DecToBin(Qfloat x)
	{
		string ans;
		for (int i = 0; i <= 127; i++)
			ans.push_back((char)x.GetBitI(i) + '0');
		return ans;
	}

	Qfloat Qfloat::BinToDec(string bit)
	{
		Qfloat ans;
		int n = bit.size();
		while (n < 128) {
			bit.push_back('0');
			n++;
		}
		for (int i = 0; i <= 127; i++) ans.SetBitI(i, bit[i] - '0');
		return ans;
	}

	Qfloat Qfloat::operator+(Qfloat x)
	{
		Qfloat ans, zero;
		if (x == zero) return *this;
		if (*this == zero) return x;
		//lấy dấu
		bool sign1 = GetBitI(0), sign2 = x.GetBitI(0), sign;
		//lấy phần mũ
		int e1 = 0, e2 = 0, e = 0;
		for (int i = 15; i >= 1; i--)
		{
			e1 += GetBitI(i) * (1 << (15 - i));
			e2 += x.GetBitI(i) * (1 << (15 - i));
		}
		//nếu phần mũ số nào = 0 thì trả về số còn lại
		if (e1 == 0) return x;
		if (e2 == 0) return *this;
		//lấy phần thập phân
		vector <bool> s1, s2, s;
		s1.clear(); s2.clear(); s.clear();
		for (int i = 16; i <= 127; i++)
		{
			s1.push_back(GetBitI(i));
			s2.push_back(x.GetBitI(i));
		}
		//đỗi số có mũ lớn ra trước
		bool change = 0;
		if (e1 < e2)
		{
			bool tsign = sign1; sign1 = sign2; sign2 = tsign;
			int te = e1; e1 = e2; e2 = te;
			vector <bool> ts = s1; s1 = s2; s2 = ts;
			change = 1;
		}
		e = e1;
		if (e1 == e2)
		{
			if (sign1 != sign2)
			{
				bool carry = 0;
				//trừ 2 dãy bit (đã liệt kê tất cả trường hợp và tìm được quy luật :D)
				s = subBit(s1, s2, carry);
				//nếu carry != 0 thì kết quả trừ sẽ là số bù 2 => lấy bù 2 lần nữa để trở về ban đầu.
				if (carry)
				{
					int i;
					//lấy bù 2:
					for (i = 0; i < s.size(); i++) s[i] = !s[i];
					for (i = s.size() - 1; i >= 0; i--)
						if (!s[i])
						{
							s[i] = 1;
							for (int j = i + 1; j < s.size(); j++) s[j] = 0;
							break;
						}
					//shift left đưa dãy bit về dạng chuẩn
					for (i = 0; i < s.size(); i++)
					{
						if (!s[0])
						{
							s.erase(s.begin());
							s.push_back(0);
							e--;
						}
						else
						{
							s.erase(s.begin());
							s.push_back(0);
							e--;
							break;
						}
					}
					//carry != 0 => s1 < s2 => kết quả trái dấu với s1
					sign = !sign1;
				}
				else
				{
					//carry = 0 thì chỉ cần shift left đưa dãy bit về dạng chuẩn
					int i;
					for (i = 0; i < s.size(); i++)
					{
						if (!s[0])
						{
							s.erase(s.begin());
							s.push_back(0);
							e--;
						}
						else break;
					}
					// vì số mũ giống nhau từ đầu và đây là phép trừ nên 2 phần nguyên triệt tiêu nhau
					// => i lớn hơn số bit cho phép của phần thập phân có nghĩa là phần thập phân = 0 => là số 0
					if (i >= s.size()) e = 0;
					else
					{
						//ngược lại thì phần thập phân != 0 và hiện tại đang có dạng 0.1xxxxxx
						//=> shift left 1 lần nữa để đưa về dạng chuẩn
						s.erase(s.begin());
						s.push_back(0);
						e--;
					}
					//carry = 0 => s1 >= s2 => kết quả cùng dấu s1
					sign = sign1;
				}
			}
			else
			{
				//cùng dấu thì cộng 2 số, kết quả cùng dấu cả 2 số
				bool carry = 0;
				s = addBit(s1, s2, carry);
				s.insert(s.begin(), carry);
				s.pop_back();
				e++;
				sign = sign1;
			}
		}
		else
		{
			//chỉnh lại cho phần mũ = nhau
			int d = e1 - e2;
			while (e1 > e2)
			{
				s2.insert(s2.begin(), 0);
				s2.pop_back();
				e2++;
			}
			if (d > 0 && d <= s2.size()) s2[d - 1] = 1; //vì số dạng chuẩn 1.xxxx... shift right d lần
														//thực hiện phép tính
			if (sign1 != sign2)
			{
				//khác dấu thì lấy số trước trừ số sau, kết quả cùng dấu số trước
				bool carry = 0;
				s = subBit(s1, s2, carry);
				//nếu còn nhớ thì bỏ số 1 trong phần nguyên đi, shift left để số 1 trong phần thập phân lên thay và giảm mũ
				if (carry)
				{
					while (s[0] == 0)
					{
						s.erase(s.begin());
						s.push_back(0);
						e--;
					}
					s.erase(s.begin());
					s.push_back(0);
					e--;
				}
				sign = sign1;
			}
			else
			{
				//cùng dấu thì cộng 2 số, kết quả cùng dấu cả 2 số
				bool carry = 0;
				s = addBit(s1, s2, carry);
				//nếu còn nhớ thì số phần nguyên sẽ thành 10.xxxx..., shift right để có dạng chuẩn và tăng mũ
				if (carry)
				{
					s.insert(s.begin(), 0);
					s.pop_back();
					e++;
				}
				sign = sign1;
			}
		}
		//đưa kết quả về biến ans
		ans.SetBitI(0, sign);
		for (int i = 15; i >= 1; i--)
		{
			ans.SetBitI(i, e % 2);
			e /= 2;
		}
		for (int i = 16; i <= 127; i++)
			ans.SetBitI(i, s[i - 16]);
		return ans;
	}

	Qfloat Qfloat::operator-(Qfloat x)
	{
		// a - b = a + (-b)
		x.SetBitI(0, !x.GetBitI(0));
		return *this + x;
	}

	Qfloat Qfloat::operator*(Qfloat x)
	{
		Qfloat zero, ans;
		if (*this == zero || x == zero) return zero;
		//lấy dấu
		bool sign1 = GetBitI(0), sign2 = x.GetBitI(0), sign = sign1 ^ sign2;
		//lấy phần mũ
		int e1 = 0, e2 = 0, e = 0;
		for (int i = 15; i >= 1; i--)
		{
			e1 += GetBitI(i) * (1 << (15 - i));
			e2 += x.GetBitI(i) * (1 << (15 - i));
		}
		//lấy phần thập phân
		vector <bool> s1, s2, s;
		s1.clear(); s2.clear(); s.clear();
		for (int i = 16; i <= 127; i++)
		{
			s1.push_back(GetBitI(i));
			s2.push_back(x.GetBitI(i));
		}
		//đưa về dạng 0.1xxxxxxx
		e1++; e2++;
		s1.insert(s1.begin(), 1);
		s2.insert(s2.begin(), 1);
		//hiện tại phần thập phân có dạng 1xxxxxx => thuật toán booth sẽ nhầm là số âm nên thêm 1 số 0 nữa
		e1++; e2++;
		s1.insert(s1.begin(), 0);
		s2.insert(s2.begin(), 0);
		e = e1 + e2 - 16383;
		//nhân 2 dãy bit
		for (int i = 0; i < s1.size(); i++)
			s.push_back(0);
		bool q = 0; //Q-1
		for (int i = 0; i < s.size(); i++)
		{
			if (q != s1[s1.size() - 1])
			{
				bool t = 0;
				if (q == 0) s = subBit(s, s2, t);
				else s = addBit(s, s2, t);
			}
			q = s1[s1.size() - 1];
			s1.insert(s1.begin(), s[s.size() - 1]);
			s1.pop_back();
			s.insert(s.begin(), s[0]);
			s.pop_back();
		}
		while (!s[0])
		{
			s.erase(s.begin());
			s.push_back(s1[0]);
			s1.erase(s1.begin());
			e--;
		}
		s.erase(s.begin());
		s.push_back(s1[0]);
		e--;
		//số vô cùng
		if (e >= 32767)
		{
			for (int i = 15; i >= 1; i--) ans.SetBitI(i, 1);
			return ans;
		}
		//số 0
		if (e <= 0) return ans;
		//đưa kết quả về biến ans
		ans.SetBitI(0, sign);
		for (int i = 15; i >= 1; i--)
		{
			ans.SetBitI(i, e % 2);
			e /= 2;
		}
		for (int i = 16; i <= 127; i++)
			ans.SetBitI(i, s[i - 16]);
		return ans;
	}

	Qfloat Qfloat::operator/(Qfloat x)
	{
		Qfloat ans, zero;
		if (*this == zero) return ans;
		if (x == zero)
		{
			for (int i = 1; i <= 16; i++)
				ans.SetBitI(i, 1);
			return ans;
		}
		//lấy dấu
		bool sign1 = GetBitI(0), sign2 = x.GetBitI(0), sign = sign1 ^ sign2;
		//lấy phần mũ
		int e1 = 0, e2 = 0, e = 0;
		for (int i = 15; i >= 1; i--)
		{
			e1 += GetBitI(i) * (1 << (15 - i));
			e2 += x.GetBitI(i) * (1 << (15 - i));
		}
		//lấy phần thập phân
		vector <bool> s1, s2, s;
		s1.clear(); s2.clear(); s.clear();
		for (int i = 16; i <= 127; i++)
		{
			s1.push_back(GetBitI(i));
			s2.push_back(x.GetBitI(i));
		}
		s1.insert(s1.begin(), 1);
		s2.insert(s2.begin(), 1);
		e = e1 - e2 + 16383;
		//chia
		bool greater = 0;
		while (s.size() < s2.size())
		{
			if (compBit(s1, s2))
			{
				bool t = 0;
				s.push_back(1);
				if (s1.size() > s2.size()) s1.erase(s1.begin());
				s1 = subBit(s1, s2, t);
			}
			else s.push_back(0);
			s1.push_back(0);
			if (!s1[0]) s1.erase(s1.begin());
		}
		while (!s[0])
		{
			s.erase(s.begin());
			s.push_back(0);
			e--;
		}
		s.erase(s.begin());
		//đưa kết quả về biến ans
		ans.SetBitI(0, sign);
		for (int i = 15; i >= 1; i--)
		{
			ans.SetBitI(i, e % 2);
			e /= 2;
		}
		for (int i = 16; i <= 127; i++)
			ans.SetBitI(i, s[i - 16]);
		return ans;
	}


	//Chuẩn hóa chuỗi - bỏ số 0 dư
	std::string NormalizeString(std::string &s) {
		int i = 0;
		bool neg = false;
		if (s[0] == '-' || s[0] == '+') {
			if (s[0] == '-')
				neg = true;
			i++;
		}

		//Nếu không có dấu chấm thì thêm vào
		bool point = false;
		for (int i = 0; i < s.size(); i++)
			if (s[i] == '.') {
				point = true;
				break;
			}
		if (!point)
			s.push_back('.');

		while (s[i] == '0')		//Bỏ các số 0 đầu
			i++;

		s = s.substr(i);
		i = s.size() - 1;
		while (s[i] == '0') {	//Bỏ các số 0 cuối
			s.pop_back();
			i--;
		}

		if (neg)
			s.insert(s.begin(), '-');


		return s;
	}

	//Lấy phần nguyên của chuỗi
	string SubString1(string s) {
		string ans;
		int i = 0;
		while (s[i] != '.') {
			ans.push_back(s[i]);
			i++;
		}

		return ans;
	}

	//Lấy phần thập phân của chuỗi
	string SubString2(string s) {
		string ans;
		int i = -1;
		while (s[++i] != '.')
			;

		ans = s.substr(i);
		return ans;
	}

	//Cộng 2 chuỗi không âm
	string AddString(string a, string b) {

		Padding(a, b);

		string ans;
		char temp = 0;
		for (int i = a.size() - 1; i >= 0; i--) {
			if (a[i] == '.') {
				ans.insert(ans.begin(), '.');
				continue;
			}

			char val = a[i] - '0' + b[i] - '0' + temp;

			//Nhớ
			if (val >= 10)
				temp = 1;
			else
				temp = 0;

			val %= 10;

			ans.insert(ans.begin(), val + '0');
		}

		if (temp)
			ans.insert(ans.begin(), '1');

		return ans;
	}

	//Mặc định a lớn hơn b (kiểm tra trước khi gọi hàm này)
	//Lấy từ hàm cộng
	string SubtractString(string a, string b) {
		Padding(a, b);

		string ans;
		char temp = 0;
		for (int i = a.size() - 1; i >= 0; i--) {
			if (a[i] == '.') {
				ans.insert(ans.begin(), '.');
				continue;
			}

			char val = a[i] - b[i] - temp;

			//Nhớ
			if (val < 0) {
				val += 10;
				temp = 1;
			}
			else
				temp = 0;

			ans.insert(ans.begin(), val + '0');
		}

		return ans;
	}

	//string NormalizeNum(string s) { return s; }

	//Chia phần nguyên cho số cần đổi, chia thập phân để lấy base của 2
	//Chia nguyên bỏ dư, chia thập phân thì cứ chia tới hết digit :) 
	string DivideString2(string s) {
		string ans;
		int n = s.size();
		char q, t, r = 0;

		//Chuỗi là phần Thập phân
		if (s[0] == '.') {
			ans.push_back('.');
			int i = 1;
			while (i < n || r) { //Chia hết chiều dài hoặc khi còn dư
				if (i < n)
					t = s[i] - '0';
				else
					t = 0;
				q = (10 * r + t) / 2;
				r = t % 2;
				ans.push_back(q + '0');
				i++;
			}
		}

		//Chuỗi là Số nguyên
		else {
			int i = 0;
			while (i < n) {
				t = s[i] - '0';
				q = (10 * r + t) / 2;
				r = t % 2;

				ans.push_back(q + '0');
				i++;
			}
			if (ans[0] == '0')
				ans = ans.substr(1);
		}
		return ans;
	}

	//Chuỗi nguyên
	bool ModString2(string s) {
		int n = s.size();
		return (s[n - 1] - '0') % 2;
	}

	//Chuỗi đã chuẩn hóa
	bool isStrZero(string s) {
		int n = s.size();
		if (n == 0 || (n == 1 && s[0] == '.'))
			return true;
		return false;
	}

	bool isLargerOrEqual(string a, string b) {
		Padding(a, b);
		int n = a.size();
		for (int i = 0; i < n; i++) {
			if (a[i] > b[i])
				return true;
			if (a[i] < b[i])
				return false;
		}
		return true;
	}

	void Padding(string &a, string &b) {
		NormalizeString(a);
		NormalizeString(b);
		int la, lb, la1, lb1;		//Chiều dài chuỗi a, b, chiều dài phần nguyên a, b
		la1 = lb1 = 0;

		la = a.size();
		lb = b.size();

		while (a[la1] != '.')
			la1++;

		while (b[lb1] != '.')
			lb1++;

		//Chiều dài phần TP
		la = la - la1;
		lb = lb - lb1;


		//Thêm 0 vào trước cho chiều dài nguyên bằng nhau
		while (la1 < lb1) {
			a.insert(a.begin(), '0');
			la1++;
		}

		while (lb1 < la1) {
			b.insert(b.begin(), '0');
			lb1++;
		}

		//Thêm 0 vào TP
		while (la < lb) {
			a.push_back('0');
			la++;
		}

		while (lb < la) {
			b.push_back('0');
			lb++;
		}
	}

	void ScanQfloat(Qfloat &x) {
		x.Scan();
	}

	void PrintQfloat(Qfloat x) {
		x.Print();
	}

	vector <bool> addBit(vector <bool> s1, vector <bool> s2, bool &carry)
	{
		vector <bool> s;
		for (int i = s1.size() - 1; i >= 0; i--)
		{
			if (s1[i] == s2[i])
			{
				s.insert(s.begin(), carry);
				carry = s1[i];
			}
			else
			{
				s.insert(s.begin(), !carry);
			}
		}
		return s;
	}

	vector <bool> subBit(vector <bool> s1, vector <bool> s2, bool &carry)
	{
		vector <bool> s;
		for (int i = s1.size() - 1; i >= 0; i--)
		{
			if (s1[i] == s2[i])
			{
				s.insert(s.begin(), carry);
			}
			else
			{
				s.insert(s.begin(), !carry);
				carry = s2[i];
			}
		}
		return s;
	}

	bool compBit(vector <bool> s1, vector <bool> s2)
	{
		if (s1.size() != s2.size()) return s1.size() > s2.size();
		int i;
		for (i = 0; i < s1.size(); i++)
			if (s1[i] != s2[i]) return s1[i];
		return (i == s1.size());
	}

}
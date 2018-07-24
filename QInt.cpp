#include "QInt.h"
#include <vector>
using namespace std;


namespace QInt {

	bool ModString2(string);
	string DivString2(string);
	string AddString(string, string); //Cộng 2 chuỗi dương
	bool isStrZero(string);
	string Simplify(string&);



	QInt::QInt()
	{
		memset(a, 0, sizeof(a));
	}

	QInt::QInt(string s)
	{
		bool iVal = 0, negative = false;
		char pos = 127;
		Simplify(s);
		if (s[0] == '-')
			negative = true;
		while (!isStrZero(s))
		{
			iVal = ModString2(s);
			SetBitI(pos, iVal);
			s = DivString2(s);
			pos--;
		}

		if (negative)
			*this = Complement2();
	}

	QInt::QInt(char* s)
	{
		string s1(s);
		QInt tmp(s1);
		*this = tmp;
	}

	QInt::QInt(const QInt &x)
	{
		for (int i = 0; i < 4; i++)
			a[i] = x.a[i];
	}

	QInt::QInt(int i) {
		int iVal;
		char pos = 127;
		bool neg = i < 0;
		if (neg)
			i = -i;

		while (i) {
			iVal = i & 1;
			SetBitI(pos, iVal);
			i = i >> 1;
			pos--;
		}


		if (neg)
			*this = Complement2();
	}

	QInt& QInt::operator=(const QInt &x) {
		for (int i = 0; i < 4; i++)
			a[i] = x.a[i];
		return *this;
	}

	QInt QInt::Complement2()
	{
		QInt ans(*this);
		int i;
		for (i = 0; i < 4; i++)
			ans.a[i] = ~ans.a[i];

		i = 127;
		while (ans.GetBitI(i))
		{
			i--;
		}

		ans.SetBitI(i++, true);
		for (; i <= 127; i++)
			ans.SetBitI(i, false);

		return ans;
	}

	void QInt::Scan(string s) {
		if (s == "")
			cin >> s;
		QInt tmp(s);
		*this = tmp;
	}

	//Trả về chuỗi là giá trị, nếu tham số là true thì in ra màn hình
	string QInt::Print(bool print_console) {
		string base2[128] = {
			"1",
			"2",
			"4",
			"8",
			"16",
			"32",
			"64",
			"128",
			"256",
			"512",
			"1024",
			"2048",
			"4096",
			"8192",
			"16384",
			"32768",
			"65536",
			"131072",
			"262144",
			"524288",
			"1048576",
			"2097152",
			"4194304",
			"8388608",
			"16777216",
			"33554432",
			"67108864",
			"134217728",
			"268435456",
			"536870912",
			"1073741824",
			"2147483648",
			"4294967296",
			"8589934592",
			"17179869184",
			"34359738368",
			"68719476736",
			"137438953472",
			"274877906944",
			"549755813888",
			"1099511627776",
			"2199023255552",
			"4398046511104",
			"8796093022208",
			"17592186044416",
			"35184372088832",
			"70368744177664",
			"140737488355328",
			"281474976710656",
			"562949953421312",
			"1125899906842624",
			"2251799813685248",
			"4503599627370496",
			"9007199254740992",
			"18014398509481984",
			"36028797018963968",
			"72057594037927936",
			"144115188075855872",
			"288230376151711744",
			"576460752303423488",
			"1152921504606846976",
			"2305843009213693952",
			"4611686018427387904",
			"9223372036854775808",
			"18446744073709551616",
			"36893488147419103232",
			"73786976294838206464",
			"147573952589676412928",
			"295147905179352825856",
			"590295810358705651712",
			"1180591620717411303424",
			"2361183241434822606848",
			"4722366482869645213696",
			"9444732965739290427392",
			"18889465931478580854784",
			"37778931862957161709568",
			"75557863725914323419136",
			"151115727451828646838272",
			"302231454903657293676544",
			"604462909807314587353088",
			"1208925819614629174706176",
			"2417851639229258349412352",
			"4835703278458516698824704",
			"9671406556917033397649408",
			"19342813113834066795298816",
			"38685626227668133590597632",
			"77371252455336267181195264",
			"154742504910672534362390528",
			"309485009821345068724781056",
			"618970019642690137449562112",
			"1237940039285380274899124224",
			"2475880078570760549798248448",
			"4951760157141521099596496896",
			"9903520314283042199192993792",
			"19807040628566084398385987584",
			"39614081257132168796771975168",
			"79228162514264337593543950336",
			"158456325028528675187087900672",
			"316912650057057350374175801344",
			"633825300114114700748351602688",
			"1267650600228229401496703205376",
			"2535301200456458802993406410752",
			"5070602400912917605986812821504",
			"10141204801825835211973625643008",
			"20282409603651670423947251286016",
			"40564819207303340847894502572032",
			"81129638414606681695789005144064",
			"162259276829213363391578010288128",
			"324518553658426726783156020576256",
			"649037107316853453566312041152512",
			"1298074214633706907132624082305024",
			"2596148429267413814265248164610048",
			"5192296858534827628530496329220096",
			"10384593717069655257060992658440192",
			"20769187434139310514121985316880384",
			"41538374868278621028243970633760768",
			"83076749736557242056487941267521536",
			"166153499473114484112975882535043072",
			"332306998946228968225951765070086144",
			"664613997892457936451903530140172288",
			"1329227995784915872903807060280344576",
			"2658455991569831745807614120560689152",
			"5316911983139663491615228241121378304",
			"10633823966279326983230456482242756608",
			"21267647932558653966460912964485513216",
			"42535295865117307932921825928971026432",
			"85070591730234615865843651857942052864",
			"170141183460469231731687303715884105728"
		};

		string ans = "0";
		//Số âm 
		if (GetBitI(0))
		{
			if (print_console)
				cout << "-";

			//Số -2^127
			if (Complement2() == *this) {
				if (print_console)
					cout << base2[127];
				ans = base2[127];
			}
			else
				ans = Complement2().Print();

			ans.insert(ans.begin(), '-');
			return ans;
		}


		//Lấy từng bit, cộng vào kết quả
		for (int i = 127; i > 0; i--)
			if (GetBitI(i))
				ans = AddString(ans, base2[127 - i]);

		if (print_console)
			cout << ans << endl;
		return ans;
	}

	void QInt::PrintBit()
	{
		for (int i = 0; i <= 127; i++)
		{
			if (i % 8 == 0 && i > 0)
				cout << ' ';
			cout << GetBitI(i);
		}
		cout << endl;
	}

	//Gán giá trị bit thứ pos bằng val
	void QInt::SetBitI(char pos, bool val) {
		int n = pos / 32, m = pos % 32;
		uint32_t b = 1;

		b <<= (31 - m); // Dịch trái số 1 (31-m) bit
		if (!val)// Nếu bit cần sửa là 0 thì phải đảo hết cả chuỗi bit thành 11...0..111
			b = ~b;

		if (val)
			a[n] = a[n] | b;
		else
			a[n] = a[n] & b;
	}

	//Lấy giá trị bit thứ pos
	bool QInt::GetBitI(char pos)
	{
		int n = pos / 32, m = pos % 32;
		return (a[n] >> (31 - m)) & 1;
	}

	//So sánh từng bit
	bool QInt::operator==(QInt x)
	{
		for (int i = 0; i < END; i++)
		{
			if (x.GetBitI(i) != this->GetBitI(i))
				return false;
		}
		return true;
	}

	//Khác = NOT bằng
	bool QInt::operator!=(QInt x) {
		return !(*this == x);
	}

	QInt& QInt::operator++() {
		QInt t("1");
		*this = *this + t;
		return *this;
	}

	QInt& QInt::operator--() {
		QInt t("1");
		*this = *this - t;
		return *this;
	}

	QInt QInt::operator--(int) {
		QInt t;
		t = *this;
		--*this;
		return t;
	}

	QInt QInt::operator++(int) {
		QInt t;
		t = *this;
		++*this;
		return t;
	}

	//Hàm cộng 2 chuỗi thập phân
	//Để độ dài bằng nhau rồi cộng
	string AddString(string a, string b)
	{
		string c;
		int na, nb, i;
		na = a.size();
		nb = b.size();
		int add = 0;			//Phần nhớ

								//Số "ngắn" hơn là a, chèn thêm 0 vào phía trước
		if (na > nb)
		{
			swap(a, b);
			swap(na, nb);
		}

		for (i = 0; i < nb - na; i++)
			a.insert(a.begin(), '0');

		//Cộng có nhớ 2 số dương
		for (i = nb - 1; i >= 0; i--)
		{
			int t = b[i] - '0' + a[i] - '0' + add;

			if (t / 10)
				add = 1;
			else
				add = 0;

			t %= 10;
			c.insert(c.begin(), t + '0');
		}

		if (add)
			c.insert(c.begin(), '1');

		return c;
	}


	void ScanQInt(QInt& x) {
		x.Scan();
	}

	void PrintQInt(QInt x) {
		x.Print();
	}


	//Chia nguyên chuỗi s cho 2
	string DivString2(string s) {
		string ans;
		bool neg = false;
		if (s[0] == '-') {
			s = s.substr(1);
			neg = true;
		}

		int i = 0, t, r;
		int n = s.size();
		char q;
		r = 0;
		for (; i < n; i++) {
			t = s[i] - '0' + 10 * r;
			q = t / 2 + '0';
			r = t % 2;
			ans.push_back(q);
		}
		if (ans[0] == '0' && ans.size() != 1)
			ans = ans.substr(1);

		if (neg)
			ans.insert(ans.begin(), '-');
		return ans;
	}

	//Lấy s mod 2
	bool ModString2(string s) {
		return (s[s.size() - 1] - '0') % 2;
	}

	//Kiểm tra s có bằng không hay không
	//True nếu bằng không
	bool isStrZero(string s) {
		return (s == "-0") || (s == "0");
	}

	//Đơn giản chuỗi, xóa bớt số 0
	string Simplify(string &s) {
		int n = s.size();
		int i = 0;
		bool neg = false;
		if (s[0] == '+')
			i++;
		else if (s[0] == '-')
			neg = true, i++;
		while (i < n && s[i] == '0')
			i++;

		if (i < n)
		{
			s = s.substr(i);
			if (neg)
				s.insert(s.begin(), '-');
		}
		else
		{
			s.clear();
			s.push_back('0');
		}
		return s;
	}

	//Dịch trái toàn bộ bit (bit cuối bằng 0)
	void QInt::shiftLeft()
	{
		for (int i = 0; i <= END - 2; i++)
		{
			this->SetBitI(i, GetBitI(i + 1));
		}
		this->SetBitI(END - 1, FIRST);
	}

	//Dịch phải toàn bộ bit, giữ bit dấu
	void QInt::shiftRight()
	{
		bool check = GetBitI(FIRST);
		for (int i = END - 1; i >= 1; i--)
		{
			this->SetBitI(i, GetBitI(i - 1));
		}
		SetBitI(0, check);
	}

	//And trên kiểu QInt
	QInt QInt::operator & (QInt x)
	{
		QInt ans;
		QInt a(*this), b(x);
		bool temp = 0;
		for (int i = 0; i < END; i++)
		{
			temp = a.GetBitI(i) & b.GetBitI(i);
			ans.SetBitI(i, temp);
		}
		return ans;
	}

	//Or trên kiểu QInt
	QInt QInt::operator | (QInt x)
	{
		QInt ans;
		QInt a(*this), b(x);
		bool temp = 0;
		for (int i = 0; i < END; i++)
		{
			temp = a.GetBitI(i) | b.GetBitI(i);
			ans.SetBitI(i, temp);
		}
		return ans;
	}

	//XOR trên kiểu QInt
	QInt QInt::operator ^ (QInt x)
	{
		QInt ans;
		QInt a(*this), b(x);
		bool temp = 0;
		for (int i = 0; i < END; i++)
		{
			temp = a.GetBitI(i) ^ b.GetBitI(i);
			ans.SetBitI(i, temp);
		}
		return ans;
	}

	//NOT trên QInt
	QInt QInt::operator~ ()
	{
		QInt ans;
		for (int i = 0; i < END; i++)
		{
			ans.SetBitI(i, !GetBitI(i));
		}
		return ans;
	}

	//Lấy trị tuyệt đối
	QInt QInt::returnPosNumber()
	{
		if (!GetBitI(FIRST))
		{
			return *this;
		}
		else
		{
			return this->oppositeNumber();
		}
	}

	//Lấy số đối
	QInt QInt::oppositeNumber()
	{
		if (!GetBitI(FIRST))
		{
			return this->Complement2();
		}
		else
		{
			QInt temp(1);
			temp = (*this) - temp;
			for (int i = 0; i < 4; i++)
			{
				temp.a[i] = ~temp.a[i];
			}
			return temp;
		}

	}

	//Trả biểu diễn nhị phân từ QInt
	string DecToBin(QInt x)
	{
		string ans;

		for (int i = 127; i >= 0; i--)
		{
			ans.insert(ans.begin(), x.GetBitI(i) + '0');
		}
		return ans;
	}

	//Từ chuỗi nhị phân -> QInt
	QInt BinToDec(string bit)
	{
	
		int size = bit.size();
		QInt ans;
		int pos = 128 - size;
		for (int i=0;i<bit.size();i++)
		{
			if (bit[i] != '0' && bit[i] != '1')
			{
				return ans;
			}
		}
		for (int i = 127; i >= pos; i--)
		{
			ans.SetBitI(i, (bit[size - (128 - i)] - '0') > 0);
		}
		return ans;
	}

	//Input: chuỗi nhị phân -> Output chuỗi Hexa
	string BintoHex(string bit)
	{
		string ans;
		int size = bit.size();
		int m = size % 4;
		for (int i = 0; i<bit.size(); i++)
		{
			if (bit[i] != '0' && bit[i] != '1')
			{
				ans = "0";
				return ans;
			}
		}
		for (int i = 1; i <= m; i++)
		{
			bit.insert(bit.begin(), '0');
		}
		for (int i = 0; i <= bit.size() - 4; i += 4)
		{
			string temp = bit.substr(i, 4);
			if (temp == "0000") ans += "0";
			else if (temp == "0001") ans += "1";
			else if (temp == "0010") ans += "2";
			else if (temp == "0011") ans += "3";
			else if (temp == "0100") ans += "4";
			else if (temp == "0101") ans += "5";
			else if (temp == "0110") ans += "6";
			else if (temp == "0111") ans += "7";
			else if (temp == "1000") ans += "8";
			else if (temp == "1001") ans += "9";
			else if (temp == "1010") ans += "A";
			else if (temp == "1011") ans += "B";
			else if (temp == "1100") ans += "C";
			else if (temp == "1101") ans += "D";
			else if (temp == "1110") ans += "E";
			else if (temp == "1111") ans += "F";
		}
		return ans;
	}

	//Trả về biểu diễn Hexa
	string DecToHex(QInt x)
	{
		string temp;
		for (int i = 127; i >= 0; i--)
		{
			temp.insert(temp.begin(), x.GetBitI(i) + '0');
		}
		string ans = BintoHex(temp);
		return ans;
	}


	QInt QInt::operator+ (QInt a)
	{
		QInt c;
		int add = 0;
		for (int i = 127; i >= 0; i--)
		{
			int t = a.GetBitI(i) + GetBitI(i) + add;

			if (t / 2)
				add = 1;
			else
				add = 0;

			t %= 2;
			//if(t)
			c.SetBitI(i, t);
		}

		return c;
	}

	//Từ chuỗi Hexa lưu vào QInt
	QInt HexToDec(string hex)
	{
		string temp = HexToBin(hex);
		return BinToDec(temp);
	}

	//Chuỗi hexa -> chuỗi bit
	string HexToBin(string hex)
	{
		string ans;
		for (int i = 0; i < hex.size(); i++)
		{
			if (hex[i] == '0') ans += "0000";
			else if (hex[i] == '1') ans += "0001";
			else if (hex[i] == '2') ans += "0010";
			else if (hex[i] == '3') ans += "0011";
			else if (hex[i] == '4') ans += "0100";
			else if (hex[i] == '5') ans += "0101";
			else if (hex[i] == '6') ans += "0110";
			else if (hex[i] == '7') ans += "0111";
			else if (hex[i] == '8') ans += "1000";
			else if (hex[i] == '9') ans += "1001";
			else if (hex[i] == 'A') ans += "1010";
			else if (hex[i] == 'B') ans += "1011";
			else if (hex[i] == 'C') ans += "1100";
			else if (hex[i] == 'D') ans += "1101";
			else if (hex[i] == 'E') ans += "1110";
			else if (hex[i] == 'F') ans += "1111";
		}
		return ans;
	}

	QInt QInt::operator-(QInt a)
	{
		QInt checkNull;
		if (a == checkNull)
			return *this;
		a = a.Complement2();
		return a + *this;
	}

	QInt QInt::operator * (QInt x)
	{
		// a=Q, b=M
		QInt a, b;
		a = *this;
		b = x;

		QInt tempA;	//A
		if (a == tempA || b == tempA)
			return tempA;
		bool check = 0;  // Q-1;
		for (int i = 1; i <= END; i++) //128 buoc
		{
			if (check != a.GetBitI(END - 1))
			{
				if (a.GetBitI(END - 1) && check == 0)//TH 1-0
				{
					tempA = tempA - b;
				}
				else //TH 0 1
				{
					tempA = tempA + b;
				}
			}
			check = a.GetBitI(END - 1);
			a.shiftRight();
			a.SetBitI(FIRST, tempA.GetBitI(END - 1));
			tempA.shiftRight();
		}
		return a;
	}

	QInt QInt::operator/ (QInt x)
	{
		// a=Q, b=M
		QInt checkNULL;
		QInt a, b;
		a = *this;
		b = x;
		if (b == checkNULL || a == checkNULL) // kiểm tra nếu số bị chia là 0
		{
			return checkNULL;
		}
		bool check = 0; // check nếu có 2 số trái dấu
		if (a.GetBitI(FIRST) != b.GetBitI(FIRST))
		{
			check = 1;
		}
		a = a.returnPosNumber();
		b = b.returnPosNumber();
		QInt temp;	//A
		for (int i = 1; i <= END; i++) //128 buoc
		{
			temp.shiftLeft();
			temp.SetBitI(END - 1, a.GetBitI(FIRST));
			a.shiftLeft();
			temp = temp - b;
			if (temp.GetBitI(FIRST)) //tempA < 0
			{
				a.SetBitI(END - 1, FIRST);
				temp = temp + b;
			}
			else
			{
				a.SetBitI(END - 1, 1);
			}
		}
		if (check)
			a = a.oppositeNumber();
		return a;
	}
}
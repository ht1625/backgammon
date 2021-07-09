/******************************************************************************
HÜSNİYE TANŞA 1306170039  BİLGİSAYAR MÜHENDİSLİĞİ İÜC
******************************************************************************/

#include <string>
#include <vector>
#include <map>
#include  <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>
using namespace std;
#include <stdio.h> 
#include <stdlib.h>   
#include <time.h>
#include <random>
#include <ctime>

//biri kazanınca kapanacak olan kapı
bool game_over = true;
//burada da verilen stringden adresi bulmak daha kolay olsun diye
string places[24];
//board umuz local olmalı ki iki kullanıcı da aynı yerden ulaşsın
int board[24][2];

//fonksiyon bildirimi
void broke_stamp_x();
void broke_stamp_y();

//burdaki de oldgame datası
vector<string> result;

//önce dosyayı açmak istedim
ofstream dat_file("C:\\Users\\tnshu\\source\\repos\\ConsoleApplication1\\Data.txt", ios::in | ios::out | ios::app);
ofstream FHandle("C:\\Users\\tnshu\\source\\repos\\ConsoleApplication1\\Table.txt", ios::out | ios::app);

int random_zar() {
	srand((unsigned int)time(NULL));
	return   (rand() % 6) + 1;
}

class player {
public:
	int line;//sayısına göre kimin nerde olacağı ve ilk kimin başlayacağı durumu
	char name;
	string direction;//plus mı minus mı olacağı
	int who;//buda onun bi nevi kimliği
	//aşağıdakiler gelen harekete göre gideceği yerler
	int go_dice;
	int now_loc;
	//toplanan pul sayısı
	int col_stamp = 0;
	//kırılan pul sayısı
	int broke_stamp = 0;
	//toplama time geldi
	bool door_col = false;
	player() {};
	void define_player(int ln, char nm) {
		this->line = ln;
		this->name = nm;
		this->findyon();
	}
	void findyon() {
		if (this->line == 1) {
			this->direction = "plus";
			this->put_stamp_plus();
		}
		else {
			this->direction = "minus";
			this->put_stamp_minus();
		}
	}//eğer birinci ise plus dır yani sağ altta pulları toplayacak
	void put_stamp_plus() {
		this->who = int(this->name);
		board[0][1] = this->who;
		board[11][1] = this->who;
		board[16][1] = this->who;
		board[18][1] = this->who;
	}//eğer ikinci ise minus dır yani sağ üstte pulları toplayacak
	void put_stamp_minus() {
		this->who = int(this->name);
		board[5][1] = this->who;
		board[7][1] = this->who;
		board[12][1] = this->who;
		board[23][1] = this->who;
	}
	bool check_place(int dice1, string place) {
		bool klm = false;
		for (int i = 0; i < 24; i++) {
			if (places[i] == place) {//yanlış adres kontrolü burda gerçekleşiyor
				this->go_dice = dice1;
				this->now_loc = i;
				klm = this->go_goto_next();//buraya girebiliyorsa adres doğrudur daha sonra kontroller
			}
		}
		return klm;//burdan eğer false gelirse hem yanlış adres yazmış yada dolu yer olabilir
	}
	//kırığı olan oyuncunun giriş kontrolü
	bool control_broken_stamp(int dic1) {
		if (this->direction == "plus") {
			return this->for_control_broke(dic1 - 1);
		}
		else {
			return this->for_control_broke(24 - dic1);
		}
	}
	bool for_control_broke(int val) {
		if (board[val][0] == 0 || board[val][1] == this->who) {
			board[val][0]++;
			board[val][1] = this->who;
			this->broke_stamp--;
			cout << "Enter your broken stamp\n";
			return true;
		}
		else if (board[val][0] == 1 && board[val][1] != this->who) {
			board[val][0]++;
			board[val][1] = this->who;
			this->broke_stamp--;
			this->broke_stamp_rival();
			cout << "Enter your broken stamp\n";
			return true;
		}
		cout << "Dont enter your broken stamp for your dice\n";
		return false;
	}
	bool go_goto_next() {
		//burda da gideceği adrese gidebiliyorsa gitsin sonrada gittikten sonra olacak şeyler kontrol edilecek
		if (this->direction == "plus") {
			if (board[this->now_loc][0] > 0 && board[this->now_loc][1] == this->who) {
				
				if (this->door_col && this->now_loc+this->go_dice >=24) {
					//burada toplama yapıcak fonksiyona gitmeli
					this->collect_stamp_forplus();				
				}
				else {//buraya girebiliyorsa demek ki ilerletme adımı çok değil
					if (board[this->now_loc + this->go_dice][1] == this->who || board[this->now_loc + this->go_dice][1] == 0) {
						//burada sadece artırma işlemi yapılacak olan fonksiyona gitsin
						this->carry_stamp(this->now_loc + this->go_dice);
					}
					else if (board[this->now_loc + this->go_dice][0] == 1 && board[this->now_loc + this->go_dice][1] != this->who) {
						//kırma işlemi yapıcak fonksiyon
						this->broke_stampp(this->now_loc - this->go_dice);
					}
					else {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
		else {
			if (board[now_loc][0] > 0 && board[now_loc][1] == this->who) {
				if (this->door_col && this->now_loc-this->go_dice < 0) {
					//burada toplama yapıcak fonksiyona gitmeli
					this->collect_stamp_forminus();
				}
				else {//buraya girebiliyorsa demek ki ilerletme adımı çok değil
					if (board[this->now_loc - this->go_dice][1] == this->who || board[this->now_loc - this->go_dice][1] == 0) {
						//burada çıkartma işlemi yapılacak olan fonksiyona gitsin
						this->carry_stamp(this->now_loc - this->go_dice);
					}
					else if (board[this->now_loc - this->go_dice][0] == 1 && board[this->now_loc + this->go_dice][1] != this->who) {
						//kırma işlemi yapıcak fonksiyon
						this->broke_stampp(this->now_loc - this->go_dice);
					}
					else {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
		return true;
	}
	//burda sadece toplama zamanının olup olmadığını kontrol ediyor for plus
	bool plus_pick_stamp() {
		for (int i = 0; i < 18; i++) {
			if (board[i][1] == this->who && board[i][0] > 0) {
				return false;
			}
		}
		return true;
	}
	//burda sadece toplama zamanının olup olmadığını kontrol ediyor for minus
	bool minus_pick_stamp() {
		for (int i = 23; i > 5; i--) {
			if (board[i][1] == this->who && board[i][0] != 0) {
				return false;
			}
		}
		return true;
	}
	//burda sadece taşıma işlemi yapıcak
	void carry_stamp(int val) {
		board[this->now_loc][0]--;
		if (board[this->now_loc][0] == 0) {
			board[this->now_loc][1] = 0;
		}
		board[val][0]++;
		if (board[val][1] == 0) {
			board[val][1] = this->who;
		}
	}
	//burda da tüm pulları toplama bölgesinde olduğu için artık toplayabilir for plus
	void collect_stamp_forplus() {
		if (board[24 - this->go_dice][0] > 0 && board[24 - this->go_dice][1] == this->who) {
			this->col_stamp++;
			board[24 - this->go_dice][0]--;
			if (board[24 - this->go_dice][0] == 0) {
				board[24 - this->go_dice][1] = 0;
			}
		}
		else {
			for (int i = 6; i > 0; i--) {
				if (board[24 - i][0] > 0 && board[24 - i][1] == this->who) {
					this->col_stamp++;
					board[24 - i][0]--;
					if (board[24 - i][0] == 0) {
						board[24 - i][1] = 0;
					}
					break;
				}
			}
		}
	}

	//burda da tüm pulları toplama bölgesinde olduğu için artık toplayabilir for minus
	void collect_stamp_forminus() {
		if (board[this->go_dice-1][0] > 0 && board[this->go_dice-1][1] == this->who) {
			this->col_stamp++;
			board[this->go_dice-1][0]--;
			if (board[this->go_dice-1][0] == 0) {
				board[this->go_dice-1][1] = 0;
			}
		}
		else {
			for (int i = 6; i > 0; i--) {
				if (board[i-1][0] > 0 && board[i-1][1] == this->who) {
					this->col_stamp++;
					board[i-1][0]--;
					if (board[i-1][0] == 0) {
						board[i-1][1] = 0;
					}
				}
			}
		}
	}

	//burada da kırma işlemi yapılacak for plus
	void broke_stampp(int val) {
		board[val][1] = this->who;
		board[this->now_loc][0]--;
		if (board[this->now_loc][0] == 0) {
			board[this->now_loc][1] = 0;
		}
		this->broke_stamp_rival();//rakip kırmasını artırma
	}
	void broke_stamp_rival() {
		if (this->name == 'X') {
			broke_stamp_y();
		}
		else {
			broke_stamp_x();
		}
	}
	//çıkan zarla hareket kontrolü olup olmadığına bakıyoruz
	bool check_movee(int dice) {
		if (this->direction == "plus") {
			if (this->plus_pick_stamp() == true) {
				this->door_col = true;
				return true;
			}
			for (int i = 0; i < 18; i++) {
				if (board[i][0] > 0 && board[i][1] == this->who) {
					if (board[i + dice][0] <= 1) {
						return true;
					}
					else if (board[i + dice][1] == this->who) {
						return true;
					}
				}
			}
		}
		else {
			if (this->minus_pick_stamp() == true) {
				this->door_col = true;
				return true;
			}
			for (int i = 23; i >= 6; i--) {
				if (board[i][0] > 0 && board[i][1] == this->who) {
					if (board[i - dice][0] <= 1) {
						return true;
					}
					else if (board[i - dice][1] == this->who) {
						return true;
					}
				}
			}
		}
		return false;
	}
};/////////////class bitişi

//burda da tahtayı gereken ilk  bilgi iler dolduruyoruz 
void put_stamp() {
	board[0][0] = 2;
	board[23][0] = 2;
	board[16][0] = 3;
	board[7][0] = 3;
	board[5][0] = 5;
	board[11][0] = 5;
	board[12][0] = 5;
	board[18][0] = 5;
}

//int den string dönüşümü yapmak için
string str_fromint(int num) {
	string str;
	stringstream ss;
	ss << num;
	ss >> str;
	return str;
}

//string den int dönüşümü yapmak için
int int_fromstr(string str) {
	int num;
	stringstream ss;
	ss << str;
	ss >> num;
	return num;
}

//burada diziye yerlerin adlarını koyuyorum bulması daha kolay olsun diye
void put_place() {
	string kl = str_fromint(1);
	for (int i = 0; i < 12; i++) {
		places[i] = kl + char(76 - i);
	}
	kl = str_fromint(2);
	for (int i = 12; i < 24; i++) {
		places[i] = kl + char(53 + i);
	}
}

//aşağıdaki de oyuncular
int gamer_X, gamer_Y;
player clgame_x, clgame_y;

//oyuncuların kırma işlemini yapmak için
void broke_stamp_x() {
	clgame_x.broke_stamp++;
}
void broke_stamp_y() {
	clgame_y.broke_stamp++;
}

//kırığı kontrol edip kırıcak
bool do_broken(int dice, char next) {
	if (next == 'X') {
		return  clgame_x.control_broken_stamp(dice);
	}
	else {
		return clgame_y.control_broken_stamp(dice);
	}
}

//burada tabloda gösterme kısmı
void show_backgommon() {


	FHandle << "  ____1A____  ____1B____  ____1C____  ____1D____  ____1E____  ____1F____  ____1G____  ____1H____  ____1I____  ____1J____  ____1K____  ____1L____ \n";
	for (int j = 0; j < 12; j++) {

		if (j == 5) {
			FHandle << " _________||";
		}
		else {
			FHandle << " ___________";
		}
	}
	FHandle << "\n";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 0) {
				if (board[11 - j][0] == 0) {
					FHandle << "|           ";
				}
				else {
					FHandle << "|     " << board[11 - j][0] << char(board[11 - j][1]) << "    ";
				}
				if (j == 11) {
					FHandle << endl;
					for (int k = 0; k < 12; k++) {
						FHandle << "|___________";
					}
				}
			}
			else if (i == 4) {
				 if (board[12 + j][0] == 0) {
					FHandle << "|           ";
				 }
				 else {
					FHandle << "|     " << board[12 + j][0] << char(board[12 + j][1]) << "    ";
				 }
				 if (j == 11) {
					FHandle << endl;
					for (int k = 0; k < 12; k++) {
						FHandle << "|___________";
					}
				 }
			}
			else if (i == 2 && j == 4) {
				FHandle << "|Brokenx = " << clgame_x.broke_stamp;
			}
			else if (i == 2 && j == 6) {
				FHandle << "|Brokeny = " << clgame_y.broke_stamp;
			}
			else {
				FHandle << "|___________";
			}
		}
		FHandle << "|" << endl;
		if (i != 3) {
			FHandle << endl;
		}
	}
	FHandle << "  ____2A____  ____2B____  ____2C____  ____2D____  ____2E____  ____2F____  ____2G____  ____2H____  ____2I____  ____2J____  ____2K____  ____2L____ \n\n\n";
}


//oynama fonksiyonu kırığı yok
void do_play(int dice, char next) {

	bool ok = false;
	while (ok == false) {
		string chose_place;
		cout << "Choose stamp what you want to carry : ";
		cin >> chose_place;
		if (next == 'X') {//eğer pulu taşıyabiliyorsa burdaki voidde o işlemi yapıyor
			ok = clgame_x.check_place(dice, chose_place);
		}
		else {
			ok = clgame_y.check_place(dice, chose_place);
		}
		if (ok == false) {
			cout << "Enter a valid place!\n";
		}
	}
}

//hareket edip edemeyeceği kontrolü
bool check_move(int dice, char person) {
	if (person == 'X') {
		return clgame_x.check_movee(dice);
	}
	else {
		return clgame_y.check_movee(dice);
	}
}

//eski oyun datasını alıyorum
void do_split(string kl) {
	stringstream ss(kl);
	string item;
	while (getline(ss, item, ' ')) {
		result.push_back(item);
	}
	cout << result.size();
}

//devam etmek isteyen oyunu kaydediyoruz
void deposit() {	
	if (result[1] == "X") {
		clgame_x.define_player(1, 'X');
		clgame_y.define_player(2, 'Y');
	}
	else if (result[1] == "Y") {
		clgame_y.define_player(1, 'Y');
		clgame_x.define_player(2, 'Y');
	}
	int sayac = 0;
	for (int i = 3; i < result.size(); i++) {
		if (i % 2 == 0) {
			board[sayac][1] =int_fromstr(result[i]);
			sayac++;
		}
		else {
			board[sayac][0] =int_fromstr(result[i]);
		}
	}
}

//eski bir oyunu olup olmadığını kontrol ediyorum
bool check_oldgame() {
	ifstream dat_read_file("C:\\Users\\tnshu\\source\\repos\\ConsoleApplication1\\Data.txt");
	string tm, last_line="";	
	while (getline(dat_read_file, tm)) {
		if (tm[0] == '*') {
			last_line = tm;
		}
	}
	if (last_line != "") {
		string devam;
		cout << "You have old game. So İf you want to be continue press to 1 or anyone key:";
		cin >> devam;
		if (devam == "1") {//önceki kalan oyundaki datayı almalıyız
			do_split(last_line);
			return false;
		}
		else {
			return true;
		}
	}		
	return true;
}

int main()
{
	put_place();
	char next='X';
	bool doorgame = check_oldgame();

	if (doorgame) {//yarım kalan oyuna dönmeyecekleri için yeniden başlıyor oyun

		put_stamp();
		cout << "Welcome the new ht_game (like backgammon)\n";
		cout << "There are two player call X and Y.\n";
		cout << "The player starting the game according to the dice cast: ";

		//zarlar atılır ve oyuna kimin ilk başlayacağı belirlenir
		//ona göre de oyuncular ve pulları atılır
		bool door = true;

		while (door) {
			gamer_X = random_zar();
			gamer_Y = random_zar();
			if (gamer_Y > gamer_X) {
				cout << "Y";
				next = 'Y';
				door = false;
				clgame_y.define_player(1, 'Y');
				clgame_x.define_player(2, 'X');
			}
			else if (gamer_X > gamer_Y) {
				cout << "X";
				next = 'X';
				door = false;
				clgame_x.define_player(1, 'X');
				clgame_y.define_player(2, 'Y');
			}
		}
		dat_file << gamer_X << endl << gamer_Y << endl;
	}
	else {
		deposit();
		string klm = result[2];
		next = klm[0];
	}
	

	cout << "\nThe starting game!\n" << endl;
	//bir oyuncu kazanana kadar veya çıkış yapılana kadar oyun devam eder
	while (game_over) {
		show_backgommon();
		cout << "if you want to exit from game, press to 'e' or anyone key\n";
		string anns;//oyundan çıkmak isteyip istemediğini sordum
		cin>>anns;
		if (anns == "e" || anns == "E") {
		    dat_file << "* ";
			if (clgame_x.direction == "plus") {//önce plus olan oyuncu yazılacak
				dat_file << "X ";
			}
			else  dat_file << "Y ";
			dat_file << next;//sonra sıradaki oyuncu
			for (int i = 0; i < 24; i++) {
				dat_file << " " << board[i][0];
				dat_file << " " << board[i][1];
			}
			dat_file << endl;
			return 0; //exit (0);
		}
		else {
			cout<<endl;
		}
		cout << "Collect stamp X:" << clgame_x.col_stamp << endl << "Collect stamp Y:" << clgame_y.col_stamp << endl;
		cout << "Broken stamp X:" << clgame_x.broke_stamp << endl << "Broken stamp Y:" << clgame_y.broke_stamp << endl;
		int dice1 = random_zar();
		int dice2 = random_zar();
		if (dice1 % 2 == 0) {//burda da zarlar randomla hep aynı gelince bazen aynı gelmesi bazen de farklı olması için yazdım
			while (dice1 == dice2) {
				dice2 = random_zar();
			}
		}
		dat_file << next << "  " << dice1 << "  " << dice2 << endl;
		//oyuncu için ilk zarlar atılır ve oyuncunun kırık sayısına fonksiyonlar çalışır
		if (next == 'X' && clgame_x.broke_stamp > 0) {
			cout << "You have broken stamp and your dices " << dice1 << " " << dice2 << " so wait...\n";
			if (dice1 == dice2) {//iki zar eşit geldi ve kırığı var
				for (int i = 0; i < 4; i++) {
					if (clgame_x.broke_stamp > 0) {
						bool kapi = do_broken(dice1, 'X');
						if (!kapi) break;//bir zar için çalışmadığında devam etmenin bi önemi yok
					}
					else {
						if (check_move(dice1, next)) {
							do_play(dice1, next);
						}
					}
				}
			}
			else {//farklı zarlar ve kırık sayısına göre if lenir
				if (clgame_x.broke_stamp == 1) {
					if (!do_broken(dice1, 'X')) {
						if (do_broken(dice2, 'X')) {
							if (check_move(dice1, next)) {
								do_play(dice1, next);
							}
						}
					}
					else {
						if (check_move(dice2, next)) {
							do_play(dice2, next);
						}
					}
				}
				else {
					do_broken(dice2, 'X');
					do_broken(dice1, 'X');
				}
			}
		}
		else if (next == 'Y' && clgame_y.broke_stamp > 0) {
			cout << "You have broken stamp and your dices " << dice1 << " " << dice2 << " so wait...\n";
			if (dice1 == dice2) {
				for (int i = 0; i < 4; i++) {
					if (clgame_y.broke_stamp > 0) {
						bool kapi = do_broken(dice1, 'Y');
						if (!kapi) break;
					}
					else {
						if (check_move(dice1, next)) {
							do_play(dice1, next);
						}
					}
				}
			}
			else {
				if (clgame_y.broke_stamp == 1) {
					if (!do_broken(dice1, 'Y')) {
						if (do_broken(dice2, 'Y')) {
							if (check_move(dice1, next)) {
								do_play(dice1, next);
							}
						}
					}
					else {
						if (check_move(dice2, next)) {
							do_play(dice2, next);
						}
					}
				}
				else {
					do_broken(dice2, 'Y');
					do_broken(dice1, 'Y');
				}
			}
		}
		else if (dice1 == dice2) {//kırığı yok ve çift zar
			cout << "Dice for " << next << " player: " << dice1 << "  " << dice2 << endl;
			cout << "Your dice are double so you playing for four times.\n";
			for (int i = 0; i < 4; i++) {
				if (check_move(dice1, next) == true) {
					if (check_move(dice1, next)) {
						do_play(dice1, next);
					}
				}
				else {
					cout << "No more valid moves :(\n";
					break;
				}
				cout << "Complete of move step the next step...\n";
			}
		}
		else {//kırığı yok ve farklı zar
			int sayac[2];
			cout << "Dice for " << next << " player: " << dice1 << "  " << dice2 << endl;
			cout << "Press to 1 for dice " << dice1 << " or press 2 for dice " << dice2 << " : ";
			int chose_dice;
			cin >> chose_dice;
			if (chose_dice == 1) {
				sayac[0] = dice1;
				sayac[1] = dice2;
			}
			else {
				sayac[0] = dice2;
				sayac[1] = dice1;
			}
			for (int i = 0; i < 2; i++) {
				if (check_move(sayac[i], next) == true) {
					if (check_move(sayac[i], next)) {
						do_play(sayac[i], next);
					}
				}
				else {
					cout << "No more valid move for dice\n";
					continue;
				}
				cout << "Complete of move step the next step...\n";
			}
		}
		cout << "Complete all of move steps for " << next;
		if (next == 'X') {
			if (clgame_x.col_stamp == 15) {
				break;
				game_over = false;
			}
			next = 'Y';
		}
		else {
			if (clgame_y.col_stamp == 15) {
				break;
				game_over = false;
			}
			next = 'X';
		}
		cout << "\n\nNow time of the next player " << next << " \n";
	}

	cout << next << " is the winner.\n";
	
	return 0;

}

// Console_TXT_Editor.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#include <iostream>
#include <sstream>
#include <vector>
#include<fstream>
#include <string>

using namespace std;

// 핵심 자료구조들, Lines -> 25 * 75 bytes 배열, Last_Words_Index_Line = 해당 라인의 마지막 단어
struct MyWords {
	vector<string> Lines;
	vector<string> words; // Create vector to hold our words < 핵심 자료구조!! > 다 이걸 통해 txt를 수정함!!
	vector<int> Last_Words_Index_Line;
	int Indicater;
};
// TXT에서 뽑아낸 단어들의 집합
vector<string> words; // Create vector to hold our words < 핵심 자료구조!! > 다 이걸 통해 txt를 수정함!!

// Command 클래스, 다른 하위 클래스의 부모 클래스입니다.
class Command {
public:
	MyWords MainWords;

	Command(MyWords MainWords) {
		this->MainWords = MainWords;
	}

	MyWords Made(vector<string> words) {
		vector<string> Lines;
		vector<string> Mywords = words;
		string temp;
		vector<int> Last_Words_Index_Line;

		for (int i = 0; i < words.size(); i++) {
			
			temp = temp + words[i] + " ";

			if (temp.length() > 75) { // 더한 경우, Line 길이가 75보다 클 경우
				if (temp.length() == 76 && temp[temp.length() - 1] == ' ') { //만약 공백 빼고 75로 딱 맞춰 진다면??
					temp.erase(temp.length() - 1, temp.length()); // 공백 빼고
					Lines.push_back(temp);                        // 그걸 Line에 넣고
					temp = "";                                    // temp을 초기화한다.
					Last_Words_Index_Line.push_back(i);

					continue;
				}
				int wordSize = words[i].length() + 1; // +1은 " " 까지 포함한 것이다. 
				temp.erase(temp.length() - wordSize, temp.length()); // 더한 단어를 빼고
				Lines.push_back(temp); // 그냥 그 Line을 넣는다.

				temp = words[i] + " "; // temp을 초기화한다.
				Last_Words_Index_Line.push_back(i - 1);
			}
			//마지막 문장 ( 75보다 작을 때 )
			if (i == words.size() - 1 && temp.length() <= 75) {
				Lines.push_back(temp);
				temp = "";
			}
		}
		return { Lines, Mywords, Last_Words_Index_Line, 0 };
	}
	
	virtual MyWords Perform() = 0; // For Command t,n,p
	virtual MyWords Perform(MyWords MainWords) = 0;
	virtual MyWords Perform(string word, MyWords MainWords) = 0; // For Command s
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) = 0; //For Command c
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) = 0; //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) = 0; // For Command I

};

//
class CommandI : public Command {
public:
	int line;
	int number;
	string word;
	vector<int> Last_Words_Index_Line;

	CommandI(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) : Command(MainWords) {
		this->line = line;
		this->number = number;
		this->word = word;
		this->MainWords = MainWords;
		this->Last_Words_Index_Line = Last_Words_Index_Line;
	}
	virtual MyWords Perform(){ return MainWords;  }
	virtual MyWords Perform(MyWords MainWords) { return MainWords; }
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords;  }
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords;  }
	
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) {
		if (line == 1) {
			// 한 문장에서 범위를 넘어 설 시.
			if (number > Last_Words_Index_Line[0] + 1) {
				throw out_of_range("문장 넘어섬!");
			}
			MainWords.words.insert(MainWords.words.begin() + number, word);
			MyWords Inserted_words = Made(MainWords.words);
			return Inserted_words;
		}
		else {
			if (line > MainWords.Lines.size()) {
				throw out_of_range("문장 넘어섬!");
			}
			int idx = Last_Words_Index_Line[line - 2] + number;
			
			// 한 문장에서 범위를 넘어 설 시.
			if(idx > Last_Words_Index_Line[line - 1] + 1) {
				throw out_of_range("문장 넘어섬!");
			}
			MainWords.words.insert(MainWords.words.begin() + idx + 1, word);
			MyWords Inserted_words = Made(MainWords.words);
			return Inserted_words;
		}
	}
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords;  }
};

class CommandD : public Command {
public:
	int line;
	int number;
	vector<int> Last_Words_Index_Line;

	CommandD(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) : Command(MainWords) {
		this->line = line;
		this->number = number;
		this->MainWords = MainWords;
		this->Last_Words_Index_Line = Last_Words_Index_Line;
	}
	
	virtual MyWords Perform() { return MainWords; }
	virtual MyWords Perform(MyWords MainWords) { return MainWords; }
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords; }
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords;  }

	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) {
		if (line == 1) {
			// 한 문장에서 범위를 넘어 설 시.
			if (number > Last_Words_Index_Line[0] + 1) {
				throw out_of_range("문장 넘어섬!");
			}
			//std::cout << "Deleted Word! : " << MainWords.words[number - 1] << endl;
			MainWords.words.erase(MainWords.words.begin() + number - 1);
			MyWords deleted_words = Made(MainWords.words);
			return deleted_words;
		}
		else {
			if (line > MainWords.Lines.size()) {
				throw out_of_range("문장 넘어섬!");
			}
			int idx = Last_Words_Index_Line[line - 2] + number;
			if (idx > Last_Words_Index_Line[line - 1] + 1) {
				throw out_of_range("문장 넘어섬!");
			}
			//std::cout << "Deleted Word! : " << MainWords.words[idx] << endl; // print in Console Message!
			MainWords.words.erase(MainWords.words.begin() + idx);
			MyWords deleted_words = Made(MainWords.words);
			return deleted_words;
		}
	}
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords;  }
};

class CommandS : public Command { // 해당 word가 첫 째 라인에 위치하도록!!!!!!!!!!!!!!!!!!!!!!!
public:
	string word;
	CommandS(string word) : Command(MainWords) {
		this->word = word;
	}
	virtual MyWords Perform() { return MainWords; }
	virtual MyWords Perform(MyWords MainWords) { return MainWords; }
	
	virtual MyWords Perform(string word, MyWords MainWords) {
		int idx = -1;

		for (int i = 0; i < MainWords.words.size(); i++) {
			if (word == MainWords.words[i]) {
				idx = i;
				break;
			}

			if (i == MainWords.words.size() - 1) {
				throw out_of_range("단어 없음!");
			}
		}

		int ShowLine = 0;
		int TotalLines = MainWords.Lines.size();
		int PageNum = TotalLines / 20;

		for (int j = 0; j < MainWords.Last_Words_Index_Line.size(); j++) {

			if (idx > MainWords.Last_Words_Index_Line[TotalLines-2] && idx <= MainWords.Last_Words_Index_Line[TotalLines - 1]) {
				ShowLine = TotalLines - 1;
				break;
			}

			else if (MainWords.Last_Words_Index_Line[j] < idx && MainWords.Last_Words_Index_Line[j+1] >= idx) {
				ShowLine = j + 1;
				break;
			}

			else if (idx <= MainWords.Last_Words_Index_Line[0]) {
				ShowLine = 0;
				break;
			}
		}

		MyWords ResWords;
		if (ShowLine + 20 >= PageNum * 20) { // 새롭게 출력할 라인 수가 남았을 경우
			ShowLine = TotalLines - 20;
			ResWords = Made(MainWords.words);
			ResWords.Indicater = ShowLine;
		}
		else { // 아니면 그냥 출력하게 함.
			ResWords = Made(MainWords.words);
			ResWords.Indicater = ShowLine;
		}

		return ResWords;
	}
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords; } //For Command c
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } // For Command I

};

class CommandC : public Command {
public:
	string word1;
	string word2;

	CommandC(string word1, string word2): Command(MainWords) {
		this->word1 = word1;
		this->word2 = word2;
	}
	virtual MyWords Perform() { return MainWords; }
	virtual MyWords Perform(MyWords MainWords) { return MainWords;  }
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords; } // For Command s
	
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { 
		
		int navigater = -1;
		vector<string> ResWord = MainWords.words;
		
		for (int i = 0; i < ResWord.size(); i++) {
			
			if (word1 == MainWords.words[i]) {
				navigater = 1;
				ResWord[i] = word2;
			}
		}
		if (navigater == -1) {
			throw range_error("Cant Find Word!");
		}

		MyWords Res = Made(ResWord);
		return Res;
	} //For Command c
	
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; }// For Command I
};

class CommandT : public Command {
public:

	CommandT() : Command(MainWords) { }

	virtual MyWords Perform() { return MainWords; }
	virtual MyWords Perform(MyWords MainWords) { 
		ofstream Copen;
		Copen.open("C:/Users/MYCOM/source/repos/Console_TXT_Editor/Res.txt");

		MyWords ResWord = MainWords;
		for (int i=0; i < ResWord.Lines.size(); i++) {
			Copen.write(ResWord.Lines[i].c_str(), 75);
		}
		
		cout << "======================= 저장하고 종료합니다! =======================" << endl;
		Copen.close();
		exit(0);
		return ResWord; 
	}
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords; }// For Command s
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords; } //For Command c
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } // For Command I
};

class CommandN : public Command {
public:
	int navigater = -1;

	CommandN() : Command(MainWords) { }
	virtual MyWords Perform() { return MainWords; }

	virtual MyWords Perform(MyWords MainWords) {
		int TotalLines = MainWords.Lines.size();
		int PageNum = TotalLines / 20;
		//int LastNum = TotalLines % 20;
		MyWords NextMainWords;

		if (MainWords.Indicater + 20 >= PageNum*20 ) { // 새롭게 출력할 라인 수가 남았을 경우
			int NextIndicater = TotalLines - 20;
			NextMainWords = Made(MainWords.words);
			NextMainWords.Indicater = NextIndicater;
			navigater = 1;
		}
		else { // 아니면 그냥 출력하게 함.
			int NextIndicater = MainWords.Indicater + 20;
			NextMainWords = Made(MainWords.words);
			NextMainWords.Indicater = NextIndicater;
		}
		
		return NextMainWords;
	}
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords; }// For Command s
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords; } //For Command c
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } // For Command I
};

class CommandP : public Command {
public:
	int navigater = -1;
	CommandP() : Command(MainWords) { }
	virtual MyWords Perform() { return MainWords; }

	virtual MyWords Perform(MyWords MainWords) {
		int TotalLines = MainWords.Lines.size();
		int PageNum = TotalLines / 20;
		MyWords PrevMainWords;


		if (MainWords.Indicater - 20 <= 0 || MainWords.Indicater == 0 || MainWords.Indicater < 20 ) { // 새롭게 출력할 라인 수가 남았을 경우
			int PrevIndicater = 0;
			PrevMainWords = Made(MainWords.words);
			PrevMainWords.Indicater = PrevIndicater;
			navigater = 1;
		}
		else { // 아니면 그냥 출력하게 함.
			int PrevIndicater = MainWords.Indicater - 20;
			PrevMainWords = Made(MainWords.words);
			PrevMainWords.Indicater = PrevIndicater;

		}

		return PrevMainWords;
	}
	virtual MyWords Perform(string word, MyWords MainWords) { return MainWords; } // For Command s
	virtual MyWords Perform(string word1, string word2, MyWords MainWords) { return MainWords; } //For Command c
	virtual MyWords Perform(int line, int number, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } //For Command D
	virtual MyWords Perform(int line, int number, string word, MyWords MainWords, vector<int> Last_Words_Index_Line) { return MainWords; } // For Command I
};

// Console editor, 출력을 하고, 커맨드를 불러오고, TXT를 읽는 역할을 수행합니다.
class Console_Editer {

	// Member
private:
	//ifstream Fopen;
	string input;
	

public:
	//vector<string> remainderWords; // 명령어 s를 따로 처리하기 위한 벡터..
	MyWords MainWords;  //  < 핵심 자료구조!! > 다 이걸 통해 txt를 수정함!!
	string command = "안녕하세요?";
	int Start = 0; // Console View 반복 횟수
	int Indicater = 0; // Indicater + 20 => Page 단위!

	// Method
public:

	//TXT 파일을 열고, words벡터에 단어를 넣고 반환합니다.
	vector<string> Open_And_MakeWords() {
		vector<string> words;
		ifstream Fopen;
		Fopen.open("C:/Users/MYCOM/source/repos/Console_TXT_Editor/test.txt", ios_base::in);

		Fopen.seekg(0, ios::end);
		int end = Fopen.tellg();
		input.resize(end);
		Fopen.seekg(0, ios::beg);

		Fopen.read(&input[0], end);

		// Get words from Input String, Input string has all data from txt file. ///////////////////////
		string buf;                 // Have a buffer string
		stringstream ss(input);     // Insert the string into a stream

		while (ss >> buf)
			words.push_back(buf);

		Fopen.close();
		return words;
	}

	// Words를 받아서 75 바이트의 배열로 변환합니다.
	MyWords Made(vector<string> words) {
		vector<string> Lines;
		vector<string> Mywords = words;
		string temp;
		vector<int> Last_Words_Index_Line;

		for (int i = 0; i < words.size(); i++) {

			temp = temp + words[i] + " ";

			if (temp.length() > 75) { // 더한 경우, Line 길이가 75보다 클 경우
				if (temp.length() == 76 && temp[temp.length() - 1] == ' ') { //만약 공백 빼고 75로 딱 맞춰 진다면??
					temp.erase(temp.length() - 1, temp.length()); // 공백 빼고
					Lines.push_back(temp);                        // 그걸 Line에 넣고
					temp = "";                                    // temp을 초기화한다.
					Last_Words_Index_Line.push_back(i);

					continue;
				}
				int wordSize = words[i].length() + 1; // +1은 " " 까지 포함한 것이다. 
				temp.erase(temp.length() - wordSize, temp.length()); // 더한 단어를 빼고
				Lines.push_back(temp); // 그냥 그 Line을 넣는다.

				temp = words[i] + " "; // temp을 초기화한다.
				Last_Words_Index_Line.push_back(i - 1);
			}
			//마지막 문장 ( 75보다 작을 때 )
			if (i == words.size() - 1 && temp.length() <= 75) {
				Lines.push_back(temp);
				temp = "";
			}
		}
		return { Lines, Mywords, Last_Words_Index_Line, 0 };
	}

	MyWords Convert_Command(string YourInput, MyWords MainWords, int Indicater, string command) {

		vector<int> Last_Words_Index_Line = MainWords.Last_Words_Index_Line;
		MyWords YourMainWords = MainWords;
		
		char main = YourInput[0];
		vector<string> answer;
	
		// ex) d(1,2) => get "d" + 명령어 1개만 들어왔을 경우, err 처리!
		if (YourInput.size() == 1) {
			if (main == 'n' || main == 'p' || main == 't') {
			}
			else{
				this->command = "잘못된 한 단어 명령어 입니다. 유효한 명령어를 입력하세요.";
				return YourMainWords;
			}
		}
		
		//  ex) d(1,2) => get " 1, 2 "
		if (main == 'd' || main == 'i' || main == 's' || main == 'c') {

			//err -> 괄호 없을 경우, 공백이 포함 된 경우 처리 (2중 처리되긴 함)
			if (YourInput.find(')') == string::npos || YourInput.find('(') == string::npos)
			{
			this->command = "길이가 맞지 않습니다. 괄호와 매개변수를 포함하세요.";
			return YourMainWords;
				} 
			else if (YourInput.at(2) == ')' ) {
				this->command = "매개변수를 넣어야합니다.";
				return YourMainWords;
			}
			string param = YourInput.substr(2, YourInput.length() - YourInput.find('('));
			param.resize(param.length() - 1); // remove )

			stringstream ss(param);
			string temp;
			// ex) answer = [1,2]
			while (getline(ss, temp, ',')) {
					answer.push_back(temp);
				
			}
		} 
		
		for (int idx = 0; idx < answer.size(); idx++) {
			if (answer[idx] == "") {
					this->command = ",로만 입력은 불가능합니다. 꼭 매개변수를 넣으세요!";
					return YourMainWords;
			}
		}

		// 해당 케이스에서 예외상황을 처리한다.
		if (main == 'd') {
			int line = atoi(answer[0].c_str()); // 실제 정수로 변환
			int number = atoi(answer[1].c_str());
			
			if (line == 0 || number == 0) {
				this->command = "인자 값은 숫자여야 합니다.";
				return YourMainWords;
			}

			CommandD *commandD = new CommandD(line, number, MainWords, Last_Words_Index_Line);
			MyWords res;

			try {
				this->command = "단어가 성공적으로 삭제되었습니다!";
				res = commandD->Perform(line, number, MainWords, Last_Words_Index_Line);
			}
			catch (const std::out_of_range& e) {
				this->command = "입력된 문자 위치는 해당 문장 길이를 넘어섰습니다. 다시 입력하세요.";
				res = MainWords;
			}
			
			return res;
		}
		else if (main == 'i') {
			int line = atoi(answer[0].c_str()); // 실제 정수로 변환
			int number = atoi(answer[1].c_str());

			if (line == 0 || number == 0) {
				this->command = "인자 값은 숫자여야 합니다. 또는 인자 값이 0을 가질 수 없습니다.";
				return YourMainWords;
			}

			string word = answer[2];
			
			CommandI* commandI = new CommandI(line, number, word, MainWords, Last_Words_Index_Line);
			MyWords res;

			try {
				if (word.size() >= 75) {
					this->command = "입력된 문자는 해당 문장 길이를 넘어섰습니다. 다시 입력하세요.";
					res = MainWords;
					return res;
				}
				res = commandI->Perform(line, number, word, MainWords, Last_Words_Index_Line);
			}
			catch (const std::out_of_range& e) {
				this->command = "입력된 문자 위치는 해당 문장 길이를 넘어섰습니다. 다시 입력하세요.";
				res = MainWords;
				return res;
			}
			this->command = word + " 가 성공적으로 삽입되었습니다!";
			return res;
		}
		else if (main == 's') {
			string word = answer[0];
			MyWords Res;
			CommandS* commandS = new CommandS(word);
			
			try {
				Res = commandS->Perform(word, MainWords);
			}
			catch (const std::out_of_range& e) {
				this->command = "해당 단어는 존재하지 않습니다.";
				Res = MainWords;
				return Res;
			}
			this->command = "해당 단어는 맨 위에 출력됩니다.";
			return Res;
		}
		else if (main == 'c') {
			string word1 = answer[0];
			string word2 = answer[1];
			MyWords res;
			
			CommandC* commandC = new CommandC(word1, word2);
			
			try {
				res = commandC->Perform(word1, word2, MainWords);
			}
			catch (const std::range_error& e) {
				this->command = "해당되는 단어가 존재하지 않습니다. 못 바꿉니다.";
				res = MainWords;
				return res;
			}
			this->command = word1 + " 은 " + word2 + "로 바뀌어 졌습니다!";
			return res;
		}
		else if (main == 't') {
			CommandT* commandt = new CommandT();
			this->command = "저장하고 종료합니다!";
			return commandt->Perform(MainWords);
		}
		else if (main == 'n') {
			CommandN* commandN = new CommandN();
			MyWords res = commandN->Perform(MainWords);
			if (commandN->navigater == 1) {
				this->command = "해당 페이지는 마지막페이지 입니다!";
			}
			else {
				this->command = "다음 페이지로 이동했습니다.";
			}

			return res;
		}
		else if (main == 'p') {
			CommandP* commandP = new CommandP();
			MyWords res = commandP->Perform(MainWords);
			if (commandP->navigater == 1) {
				this->command = "해당 페이지는 맨 앞 페이지 입니다!";
			}
			else {
				this->command = "이전 페이지로 이동했습니다.";
			}


			return res;
		}
		else {
			// 잘못된 명령어, command -> 오류 처리
			this->command = "잘못된 입력값입니다.";
			
			return YourMainWords;
		}

	}


	string View() {

		for (int i = 0; i < 20; i++) {
			cout << this->MainWords.Indicater + i + 1 << "| " << this->MainWords.Lines[this->MainWords.Indicater + i] << endl;
		}
		cout << "--------------------------------------------------------------------------------------------" << endl;
		cout << "n: 다음 페이지, p: 이전 페이지, i: 삽입. d: 삭제, c: 변경, s:찾기, t:저장 후 종료 " << endl;
		cout << "--------------------------------------------------------------------------------------------" << endl;
		cout << "( Console Message ) : " << this->command << endl;
		cout << "--------------------------------------------------------------------------------------------" << endl;
		string YourInput;
		cout << "입력 : ";
		cin >> YourInput;
		
		this->MainWords = Convert_Command(YourInput, MainWords, MainWords.Indicater, command);
		cout << "--------------------------------------------------------------------------------------------" << endl;
		
		return YourInput;
	}

	void Execute() {
		vector<string> words = Open_And_MakeWords();
		
		this->MainWords = Made(words);
		int LineSize = MainWords.Lines.size();
		
		string prev;
		while (true) {
			prev = View(); // 이전 명령어 get!
			
			Start += 1;
		}
	}
};

//Main 함수
int main() {
	Console_Editer* console = new Console_Editer();
	console->Execute();
	
	return 0;
}

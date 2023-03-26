//23.3.2023
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
bool is_operator(string possible_operator){
	vector<char> operators = {'+','-','*','/','=','!','<','>'};
	for(char letter:possible_operator){
		if(!(count(operators.begin(), operators.end(), letter))){
			return false;
		}
	}
	return true;
}

bool is_operator(char possible_operator){
	vector<char> operators = {'+','-','*','/','=','!','<','>'};
	if(!(count(operators.begin(), operators.end(), possible_operator))){
		return false;
	}
	return true;
}

vector<string> split_line(string line){
	vector<string> splitted_line;
	int letter_index = 0;
	bool inString = false;
	bool inOperator = false;
	string value = "";
	char letter;
	while(letter_index < line.size()){
		letter = line[letter_index];
		if(!(is_operator(letter)) && inOperator == 1){
			if(!(value == "")){
				splitted_line.push_back(value);
				value = "";
			}
			inOperator = false;
		}
		if(letter == '	'){
			if(inString){
				value += letter;
			}
		}
		else if(is_operator(letter)){
			if(!(inString)){
				if(!(inOperator)){
					if(!(value == "")){
						splitted_line.push_back(value);
					}
					value = string(1,letter);
					inOperator = true;
				}
				else{
					value += letter;
				}
			}
			else{
				value += letter;
			}
		}
		else if(letter == '"'){
			if(inString){
				inString = false;
				value += letter;
				if(!(value == "")){
					splitted_line.push_back(value);
					value = "";
				}
			}
			else{
				inString = true;
				if(!(value == "")){
					splitted_line.push_back(value);
					value = "";
				}
				value += letter;
			}
		}
		else if(letter == '('){
			if(!(inString)){
				if(!(value == "")){
					splitted_line.push_back(value);
				}
				value = "(";
				int inBrackets = 1;
				while(0 < inBrackets){
					letter_index++;
					value += line[letter_index];
					if(line[letter_index] == '(' && !(inString)){
						inBrackets++;
					}
					else if(line[letter_index] == ')' && !(inString)){
						inBrackets--;
					}
					else if(line[letter_index] == '"'){
						if(inString){
							inString = false;
						}
						else{
							inString = true;
						}
					}
				}
			splitted_line.push_back(value);
			value = "";
			}
			else{
				value += letter;
			}
		}
		else if(letter == ' '){
			if(!(inString)){
					if(!(value == "")){
						splitted_line.push_back(value);
						value = "";
					}
				}
			else{
				value += letter;
			}
		}
		else{
			value += letter;
		}
		letter_index++;
	}
	if(!(value == "")){
		splitted_line.push_back(value);
	}
	return splitted_line;
}

void compile(vector<string> lines, string name = "out"){
	cout<<"compiling process started\n";
	ofstream cppsource(name + ".cpp");
	cppsource << "#include <iostream>\n";
	int line_index = 1;
	for(auto i:lines){
		if(i == ""){
			continue;
		}
		auto line = split_line(i);
		if(line[0] == "write"){
			cppsource << "std::cout<<" + line[1] + ";\n";
		}
		else if(line[0] == "system"){
			cppsource << "system(std::string(" + line[1] + ").c_str());\n";
		}
		else if(line[0] == "func"){
			cppsource << line[1] + " " + line[2] + line[3] + "{\n";
		}
		else if(line[0] == "end"){
			cppsource << "}\n";
		}
		else if(line[0] == "var"){
			cppsource << "auto " + line[1] + line[2] + line[3] + ";\n";
		}
		else if(line.size() == 3 && line[1] == "="){
			cppsource << line[0] + "=" + line[2] + ";\n";
		}
		else if(line.size() == 3 && line[1] == ":"){
			cppsource << line[2] + " " + line[0] + ";\n";
		}
		else if(line[0] == "if"){
			cppsource << "if " + line[1] + "{\n";
		}
		else if(line[0] == "while"){
			cppsource << "while " + line[1] + "{\n";
		}
		else if(line[0] == "for"){
			cppsource << "for " + line[1] + "{\n";
		}
		else if(line[0] == "return"){
			cppsource << "return " + line[1] + ";\n";
		}
		else if(line[0] == "else"){
			if(line.size() == 1){
				cppsource << "else{\n";
			}
			else{
				cppsource << "else if " + line[2] + "{\n";
			}
		}
		else if(line.size() == 2 && line[1].back() == ')'){
				cppsource << line[0] + line[1] + ";\n";
		}
		else if(line[0] == "input"){
			cppsource << "std::getline(std::cin, " + line[1] + ");\n";
		}
		else{
			cppsource.close();
			remove((name + ".cpp").c_str());
			cout<<"error in line "<<line_index<<": unknown syntax "<<"\""<<line[0]<<"\"";
			return;
		}
		line_index++;
	}
	cppsource.close();
	system(("g++ " + name + ".cpp" + " -o " + name +".exe").c_str());
	//remove((name + ".cpp").c_str());
	cout<<"finished\n";
}


int main(int argc, char** argv){
	string filename;
	ifstream file;
	if(argc < 2){
		cout<<"error: too few arguments\n";
		return 1;
	}
	else{
		filename = argv[1];
		file.open(filename);
		if(file.fail()){
			cout<<"error: cannot read file\n";
			return 1;
		}
	}
	vector<string> lines;
	string line;
	while(!file.eof()){
		getline(file, line);
		lines.push_back(line);
	}
	compile(lines);
	return 0;
}

#include<bits/stdc++.h>

using namespace std;

void storeArgumentsAndFlags(int argc, char **argv, char &flag, vector<string> &filePaths){
    for(int i = 1;i<argc;i++){
        if(argv[i][0] == '-'){
            flag = argv[i][1];
            continue;
        }

        filePaths.push_back(argv[i]);
    }
}

bool isLabel(const string &s){
    return s[s.size()-1] == ':';
}

string labelToWord(const string &s){
    return  s.substr(0,s.size()-1);
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool is_hexadecimal(const std::string& s) {
    regex pattern("^0x[0-9a-fA-F]+$");
    return regex_match(s, pattern);
}

string stringToUpper(string &s){
    for (char &c : s) {
        c = toupper(c);
    }
    return s;
}

bool hasOnlyLabel(string &s){
    istringstream iss(s);
    string word;
    int i = 0;
    bool hasLabel = false;

    while(iss >> word){
        i++;
        if(word[word.size()-1] == ':'){
            hasLabel = true;
        }
        if(i >= 2){
            return false;
        }
    }

    if(!hasLabel){
        return false;
    }

    return true;
}
//Lembrar: tornar tudo uppercase
//modificar hexadecimal para decimal
class PreProcessor{
    string filePath;
    string outputFilePath;
    fstream inputFile;
    fstream outputFile;
    map<string,string> EQUMapper;
    public:
        PreProcessor(string filePath){
            this->filePath = filePath;
            size_t lastIndex = filePath.find_last_of("."); 
            this->outputFilePath = filePath.substr(0, lastIndex) + ".pre"; 
        }

        string rmUnnecessarySpacesAndTabs(string line){
            string processedLine = "";
            istringstream iss(line);
            string word;
            while(iss >> word){
                processedLine += word + " ";
            }
            
            return processedLine;
        }

        string convertAllToDecimal(string line){
            string processedLine = "";
            istringstream iss(line);
            string word;
            while(iss >> word){
                if(is_hexadecimal(word)){
                    word = to_string(stoi(word,0,16));
                }
                processedLine += word + " ";
            }
            
            return processedLine;
        }

        string rmLineCommentary(string line){
            string processedLine = "";
            istringstream iss(line);
            string word;

            while(iss >> word){
                if(word[0] == ';'){
                    break;
                }
                processedLine += word +  " ";
            }
            
            return  processedLine;
        }

        vector<string> rmUnnecessaryEnters(const vector<string> &lines){
            vector<string> processedLines;
            string previousLine = "";

            for(int i = 0;i<lines.size();i++){
                string nextLine = lines[i];
                if(nextLine.empty()){
                    continue;
                }

                if(hasOnlyLabel(nextLine)){
                    previousLine = nextLine;
                    continue;
                }

                if(!previousLine.empty()){
                    nextLine = previousLine + nextLine;
                    previousLine = "";
                }

                processedLines.push_back(nextLine);
            }
            return processedLines;
        }

        void getEQUMapper(const vector<string> &lines, int &index){
            map<string,string> EQUMapper;
            for(index=0;index<lines.size();index++){
                istringstream iss(lines[index]);
                string label;
                iss >> label;
                if(!isLabel(label)){
                    break;
                }

                string word;

                iss >> word;
                
                if(word != "EQU"){
                    break;
                }

                iss >> word;

                label = label.substr(0,label.size()-1);
                
                EQUMapper.insert({label,word});
            }
            this->EQUMapper = EQUMapper;
        }

        vector<string> resolveEQU(const vector<string> &lines){
            int index = 0;
            getEQUMapper(lines,index);
            vector<string> processedLines;

            for(;index<lines.size();index++){
                istringstream iss(lines[index]);
                string word;
                string  newString = "";

                while(iss >> word){
                    if(EQUMapper.find(word) != EQUMapper.end()){
                        newString += EQUMapper[word] + " ";
                        continue;
                    }
                    newString += word + " ";
                }
                processedLines.push_back(newString);
            }
            return processedLines;
        }

        vector<string> resolveIF(const vector<string> &lines){
            vector<string> processedLines;

            for(int i = 0;i<lines.size();i++){
                istringstream iss(lines[i]);
                string word;
                bool isIF = false;

                while(iss >> word){
                    if(word == "IF"){
                        isIF = true;
                        continue;
                    }

                    if(isIF and (is_number(word) and stoi(word)==0)){
                        i++;
                    }
                }
                if(isIF){
                    continue;
                }
                processedLines.push_back(lines[i]);
            }
            return processedLines;
        }

        void preProcessFile(){
            vector<string> processedLines;
            string line;
            inputFile.open(filePath,ios::in);
            if(!inputFile.is_open()){
                cout << "Error opening file\n";
                return;
            }

            while(getline(inputFile,line)){
                line = convertAllToDecimal(line);
                line = stringToUpper(line);
                line = rmLineCommentary(line);
                line = rmUnnecessarySpacesAndTabs(line);
                processedLines.push_back(line);
            }

            processedLines = rmUnnecessaryEnters(processedLines);
            processedLines = resolveEQU(processedLines);
            processedLines = resolveIF(processedLines);

            outputFile.open(outputFilePath,ios::out);

            if(!outputFile.is_open()){
                cout << "Error creating file\n";
                return; 
            }

            for(int i = 0;i<processedLines.size();i++){
                outputFile << processedLines[i] << endl;
            }

            /*
            for(int i = 0;i<processedLines.size();i++){
                cout << processedLines[i] << "\n";
            }
            */
            
            outputFile.close();
            inputFile.close();
        }
};

/*
Fazer:
adicionar possibilidade para space de varias  posicoes
*/
class Assembler{
    string filePath;
    string outputFilePath;
    fstream inputFile;
    fstream outputFile;
    bool hasBegin = false;
    int linesBeforeBegin = 0;
    struct instructionAttributes{
        string code;
        int numberOperands;
    };
    map<string,instructionAttributes> rom = {
        {"ADD",{"01",1}},
        {"SUB",{"02",1}},
        {"MUL",{"03",1}},
        {"DIV",{"04",1}},
        {"JMP",{"05",1}},
        {"JMPN",{"06",1}},
        {"JMPP",{"07",1}},
        {"JMPZ",{"08",1}},
        {"COPY",{"09",2}},
        {"LOAD",{"10",1}},
        {"STORE",{"11",1}},
        {"INPUT",{"12",1}},
        {"OUTPUT",{"13",1}},
        {"STOP",{"14",0}}
    };
    map<string, string> useTable;
    map<string,string> definitionTable;
    string mapaBits = "";
    vector<string> erros;
    struct position{
        int line;
        int column;
        int address;
    };
    struct SimbolTableContent{
        bool defined = false;
        int value;
        vector<position> lineAndColumnPending = {};
        bool isExtern = false;
        bool isPublic = false;
    };
    

    /*
    Mapping mapeado por label.
    Referencia:
    -par:
    -first: par de simbolo definido(first) e seu valor(second)
    -second: vetor contendo linha e coluna de cada referencia do label
    */
    map<string,SimbolTableContent> simbolTable;
    vector<vector<int>> assembledLines;

    public:
        Assembler(string filePath){
            this->filePath = filePath;
            size_t lastIndex = filePath.find_last_of("."); 
            this->outputFilePath = filePath.substr(0, lastIndex) + ".o"; 
        }

        int getLineNumber(int line){
            return line + linesBeforeBegin + 1;
        }

        bool hasLabelErros(string label, int line){
            if(simbolTable.find(label) != simbolTable.end() and simbolTable[label].defined){
                erros.push_back("Rotulo redefinido: " + label + "- linha: " + to_string(getLineNumber(line)));
                return true;
            }
            regex pattern("^[a-zA-Z_][a-zA-Z0-9_]*$");

            if(!regex_match(label,pattern)){
                erros.push_back("Erro lexico - linha: " + to_string(getLineNumber(line)));
                return true;
            }
            return false;
        }

        bool isValidInstruction(string instruction, int line){
            if(rom.find(instruction) != rom.end()){
                return true;
            }
            erros.push_back("Instrucao invalida - " + instruction + " linha: " + to_string(getLineNumber(line)));
            return false;
        }

        void writeErros(){
            if(!erros.empty()){
                cout << "Erros no montador:\n";
                for(int i = 0;i<erros.size();i++){
                    cout << erros[i] << "\n";
                }
                return;
            }
        }

        string getInstructionCode(const string &s){
            return rom[s].code;
        }

        int getNumberOperands(const string &s){
            return rom[s].numberOperands;
        }

        bool definedInSimbolTable(string word){
            if(simbolTable.find(word) != simbolTable.end() and simbolTable[word].defined == true){
                return true;
            }
            return false;
        }

        string resolveSimbol(string  word, int column, int line, int address){
            if(!definedInSimbolTable(word)){
                addToSimbolTable(word,column,line, address);
                return word;
            }

            return to_string(getSimbolValue(word));
        }

        int getSimbolValue(string word){
            return simbolTable[word].value;
        }


        void addToSimbolTable(string word, int column, int line, int address){
            if(simbolTable.find(word) != simbolTable.end()){
                simbolTable[word].lineAndColumnPending.push_back({line,column,address});
                return;
            }

            SimbolTableContent content;
            content.defined = false;
            content.lineAndColumnPending.push_back({line,column,address});
            simbolTable[word] = content;
        }

        void addValueToSimbolTable(string word, int value){
            if(simbolTable.find(word) != simbolTable.end()){
                simbolTable[word].defined = true;
                simbolTable[word].value = value;
                return;
            }

            SimbolTableContent content;
            content.defined = true;
            content.value = value;

            simbolTable[word] = content;
        }


        void processArguments(string  instruction, string args, vector<string> &procesedLine, int column, int line, int &current_postion){
            istringstream iss(args);
            string word;
            int correctNumberOperands = getNumberOperands(instruction);

            int argumentNumber = 0;
            while(getline(iss,word,',')){
                if(is_number(word)){
                    procesedLine.push_back(word);
                }else{
                    string arg = resolveSimbol(word,column+argumentNumber,line,current_postion);
                    procesedLine.push_back(arg);
                    mapaBits += "1";
                }

                current_postion++;
                argumentNumber++;
            }



            if(argumentNumber!=correctNumberOperands){
                erros.push_back("Numero incorreto de operandos - linha: " + to_string(getLineNumber(line)));
                procesedLine = {};
            }

        }

        vector<string> makeLinePassage(string line, int line_number, int &current_position){
            istringstream iss(line);
            string word;
            vector<string> processedLine;
            bool passedInstruction = false;
            string label = "";
            string instruction = "";
            bool isConst = false;

            for(int column = 0;iss >> word;column++){
                if(word == "END" and column == 0){
                    return processedLine;
                }

                if(isLabel(word)){
                    label = word.substr(0,word.size()-1);
                    if(!hasLabelErros(label,line_number)){
                        addValueToSimbolTable(label,current_position);
                    }else{
                        return {};
                    }
                    column--;
                    continue;
                }

                if(word == "CONST"){
                    isConst = true;
                    continue;
                }

                if(word == "SPACE"){
                    addValueToSimbolTable(label,current_position);
                    processedLine.push_back("0");
                    current_position++;
                    mapaBits += "0";
                    continue;
                }

                if(isConst){
                    addValueToSimbolTable(label,current_position);
                    processedLine.push_back(word);
                    current_position++;
                    mapaBits += "0";
                    continue;
                }

                if(!passedInstruction){
                    passedInstruction = true;
                    if(!isValidInstruction(word, line_number)){
                        return {};
                    }
                    instruction = word;
                    processedLine.push_back(getInstructionCode(word));
                    current_position++;
                    mapaBits += "0";
                    continue;
                }

                processArguments(instruction,word,processedLine,column,line_number, current_position);
            }
            return processedLine;
        }

        void resolveRemainingSimbolTable(vector<vector<string>> &lines){
            for(const auto &simbol: simbolTable){

                vector<position> toBeChanged = simbol.second.lineAndColumnPending;
                int value = simbol.second.value;

                for(const position &position: toBeChanged){
                    if(simbol.second.defined or simbol.second.isExtern){
                        int linha = position.line;
                        int coluna = position.column;
                        lines[linha][coluna] = to_string(value);
                    }else{
                        erros.push_back("Rotulo ausente: " + simbol.first + " linha: " + to_string(getLineNumber(position.line)));
                    }
                }
            }
        }

        
        void writeUseAndDefinitionTable(){
            outputFile << "TABELA USO\n";
            for(const auto &simbol: simbolTable){
                if(!simbol.second.isExtern){
                    continue;
                }
                outputFile << simbol.first << " ";
                for(const auto &position: simbol.second.lineAndColumnPending){
                    outputFile << position.address << " ";
                }
                outputFile << "\n";
            }
            outputFile << "\n";

            outputFile << "TABELA DEF\n";

            for(const auto &simbol: simbolTable){
                if(!simbol.second.isPublic){
                    continue;
                }
                outputFile << simbol.first << " " << simbol.second.value;
                /*for(const auto &position: simbol.second.lineAndColumnPending){
                    outputFile << position.address << " ";
                }*/




                outputFile << "\n";
            }
            outputFile << "\n";

            outputFile << "MAPA DE BITS\n";
            for(char s: mapaBits){
                outputFile << s << " ";
            }
            outputFile << "\n\n";  
        }
        

        bool resolveBeginPublicExtern(const string  &line){
            istringstream iss(line);
            string word;
            string label = "";
            int i = 0;
            while(iss >> word){
                if(isLabel(word)){
                    label = word;
                    i++;
                    continue;
                }

                if(!label.empty() and word == "BEGIN" and i == 1){
                    hasBegin = true;
                    continue;
                }

                if(!label.empty() and  word == "EXTERN" and i == 1 and  hasBegin){
                    SimbolTableContent content;
                    content.isExtern = true;
                    content.value = 0;
                    simbolTable[labelToWord(label)] = content;
                    continue;
                }

                if(word == "PUBLIC" and hasBegin){
                    SimbolTableContent content;
                    content.isPublic = true;
                    iss >> word;
                    simbolTable[word] = content;
                    continue;
                }
                return false;
            }
            return true;
        }

        void makeUniquePassage(){
            vector<vector<string>> processedLines;
            string line;
            inputFile.open(filePath,ios::in);
            if(!inputFile.is_open()){
                cout << "Error opening file\n";
                return;
            }

            int i = 0;
            int currentAdress = 0;
            int current_position = 0;

            while(getline(inputFile,line)){
                if(!resolveBeginPublicExtern(line)) break;
                linesBeforeBegin++;
            }

            processedLines.push_back(makeLinePassage(line,i,current_position));
            i++;

            while(getline(inputFile,line)){
                vector<string> processsedLine = makeLinePassage(line,i,current_position);
                if(processsedLine.empty()){
                    break;
                }

                processedLines.push_back(processsedLine);
                i++;
            }


            if(!erros.empty()){
                writeErros();
                return;
            }

            resolveRemainingSimbolTable(processedLines);

            if(!erros.empty()){
                writeErros();
                return;
            }


            outputFile.open(outputFilePath,ios::out);

            writeUseAndDefinitionTable();

            for(int i = 0;i<processedLines.size();i++){
                for(int j = 0;j<processedLines[i].size();j++){
                    outputFile << processedLines[i][j] << " ";
                }
            }
            

            outputFile.close();
            inputFile.close();
        }
};


int main(int argc, char **argv){
    char flag;
    vector<string> filePaths;
    
    storeArgumentsAndFlags(argc,argv,flag,filePaths);

    if(flag == 'p'){
        PreProcessor preProcessor(filePaths[0]);
        preProcessor.preProcessFile();
    }else if(flag == 'o'){
        Assembler assembler(filePaths[0]);
        assembler.makeUniquePassage();
    }

    
   
    return 0;
}

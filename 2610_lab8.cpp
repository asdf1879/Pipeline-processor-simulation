#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<string> Icache(128, "");
vector<string> Dcache(256, "");

vector<int> RegisterFile(16);

int totalinstructions = 0;
int totalcycles = 0;
int stalls = 0;
int datastalls = 0;
int controlstalls = 0;
int arithmeticinstructions = 0;
int logicalinstructions = 0;
int shiftinstructions = 0;
int memoryinstructions = 0;
int loadimmediateinstructions = 0;
int controlinstructions = 0;
int haltinstructions = 0;

string inttohex(int x)
{
  x %= 256;
  string ans = "";
  while (x)
  {
    int rem = x % 16;
    if (rem < 10)
    {
      ans += (rem + '0');
    }
    else
    {
      ans += (rem - 10 + 'a');
    }
    x /= 16;
  }
  if (ans.size() == 1)
  {
    ans += '0';
  }
  reverse(ans.begin(), ans.end());
  return ans;
}

char bh(string s)
{
  if (s == "0000")
    return '0';
  if (s == "0001")
    return '1';
  if (s == "0010")
    return '2';
  if (s == "0011")
    return '3';
  if (s == "0100")
    return '4';
  if (s == "0101")
    return '5';
  if (s == "0110")
    return '6';
  if (s == "0111")
    return '7';
  if (s == "1000")
    return '8';
  if (s == "1001")
    return '9';
  if (s == "1010")
    return 'a';
  if (s == "1011")
    return 'b';
  if (s == "1100")
    return 'c';
  if (s == "1101")
    return 'd';
  if (s == "1110")
    return 'e';
  if (s == "1111")
    return 'f';
  return '0';
}

string BintoHex(string bin)
{
  if (bin.length() != 16)
  {

    return "|";
  }
  string upper = bin.substr(0, 4);
  string lower = bin.substr(4, 4);
  string upper2 = bin.substr(8, 4);
  string lower2 = bin.substr(12, 4);

  string ans = "";
  ans.push_back(bh(upper));
  ans.push_back(bh(lower));
  ans.push_back(bh(upper2));
  ans.push_back(bh(lower2));
  return ans;
}

string hexa_binary4(string hexa)
{
  if (hexa.length() != 4)
  {
    cout << hexa << "Invalid Hexadecimal String" << endl;
    return "";
  }
  string binary = "";
  for (int i = 0; i < 4; i++)
  {
    int temp;
    if (hexa[i] >= '0' && hexa[i] <= '9')
    {
      temp = hexa[i] - '0';
    }
    else
    {
      temp = hexa[i] - 'a' + 10;
    }
    for (int j = 3; j >= 0; j--)
    {
      if (temp & (1 << j))
      {
        binary.push_back('1');
      }
      else
      {
        binary.push_back('0');
      }
    }
  }
  return binary;
}
int bintodeci(string s)
{
  int ans = 0;
  for (int i = 0; i < s.length(); i++)
  {
    ans = ans * 2 + (s[i] - '0');
  }
  return ans;
}
class Instruction
{
public:
  int rs1;
  int rs2;
  int rd;
  int imm;
  string op;
  string opcode;
  string instr;
  int ALUout;
  int LMD;

  Instruction()
  {
    rs1 = -1;
    rs2 = -1;
    rd = -1;
    imm = -1;
    op = "";
    ALUout = -1;
  }

  void fetchInstr(int &pc)
  {
    string instr = Icache[pc / 2];
    op = hexa_binary4(instr);
    pc = pc + 2;
  }

  void decode(int &pc)
  {
    string istr_name = op.substr(0, 4);
    if (istr_name == "0000")
    {
      instr = "ADD";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0001")
    {
      instr = "SUB";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0010")
    {
      instr = "MUL";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0011")
    {
      instr = "INC";
      rd = bintodeci(op.substr(4, 4));
      rs1 = rd;
    }
    if (istr_name == "0100")
    {
      instr = "AND";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0101")
    {
      instr = "OR";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0110")
    {
      instr = "XOR";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      rs2 = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "0111")
    {
      instr = "NOT";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
    }
    if (istr_name == "1000")
    {
      instr = "SLLI";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      imm = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "1001")
    {
      instr = "SRLI";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      imm = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "1010")
    {
      instr = "LI";
      rd = bintodeci(op.substr(4, 4));
      imm = bintodeci(op.substr(8, 8));
    }
    if (istr_name == "1011")
    {
      instr = "LD";
      rd = bintodeci(op.substr(4, 4));
      rs1 = bintodeci(op.substr(8, 4));
      imm = bintodeci(op.substr(12, 4));
    }
    if (istr_name == "1100")
    {
      instr = "ST";
      rs1 = bintodeci(op.substr(4, 4));
      rs2 = bintodeci(op.substr(8, 4));
      imm = bintodeci(op.substr(12, 4));
      // cout<<rs1<<endl;
    }
    if (istr_name == "1101")
    {
      instr = "JMP";
      imm = bintodeci(op.substr(4, 8));
    }
    if (istr_name == "1110")
    {
      instr = "BEQZ";
      rs1 = bintodeci(op.substr(4, 4));
      imm = bintodeci(op.substr(8, 8));
    }
    if (istr_name == "1111")
    {
      instr = "HLT";
    }
  }

  void execute(int &pc)
  {
    // totalinstructions++;
    if (instr == "ADD")
    {
      ALUout = RegisterFile[rs1] + RegisterFile[rs2];
      ALUout %= 256;
      arithmeticinstructions++;
    }
    if (instr == "SUB")
    {
      ALUout = RegisterFile[rs1] - RegisterFile[rs2];
      if (ALUout < 0)
      {
        ALUout += 256;
      }
      else
      {
        ALUout %= 256;
      }
      arithmeticinstructions++;
    }
    if (instr == "MUL")
    {
      ALUout = RegisterFile[rs1] * RegisterFile[rs2];
      ALUout %= 256;
      arithmeticinstructions++;
    }
    if (instr == "INC")
    {
      ALUout = RegisterFile[rs1] + 1;
      ALUout %= 256;
      arithmeticinstructions++;
    }
    if (instr == "AND")
    {
      ALUout = RegisterFile[rs1] & RegisterFile[rs2];
      logicalinstructions++;
    }
    if (instr == "OR")
    {
      ALUout = RegisterFile[rs1] | RegisterFile[rs2];
      logicalinstructions++;
    }
    if (instr == "XOR")
    {
      ALUout = RegisterFile[rs1] ^ RegisterFile[rs2];
      logicalinstructions++;
    }
    if (instr == "NOT")
    {
      ALUout = 256 - RegisterFile[rs1];
      logicalinstructions++;
    }
    if (instr == "SLLI")
    {
      ALUout = (RegisterFile[rs1]) << imm;
      ALUout %= 256;
      shiftinstructions++;
    }
    if (instr == "SRLI")
    {
      ALUout = (RegisterFile[rs1]) >> imm;
      ALUout %= 256;
      shiftinstructions++;
    }
    if (instr == "LI")
    {
      ALUout = imm;
      loadimmediateinstructions++;
    }
    if (instr == "LD")
    {
      if (imm >= 8)
      {
        imm -= 16;
      }
      ALUout = RegisterFile[rs1] + imm;
      ALUout %= 256;
      memoryinstructions++;
    }
    if (instr == "ST")
    {
      if (imm >= 8)
      {
        imm -= 16;
      }
      ALUout = RegisterFile[rs2] + imm;
      ALUout %= 256;
      memoryinstructions++;
    }
    if (instr == "JMP")
    {
      // if (imm >= 128)
      // {
      //   imm -= 256;
      // }
      pc = pc + 2 * imm; // look at 2 compliment
      pc %= 256;
      controlinstructions++;
    }
    if (instr == "BEQZ")
    {
      if (RegisterFile[rs1] == 0)
      {
        // if (imm >= 128)
        // {
        //   imm -= 256;
        // }
        pc = pc + 2 * imm;
        pc %= 256;
      }
      controlinstructions++;
    }
  }

  void memory(int &pc)
  {
    if (instr == "LD")
    {
      string temp = "00";
      if (ALUout < 0)
      {
        ALUout += 256;
      }
      temp += (Dcache[ALUout]);
      // cout<<temp<<endl;
      LMD = bintodeci(hexa_binary4(temp));
    }
    if (instr == "ST")
    {
      if (RegisterFile[rs1] < 0)
      {
        RegisterFile[rs1] = 256 + RegisterFile[rs1];
      }
      Dcache[ALUout] = inttohex(RegisterFile[rs1]);
      // cout << ALUout << " " << RegisterFile[rd] << endl;
    }
  }

  void WriteBack(int &pc)
  {
    if (instr == "ADD" || instr == "SUB" || instr == "MUL" || instr == "INC" || instr == "AND" || instr == "OR" || instr == "XOR" || instr == "NOT" || instr == "SLLI" || instr == "SRLI" || instr == "LI")
    {
      RegisterFile[rd] = ALUout;
    }
    if (instr == "LD")
    {
      RegisterFile[rd] = LMD;
    }
  }
  void clear()
  {
    rs1 = -1;
    rs2 = -1;
    rd = -1;
    imm = -1;
    op = "";
    ALUout = -1;
    instr = "";
  }
};

vector<int> registerStat(16, 0);
int jumpstatus = 0;
int khatam = 0;
int32_t main(int argc, char *argv[])
{

  bool stall = 0;
  int pc = 0;

  Instruction Fetch_stage;
  Instruction Decode_stage;
  Instruction Execute_stage;
  Instruction Memory_stage;
  Instruction WriteBack_stage;

  ifstream ifile(argv[1]);
  ifstream dfile(argv[2]);
  ifstream rfile(argv[3]);
  string line;
  ofstream ofile(argv[4], ios::out);

  int i = 0;
  while (i < 256)
  {
    ifile >> line;
    Icache[i / 2] += line;
    i++;
  }
  i = 0;
  while (i < 256)
  {
    dfile >> line;
    Dcache[i] += line;
    i++;
  }
  i = 0;
  while (i < 16)
  {
    rfile >> line;
    // 2bit hexa to decimal
    string temp = "00";
    temp += line;
    RegisterFile[i] = bintodeci(hexa_binary4(temp));
    // if(RegisterFile[i]>=128){
    //   RegisterFile[i]-=256;
    // }
    i++;
  }
  // for (int i = 0; i < 128; i++)
  // {
  //   cout << Icache[i] << endl;
  // }
  while (pc < 256)
  {
    // RegisterFile[0] = 0;
    totalcycles++;
    // if(totalcycles>200){
    //   break;
    // }
    cout << BintoHex(Fetch_stage.op) << " " << BintoHex(Decode_stage.op) << " " << BintoHex(Execute_stage.op) << " " << BintoHex(Memory_stage.op) << " " << BintoHex(WriteBack_stage.op) << endl;
    //  cout << "hi " << endl;
    WriteBack_stage = Memory_stage;
    if (WriteBack_stage.op != "")
    {
      WriteBack_stage.WriteBack(pc);
      if (WriteBack_stage.instr == "HLT")
      {
        break;
      }
      if (WriteBack_stage.rd >= 0)
        registerStat[WriteBack_stage.rd] = max(registerStat[WriteBack_stage.rd] - 1, 0);
    }

    Memory_stage = Execute_stage;
    if (Memory_stage.op != "")
    {
      if (jumpstatus == 1 && (Memory_stage.instr == "BEQZ" || Memory_stage.instr == "JMP"))
      {
        jumpstatus = 0;
      }
      Memory_stage.memory(pc);
    }

    Execute_stage = Decode_stage;
    if (Execute_stage.op != "")
    {
      Execute_stage.execute(pc);
    }

    Decode_stage = Fetch_stage;
    if (Decode_stage.op != "")
    {
      Decode_stage.decode(pc);
      if (Decode_stage.instr == "HLT")
      {
        haltinstructions++;
        khatam = 1;
        Fetch_stage.clear();
      }
      if (Decode_stage.instr == "BEQZ" && registerStat[Decode_stage.rs1] >= 1)
      {
        Decode_stage.clear();
        stalls++;
        datastalls++;
        continue;
      }
      if ((Decode_stage.instr == "BEQZ" && registerStat[Decode_stage.rs1] == 0) || Decode_stage.instr == "JMP")
      {
        jumpstatus = 1;
        Fetch_stage.clear();
        controlstalls += 2;
        stalls += 2;
        // Decode_stage.clear();
        continue;
      }

      if ((Decode_stage.rs1 >= 0 && registerStat[Decode_stage.rs1] == 1) || (Decode_stage.rs2 >= 0 && registerStat[Decode_stage.rs2] == 1))
      {
        Decode_stage.clear();
        stalls++;
        datastalls++;
        continue;
      }
      if (Decode_stage.rd >= 0)
        registerStat[Decode_stage.rd]++;
    }
    if (jumpstatus != 1 && khatam != 1)
      Fetch_stage.fetchInstr(pc);

    // cout<<Fetch_stage.op<<" "<<Decode_stage.op<<" "<<Execute_stage.op<<" "<<Memory_stage.op<<" "<<WriteBack_stage.op<<endl;
  }

  // Total number of instructions executed        : 72
  // Number of instructions in each class
  // Arithmetic instructions                      : 17
  // Logical instructions                         : 0
  // Shift instructions                           : 0
  // Memory instructions                          : 32
  // Load immediate instructions                  : 5
  // Control instructions                         : 17
  // Halt instructions                            : 1
  // Cycles Per Instruction                       : 1.77778
  // Total number of stalls                       : 52
  // Data stalls (RAW)                            : 18
  // Control stalls                               : 34

  totalinstructions = arithmeticinstructions + logicalinstructions + shiftinstructions + memoryinstructions + loadimmediateinstructions + controlinstructions + haltinstructions;
  ofile << "Total number of instructions executed        : " << totalinstructions << endl;
  ofile << "Number of instructions in each class     " << endl;
  ofile << "Arithmetic instructions                      : " << arithmeticinstructions << endl;
  ofile << "Logical instructions                         : " << logicalinstructions << endl;
  ofile << "Shift instructions                           : " << shiftinstructions << endl;
  ofile << "Memory instructions                          : " << memoryinstructions << endl;
  ofile << "Load immediate instructions                  : " << loadimmediateinstructions << endl;
  ofile << "Control instructions                         : " << controlinstructions << endl;
  ofile << "Halt instructions                            : " << haltinstructions << endl;
  ofile << "Cycles Per Instruction                       : " << (float)totalcycles / totalinstructions << endl;
  ofile << "Total number of stalls                       : " << stalls << endl;
  ofile << "Data stalls (RAW)                            : " << datastalls << endl;
  ofile << "Control stalls                               : " << controlstalls << endl;

  // write from Dcache vector to file Dcache.txt to overwrite the file
  ofstream ofile1(argv[5], ios::out);
  for (int i = 0; i < 256; i++)
  {
    ofile1 << Dcache[i] << endl;
  }

  // ofile.close();
  return 0;
}
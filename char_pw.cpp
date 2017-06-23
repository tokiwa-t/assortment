#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

int read(string &str, char *arg[], vector< vector<char> > &mem){
  int i=0,j;
  ifstream ifs(arg[2]);
  if (ifs.fail()){
    cerr << "Failed." << endl;
    exit(-1);
  }
  while (true){
    getline(ifs, str);
    if(ifs.eof())
      break;
    //cout << str << endl;
    for(j=0;str[j]!='\0';j++){
      mem[i].push_back(str[j]);
    }
    mem[i].push_back('\0');
    i++;
  }
  ifs.close();
  return i;
}

void encode(int &count, char *arg[], vector<vector<char> > &mem, vector<vector<char> > &enc){
  int i,j,n;
  for(i=0;i<count;i++){
    for(j=0,n=(int)(arg[5][0]%4);mem[i][j]!='\0';j++,++n){
      if(n==0)
	enc[i].push_back(mem[i][j]+(arg[5][n]%5));
	enc[i][j] += (arg[5][3-n]%7);
      if(n==1)
	enc[i].push_back(mem[i][j]-(arg[5][n]%7));
	enc[i][j] -= (arg[5][3-n]%5);
      if(n==2)
	enc[i].push_back(mem[i][j]+(arg[5][n]%7));
	enc[i][j] += (arg[5][3-n]%5);
      if(n==3){
	enc[i].push_back(mem[i][j]-(arg[5][n]%5));
	enc[i][j] -= (arg[5][3-n]%7);
	n=-1;
      }
      //enc[i].push_back(mem[i][j]+2);
    }
    enc[i].push_back('\0');
  }
}

void decode(int &count, char *arg[], vector<vector<char> > &mem, vector<vector<char> > &dec){
  int i,j,n;
  for(i=0;i<count;i++){
    for(j=0,n=(int)(arg[5][0]%4);mem[i][j]!='\0';j++,++n){
      if(n==0)
	dec[i].push_back(mem[i][j]-(arg[5][n]%5));
	dec[i][j] -= (arg[5][3-n]%7);
      if(n==1)
	dec[i].push_back(mem[i][j]+(arg[5][n]%7));
	dec[i][j] += (arg[5][3-n]%5);
      if(n==2)
	dec[i].push_back(mem[i][j]-(arg[5][n]%7));
	dec[i][j] -= (arg[5][3-n]%5);
      if(n==3){
	dec[i].push_back(mem[i][j]+(arg[5][n]%5));
	dec[i][j] += (arg[5][3-n]%7);
	n=-1;
      }
      //dec[i].push_back(mem[i][j]-2);
    }
    dec[i].push_back('\0');
  }
}

void show(int &count, char *arg[], vector<vector<char> > &tmp){
  int i,j;
  ofstream outputfile(arg[3]);
  for(i=0;i<count;i++){
      for(j=0;tmp[i][j]!='\0';j++){
	cout << tmp[i][j]; outputfile << tmp[i][j];
      }
      cout << endl; outputfile << "\n";
    }
  cout << endl; outputfile << "\n";
  outputfile.close();
}
  
  

int main(int argc, char *argv[]){
  string str;
  vector< vector<char> > mem;
  mem.reserve(3000);
  int count;

  count = read(str, argv, mem);

  if(argv[1][1]=='e'&&argv[4][1]=='p'){
    vector< vector<char> > enc;
    enc.reserve(3000);
    encode(count, argv, mem, enc);
    show(count, argv, enc);
    
  }

  if(argv[1][1]=='d'&&argv[4][1]=='p'){
    vector< vector<char> > dec;
    dec.reserve(3000);
    decode(count, argv, mem, dec);
    show(count, argv, dec);
  }
  return 0;
}

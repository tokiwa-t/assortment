#include<iostream>
using namespace std;

void encode(int *c, char *arg[], char enc[][128]){
  int i,j,k;
  for(i=1;i<*c;i++){
    for(j=0,k=0;arg[i][j]!='\0';j++){
      cout << arg[i][j];

      // If Small To Large
      if(arg[i][j]>=97&&arg[i][j]<=122)
	arg[i][j] = arg[i][j]-32;

      // Encode
      if(arg[i][j]=='A'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='B'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='C'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='D'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='E'){
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='F'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='G'){
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='H'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='I'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='J'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='K'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='L'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='M'){
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='N'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='O'){
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='P'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='Q'){
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='R'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='S'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='T'){
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='U'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='V'){
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='W'){
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='X'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='Y'){
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]=' ';
      }else if(arg[i][j]=='Z'){
	enc[i][k++]='-';
	enc[i][k++]='-';
	enc[i][k++]='+';
	enc[i][k++]='+';
	enc[i][k++]=' ';
      }
    }
    cout << " ";
  }
  cout << endl;
}

void decode(int *c, char *arg[], char dec[]){
  int i,j,k;
  char tmp[4];

  for(i=1;i<*c;i++){

    // All Reset
    for(k=0;k<4;k++){
      tmp[k] = '\0';
    }


    // Load Morse
    for(j=0;arg[i][j]!='\0';j++){
      tmp[j] = arg[i][j];
    }

    // Write Character
    if(j==2&&tmp[0]=='+'&&tmp[1]=='-')
      dec[i] = 'A';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='+'&&tmp[3]=='+')
      dec[i] = 'B';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='-'&&tmp[3]=='+')
      dec[i] = 'C';
    else if(j==3&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='+')
      dec[i] = 'D';
    else if(j==1&&tmp[0]=='+')
      dec[i] = 'E';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='+'&&tmp[2]=='-'&&tmp[3]=='+')
      dec[i] = 'F';
    else if(j==3&&tmp[0]=='-'&&tmp[1]=='-'&&tmp[2]=='+')
      dec[i] = 'G';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='+'&&tmp[2]=='+'&&tmp[3]=='+')
      dec[i] = 'H';
    else if(j==2&&tmp[0]=='+'&&tmp[1]=='+')
      dec[i] = 'I';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='-'&&tmp[2]=='-'&&tmp[3]=='-')
      dec[i] = 'J';
    else if(j==3&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='-')
      dec[i] = 'K';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='-'&&tmp[2]=='+'&&tmp[3]=='+')
      dec[i] = 'L';
    else if(j==2&&tmp[0]=='-'&&tmp[1]=='-')
      dec[i] = 'M';
    else if(j==2&&tmp[0]=='-'&&tmp[1]=='+')
      dec[i] = 'N';
    else if(j==3&&tmp[0]=='-'&&tmp[1]=='-'&&tmp[2]=='-')
      dec[i] = 'O';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='-'&&tmp[2]=='-'&&tmp[3]=='+')
      dec[i] = 'P';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='-'&&tmp[2]=='+'&&tmp[3]=='-')
      dec[i] = 'Q';
    else if(j==3&&tmp[0]=='+'&&tmp[1]=='-'&&tmp[2]=='+')
      dec[i] = 'R';
    else if(j==3&&tmp[0]=='+'&&tmp[1]=='+'&&tmp[2]=='+')
      dec[i] = 'S';
    else if(j==1&&tmp[0]=='-')
      dec[i] = 'T';
    else if(j==3&&tmp[0]=='+'&&tmp[1]=='+'&&tmp[2]=='-')
      dec[i] = 'U';
    else if(j==4&&tmp[0]=='+'&&tmp[1]=='+'&&tmp[2]=='+'&&tmp[3]=='-')
      dec[i] = 'V';
    else if(j==3&&tmp[0]=='+'&&tmp[1]=='-'&&tmp[2]=='-')
      dec[i] = 'W';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='+'&&tmp[3]=='-')
      dec[i] = 'X';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='+'&&tmp[2]=='-'&&tmp[3]=='-')
      dec[i] = 'Y';
    else if(j==4&&tmp[0]=='-'&&tmp[1]=='-'&&tmp[2]=='+'&&tmp[3]=='+')
      dec[i] = 'Z';
  }
}

int main(int argc, char *argv[]){
  
  int i,j;
  char enc[argc][128];
  char dec[128];

  if(argv[1][0]>=65&&argv[1][0]<=122){

    // Send Encoder
    encode(&argc, argv, enc);

    // Show
    for(i=0;i<argc;i++){
      for(j=0;enc[i][j]!='\0';j++){
	cout << enc[i][j];
      }
    }
    cout << endl;
  }else if(argv[1][0]=='+'||argv[1][0]=='-'){
    
    //Send Decoder
    decode(&argc, argv, dec);

    // Show
    for(i=1;i<argc;i++){
      cout << dec[i] << " ";
    }
    cout << endl;
  }else{
    cout << "ERROR" << endl;
  }
  
  return 0;
}

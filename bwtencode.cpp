#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>

char delimiter;

struct suffix
{
  int index;
  int rank[2];
  bool operator<(suffix other) const
  {
    return (rank[0] == other.rank[0])? (rank[1] < other.rank[1] ? 1:0) : (rank[0]<other.rank[0]? 1:0);
  }
};

void bwtencode_string_wise(char* text, int length)
{
  struct suffix suffixes[length];

  for(int i=0; i<length; i++)
  {
    suffixes[i].index = i;
    suffixes[i].rank[0] = text[i] - 'a';
    suffixes[i].rank[1] = ((i+1) < length) ? (text[i+1] - 'a'): -1;
  }

  std::sort(suffixes, suffixes+length);

  int ind[length];

  for (int k = 4; k < 2*length; k = k*2)
  {
      int rank = 0;
      int prev_rank = suffixes[0].rank[0];
      suffixes[0].rank[0] = rank;
      ind[suffixes[0].index] = 0;

      for (int i = 1; i < length; i++)
      {
          if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i-1].rank[1])
          {
              prev_rank = suffixes[i].rank[0];
              suffixes[i].rank[0] = rank;
          }
          else
          {
              prev_rank = suffixes[i].rank[0];
              suffixes[i].rank[0] = ++rank;
          }
          ind[suffixes[i].index] = i;
      }

      for (int i = 0; i < length; i++)
      {
          int nextindex = suffixes[i].index + k/2;
          suffixes[i].rank[1] = (nextindex < length)? suffixes[ind[nextindex]].rank[0]: -1;
      }
      std::sort(suffixes, suffixes+length);
    }

    int *suffixArr = new int[length];
    for (int i = 0; i < length; i++)
      suffixArr[i] = suffixes[i].index;

    char *bwtCode = new char[length];

    for(int i=0; i<length; i++) bwtCode[i] = text[i];
    for(int i=0; i<length; i++)
    {
      if(suffixArr[i]>0)
        text[i] = bwtCode[suffixArr[i] - 1];
      else text[i] = delimiter;
    }
    //printf("%s\n",text );
}




char *bwtCode = new char[50000004];
struct suffix suffixes[50000004];
int ind[50000004];
int *suffixArr = new int[50000004];


void bwtencode_all_text(char* text, int length)
{


  for(int i=0; i<length; i++)
  {
    suffixes[i].index = i;
    suffixes[i].rank[0] = text[i] - 'a';
    suffixes[i].rank[1] = ((i+1) < length) ? (text[i+1] - 'a'): -1;
  }

  std::sort(suffixes, suffixes+length);



  for (int k = 4; k < 2*length; k = k*2)
  {
      int rank = 0;
      int prev_rank = suffixes[0].rank[0];
      suffixes[0].rank[0] = rank;
      ind[suffixes[0].index] = 0;

      for (int i = 1; i < length; i++)
      {
          if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i-1].rank[1])
          {
              prev_rank = suffixes[i].rank[0];
              suffixes[i].rank[0] = rank;
          }
          else
          {
              prev_rank = suffixes[i].rank[0];
              suffixes[i].rank[0] = ++rank;
          }
          ind[suffixes[i].index] = i;
      }

      for (int i = 0; i < length; i++)
      {
          int nextindex = suffixes[i].index + k/2;
          suffixes[i].rank[1] = (nextindex < length)? suffixes[ind[nextindex]].rank[0]: -1;
      }
      std::sort(suffixes, suffixes+length);
    }


    for (int i = 0; i < length; i++)
      suffixArr[i] = suffixes[i].index;

    for(int i=0; i<length; i++) bwtCode[i] = text[i];
    for(int i=0; i<length; i++)
    {
      if(suffixArr[i]>0)
        text[i] = bwtCode[suffixArr[i] - 1];
      else text[i] = delimiter;
    }
    //printf("%s\n",text );
}











void encode_string_wise(int args, char* argv[])
{
  FILE* infile = fopen(argv[3],"r");
  std::ofstream outfile (argv[4]);
  if(!outfile.is_open())
    printf("Error in opening encoded file.\n");
  else
  {
    char c[1], myString[5000];
    delimiter = argv[1][0];
    int idx = 0;
    while( (*c = fgetc(infile) ) != EOF)
    {
      if(*c == ' ') continue;
      myString[idx++]=*c;
      if(*c == delimiter)
      {
        myString[idx]='\0';
        if(idx > 1)
        {
          bwtencode(myString, idx);
          outfile << myString;
        }
        idx=0;
      }
    }
  }
  outfile.close();
  return;
}



char superstring[50000004];
void encode_all_text(int args, char* argv[])
{
  FILE* infile = fopen(argv[3],"r");
  std::ofstream outfile (argv[4]);
  if(!outfile.is_open())
    printf("Error in opening encoded file.\n");
  else
  {
    char c, myString[5000];
    delimiter = argv[1][0];
    int idx = 0;
    while( (c = getc(infile) ) != EOF)
    {
      superstring[idx++] = c;
    }
    printf("%s\n","I got till here." );
    bwtencode2(superstring, idx);
    printf("%s\n","I got till here too." );
    outfile << superstring;
  }
  outfile.close();
  return;
}

int main(int args, char* argv[])
{
  if(args != 5)
    printf("Incorrect number of arguments.\n");
  else
    encode_all_text(args, argv);

  return 0;
}

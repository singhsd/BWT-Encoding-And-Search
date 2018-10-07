// includes
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <set>

// defines
#define block_elements 98
#define uint unsigned int
#define block_size (block_elements * sizeof(unsigned int))

/*
Make a custom index class that optimizes the search procedure
*/

class Index
{
  // private variables
  uint char_table[block_elements + 1];
  uint char_mapping[128];
  // private methods
  void initialize_char_map()
  {
    char_mapping[9]=0;
    char_mapping[10]=1;
    char_mapping[13]=2;
    for(unsigned char c=3; c<=block_elements; c++)
      char_mapping[c+29]=c;
    char_mapping[11]=char_mapping[12]=2;
    for(unsigned char c=14; c<32; c++) char_mapping[c]=3;
  }

  public:
  // public variables
  const char *index_file;

  // public methods
  Index(const char* file_name, const char* index_file_name)
  {
    index_file = index_file_name;
    std::ifstream in(file_name);
    std::ifstream idx_in(index_file, std::ifstream::binary);

    initialize_char_map();

    uint blocks_written;
    if(!idx_in.good())
    {
      idx_in.close();
      blocks_written = create_occ_index(in);
      idx_in.open(index_file, std::ifstream::binary);
    }
    else
    {
      idx_in.seekg(0, idx_in.end);
      blocks_written = idx_in.tellg() / block_size;
    }

    generate_char_table(blocks_written, idx_in, in);
  }

  uint occ (unsigned char c, uint q, std::istream& in, std::istream& idx_in)
  {
    uint block_offset = q/block_size;
    unsigned char char_index = char_mapping[c];
    uint instances = 0;
    uint i = 0;

    if(block_offset)
    {
      idx_in.seekg((block_offset-1)*block_size + (char_index * sizeof(uint)), idx_in.beg);
      idx_in.read((char*) &instances, sizeof(uint));
      in.seekg(block_offset*block_size, in.beg);
      i = block_offset * block_size;
    }
    else in.seekg(0, in.beg);

    char read_char;
    for(; i<q; i++)
    {
      if(!in.get(read_char)) break;
      if(c == read_char) instances++;
    }

    return instances;
  }

uint occ(unsigned char c, uint q, const char* file_name, std::istream& idx_in, uint ii, uint diff)
  {
	std::ifstream in(file_name,std::ifstream::binary);
    uint block_offset = q/block_size;
    //unsigned char char_index = char_mapping[c];
    uint instances = ii;
    uint i = 0;
    uint yy=q;

    if(block_offset)
    {
    	if(diff == 22)//22 isjust a random number for checking
    		in.seekg(0);
    	else
    	{	in.seekg(diff);
    		yy = q-diff;
    }}

    else{
    	in.seekg(0, in.beg);
    	diff = q;
    }

    char read_char;
    for(; i<yy; i++)
    {
    	if(!in.get(read_char)) break;
    	if(c == read_char) instances++;
    }
    yy = q;
    return instances;
  }

  uint get_char(unsigned char c) { return char_table[ char_mapping[ c ] ]; }
  uint create_occ_index(std::istream& in)
  {
    uint index_array[block_elements] = {0};
    uint blocks_written = 0;
    std::ofstream out(index_file, std::ofstream::trunc | std::ofstream::binary);

    unsigned char char_index;
    char file_buffer[block_size];
    while(1)
    {
      in.read(file_buffer, block_size);
      if(in.gcount() == block_size)
      {
        for(int i=0; i<block_size; i++)
        {
          char_index = char_mapping[file_buffer[i]];
          index_array[char_index]++;
        }
        write_block_to_index(index_array, out);
        blocks_written++;
      }
      else break;
    }
    // in.close();
    out.close();
    return blocks_written;
  }
  void write_block_to_index(uint index_array[block_elements], std::ofstream& out)
  {
      out.write((char*)index_array, block_size);
  }
  void generate_char_table(uint blocks_written, std::istream& idx_in, std::istream& in)
  {
    std::memset(char_table, 0, sizeof char_table);
    uint instances = 0;
    if(blocks_written)
    {
      idx_in.seekg((blocks_written-1) * block_size, idx_in.beg);
      idx_in.read((char*) char_table + sizeof(uint), block_size - sizeof(uint));
    }
    in.seekg(blocks_written * block_size, in.beg);

    char c;
    while(in.get(c))
    {
      char_table[ char_mapping[c] + 1]++;
    }
    for(int i=1; i<=block_elements; i++)
      char_table[i]+=char_table[i-1];
  }
};

/*
structure to query search results
*/

struct search_result
{
  uint first, last;
};

/*
utility functions
*/

search_result backward_search(const char* pattern, const char* file_name, Index* index)
{
  search_result result;

  std::ifstream in(file_name,std::ifstream::binary);
  std::ifstream idx_in(index->index_file, std::ifstream::binary);
  uint i = std::strlen(pattern) - 1;
  unsigned char c = pattern[i];
  result.first = index->get_char(c);
  result.last = index->get_char(c)-1;
  uint diff = result.last-result.first;
  uint instances=0;

  while((result.first <= result.last)&&i)
  {
	 uint rr = result.first;
     c = pattern[i-1];
     instances = index->occ(c, result.first, file_name, idx_in, instances, 22);
     result.first = index->get_char(c) + instances;
     result.last = index->get_char(c) + index->occ(c, result.last, file_name, idx_in, instances, rr) -1;
	 diff = result.last-result.first;
     i = i-1;
     instances = 0;
  }

  idx_in.close();
  in.close();
  return result;
}

std::set<uint>* find_records(search_result result, const char* file_name, Index* index)
{
  std::ifstream in(file_name);
  std::ifstream idx_in(index->index_file);

  uint record, scale, next;
  bool reached_offset;
  char c = 0;

  std::set<uint>* records = new std::set<uint>();
  for(uint i=result.first; i<=result.last; i++)
  {
    reached_offset = false;
    record = 0;
    scale = 1;
    in.seekg(i, in.beg);
    in.get(c);
    next = index->occ(c,i,in,idx_in) + index->get_char(c);
    while(c!='[')
    {
      if(c==']') reached_offset = true;
      else if(reached_offset)
      {
        record += (c-'0')*scale;
        scale *= 10;
      }
      in.seekg(next, in.beg);
      in.get(c);
      next = index->occ(c, next, in, idx_in) + index->get_char(c);
    }
    records->insert(record);
  }
  return records;
}

void interpret_results(search_result result, const char* mode, const char* file_name, Index* index)
{
  if(!std::strcmp(mode, "-n"))
  {
    if(result.last < result.first) printf("0\n");
    else printf("%d\n", result.last - result.first +1);
  }
  else
  {
    bool list_records = std::strcmp(mode, "-r");
    std::set<uint>* records;
    records = find_records(result, file_name, index);
    if(list_records)
    {
      for(std::set<uint>::iterator it = records->begin(); it!=records->end(); it++)
        printf("[%d]\n", *it);
    }
    else printf("%d\n", (uint)records->size());
  }
}

int main(int argc, char* argv[])
{
  if(argc <5) printf("Wrong number of arguments\n");
  else
  {
    const char* mode = argv[4];
    const char* file_name = argv[2];
    const char* index_file = argv[3];
    const char* pattern = argv[5];

    Index* index = new Index(file_name, index_file);
    search_result result = backward_search(pattern, file_name, index);
    interpret_results(result, mode, file_name, index);
  }
  return 0;
}


// for reference, visit https://github.com/Hagmar/BWT-bs

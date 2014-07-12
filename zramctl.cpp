/*
 * Developed by Timofey Titovets nefelim4ag@gmail.com
 * Licensed under GPL
 * zramctl simple tool to setup zram devices
 */

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

void help();
bool used(string path);
void reset(string name);

int main(int argc, char* argv[]) {
  int count=argc-1;
  string name = argv[1];
  if (count==0 || name == "--help" || name == "-h") help();
  if (count>=1) {
    string path="/sys/block/";
    if (name[0] == 'z') {
      path+=name;
    }
    if (name == "reset") {
      if (count==2) reset(argv[2]);
      else help();
      return 0;
    }
    if (name == "find") {
      for (int i=0; i<32; i++) {
        string path_disksize=path+"zram"+to_string(i)+"/disksize";
        if (!used(path_disksize)) {
          name="zram"+to_string(i);
          path+=name;
          cout << name+"\n";
          break;
        }
      }
    }
    if (count>4 ) help();
    if (count>=2) {
      reset(name);
      if (count==4) {
        string threads = argv[4];
        fstream max_comp_streams;
        string path_max_comp_streams=path+"/max_comp_streams";
        max_comp_streams.open(path_max_comp_streams);
        max_comp_streams << threads;
        max_comp_streams.close();
      }
      if (count>=3) {
        string alg = argv[3];
        fstream comp_algorithm;
        string path_comp_algorithm=path+"/comp_algorithm";
        comp_algorithm.open(path_comp_algorithm);
        comp_algorithm << alg;
        comp_algorithm.close();
      }
      string size = argv[2];
      fstream disksize;
      string path_disksize=path+"/disksize";
      disksize.open(path_disksize);
      disksize << size;
      disksize.close();
    }
  }
  return 0;
}

void help(){
  cout
  << "Usage: zramctl <name> <size <alg> <threads> \n"
  << "zramctl zram0 1024M lz4 4                   \n"
  << "zramctl find  1024M lz4 4                   \n"
  << "zramctl reset zram0                         \n"
  << "lzo|lz4    # compress algorithm             \n"
  << "*|{K|M|G}  # size of zram disk              \n"
  << "<name>     # zram* or find                  \n"
  << "           # if find, print and setup first free device\n";
}

bool used(string path) {
  ifstream disksize(path);
  string str((istreambuf_iterator<char>(disksize)), istreambuf_iterator<char>());
  if (str[0] == '0') return 0;
  else return 1;
}

void reset(string name) {
  ofstream reset;
  string path_reset="/sys/block/"+name+"/reset";
  reset.open(path_reset);
  reset << 1;
  reset.close();
}
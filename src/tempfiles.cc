#include <iostream>
#include <stack>
#include <stdlib.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include "tempfiles.hh"
#include <assert.h>
using namespace std;

string TEMPFILES::nextFileName() {
  stack<unsigned int>  names;
  char buf[100];
  string fullpath(this->tempdir + "/" + this->basedir);

  unsigned int i = this->count;
  unsigned int B = this->S;
  int r, f, d;

  r = i % B; 
  f = r;
  i = (i - r)/B;

  while( i > 0) {
    r = i % B; 
    d =  r-1 >=0 ? r-1 : B-1;
    names.push(d);
    i = (i - r)/B;
  }

  struct stat st;
  if(stat(fullpath.c_str(), &st)!=0 ) {
    mkdir(fullpath.c_str(), 0777);
  }

  while(!names.empty()) {
    fullpath = fullpath + string("/") + dirname(names.top());        

    if(stat(fullpath.c_str(), &st)!=0 ) {
      mkdir(fullpath.c_str(), 0777);
    }
    names.pop();
  }

  fullpath = fullpath + string("/") + filename(f);        

  this->filenames.push_back(fullpath);

  this->count++;

  return fullpath;  
}

unsigned int TEMPFILES::size() {
  return filenames.size();
}

string TEMPFILES::filename(unsigned int i)  {
  return string("file") + toString(i) ;
}

string TEMPFILES::dirname(unsigned int i)  {
  return string("dir") + toString(i);
}

string TEMPFILES::toString(unsigned int i ) {
  char buf[100];
  sprintf(buf, "%d",i);
  return string(buf);  
}


vector<string> TEMPFILES::getFileNames(){
  return this->filenames;
}

void TEMPFILES::clear(){
  char path[500];
  string fullpath(this->tempdir + "/" + this->basedir);
  strcpy(path, fullpath.c_str());
  remove_dir(path);
  this->count =0 ;
  this->filenames.clear();
}

void TEMPFILES::remove_dir(char *path){
  struct dirent *entry = NULL;
  DIR *dir = NULL;

  dir = opendir(path);

  if( dir ==0) return;
  while(entry = readdir(dir)){   
    DIR *sub_dir = NULL;
    FILE *file = NULL;
    char abs_path[200] = {0};
    if( strcmp(entry->d_name, ".")  && strcmp(entry->d_name, "..")){   
      sprintf(abs_path, "%s/%s", path, entry->d_name);

      if(sub_dir = opendir(abs_path)){//if it is a directory   
        closedir(sub_dir);
        remove_dir(abs_path);
      }else{   
        if(file = fopen(abs_path, "r")){   
          fclose(file);
          remove(abs_path);
        }else{
          remove(abs_path);
        }
      }   
    }   
  }   
  closedir(dir);
  remove(path);
}

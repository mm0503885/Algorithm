#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <limits.h>
using namespace std;

int main()
{
    ifstream fin;
    ofstream fout;
    fin.open("input.txt");
    fout.open("output.txt");
    char c,op;
    unsigned int i;
    while(fin.get(c))
    {
      unsigned int matrix_num=0;
      while(c>=48 && c<=57)
      {
        matrix_num*=10;
        i=c-'0';
        matrix_num+=i;
        fin.get(c);
      }
      for(int turn=0;turn<matrix_num;turn++)
      {
        unsigned int m=0,n=0,element=0,insert_data=0;
        bool compute_n=true;
        queue<unsigned int> insert_q,element_q;
        while(fin.get(c))
        {
          if(c>=48 && c<=57)
            break;
        }
        op=c;
        fin.get(c);
        if(op=='1')
        {
          while(fin.get(c))
          {
            if(c=='\n')
            {
              insert_q.push(insert_data);
              insert_data=0;
              break;
            }
            else if(c==' ')
            {
              insert_q.push(insert_data);
              insert_data=0;
            }
            else
            {
              insert_data*=10;
              i=c-'0';
              insert_data+=i;
            }
          }
        }
        while(fin.get(c))
        {
          if(c=='\n')
          {
            element_q.push(element);
            element=0;
            if(compute_n) n++;
            compute_n=false;
            m++;
            if(fin.peek()=='\n') break;
          }
          else if(c==' ')
          {
            element_q.push(element);
            element=0;
            if(compute_n) n++;
          }
          else if(c=='x')
          {
            element=UINT_MAX;
          }
          else
          {
            element*=10;
            i=c-'0';
            element+=i;
          }
        }
        unsigned int young_tableaus[m][n];
        for(unsigned int a=0;a<m;a++)
          for(unsigned int b=0;b<n;b++)
          {
            young_tableaus[a][b]=element_q.front();
            element_q.pop();
          }
        if(op=='1')
        {
          fout<<"Insert ";
          while(!insert_q.empty())
          {
            if(young_tableaus[m-1][n-1]!=UINT_MAX)
            {
              cout<<"young_tableaus full"<<endl;
              break;
            }
            unsigned int insert_pos_x=0,insert_pos_y=0;
            bool insert_find=false;
            for(unsigned int a=0;a<m;a++)
            {
              for(unsigned int b=0;b<n;b++)
              {
                if(young_tableaus[a][b]==UINT_MAX)
                {
                  insert_pos_x=a;
                  insert_pos_y=b;
                  insert_find=true;
                  break;
                }
              }
              if(insert_find)
                break;
            }
            young_tableaus[insert_pos_x][insert_pos_y]=insert_q.front();

            while(1)
            {
              if(insert_pos_x==0 && insert_pos_y==0) break;
              else if(insert_pos_x==0 && insert_pos_y!=0)
              {
                if(young_tableaus[insert_pos_x][insert_pos_y]<young_tableaus[insert_pos_x][insert_pos_y-1])
                {
                  swap(young_tableaus[insert_pos_x][insert_pos_y],young_tableaus[insert_pos_x][insert_pos_y-1]);
                  insert_pos_y-=1;
                }
                else
                  break;
              }
              else if(insert_pos_x!=0 && insert_pos_y==0)
              {
                if(young_tableaus[insert_pos_x][insert_pos_y]<young_tableaus[insert_pos_x-1][insert_pos_y])
                 {
                  swap(young_tableaus[insert_pos_x][insert_pos_y],young_tableaus[insert_pos_x-1][insert_pos_y]);
                  insert_pos_x-=1;
                }
                else
                  break;
              }
              else
              {
                if(young_tableaus[insert_pos_x][insert_pos_y]<young_tableaus[insert_pos_x-1][insert_pos_y]
                  && young_tableaus[insert_pos_x-1][insert_pos_y]>=young_tableaus[insert_pos_x][insert_pos_y-1])
                {
                  swap(young_tableaus[insert_pos_x][insert_pos_y],young_tableaus[insert_pos_x-1][insert_pos_y]);
                  insert_pos_x-=1;
                }
                else if(young_tableaus[insert_pos_x][insert_pos_y]<young_tableaus[insert_pos_x][insert_pos_y-1]
                  && young_tableaus[insert_pos_x][insert_pos_y-1]>young_tableaus[insert_pos_x-1][insert_pos_y])
                {

                  swap(young_tableaus[insert_pos_x][insert_pos_y],young_tableaus[insert_pos_x][insert_pos_y-1]);
                  insert_pos_y-=1;
                }
                else
                {
                  break;
                }
              }
            }
            fout<<insert_q.front();
            insert_q.pop();
            if(!insert_q.empty()) fout<<' ';
          }
          fout<<endl;
          for(unsigned int a=0;a<m;a++)
          {
            for(unsigned int b=0;b<n;b++)
            {
              if(young_tableaus[a][b]==UINT_MAX){
                fout<<"x";
                if(b!=n-1) fout<<' ';
              }
              else
              {
                fout<<young_tableaus[a][b];
                if(b!=n-1) fout<<' ';
              }
            }
            fout<<endl;
          }
       }
       else
       {
         fout<<"Extract-min "<<young_tableaus[0][0]<<endl;
         unsigned int delete_pos_x=0,delete_pos_y=0;
            bool delete_find=false;
            for(unsigned int a=0;a<m;a++)
            {
              for(unsigned int b=0;b<n;b++)
              {
                if((b==(n-1) && a==(m-1)) ||
                   (b==(n-1) && young_tableaus[a+1][b]==UINT_MAX) ||
                   (a==(m-1) && young_tableaus[a][b+1]==UINT_MAX) ||
                   (young_tableaus[a+1][b]==UINT_MAX && young_tableaus[a][b+1]==UINT_MAX))
                  {
                    delete_pos_x=a;
                    delete_pos_y=b;
                    delete_find=true;
                    break;
                  }
              }
              if(delete_find)
                break;
            }
         young_tableaus[0][0]=young_tableaus[delete_pos_x][delete_pos_y];
         young_tableaus[delete_pos_x][delete_pos_y]=UINT_MAX;
         delete_pos_x=0,delete_pos_y=0;
         while(1)
            {
              if(delete_pos_x==(m-1) && delete_pos_y==(n-1)) break;
              else if(delete_pos_x==(m-1) && delete_pos_y!=(n-1))
              {
                if(young_tableaus[delete_pos_x][delete_pos_y]>young_tableaus[delete_pos_x][delete_pos_y+1])
                {
                  swap(young_tableaus[delete_pos_x][delete_pos_y],young_tableaus[delete_pos_x][delete_pos_y+1]);
                  delete_pos_y+=1;
                }
                else
                  break;
              }
              else if(delete_pos_x!=(m-1) && delete_pos_y==(n-1))
              {
                if(young_tableaus[delete_pos_x][delete_pos_y]>young_tableaus[delete_pos_x+1][delete_pos_y])
                 {
                  swap(young_tableaus[delete_pos_x][delete_pos_y],young_tableaus[delete_pos_x+1][delete_pos_y]);
                  delete_pos_x+=1;
                }
                else
                  break;
              }
              else
              {
                if(young_tableaus[delete_pos_x][delete_pos_y]>young_tableaus[delete_pos_x+1][delete_pos_y]
                  && young_tableaus[delete_pos_x+1][delete_pos_y]<=young_tableaus[delete_pos_x][delete_pos_y+1])
                {
                  swap(young_tableaus[delete_pos_x][delete_pos_y],young_tableaus[delete_pos_x+1][delete_pos_y]);
                  delete_pos_x+=1;
                }
                else if(young_tableaus[delete_pos_x][delete_pos_y]>young_tableaus[delete_pos_x][delete_pos_y+1]
                  && young_tableaus[delete_pos_x][delete_pos_y+1]<young_tableaus[delete_pos_x+1][delete_pos_y])
                {

                  swap(young_tableaus[delete_pos_x][delete_pos_y],young_tableaus[delete_pos_x][delete_pos_y+1]);
                  delete_pos_y+=1;
                }
                else
                {
                  break;
                }
              }
            }
         for(unsigned int a=0;a<m;a++)
          {
            for(unsigned int b=0;b<n;b++)
            {
              if(young_tableaus[a][b]==UINT_MAX){
                fout<<"x";
                if(b!=n-1) fout<<' ';
              }
              else
              {
                fout<<young_tableaus[a][b];
                if(b!=n-1) fout<<' ';
              }
            }
            fout<<endl;
          }

       }
         fout<<endl;
    }
    fin.get(c);
  }
   fin.close();
   fout.close();
   return 0;
}

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>

using namespace std;

class Travel
{
    public:
    bool *visited;
    int *routine;
    int length;

    Travel(int size)
    {
        visited = new bool[size];
        routine = new int[size];
        for(int i=0; i<size; ++i)
        {
            visited[i] = 1;
            routine[i] = 0;
        }
    }

    Travel Permute(int **dist, int size, const Travel oldone, int shortest)
    {// this function do the recursive travelling testing..
        cout<<"well here"<<endl;
        int sum=0;
        for(int i=0; i<size; ++i)// check how many cities had visited..
        { if(!oldone.visited[i]) sum++; }
        
        cout<<"sum = "<<sum<<" size = "<<size<<endl;
        if(sum == size)// all cities visited, time to get the distance..
        {
            Travel result(size);
            result = oldone;
            
            result.length = get_length(dist, result.routine, size);
            return oldone; 
        }
        else// loop all the unvisited cities..
        {
            Travel result(size);
            result = oldone;
        
            p(oldone.routine, oldone.visited, size);
            p(result.routine, result.visited, size);
            for(int i=0; i<size; ++i)// i is the number is city..
            {    
                if(oldone.visited[i] == 1)// select un-visited city..
                {
                    Travel newone(size);
                    p(oldone.routine, oldone.visited, size);
                    newone = oldone;
                    cout<<"after copy"<<endl;
                    p(newone.routine, newone.visited, size);
                    cout<<i<<" "<<sum<<endl;
                    newone.visited[i] = 0;//
                    newone.routine[sum] = i;
                    cout<<"after value"<<endl;
                    p(newone.routine, newone.visited, size);
                    if (shortest > Permute( dist, size, newone, shortest).length)
                    {    
                        shortest = get_length(dist, newone.routine, size);//Permute( dist, size, newone);
                        result = newone;
                    //    p(new_routine, new_visited, size);
                    }
                }
            }
            return result;
        }        
    }

    int get_length(int **dist, int *routine, int size)
    {
        int sum=0, X=0, Y=0;
        for(int i=0; i<size; ++i)
        {
            X = routine[i]; // X and Y are the number of cities, i is the rank of the visiting history..
            Y = routine[ (i+1)%size ];
            sum = sum + dist[X][Y];
        }
        return sum;
    }

    void p(int *r, bool *v, int size)
    {
        cout<<"routine: ";
        for(int i=0; i<size; ++i)
        cout<<r[i]<<" "; 
        
        cout<<endl;
        
        cout<<"visited: ";
        for(int i=0; i<size; ++i)
        cout<<v[i]<<" ";
        
        cout<<endl;
    }
    
    ~Travel()
    {
        delete[] visited;
        delete[] routine;
    }
}; 

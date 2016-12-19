#include<sys/time.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>

using namespace std;

int get_length(int **dist, int *visited, int size)
{
    int sum=0;
    for(int i=0; i<size; ++i)
    {
        int X=visited[i], Y; // X and Y are the number of cities, i is the rank of the visiting history..
        Y = visited[ (i+1)%size ];
        sum = sum + dist[X][Y];
    }
    return sum;
}

void print(int **dist, int size) // print the whole distance matrix out..
{
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        { cout<<dist[i][j]<<" "; }
        cout<<endl;
    }
}

void write_map(int **dist, int size) // write the map configuration into a file..
{
    ofstream map("map.dat");
    map<<size<<endl;
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        {
            map<<dist[i][j]<<" ";
        }
        map<<endl;
    }
    map.close();
}

void copy_v_r(bool *v_newone, bool *v_oldone, int *r_newone, int *r_oldone, int size)// copy routine and visiting history in to new.. 
{
    for(int i=0; i<size; ++i)
    {
        v_newone[i] = v_oldone[i];
        r_newone[i] = r_oldone[i];
    }
}

int Permute(int **dist, int size, bool *visited, int *routine, long int shortest)
{// this function do the recursive travelling testing..
    int sum=0;
    for(int i=0; i<size; ++i)// check how many cities had visited..
    { if(!visited[i]) sum++; }

    if(sum == size)// all cities visited, time to get the distance..
    {
        shortest = get_length(dist, routine, size);
    }
    else// loop all the unvisited cities..
    {
        bool *new_visited = new bool[size];
        int *new_routine = new int[size];
        for(int i=0; i<size; ++i)// i is the number is city..
        {
            if(visited[i] == 1)// select un-visited city..
            {
                copy_v_r(new_visited, visited, new_routine, routine, size);
                new_visited[i] = 0;//
                new_routine[sum] = i;
                if (shortest >  Permute( dist, size, new_visited, new_routine, shortest))
                    shortest = Permute( dist, size, new_visited, new_routine, shortest);
            }
        }
        delete[] new_visited;
        delete[] new_routine;
    }
    return shortest;
}

int main(int argc, char *argv[])
{
    int ch, size=0;
    opterr = 0;
    char *FileName;
    bool mode = 0; // 0 means generate a map, 1 means load a map.. 

    if(argc != 3)// if the input is not 1 pair of flag and parameter or 2..
    {
        cout<<"Please input as $./tsp1 -n 5 -f data.dat\nor              $./tsp1 -n 5"<<endl;
        exit(0);
    }
    
    else // if 2 pair of input 
    {
        while((ch = getopt(argc, argv, "n:f:")) != -1 )
        {
            switch(ch)
            {
            case 'n': size = atoi(optarg); 
                      mode = 0;
                      cout<<size<<" cities in the map."<<endl;
                      break;
        
            case 'f': FileName = optarg;
                      mode = 1;
                      break;
            
            default:  cout<<"Please input as $./tsp1 -n 5 -f data.dat\n"
                          <<"or              $./tsp1 -n 5"<<endl;
                      exit(0);
            }
        }
    }

/* ========== input checking finished, construct map ============= */

    ifstream mapin(FileName);
    if(mode) mapin>>size;
    
    int **dist = new int*[size];
    for(int i=0; i<size; ++i)
    { dist[i] = new int[size]; }// allocate memory to the distance matrix.. 
    
    if(mode)// input with -f from command line, read map from a file..
    {
        for(int i=0; i<size; ++i)
        {
            for(int j=0; j<size; ++j)
            {
                mapin>>dist[i][j];
            }
        }
        cout<<"Load the map from "<<FileName<<endl;
    }
    else // generate a map from the code..
    {    
        for(int i=0; i<size; ++i)
        {
            dist[i][i] = 0; // no distance from one city to it self..
            for(int j=0; j<i; ++j)
            {
                int dis = rand()%size; 
                while(!dis) dis = rand()%size; 
                // if the distance between i and j is 0, give a vew distance..
                dist[i][j] = dist[j][i] = dis;
            }
        }
        cout<<"Randomly generated map: "<<endl;
    }
    
    mapin.close();
    print(dist, size);// print the distance map out..
    
    if(!mode) write_map(dist, size);// if map is not from reading, write the configuration into the file map.dat..
    cout<<endl;

/* ========== map constructed, travelling begin ================== */

    bool *visited = new bool[size];// 0 is unvisited..
    int *routine = new int[size];
    long int shortest = 999999999999;
    for(int i=0; i<size; ++i)
    { visited[i] = 1, routine[i] = 0; }
    
    visited[0] = 0;// for visited[] 0 means visited, 1 means not..
    routine[0] = 0;// numbers means the sequence of visiting..
    
    struct timeval tm1, tm2;
    
    gettimeofday(&tm1, NULL);// time record before travelling..
    cout<<"Shortest length of this travel is: "<<Permute(dist, size, visited, routine, shortest)<<endl;
    gettimeofday(&tm2, NULL);// time record after travelling..
    
    cout<<"Permute spend: "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" us."<<endl;
    cout<<endl;

/* ========== calculate finished, free memory ==================== */

    for(int i=0; i<size; ++i)// free the distance map matrix..
    {
        delete[] dist[i];
    }
    delete[] dist;
    delete[] visited;
    delete[] routine;
    return 0;
}

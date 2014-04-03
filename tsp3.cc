#include<sys/time.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<cmath>

using namespace std;

int get_length(int **dist, int *visited, int size)// calculate the length of input routine and return..
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
    cout<<"Distance Matrix of this problem: "<<endl;
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        { cout<<dist[i][j]<<" "; }
    cout<<endl;
    }
}

void inverse(int *routine, int i, int j) // i<j, inverse the routine between i and j..
{
    int tmp = 0;
    while(i<j)
    {
        tmp = routine[i];
        routine[i] = routine[j];
        routine[j] = tmp; 
        i++;
        j--;
    }
}

void path_show(int *p, int size)// just print the path to screen like 1 -> 2 -> 3..
{
    cout<<"Routine: ";
    for(int i=0; i<size; ++i)
    cout<<p[i]+1<<" -> ";
    
    cout<<p[0]+1<<endl<<endl;
}

void copy_new_old(int *r_oldone, int *r_newone, int size)// copy routine and visiting history in to new.. 
{
    for(int i=0; i<size; ++i)
    {
        r_oldone[i] = r_newone[i];
    }
}

int converting(int i, int j, double **city)// converting the GEO type coordinate into longitude and latitude..
{
    const double PI = 3.1415926;
    const double RRR = 6378.388;
    
    double lati = PI*((int)city[0][i] + 5.0*(city[0][i] - (int)city[0][i])/3.0) / 180.0;
    double loni = PI*((int)city[1][i] + 5.0*(city[1][i] - (int)city[1][i])/3.0) / 180.0;
    double latj = PI*((int)city[0][j] + 5.0*(city[0][j] - (int)city[0][j])/3.0) / 180.0;
    double lonj = PI*((int)city[1][j] + 5.0*(city[1][j] - (int)city[1][j])/3.0) / 180.0;
    
    double q1 = cos(loni - lonj);
    double q2 = cos(lati - latj);
    double q3 = cos(lati + latj);
    
    return (int)(RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0); 
}

int main(int argc, char *argv[])
{
    ifstream mapin(argv[1]); 
    if(argc != 2)// if the input is not 1 pair of flag and parameter or 2..
    {
        cout<<"Please input as $./tsp1 data.dat"<<endl;
        exit(0);
    }
    else // if 2 pair of input 
    {
        if( ! mapin.is_open() ) 
        {
            cout<<"Cannot open: "<<argv[1]<<endl;
            mapin.close();
            exit(0);
        }
    }

/* ===================== input checked, read input file =========== */

    int size=0;
    mapin>>size;
    cout<<size<<" cities in the file"<<endl;
    double **city = new double*[2];// city store the information from the ftv file..
    city[0] = new double[size];
    city[1] = new double[size];
    
    for(int i=0; i<size; ++i)
    {
        int nouse;
        mapin>>nouse;
        mapin>>city[0][i];
        mapin>>city[1][i];
    }
    mapin.close();
   
/* ================== file read, construct map ================== */
    cout<<"NODE COORD SECTION"<<endl;
    for(int i=0; i<size; ++i)
    { cout<<" "<<i+1<<"   "<<city[0][i]<<"  "<<city[1][i]<<endl; }

    int **dist = new int*[size]; // dist is the distance matrix..
    for(int i=0; i<size; ++i)
    {
        dist[i] = new int[size]; // allocate memory to the distance matrix.. 
        dist[i][i] = 0;    
        for(int j=0; j<i; ++j)
        {
            dist[i][j] = dist[j][i] = converting(i, j, city);
        }
    }
    delete[] city[0];
    delete[] city[1];
    delete[] city;
    
    cout<<endl;
    print(dist, size);// print the distance map out..
    cout<<endl;

/* ========== map constructed, annealing begin ================== */

    int shortest = 999999;// origin length..
    int *oldroutine = new int[size];
    int *newroutine = new int[size];
    int i1, i2; // i1 and i2 will be used to have a little change in the routine..
    long long int stop_check = 0, iteration = 0;
   
 /* if reading the ulysses16.tsp this line and line 173 could be deleted to see the best answer..
 
    //The optimal solution of ulysses16 is:
    //1, 14, 13, 12, 7, 6, 15, 5, 11, 9, 10, 16, 3, 2, 4, 8
    //The length of of above solution is 6859
    //Nodes above are counted from 1, but in code are counted from 0..
    newroutine[ 0] = 0; 
    newroutine[ 1] =13; 
    newroutine[ 2] =12; 
    newroutine[ 3] =11; 
    newroutine[ 4] = 6; 
    newroutine[ 5] = 5; 
    newroutine[ 6] =14; 
    newroutine[ 7] = 4; 
    newroutine[ 8] =10; 
    newroutine[ 9] = 8; 
    newroutine[10] = 9; 
    newroutine[11] =15; 
    newroutine[12] = 2; 
    newroutine[13] = 1; 
    newroutine[14] = 3; 
    newroutine[15] = 7; 
    
    cout<<"The best solution of this problem is:"<<endl;
    path_show(newroutine, size);
    cout<<get_length(dist, newroutine, size)<<endl;
 */ 
    for(int i=0; i<size; ++i)// initialize the newroutine and oldroutine..
    {
        newroutine[i] = oldroutine[i] = i;
    }
    
    srand48(time(NULL));
    struct timeval tm1, tm2;
    gettimeofday(&tm1, NULL);// time record before travelling..
    
    while(stop_check < 90000000) // Simulated Annealing part, core part of the code.. 
    {
        i1 = lrand48()%16;
        i2 = lrand48()%16;
        while(i2 == i1) { i2 =lrand48()%16; } // this line make sure that i1 is not same with i2..
        
        if(i1 < i2) 
            inverse(newroutine, i1, i2);
        else        
        {
            inverse(newroutine, 0, i2);
            inverse(newroutine, i1, size-1);
        }
        
        int d_tmp =  get_length(dist, newroutine, size) - get_length(dist, oldroutine, size) ;
        if( d_tmp < 0 ) 
        {
           copy_new_old(oldroutine, newroutine, size);
           shortest = get_length(dist, newroutine, size);
           cout<<"At iteration = "<<iteration<<", length of this routine is "<<shortest<<endl;
           path_show(newroutine, size);
           stop_check = 0;
        }
    
    iteration++;
    stop_check++;
    }
    
    gettimeofday(&tm2, NULL);// time record after travelling..
    
    cout<<endl<<"Time cost to get the solution: "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" us."<<endl;
    cout<<endl;

/* ========== calculate finished, free memory ==================== */

    for(int i=0; i<size; ++i)// free the distance map matrix..
    {
        delete[] dist[i];
    }
    delete[] dist;
    delete[] oldroutine;
    delete[] newroutine;
    return 0;
}

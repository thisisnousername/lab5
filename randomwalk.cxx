#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;

struct colloid{
    double x,y;
};

void init(colloid* const c, const int N){
    for(int i = 0; i < N; i++){
	c[i].x = 0;
	c[i].y = 0;
    }
}

void print(const colloid* const c, const int N, const string fname){
    ofstream out(fname.c_str());
    for(int i = 0; i < N; i++)
	out << c[i].x << "\t" << c[i].y << endl;
    out.close();
}

void randomize(int* rx, int *ry, const int N){
	for(int i=0; i<N; i++){
		rx[i] = int((double(rand())/RAND_MAX)*3)-1;
		ry[i] = int((double(rand())/RAND_MAX)*3)-1;
	}
}

void push(colloid* const c, int*rx, int* ry, const int N, const double step){
	for(int i=0; i<N; i++){
		c[i].x += step*rx[i];
		c[i].y += step*ry[i];
	}
}

void evaluate(const colloid* const c, double& meanx, double& meany, double& var, const int N){
	meanx=0; meany=0; var=0;
	for(int i=0; i<N; i++){
		meanx += c[i].x;
		meany += c[i].y;
	}
	meanx /= N;
	meany /= N;
	for(int i=0; i<N; i++)
		var += pow(c[i].x-meanx, 2.0)+pow(c[i].y-meany, 2.0);
	var /= N;
}

int main(void){
  
    int N  = 50;
    cout << "Right now # of colloids is set to " << N << ", how many do you want?" << endl;
    cin  >> N;
    
    colloid*  c  = new colloid[N];	// dynamically allocate memory for N particles
    int*      rx = new int[N];		// dynamically allocate memory for movement-conditions in x direction
    int*      ry = new int[N];		// dynamically allocate memory for movement-conditions in y direction
    
    double meanx, meany, var;		// statistics
    ofstream stat("statistics.dat");	// ... to be printed out here
    
    const double step = 0.1;		// step size
    
    init(c, N);				// initialize all colloids
    
    stringstream s;			// set up stringstream -> multiple output files
    string       fname = "rwalk";	// basic name tag for multiple output files
    
    const int Nsteps = 100;		// total # of steps
    const int Nfiles = 10;		// total # of output files
    int Nsubsteps    = Nsteps / Nfiles; // resulting substeps
    
    s.str("");				// empty stringstream
    s << fname << "_0.dat";		// fill in string for initial file
    print(c, N, s.str());		// print initial positions
    
    srand(time(NULL));			// randomize random numbers
    
    for(int i = 1; i <= Nfiles; i++){
	for(int j = 0; j < Nsubsteps; j++){
		randomize(rx, ry, N);			// call to function which randomly sets up rx and ry
		push(c, rx, ry, N, step);		// call to function which pushes all colloids according to rx and ry
		evaluate(c, meanx, meany, var, N);	// call to function which evaluates statistics
	    stat << (i-1)*Nsubsteps+j << "\t" << meanx << "\t";
	    stat << meany << "\t" << var << endl;
	}
	s.str("");						// preparations for printing...
	s << fname << "_" << i << ".dat";			// ... colloid positions
	print(c, N, s.str());					
    }
    
    stat.close();			// close statistics file
    delete[] rx,ry,c;			// delete dynamically allocated arrays
    return 0;
}

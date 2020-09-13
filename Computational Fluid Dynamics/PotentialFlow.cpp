#include <iostream>
# include <stdlib.h>
# include <fstream>
# include <math.h>

using namespace std;


// DATOS

const int Nx = 2; // number of divisions along dir. X
const int Ny = 2; // number of divisions along dir. Y
const int dim = 2; // number of dimensions
const float L = 5; // length of the canal in [m]
const float H = 3; // height of the canal in [m]
const float u_inf = 100; // entrance's velocity  [m]
 
// VARIABLES

    const int nnode = (Nx+1)*(Ny+1)-1;
    float COOR[nnode][dim];
    float X [nnode][dim];
    float deltaX = 0;
    float deltaY = 0;
    float rho0 = 10;
    float rho [nnode];
    float phi [nnode][dim];
    float phiEntrance [2] = { 0 , u_inf }; 

// FUNCTIONS' MENTION

void Geometria2D (float L, float H, int Nx, int Ny);
void EquationDiscretization (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]);
void IdentifyNodes (int point, int nx, int ny, int nnode);
bool IniciLinea (int num, int nx, int ny);
bool FinalLinea (int num, int nx, int ny);
void Centroide (float COOR[nnode][dim], int Nx, int Ny);

// FUNCTIONS

void Geometria2D (float l, float h, int nx, int ny) { 
    float deltaX = l/nx;
    float deltaY = h/ny;
    int i = 0; int j = 0;

   for (j=0; j<=ny; j++) {
         for (i=0; i<=nx; i++) {
            COOR [(nx+1)*j+i][0] = (i)*deltaX;
            COOR [(nx+1)*j+i][1] = (j)*deltaY;
            //rho  [(nx+1)*j+i] = rho0;
         }
    }

    int k = 0;

    /*
    for (k=0; k<=(ny+1)*(nx+1)-1; k++) {
            cout << "Punto " << k;
            cout << " = Comp X: " << COOR[k][0];
            cout << " // Comp Y: " << COOR[k][1] << endl;
    }
    */

   Centroide (COOR, nx, ny);
}

bool Entero (int num, int N) {  // Funcion que devuelve 1 si el número al dividir es entero o 0 si no lo es
    float res = num % N; 
    bool ent = false;
    if ( res < pow(10,-3)  && res > -pow(10,-3) ) {
        ent = 1;
    } else {
        ent = 0;
    } 
    return ent; 
} 

bool FinalLinea (int num, int nx, int ny) {  // Funcion que devuelve 1 si el nodo es último de línea 

    int i = 0;
    bool ent = 0;

    while (ent == 0 && i<=Ny) {
        if(num == nx*(i+1)+i) {
            ent = 1;
        } else {
            ent = 0;
        }
        i++;
    }
    return ent;
} 

bool IniciLinea (int num, int nx, int ny) {  // Funcion que devuelve 1 si el nodo es primero de línea 

    int i = 0;
    bool ent = 0;

    while (ent == 0 && i<=Ny) {
        if(num == i*(nx+1)) {
            ent = 1;
        } else {
            ent = 0;
        }
        i++;
    }
    return ent;
} 

void IdentifyNodes (int point, int nx, int ny, int nnode) {
    
    int north = point-(nx+1); 
    int south = point+(nx+1); 
    int west = point-1; 
    int east = point+1; 


    if ( north < 0 ) {
        north = -1; 
    } 
    if ( south >= nnode ) {
        south = -1; 
    }
    if ( IniciLinea(point, nx, ny) == 1) {
        west = -1; 
    }
    if ( FinalLinea(point, nx, ny) == 1) {
        east = -1; 
    }

    //cout << "P: "<< point << " = N: " << north << " = S: " << south << " = W: " << west << " = E: " << east << endl;

}

void Centroide (float COOR[nnode][dim], int nx, int ny) {
    
    int i=0; int j=0; 

     for (j=0; j<=ny; j++) {
         for (i=0; i<=nx; i++) {
            X [(nx+1)*j+i][0] = (COOR[i][0]+COOR[i+1][j])/2;
            X [(nx+1)*j+i][1] = (COOR[i][1]+COOR[i][j])/2;
            rho  [(nx+1)*j+i] = rho0;
         }
    }

    int k=0;
/*
    for (k=0; k<(ny+1)*(nx+1); k++) {
            cout << "Punto " << k;
            cout << " = Comp X: " << X[k][0];
            cout << " // Comp Y: " << X[k][1] << endl;
    }
*/    

}

// STREAM FUNCTION DISCRETIZATION

void EquationDiscretization (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]) {

    float ae = 0; float aw = 0;
    float an = 0; float as = 0;
    float ap = 0; //float bp = 0;

    int north = 0; int south = 0; 
    int west = 0; int east = 0; 
    int point = 0;
    
    bool Trobat = false;

    int i = 0;

    for (i=0; i<=nnode; i++) {
        point = i;
        IdentifyNodes (point, nx, ny, nnode);
        if (north != -1){
            an = rho [north]/rho0 * deltaX / deltaY;
            // Linea de corriente conocida
            phi[i][0]= u_inf * X2[i][1]  ;  
            phi[i][1]= 0 ;
            Trobat = true ; 
        } else { ae = 0; }

        if (south != -1){
            as = rho [south]/rho0 * deltaX / deltaY;
            // Linea de corriente conocida
            phi[i][0]= u_inf * X2[i][1] ;  
            phi[i][1]= 0 ; 
            Trobat = true ; 
        } else { ae = 0; }

        if (west != -1){
            aw = rho [west]/rho0 * deltaY / deltaX;
            // Las lineas de corriente en la salida son incognita y las de west no existen
            phi[-1][0]= 0 ;  
            phi[-1][1]= 0 ; 
        } else { ae = 0; }

        if (east != -1){
            ae = rho [east]/rho0 * deltaY / deltaX; 
            // Linea de corriente conocida
            phi[i][0]= u_inf*X2[i][1] ; 
            phi[i][1]= 0 ;  
            Trobat = true ;  
        } else { ae = 0; }

            ap = an + as + aw + ae; 

        if (Trobat == true) {
            // ja sabem phi
            Trobat = false;
        } else {
            phi[i][0]=(an*phi[north][0] + as*phi[south][0] + ae*phi[east][0] + aw*phi[west][0]) / ap;
            phi[i][0]=(an*phi[north][1] + as*phi[south][1] + ae*phi[east][1] + aw*phi[west][1]) / ap;
        }

    }
}



// MAIN CODE

int main() 
{

    Geometria2D (L, H, Nx, Ny);
    EquationDiscretization (Nx, Ny, deltaX, deltaY, rho, rho0, COOR);


    int i=0;

    for (i=1; i<=nnode; i++) {
        cout << "item: " << i << " Phi x: " << phi[i][0] << "  Phi y: " << phi[i][1] << endl;
    }
    

    return 0; 
}
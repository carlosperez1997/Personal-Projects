#include <iostream>
# include <stdlib.h>
# include <fstream>
# include <math.h>
#include <iomanip>
#include <cstdlib>

//# include <windows.h>

using namespace std;

// CONSTANTES FÍSICAS

const float pi = 3.14159265;

// DATOS DEL PROBLEMA

const int Nx = 100; // number of divisions along dir. X
const int Ny = 100; // number of divisions along dir. Y
const int dim = 2; // number of dimensions
const float L = 20; // length of the canal in [m]
const float H = 10; // height of the canal in [m]
const float R = 3;
const float chord = 9;
const float thickness = 1.5;
const float alpha = 10;
const float u_inf = 50; // entrance's velocity  [m]


// VARIABLES

    const int nnode = Nx*Ny-1;
    float COOR[Nx+1][Ny+1][dim];
    float COORlim[Nx+2][4][dim];
    bool CosFluidlim[Nx*4];
    float X2[nnode][dim];
    float deltaX = 0;
    float deltaY = 0;
    float rho0 = 10;
    float rho [nnode];
    float pressio [nnode];
    float Temp [nnode];
    float phi [nnode][dim];
    float vel [nnode][dim];
    float vect_N [nnode][dim];
    float velEntrance [2] = { 0 , u_inf }; 
    float velocity =  pow(velEntrance[0],2) +pow(velEntrance[1],2);
    bool CosFluid[nnode];
    bool CondContorn[nnode];
    float criteriConv = pow(10,-1);

    int north = 0; int south = 0; 
    int west = 0; int east = 0; 
    int point = 0;

// FUNCTIONS' MENTION

void Geometria2D (float L, float H, int Nx, int Ny);
void EquationDiscretization (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]);
void IdentifyNodes (int point, int nx, int ny, int nnode);
bool IniciLinea (int num, int nx, int ny);
bool FinalLinea (int num, int nx, int ny);
//void PerfilNACA (float COOR[Nx+1][Ny+1][dim]);
void BlockingOffMethodCilindre (float X2[nnode][dim], float L, float H, float R);
void plotBOM (bool CosFluid [nnode], int Nx, int Ny);
void Centroide (float COOR[Nx+1][Ny+1][dim], int Nx, int Ny);
float DegtoRad (float angle);
int ParImpar (int N);
void Limits (float l, float h, int nx, int ny);
float TangCilindro (float coorX, float R);
float Circulation (float vye, float vxn, float vyw, float vxs, float deltaY, float deltaX);


void pasartablaafichero (float X2[nnode][dim], bool CosFluid[nnode], int nnode, int Nx);

float NACA_yt (float X, float t, float c);

// FUNCTIONS

void Geometria2D (float l, float h, int nx, int ny) { 
    float deltaX = l/nx;
    float deltaY = h/ny;
    int i = 0; int j = 0;

   for (j=0; j<=ny; j++) {
         for (i=0; i<=nx; i++) {
            COOR [i][j][0] = (i)*deltaX;
            COOR [i][j][1] = (j)*deltaY;
            //rho  [(nx+1)*j+i] = rho0;
         }
    }

    /*
    cout << "---------- Coordenadas globales ----------" << endl;
    for (j=0; j<=ny; j++) {
         for (i=0; i<=nx; i++) {
            cout << "Punto ( " << i << " " << j << " )";
            cout << " = Comp X: " << COOR[i][j][0];
            cout << " // Comp Y: " << COOR[i][j][1] << endl;
         }
    }
    cout << "------ End of Coordenadas globales ------" << endl;
    */

   Centroide (COOR, nx, ny);
}

void Centroide (float  COOR[Nx+1][Ny+1][dim], int nx, int ny) {
    
    int i=0; int j=0; 

   // MANERA DE HACERLO 

   for (j=0; j<ny; j++) {
         for (i=0; i<nx; i++) {
            X2 [nx*j+i][0] = (COOR[i][j][0]+COOR[i+1][j][0])/2;
            X2 [nx*j+i][1]= (COOR[i][j][1]+COOR[i][j+1][1])/2;
            rho  [(nx+1)*j+i] = rho0;
         }
    }
    
    /*
    int k=0;

    cout << "---------- Coordenadas centroides 2 ----------" << endl;    
    for (k=0; k<ny*nx; k++) {
            cout << "Punto " << k;
            cout << " = Comp X: " << X2[k][0];
            cout << " // Comp Y: " << X2[k][1] << endl;
    }
    cout << "------ End of Coordenadas centroides 2 ------" << endl;
    */

   // FIN de MANERA DE HACERLO 

    /*
    int I=0; int J=0; 

     for (j=1; j<ny+1; j++) {
         for (i=1; i<nx+1; i++) {
             I=i-1; J=j-1;
            X3 [i][j][0] = (COOR[i][j][0]+COOR[i+1][j][0])/2;
            X3 [i][j][1] = (COOR[i][j][1]+COOR[i][j+1][1])/2;
            //rho  [(nx+1)*j+i] = rho0;

         }
    }
    */
    
    /*
     // INTENTO DE MATRIZ QUE INCLUYE LAS FRONTERAS, es decir tiene nodos en las fronteras.

        int I=0; int J=0; 

    for (j=-1; j<=ny+1; j++) {
         for (i=-1; i<=nx+1; i++) {
            if (i==-1){
                I=0;
            } else if (i==nx+1) {
                I=nx;
            }else {
                I=i;
            }

           if (j==-1){
                J=0;
            } else if (j==ny+1) {
                J=ny;
            }else {
                J=j;
            }

            COOR2 [i][j][0] = COOR[I][J][0];
            COOR2 [i][j][1] = COOR[I][J][0];
            //rho  [(nx+1)*j+i] = rho0;
         }
    }
    
    cout << "---------- Coordenadas globales 2 ----------" << endl;
    for (j=-1; j<=ny+1; j++) {
         for (i=-1; i<=nx+1; i++) {
            cout << "Punto " << i << " " << j;
            cout << " // Comp X: " << COOR2[i][j][0];
            cout << " // Comp Y: " << COOR2[i][j][1] << endl;
         }
    }
    cout << "------ End of Coordenadas globales 2 ------" << endl;
    */

}

void IdentifyNodes (int point, int nx, int ny, int nnode) {
    
    north = point+(nx); 
    south = point-(nx); 
    west = point-1; 
    east = point+1; 


    if ( north >= nnode ) {
        north = -1; 
    } 
    if ( south <= 0 ) {
        south = -1; 
    }
    if ( IniciLinea(point, nx, ny) == 1) {
        west = -1; 
    }
    if ( FinalLinea(point, nx, ny) == 1) {
        east = -1; 
    }

    //cout << "P: "<< point << " / N: " << north << " / S: " << south << " / W: " << west << " / E: " << east << endl;

}

bool FinalLinea (int num, int nx, int ny) {  // Funcion que devuelve 1 si el nodo es último de línea 

    int j = 0;
    bool ent = 0;

    while (ent == 0 && j<=Ny) {
        if(num == nx*(j+1)-1) {
            ent = 1;
        } else {
            ent = 0;
        }
        j++;
    }
    return ent;

    /*bool ent = 0;
        if(I == nx) {
            ent = 1;
        } else {
            ent = 0;
        }
    return ent;
    */
} 

bool IniciLinea (int num, int nx, int ny) {  // Funcion que devuelve 1 si el nodo es primero de línea 

    int j = 0;
    bool ent = 0;

    while (ent == 0 && j<=Ny) {
        if(num == j*nx) {
            ent = 1;
        } else {
            ent = 0;
        }
        j++;
    }
    return ent;

    /*bool ent = 0;
        if(I == 0) {
            ent = 1;
        } else {
            ent = 0;
        }
    return ent; */
} 



void plotBOM (bool CosFluid [nnode], int Nx, int Ny) {
    
    int i = 0;

    for (i=0; i<=nnode; i++) {
        if (FinalLinea (i, Nx, Ny) == 0) {           
            if (CosFluid[i] == 1) {
                cout << "x" << "";
            } else {
                cout << "-" << "";
            }
        } else {
            if (CosFluid[i] == 1) {
                cout << "x" << endl;
            } else {
                cout << "-" << endl;
            } 
        } 
    }

}

float DegtoRad (float angle){
    float angleRAD = angle * pi / 180;
    return angleRAD;   
}

void Limits (float l, float h, int nx, int ny) { 
    float deltaX = l/nx;
    float deltaY = h/ny;
    int i = 0; int j = 0;

   for (j=0; j<4; j++) {
         for (i=0; i<=nx; i++) {
             if (j<2) {
                COORlim [i][j][0] = (i)*deltaX;
                COORlim [i][j][1] = 0-j*deltaY;
                CosFluidlim[nx*j+i] = 1;
             } else {
                COORlim [i][j][0] = (i)*deltaX;
                COORlim [i][j][1] = h+(j-2)*deltaY;
                CosFluidlim[nx*j+i] = 1;  
             }
            //cout << "X: " << COORlim [i][j][0] << " Y: " << COORlim [i][j][1];
            //cout << " Cos: " << CosFluidlim[nx*j+i] << endl;    
         }
    }

}

// Funcion que retorna 1 si el nodo está dentro del cuerpo o 0 si está en el fluido (caso CILINDRO)
void BlockingOffMethodCilindre (float X2[nnode][dim], float L, float H, float R) { 
    int i=0;

    // CILINDRO EN EL CENTRO DE LA MALLA

    float xc = L / 2;
    float yc = H / 2;
    float dist2 = 0;

    // si distancia del origen del cilindre al punt és més petita DINS
    // si distancia del origen del cilindre al punt és més gran fora

    for (i=0; i<=nnode ; i++){
        dist2 = pow(X2[i][0]-xc,2) + pow(X2[i][1]-yc,2);
        if ( dist2 <= pow(R,2)) {
            CosFluid[i]=1;
        } else {
            CosFluid[i]=0;
        }
    }    


    plotBOM(CosFluid, Nx, Ny);

}

float TangCilindro (float coorX, float R) {
    float yprim;
    if (coorX == R || coorX == -R) {
        yprim = -99999;
    } else {
        yprim = - coorX / pow((pow(R,2)-pow(coorX,2)),2);
    }
    

    return yprim;
}


 
int ParImpar (int N) { // Funcio que retorna 1 si el num es par o 0 si el num es impar
    if (N % 2 == 0){
        return 1;
    } else{
      return 0;
     } 
}

void CondInicials (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]) {
    
    float xc = L / 2;
    float yc = H / 2;
    float dist2 = 0;
    
    bool Trobat = false;

    int i = 0; float M = 0;
    float modul = 0;
    float x = 0;
    
    for (i=0; i<=nnode; i++) {
        point = i;
        IdentifyNodes (point, nx, ny, nnode);
        if (north == -1) {
            CondContorn [i] = true;    
            // Linea de corriente conocida
            vel [i][0] = u_inf; vel [i][1] = 0;
            phi[i][0]= - (rho[i] / rho0) * vel[i][1] * X2[i][1]; 
            phi[i][1]= (rho[i] / rho0) * vel[i][0] * X2[i][1]; 
        }

        if (south == -1){
            CondContorn [i] = true;
            // Linea de corriente conocida
            vel [i][0] = u_inf; vel [i][1] = 0;
            phi[i][0]= - (rho[i] / rho0) * vel[i][1] * X2[i][1]; 
            phi[i][1]= (rho[i] / rho0) * vel[i][0] * X2[i][1]; 
        }

        if (west == -1) { 
            CondContorn [i] = true;  
            // Las lineas de corriente en la entrada
            vel [i][0] = u_inf; vel [i][1] = 0;
            phi[i][0]= - (rho[i] / rho0) * vel[i][1] * X2[i][1]; 
            phi[i][1]= (rho[i] / rho0) * vel[i][0] * X2[i][1]; 
        }

        if (east == -1){
            phi[-1][0]= 0 ; 
            phi[-1][1]= 0 ;  
        }

        if (CosFluid[i] == 0 && ( CosFluid[north] == 1 || CosFluid[south] == 1 || CosFluid[west] == 1 || CosFluid[east] == 1 ) ){
            
            x = X2[i][0] - xc;
            
            if ( abs(x - R) < pow(10,-2)) {
                vect_N[i][0] = 0;
                vect_N[i][1] = 0;
                cout << "Se produce indeterminación en el cálculo de la tangencia" << endl;
                cout << x << endl;
            } else {
                M = TangCilindro(x, R);
    
                modul = sqrt(pow(M,2)+1);

                vect_N[i][0] = 1 / modul;
                vect_N[i][1] = M / modul;

                if ( (pow(vect_N[i][0],2) + pow(vect_N[i][1],2)) < 0.99 && (pow(vect_N[i][0],2) + pow(vect_N[i][1],2)) > 1.01) {
                  cout << "Vector vect_N no normalizado" << endl;  
                } 

                CondContorn [i] = true ; 

                vel [i][0] = velocity * vect_N[i][0];
                vel [i][1] = velocity * vect_N[i][1];

                phi[i][0]= - (rho[i] / rho0) * vel[i][1] * X2[i][1]; 
                phi[i][1]= (rho[i] / rho0) * vel[i][0] * X2[i][1]; 
                
                //cout << "pendiente: " << M << " phiX: " << phi[i][0] << "phi Y:" << phi[i][1] << " "<< (pow(phi[i][0],2) + pow(phi[i][1],2)) << endl;
            }   
        } 
        
        if (CosFluid[i] == 1) {
            phi[i][0]=0;
            phi[i][1]=0;
            vel[i][0]=0;
            vel[i][1]=0;
            CondContorn [i] = true ;
        }
        
        // SUPOSEM UNES VELOCITATS I PHIS INICIALS

        if (CondContorn [i] == false) {
            // ja sabem phi
            vel [i][0] = velocity * 0.95;
            vel [i][1] = velocity * 0.05;
            phi[i][0] = (rho[i] / rho0) * vel [i][0] * X2 [i][1];
            phi[i][1] = - (rho[i] / rho0) * vel [i][1] * X2 [i][0];
        }
    
    }

}
/*
void EquationDiscretization (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]) {

    float xc = L / 2;
    float yc = H / 2;
    float dist2 = 0;

    float ae = 0; float aw = 0;
    float an = 0; float as = 0;
    float ap = 0; //float bp = 0;
    
    bool Trobat = false;

    int i = 0; float M = 0;
    float modul = 0;
    float x = 0;

    // NOVA CONDICIO DE CONTORN
    
    if ( ParImpar (Ny) == 1 ) {
        phi[Nx*Ny/2][0] = u_inf*X2[i][0];
        phi[Nx*Ny/2][1] = 0;

        phi[Nx*Ny/2+1][0] = u_inf*X2[i][0];
        phi[Nx*Ny/2+1][1] = 0;
    } else {
        phi[Nx*Ny/2][0] = u_inf*X2[i][0];
        phi[Nx*Ny/2][1] = 0;

    }
    
   
    // PHI CONTORNO CANAL y CILINDRO
    for (i=0; i<=nnode; i++) {
        point = i;
        IdentifyNodes (point, nx, ny, nnode);
        if (north != -1){
            an = rho [north]/rho0 * deltaX / deltaY; 
        } else { 
            Trobat = true;  
            CondContorn [i] = true;   
            an = 0;  
            // Linea de corriente conocida
            phi[i][0]= u_inf*X2[i][1]  ;  
            phi[i][1]= 0 ;
            vel [i][0] = u_inf;
            vel [i][1] = 0;
        }

        if (south != -1){
            as = rho [south]/rho0 * deltaX / deltaY; 
        } else { 
            as = 0; Trobat = true ;
            CondContorn [i] = true;
            // Linea de corriente conocida
            phi[i][0]= u_inf*X2[i][1] ;  
            phi[i][1]= 0 ; 
        }

        if (west != -1){
            aw = rho [west]/rho0 * deltaY / deltaX;   
        } else { 
            aw = 0; CondContorn [i] = true ;  
            // Las lineas de corriente en la entrada
            phi[i][0]= u_inf*X2[i][1] ; 
            phi[i][1]= 0 ;  
        }

        if (east != -1){
            ae = rho [east]/rho0 * deltaY / deltaX; 
            // Linea de corriente en la salida desconocida
        } else { 
            ae = 0;
            phi[-1][0]= 0 ; 
            phi[-1][1]= 0 ;  
            }

        if (CosFluid[i] == 0 && ( CosFluid[north] == 1 || CosFluid[south] == 1 || CosFluid[west] == 1 || CosFluid[east] == 1 ) ){
            
            x = X2[i][0] - xc;

            if ( (x - R) < pow(10,-2) && (x - R) > -pow(10,-2)) {
                vect_N[i][0] = 0;
                vect_N[i][1] = 0;
                cout << "Se produce indeterminación en el cálculo de la tangencia" << endl;
            } else {
                M = TangCilindro(x, R);
    
                modul = sqrt(pow(M,2)+1);

                vect_N[i][0] = 1 / modul;
                vect_N[i][1] = M / modul;

                if ( (pow(vect_N[i][0],2) + pow(vect_N[i][1],2)) < 0.99 && (pow(vect_N[i][0],2) + pow(vect_N[i][1],2)) > 1.01) {
                  cout << "Vector vect_N no normalizado" << endl;  
                } 

                phi[i][0] = velocity * vect_N[i][0] * X2[i][1];
                phi[i][1] = - velocity * vect_N[i][0] * X2[i][1];
                Trobat = true;

                //cout << "pendiente: " << M << " phiX: " << phi[i][0] << "phi Y:" << phi[i][1] << " "<< (pow(phi[i][0],2) + pow(phi[i][1],2)) << endl;
            }   
        } 
        
        if (CosFluid[i] == 1) {
            phi[i][0]=0;
            phi[i][1]=0;
            Trobat = true;
        }

            ap = an + as + aw + ae; 
        
        if (Trobat == true) {
            // ja sabem phi
            Trobat = false;
        } else {
            phi[i][0]=(an*phi[north][0] + as*phi[south][0] + ae*phi[east][0] + aw*phi[west][0]) / ap;
            phi[i][0]=(an*phi[north][1] + as*phi[south][1] + ae*phi[east][1] + aw*phi[west][1]) / ap;
            cout << "Aquí estamos tt" << endl;
        }
        



    }

}
*/

float Circulation (float vye, float vxn, float vyw, float vxs, float deltaY, float deltaX) {
    float circ = vye*deltaY - vxn*deltaX - vyw*deltaY + vxs*deltaX;
    return circ;
}


void EquationDiscretization (int nx, int ny, float deltaX, float deltaY, float rho [nnode], float rho0, float X2[nnode][dim]) {

    float xc = L / 2;
    float yc = H / 2;
    float dist2 = 0;

    float ae = 0; float aw = 0;
    float an = 0; float as = 0;
    float ap = 0; //float bp = 0;
    
    bool Trobat = false;

    int i = 0; float M = 0;
    float modul = 0;
    float x = 0;

    for (i=0; i<=nnode; i++) {
        if ( CondContorn[i] == false) {
            point = i;
            IdentifyNodes (point, nx, ny, nnode);
            while (abs(Circulation(vel[east][1], vel[north][0], vel[west][1], vel[south][0], deltaY, deltaX)) > criteriConv) {
                
            if (north == -1){
            an = 0; 
            } else { 
            an = rho [north]/rho0 * deltaX / deltaY;
            }

            if (south == -1){
            as = 0; 
            } else { 
            as = rho [south]/rho0 * deltaX / deltaY; 
            }

            if (west == -1){
            aw = 0;   
            } else { 
            aw = rho [west]/rho0 * deltaY / deltaX;  
            }

            if (east == -1){
            ae = 0;
            } else { 
            ae = rho [east]/rho0 * deltaY / deltaX; 
            }

            ap = an + as + aw + ae; 

            //cout << "Calculating new ones"<< endl;

            // NEW PHIS

            phi[i][0]=(an*phi[north][0] + as*phi[south][0] + ae*phi[east][0] + aw*phi[west][0]) / ap;
            phi[i][1]=(an*phi[north][1] + as*phi[south][1] + ae*phi[east][1] + aw*phi[west][1]) / ap;
            
            // NEW VELOCITIES

            vel [i][0] = phi[i][1] / X2[i][1] * rho0 / rho [i] ;
            vel [i][1] = - phi[i][0] / X2[i][0] * rho0 / rho [i] ;
            } 

        }
    }
}



// MAIN CODE

int main() {

    Geometria2D (L, H, Nx, Ny);
    Limits (L, H, Nx, Ny);
    BlockingOffMethodCilindre (X2, L, H, R); 
    CondInicials (Nx, Ny, deltaX, deltaY, rho, rho0, X2);
    EquationDiscretization (Nx, Ny, deltaX, deltaY, rho, rho0, X2);

    //pasartablaafichero (X2, CosFluid, nnode, Nx);

    /*
    int i=0; int j=0;
    
    for (i=0; i<=nnode; i++) {
        cout << "item: " << i << " Phi x: " << phi[i][0] << "  Phi y: " << phi[i][1] << endl;
        cout << "Cos (1) Fluid (0) - item: " << i << " =  ( " << CosFluid[i] << " )" << endl;
    }
    */
    
    return 0; 
}

void pasartablaafichero (float X2[nnode][dim], bool CosFluid[nnode], int nnode, int Nx) {
     ofstream prod;
     prod.open("FlujoCilindro.txt");

    int i = 0; int j = 0; int k=0;

       if (prod.is_open()) {
            int i = 0; 
            prod << nnode << endl;

          while (k <= nnode) {
             //prod << "Coordenadas: "; 
             //prod << X2[i][0] <<" " << X2[i][1] << " "; 
             //prod << " Cos (1) Fluid (0): " << CosFluid[i] <<endl; 
             //prod << X2[i][0] << " " << X2[i][1] << " " << CosFluid[i] <<endl;
             
             prod << fixed << setprecision(5) << X2[k][0] << endl;
             prod << fixed << setprecision(5) << X2[k][1] << endl;
             prod << fixed << setprecision(5) << phi[k][0] << endl;
             prod << fixed << setprecision(5) << phi[k][1] << endl;
             k++;
             }  
        } else {

            cout << "No he accedido al .txt"<<endl;

        }
     prod.close();   
}      
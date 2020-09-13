#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
using namespace std;

// DATA OF THE PROBLEM
// Geometrical parameters
const int N = 10; // Number of control volumes, row and column (NxN)
const int N_row = N+2; // Number of nodes in each row
const int N_col = N+2; // Number of nodes in each column
const int N_row_stag_x = N_row -2; // Number of nodes in each row: Staggered X matrix
const int N_col_stag_x = N_col -1; // Number of nodes in each column: Staggered X matrix
const int N_row_stag_y = N_row -1; // Number of nodes in each row: Staggered Y matrix
const int N_col_stag_y = N_col -2; // Number of nodes in each column: Staggered Y matrix

// Definition of general variables
double rho = 1; // Density [Kg/m^3]
double  delta = pow(10,-6); //Error allowed
double  delta2 = pow(10,-6); //Error2 allowed
double  Re = 1000; // Reynolds Number
double  mu=1/Re; // Viscosity
double  tf=10; //Final Time of simulation [s]
double  dif_P = pow(10,0); //difference pressure max
double  dif_V = pow(10,0); //difference velocities
int time_steps =0;
int i,j,k;

// Variables 
double  dt = 0.001; // Initial dt
double  t=0; // Initializing time
double  u_max; // Maximum velocity in x-direction
double  v_max; // Maximum velocity in y-direction
double  modVel; // Maximum velocity


// Mesh
double X [N_row];
double Y [N_col];
double X2 [N_row] [N_col];
double Y2 [N_row] [N_col]; 
double stagg_Xx [N_row_stag_x] [N_col_stag_x];
double stagg_Xy [N_row_stag_x] [N_col_stag_x];
double stagg_Yx [N_row_stag_y] [N_col_stag_y]; 
double stagg_Yy [N_row_stag_y] [N_col_stag_y];

// Velocities
double U [N_row_stag_x] [N_col_stag_x];
double V [N_row_stag_y] [N_col_stag_y];
double U_prev [N_row_stag_x] [N_col_stag_x];
double V_prev [N_row_stag_y] [N_col_stag_y];

// Diffussive-Convective 
double R_U [N_row_stag_x] [N_col_stag_x];
double R_V [N_row_stag_y] [N_col_stag_y];
double R_U_pred [N_row_stag_x] [N_col_stag_x];
double R_V_pred [N_row_stag_y] [N_col_stag_y];

// Coefficients

double ap [N_row] [N_col];
double ae [N_row] [N_col];
double aw [N_row] [N_col];
double an [N_row] [N_col];
double as [N_row] [N_col];
double bp [N_row] [N_col];

double U_pred [N_row_stag_x] [N_col_stag_x];
double V_pred [N_row_stag_y] [N_col_stag_y];

// Pressure
double P [N_row][N_col];
double P_prev [N_row][N_col];

// TDMA
double P_TDMA [N_col]; 
double R_TDMA [N_col]; 

// FUNCTIONS' DEFINITIONS

void Geometria2D(double X [N_row], double Y [N_col]) ;
void Centroide (double X [N_row], double Y [N_col], double X2 [N_row][N_col], double Y2 [N_row][N_col]);
void StaggeredMesh(double X [N_row], double Y [N_col], double stagg_Xx [N_row_stag_x] [N_col_stag_x], double stagg_Xy [N_row_stag_x] [N_col_stag_x], double stagg_Yx [N_row_stag_x] [N_col_stag_x], double stagg_Yy [N_row_stag_x] [N_col_stag_x]); 
void CondicionesIniciales (double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y], double U_prev [N_row_stag_x] [N_col_stag_x], double V_prev [N_row_stag_y] [N_col_stag_y], double P [N_row][N_col], double P_prev [N_row][N_col], double R_U [N_row_stag_x] [N_col_stag_x], double U_pred [N_row_stag_x] [N_col_stag_x], double R_V [N_row_stag_y] [N_col_stag_y], double V_pred [N_row_stag_y] [N_col_stag_y]);
void TDMA (double ap [N_row] [N_col], double bp [N_row] [N_col], double ae [N_row] [N_col], double aw [N_row] [N_col], double an [N_row] [N_col], double as [N_row] [N_col], double P_TDMA [N_col], double R_TDMA [N_col], double P_prev [N_row][N_col], double P [N_row][N_col]);
double TimeStep(double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_x] [N_col_stag_x], double dt, double t);
void Solution (double R_U [N_row_stag_x] [N_col_stag_x], double R_V [N_row_stag_y] [N_col_stag_y],  double R_U_pred [N_row_stag_x] [N_col_stag_x], double R_V_pred [N_row_stag_y] [N_col_stag_y], double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y], double ap [N_row] [N_col], double bp [N_row] [N_col], double ae [N_row] [N_col], double aw [N_row] [N_col], double an [N_row] [N_col], double as [N_row] [N_col], double P_TDMA [N_col], double R_TDMA [N_col], double P [N_row][N_col], double P_prev [N_row][N_col] ) ;

// FUNCTIONS

void Geometria2D(double X [N_row], double Y [N_col]) {
    double dx =1/double(N);
    
    // COORD X
    X[0]=0; X[1]=dx/2; 
    for (i=2;i<=N;i++){
        X[i]=X[i-1]+dx; 
    }
    X[N+1]=X[N]+dx/2; 

    // COORD Y
    Y[0]=0;  Y[1]=dx/2; 
    for (i=2;i<=N;i++){
        Y[i]=Y[i-1]+dx; 
    }
        Y[N+1]=Y[N]+dx/2; 
    
    /*
    for (i=0;i<=N;i++){
        cout << "X: " << X[i] << " Y:" << Y[i] << endl;
    }
    */

    Centroide (X, Y, X2, Y2);
}

void Centroide (double X [N_row], double Y [N_col], double X2 [N_row][N_col], double Y2 [N_row][N_col]) {
    // COOR X
    for (i=0;i<=N_row-1;i++){
    for (j=0;j<=N_col-1;j++){
        X2[i][j] = X[j]; 
    }
    }

    // COOR Y
    for (i=0;i<=N_row-1;i++){
    for (j=0;j<=N_col-1;j++){
        Y2[i][j] = Y[N_row-1-i]; 
    }
    }

    /*
    for (i=0;i<=N_row-1;i++){
    for (j=0;j<=N_col-1;j++){
        cout << i << " " << j << " // X: " << X2[i][j] << " Y:" << Y2[i][j] << endl;
    }
    }
    */

   cout << " Matriz calculada" << endl;

}

void StaggeredMesh(double X [N_row], double Y [N_col], double stagg_Xx [N_row_stag_x] [N_col_stag_x], double stagg_Xy [N_row_stag_x] [N_col_stag_x], double stagg_Yx [N_row_stag_y] [N_col_stag_y], double stagg_Yy [N_row_stag_y] [N_col_stag_y]){
    cout << "Calculando Matriz Staggered..." << endl;

    double dx =1/double(N);

    // STAGGERED COORD X

    double x[N+1]; 
    x[0]=0; 
    for (i=1;i<=N;i++){
        x[i]=x[i-1]+dx; 
    }

    for (i=0;i<=N_row_stag_x-1;i++){
        for (j=0;j<=N_col_stag_x-1;j++){
            stagg_Xx[i][j]=x[j]; 
            stagg_Xy[i][j]=Y[N_row_stag_x-i];
        }
    }

    // STAGGERED COORD Y

    double y[N+1]; 
        y[0]=0; 
    for (i=1;i<=N;i++){
        y[i]=y[i-1]+dx; 
    }

    for (i=0;i<=N_row_stag_y-1;i++){
        for (j=0;j<=N_col_stag_y-1;j++){
            stagg_Yx[i][j]=X[j+1]; 
            stagg_Yy[i][j]=y[N_row_stag_y-1-i]; 
        }
    }

    /*
        cout << "Staggered mesh X" << endl;
    for (i=0;i<=N_row_stag_x-1;i++){
        for (j=0;j<=N_col_stag_x-1;j++){
        cout << i << " " << j << " // X: " << stagg_Xx[i][j] << " Y:" << stagg_Xy[i][j] << endl;
    }
    }
    */

    /*
       cout << "Staggered mesh Y" << endl;
    for (i=0;i<=N_row_stag_y-1;i++){
        for (j=0;j<=N_col_stag_y-1;j++){
        cout << i << " " << j << " // X: " << stagg_Yx[i][j] << " Y:" << stagg_Yy[i][j] << endl;
        }
    }
    */

}

void CondicionesIniciales (double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y], double U_prev [N_row_stag_x] [N_col_stag_x], double V_prev [N_row_stag_y] [N_col_stag_y], double P [N_row][N_col], double P_prev [N_row][N_col], double R_U [N_row_stag_x] [N_col_stag_x], double U_pred [N_row_stag_x] [N_col_stag_x], double R_V [N_row_stag_y] [N_col_stag_y], double V_pred [N_row_stag_y] [N_col_stag_y]) {
    
    for (i=1;i<=N_row_stag_x-1;i++){
        for (j=0;j<=N_col_stag_x-1;j++){
            U[i][j]=0; 
            U_prev[i][j]=0;
            R_U[i][j] = 0;
            U_pred[i][j]=0;
        }
    }
 
    for (j=0;j<=N_col_stag_x-1;j++){
        U[0][j]= 1; 
    }
    
    for (i=0;i<=N_row_stag_y-1;i++){
        for (j=0;j<=N_col_stag_y-1;j++){
            V[i][j]=0; 
            V_prev[i][j]=0; 
            R_V[i][j] = 0;
            V_pred[i][j]=0;
        }
    }

    for (i=0;i<=N_row-1;i++){
        for (j=0;j<=N_col-1;j++){
            P[i][j]=0; 
            P_prev[i][j]=1;
        }
    }

    cout << " Condiciones iniciales establecidas" << endl;

}

void Coefficients ( double ap [N_row] [N_col], double ae [N_row] [N_col], double aw [N_row] [N_col], double an [N_row] [N_col], double as [N_row] [N_col], double bp [N_row] [N_col]) {

    cout << "Cálculo de coeficientes..." << endl;
    for (i=0; i<=N_row-1 ;i++){ 
        for (j=0; j<=N_col-1 ;j++){           
            ae[i][j]=1; aw[i][j]=1;
            an[i][j]=1; as[i][j]=1;
            bp[i][j]=0;
        }
    }
    
    // FRONTERAS
    for (i=0; i<=N_row-1 ;i++){
        aw[i][0]=0; ae[i][N_col-1]=0;
        an[i][0]=0; an[i][N_col-1]=0;
        as[i][0]=0; as[i][N_col-1]=0;
    }

    for (i=1;i<=N_row-2;i++){
        ae[i][0]=2; aw[i][1]=2;
        aw[i][N_col-1]=2; ae[i][N_col-2]=2;
    }

    for (j=0; j<=N_col-1; j++){
        an[0][j]=0; as[N_row-1][j]=0;
        aw[0][j]=0; aw[N_row-1][j]=0;
        ae[0][j]=0; ae[N_row-1][j]=0;
    }

    for (j=1; j<=N_col-2; j++){
        as[0][j]=2; an[1][j]=2;
        an[N_row-1][j]=2; as[N_row-2][j]=2;
    }

    for (i=0; i<=N_row-1; i++){
        for (j=0; j<=N_col-1; j++){ // Aparece en BBB que es Nf - 1 
            ap[i][j]=ae[i][j]+aw[i][j]+an[i][j]+as[i][j];
        }
    }

    for (i=0; i<=N_row-1; i++) {
        for (j=0; j<=N_col-1; j++){
            cout <<i<< " " <<j<<" : ae: " << ae[i][j] << " / an: " << an[i][j] << " // aw: "<<aw[i][j] <<" / as: "<<as[i][j]<< endl;
        
        }
    }

}

void factor_R (double R_U [N_row_stag_x] [N_col_stag_x], double R_V [N_row_stag_y] [N_col_stag_y],  double R_U_pred [N_row_stag_x] [N_col_stag_x], double R_V_pred [N_row_stag_y] [N_col_stag_y], double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y]) {
    
    double dx =1/double(N);

    for (i=0; i<=N_row_stag_x-1; i++){
        for (j=0; j<=N_col_stag_x-1; j++){
            R_U_pred[i][j]=R_U[i][j]; 
        }
    }

    for (i=0; i<=N_row_stag_y-1; i++){
        for (j=0; j<=N_col_stag_y-1; j++){
            R_V_pred[i][j]=R_V[i][j]; 
        }
    }
    
    for (i=1; i<=N_row_stag_x-2; i++){
        for (j=1; j<=N_col_stag_x-2; j++){
            R_U[i][j]=mu*(U[i][j+1]+U[i][j-1]+U[i+1][j]+U[i-1][j]-4*U[i][j])- rho*0.25*dx*((U[i][j]+U[i][j+1])*(U[i][j]+U[i][j+1])-(U[i][j]+U[i][j-1])*(U[i][j]+U[i][j-1])+(U[i][j]+U[i-1][j])*(V[i][j-1]+V[i][j])- (U[i][j]+U[i+1][j])*(V[i+1][j-1]+V[i+1][j]));
        }
    }

    for (i=1;i<=N_row_stag_y-2;i++){
        for (j=1;j<=N_col_stag_y-2;j++){
            R_V[i][j]=mu*(V[i][j+1]+V[i][j-1]+V[i-1][j]+V[i+1][j]-4*V[i][j])-rho*0.25*dx*((U[i][j+1]+U[i-1][j+1])*(V[i][j]+V[i][j+1])-(U[i][j]+U[i-1][j])*(V[i][j]+V[i][j-1])+(V[i][j]+V[i-1][j])*(V[i][j]+V[i-1][j])- (V[i][j]+V[i+1][j])*(V[i][j]+V[i+1][j]));
        }
    }

}

// TDMA solver (quicker than Gauss-Seidel)
void TDMA (double ap [N_row] [N_col], double bp [N_row] [N_col], double ae [N_row] [N_col], double aw [N_row] [N_col], double an [N_row] [N_col], double as [N_row] [N_col], double P_TDMA [N_col], double R_TDMA [N_col], double P_prev [N_row][N_col], double P [N_row][N_col]) {

    while (dif_P>=delta){ 
            for (i=0;i<=N_row-1;i++){
                for (j=0;j<=N_col-1;j++){
                P_prev[i][j]=P[i][j]; 
            }
        }

        for (i=1;i<=N_row-2;i++){
            
            P_TDMA[0]=ae[i][0]/ap[i][0]; 
            R_TDMA[0]=bp[i][0]/ap[i][0]; 

        for (j=1;j<=N_col-1;j++){

            P_TDMA[j]=ae[i][j]/(ap[i][j]-aw[i][j]*P_TDMA[j-1]); 
            R_TDMA[j]=(bp[i][j]+as[i][j]*P[i+1][j]+an[i][j]*P[i-1][j]+aw[i][j]*R_TDMA[j-1])/(ap[i][j]-aw[i][j]*P_TDMA[j-1]);
        }

        P[i][N_col-1]=R_TDMA[N_col-1]; 
        
        for (j=N_col-2;j>=0;j=j-1){
            P[i][j]=P_TDMA[j]*P[i][j+1]+R_TDMA[j]; 
        }
        }
        
        for (i=0;i<=N_row-1;i++){
            P[i][0]=P[i][1];
            P[i][N_col-1]=P[i][N_col-2];
        }
        for (j=0;j<=N_col-1;j++){
            P[N_row-1][j]=P[N_row-2][j];
            P[0][j]=P[1][j];
        }
            P[1][1]=0; 
            dif_P=0; 
        
        for (i=0;i<=N_row-1;i++){
            for (j=0;j<=N_col-1;j++){
                if ( abs(P[i][j]-P_prev[i][j])>=dif_P){
                    dif_P= abs(P[i][j]-P_prev[i][j]); 
                }
            }
        }
    }

}


double TimeStep(double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y][N_col_stag_y], double dt, double t){
    double dx = 1/double(N);

    double Conv_dt = 0; double Diff_dt=0; 

    double u_max=0;

    for (i=0;i<=N_row_stag_x-1;i++){
        for (j=0;j<=N_col_stag_x-1;j++){
            if (U[i][j]>u_max){
                u_max=U[i][j];
            }
        }
    }
    
    double v_max=0;
    for (i=0;i<=N_row_stag_y-1;i++){
        for (j=0;j<=N_col_stag_x-1;j++){
            if (V[i][j]>v_max){
                v_max=V[i][j];
            }
        }
    }

    double modVel=sqrt( u_max*u_max + v_max*v_max );
        
        Conv_dt = 0.35*dx/modVel; 
        Diff_dt = 0.2*rho*dx*dx/mu; 
        dt = min(Conv_dt ,Diff_dt)*1.0; 

    t=t+dt; 
    return t;
}


void Solution (double R_U [N_row_stag_x] [N_col_stag_x], double R_V [N_row_stag_y] [N_col_stag_y],  double R_U_pred [N_row_stag_x] [N_col_stag_x], double R_V_pred [N_row_stag_y] [N_col_stag_y], double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y], double ap [N_row] [N_col], double bp [N_row] [N_col], double ae [N_row] [N_col], double aw [N_row] [N_col], double an [N_row] [N_col], double as [N_row] [N_col], double P_TDMA [N_col], double R_TDMA [N_col], double P [N_row][N_col], double P_prev [N_row][N_col] ) {

    double dx =1/double(N);

    // Algorithm
        while (dif_V>=delta2 && t<=100){ 
            for (i=0;i<=N_row_stag_y-1;i++){
                for (j=0;j<=N_col_stag_y-1;j++){
                    V_prev[i][j]=V[i][j]; 
                }
            }   

        factor_R (R_U, R_V, R_U_pred, R_V_pred, U, V); 

        for (i=1;i<=N_row_stag_x-2;i++){
            for (j=1;j<=N_row_stag_x-2;j++){
                U_pred[i][j] = U[i][j]+dt*(1.5*R_U[i][j]- 0.5*R_U_pred[i][j])/(rho*dx*dx); 
            }
        }

        for (i=1;i<=N_row_stag_y-2;i++){
            for (j=1;j<=N_col_stag_y-2;j++){
                V_pred[i][j] = V[i][j]+dt*(1.5*R_V[i][j]- 0.5*R_V_pred[i][j])/(rho*dx*dx); 
            }
        }

        // Poisson Equation
        for (i=1;i<=N_row-2;i++){
            for (j=1;j<=N_col-2;j++){
                bp[i][j]=-rho*dx*(U_pred[i-1][j]-U_pred[i-1][j-1]+V_pred[i-1][j-1]-V_pred[i][j-1])/dt; 
            }
        }

        dif_P =1; 

        TDMA(ap, bp, ae, aw, an, as, P_TDMA, R_TDMA, P_prev, P); 

        for (i=0;i<=N_row_stag_x-1;i++){
            for (j=1;j<=N_col_stag_x-2;j++){
                U[i][j]= U_pred[i][j] - dt*( P[i+1][j+1] - P[i+1][j])/(2*dx); 
            }
        }

        for (i=1;i<=N_row_stag_y-2;i++){
            for (j=1;j<=N_col_stag_y-1;j++){
                V[i][j]=V_pred[i][j] - dt*( P[i][j+1] - P[i+1][j+1] )/(2*dx); 
            }
        }

        // BOUNDARY CONDITIONS
        for (i=0;i<=N_row_stag_x-1;i++){
            for (j=0;j<=N_col_stag_x-1;j++){
                U[0][j]=1;  U[N_row_stag_x-1][j]=0; 
                U[i][0]=0;  U[i][N_col_stag_x-1]=0; 
            }
        }

        for (i=0;i<=N_row_stag_y-1;i++){
            for (j=0;j<=N_col_stag_y-1;j++){
                V[0][j]=0;  V[N_row_stag_y-1][j]=0; 
                V[i][0]=0;  V[i][N_col_stag_y-1]=0; 
            }
        }

            t = TimeStep(U, V, dt, t); 
        
        if (time_steps >= 20){
            cout<<"Simulation time: "<<t<<" "<< endl; 
            time_steps=0;
        }

        time_steps++;
        dif_V = 0;

        for (i=0;i<=N_row_stag_y-1;i++){
            for (j=0;j<=N_col_stag_y-1;j++){
                if ( abs(V_prev[i][j]-V[i][j])>=dif_V){
                    dif_V = abs(V_prev[i][j]- V[i][j]); 
                }
            }
        }            

    }

} 

void Pasar_a_Fichero (double P [N_row][N_col], double U [N_row_stag_x] [N_col_stag_x], double V [N_row_stag_y] [N_col_stag_y], double X2 [N_row] [N_col], double Y2 [N_row] [N_col], double stagg_Xx [N_row_stag_x] [N_col_stag_x], double stagg_Xy [N_row_stag_x] [N_col_stag_x],  double stagg_Yx [N_row_stag_y] [N_col_stag_y], double stagg_Yy [N_row_stag_y] [N_col_stag_y]) {

    cout << "Guardando resultados..." << endl;
    
    // PRESSURE

    ofstream pressure;
    pressure.open("NS_pressure_Re1000_130x130.dat");

        for (i=0;i<=N_row-1;i++){
            for (j=0;j<=N_col-1;j++){
                pressure << fixed << setprecision(5) << X2[i][j] << "    ";
                pressure << fixed << setprecision(5) << Y2[i][j] << "    "; 
                pressure << fixed << setprecision(5) << P[i][j] << endl;
            }
        }
    
    pressure.close();  

    // VELOCITY U

    ofstream velX;
    velX.open("NS_VelX_Re1000_130x130.dat");

        for (i=0; i<=N_row_stag_x-1; i++){
            for (j=0; j<=N_col_stag_y-1; j++){
                velX << fixed << setprecision(5) << stagg_Xx[i][j] << "    ";
                velX << fixed << setprecision(5) << stagg_Xy[i][j] << "    "; 
                velX << fixed << setprecision(5) << U[i][j] << endl;
            }
        }
    
    velX.close();  

    // VELOCITY V

    ofstream velY;
    velY.open("NS_VelY_Re1000_130x130.dat");

        for (i=0; i<=N_row_stag_x-1; i++){
            for (j=0; j<=N_col_stag_y-1; j++){
                velY << fixed << setprecision(5) << stagg_Yx[i][j] << "    ";
                velY << fixed << setprecision(5) << stagg_Yy[i][j] << "    "; 
                velY << fixed << setprecision(5) << V[i][j] << endl;
            }
        }
    
    velY.close();  

    cout << "Archivos guardados en .dat" << endl;
    
}

// MAIN PROGRAM

int main () {

    Geometria2D(X,Y);
    StaggeredMesh(X, Y, stagg_Xx, stagg_Xy, stagg_Yx, stagg_Yy);
    CondicionesIniciales (U , V, U_prev, V_prev, P, P_prev, R_U, U_pred, R_V, V_pred);
    Coefficients (ap, ae, aw, an, as, bp);
    Solution (R_U, R_V, R_U_pred, R_V_pred, U, V, ap, bp, ae , aw, an, as, P_TDMA, R_TDMA, P, P_prev); 
    Pasar_a_Fichero ( P, U, V, X2, Y2, stagg_Xx, stagg_Xy, stagg_Yx, stagg_Yy );

}



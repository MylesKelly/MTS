#pragma once
#include <sundials/sundials_linearsolver.h> /* Generic Liner Solver Interface */
#include <sundials/sundials_types.h>        /* defs of realtype, sunindextype  */

#include <Eigen/Core>
#include <Eigen/Dense>

#include <fstream>

#include "gridStructures.hpp"

typedef std::function<double( double )> Fn;
using Matrix = Eigen::Matrix<realtype,Eigen::Dynamic,Eigen::Dynamic>;
using MatrixWrapper = Eigen::Map<Matrix>;
using Vector = Eigen::Matrix<realtype,Eigen::Dynamic,1>;
using VectorWrapper = Eigen::Map<Vector>;
typedef std::vector< std::pair< Interval, Eigen::Map<Eigen::VectorXd >>> Coeff_t;

class SystemSolver
{
public:
	SystemSolver(Grid const& Grid, unsigned int polyNum, unsigned int N_cells, double Dt, Fn const& rhs, Fn const& Tau, Fn const& c, Fn const& kappa, BoundaryConditions const& boundary );
	~SystemSolver() = default;

	//Initialises u, q and lambda to satisfy residual equation at t=0
	void setInitialConditions(std::function< double (double)> u_0, N_Vector& Y , N_Vector& dYdt);

	//Builds initial matrices
	void initialiseMatrices();
	
	void clearCellwiseVecs();

	// Takes n vector and parses it into the RHS of a Jacobian equation
	void sundialsToDGVecConversion(N_Vector const& g, std::vector< Eigen::VectorXd >& g1g2_cellwise);
	void sundialsToDGVecConversion(N_Vector const& Y, DGApprox& U, DGApprox& Q);

	// Returnable n_vector for sundials linear solver
	void DGtoSundialsVecConversion(DGApprox delU, DGApprox delQ, N_Vector& delY);

	//Points coeefficients to sundials vector so no copying needs to occur
	void mapDGtoSundials(DGApprox& q, DGApprox& u, N_Vector& Y);
	void mapDGtoSundials(DGApprox& u, N_Vector& Y);
	void mapDGtoSundials(std::vector< Eigen::Map<Eigen::VectorXd> >& QU_cell, N_Vector const& Y);

	// Set the q and u coefficients at each time step
	void updateCoeffs(N_Vector const& Y, N_Vector const& dYdt);

	//Creates the ABBDX cellwise matrices used at each Jacobian iteration
	void updateABBDForJacSolve(std::vector< Eigen::FullPivLU< Eigen::MatrixXd > >& tempABBDBlocks, double const alpha);

	//Solves the Jy = -G equation
	void solveJacEq(N_Vector const& g, N_Vector& delY);

	//Testing function, solving without IDA but using Sundials interface, Backward Euler used
	void solveNonIDA(N_Vector& Y, N_Vector& dYdt, double dt);

	void setAlpha(double const a) {alpha = a;}

	//print current output for u and q to output file
	void print( std::ostream& out, double t, int nOut  );

	//Find the profiles from the coefficients
	double EvalCoeffs( LegendreBasis & B, Coeff_t cs, double x );

	Fn getcfn() {return c_fn;}

	Grid grid;
	unsigned int const k; 		//polynomial degree per cell
	unsigned int const nCells;	//Total cell count
	
	std::vector< Eigen::MatrixXd > XMats;
	std::vector< Eigen::MatrixXd > ABBDBlocks;
	std::vector< Eigen::MatrixXd > CEBlocks;
	Eigen::MatrixXd K_global{};
	Eigen::VectorXd L_global{};
	Eigen::FullPivLU< Eigen::MatrixXd > H_global{};
	std::vector< Eigen::MatrixXd > CG_cellwise, RF_cellwise;
	std::vector< Eigen::MatrixXd > A_cellwise, B_cellwise, D_cellwise, E_cellwise, C_cellwise, G_cellwise, H_cellwise; //?Point the dublicated matrices to the same place?

	DGApprox u,q, dudt;
private:

	double dt;
	double t;
	bool initialised = false;
	double alpha = 1.0;

	Fn RHS; //Forcing function
	Fn c_fn,kappa_fn, tau; // convection velocity and diffusivity
	BoundaryConditions BCs;
	//std::map< double, std::pair< Coeff_t, Coeff_t > > u_of_t;
};

struct UserData {
	SystemSolver* system;
};
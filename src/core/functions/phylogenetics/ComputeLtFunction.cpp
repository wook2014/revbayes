//
//  ComputeLtFunction.cpp
//
//  Created by Rachel Warnock, Marc Manceau 30.01.2020.
//
#include "ComputeLtFunction.h"

#include <vector>
#include <iostream>
#include <cmath>

#include "RbMathMatrix.h"
#include "RbVector.h"
#include "TypedDagNode.h"
// other includes

namespace RevBayesCore { class DagNode; } //tt

using namespace RevBayesCore;

/** ComputeLtFunction
 * this is temporary until we work out where this function should live in the long term
 * @param v the vector of branching times
 */
ComputeLtFunction::ComputeLtFunction(
	const TypedDagNode< RbVector<double> > *a,
	const TypedDagNode< RbVector<double> > *b,
	const TypedDagNode< RbVector<double> > *c,
	const TypedDagNode< RbVector<double> > *d,
	const TypedDagNode< RbVector<double> > *e,
	const TypedDagNode< RbVector<double> > *f,
	const TypedDagNode< double > *t,
	const TypedDagNode< double > *l,
	const TypedDagNode< double > *m,
	const TypedDagNode< double > *p,
	const TypedDagNode< double > *o,
	const TypedDagNode< double > *rho,
	const TypedDagNode< double > *r,
	const TypedDagNode< RbVector<double> > *g
 	) : TypedFunction<double>( new double(0.0) ),
	listA ( a ), listB ( b ), listC ( c ), listD ( d ), listE ( e ), listF ( f ),
	tor ( t ), lambda ( l ), mu ( m ), psi ( p ), omega ( o ), rho ( rho ), removalPr ( r ),
	listG ( g )
{

	//poolTimes(a, b, b, c, d, e, f, g);

	//add the parameter as parents
	//this->addParameter( listA );
	//this->addParameter( listB );
	//this->addParameter( listC );
	//this->addParameter( listD );
	//this->addParameter( listE );
	//this->addParameter( listF );

	this->addParameter( tor );
	this->addParameter( lambda );
	this->addParameter( mu );
	this->addParameter( psi );
	this->addParameter( omega );
	this->addParameter( rho );
	this->addParameter( removalPr ); // I'm not sure if we need to do this

	poolTimes(); // step 1.
	update();

}

ComputeLtFunction::~ComputeLtFunction( void ){
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

/** clone function */
ComputeLtFunction* ComputeLtFunction::clone( void ) const
{
    return new ComputeLtFunction( *this );
}

// required -> update function, where you do the work
void ComputeLtFunction::update( void ) {

	const double birth = lambda->getValue();
	const double death = mu->getValue();
	const double ps = psi->getValue();
	const double om = omega->getValue();
	const double rh = rho->getValue();
	const double rp = removalPr->getValue();

	const double gamma = birth + death + ps + om;

	size_t N = 4; // accuracy of the algorithm
	size_t S = events.size();

	// step 2. initialize an empty matrix
	MatrixReal B(S, (N + 1), 0.0);

	// debugging
	//std::cout << B << std::endl;

	// step 3. // I think this is supposed to be the first entry of B
	std::vector<double> Lt;
	for ( int i = 0; i < (N + 1); i++){
		if (i == 0) Lt.push_back( pow(rh, extant) );
		else Lt.push_back( pow(rh, extant) * pow((1-rh), i) );
	}

	// step 4.
	//-> calculate the state of the process just before the next time point

	MatrixReal A( (N+1), (N+1), 0.0 );

	for(int h = 1; h < S; h++){

		double time = events[h].time;
		double next = events[h+1].time;
		std::string type = events[h].type;

		// step 5-6.
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				if(j == i) A[i][i] = -gamma * (extant + i);
				else if (j == i+1) A[i][i+1] = birth * ( (2 * extant) + i );
				else if (j == i-1) A[i][i-1] = death * i;
			}
		}

		A = A * (next - time);

		RbMath::expMatrixPade(A, A, 4);

		Lt = A * Lt;

		// step 7-10. sample Lt
		if(type == "time slice"){
			//-> what's the most efficient way to do this?
			// e.g. B[i] = Lt
			B[1][1] = 0.0; //placeholder
		}

		// step 11-12. exit loop
		//if()

		else if(type == "terminal removed"){ // step 13-14. if th is a removed leaf
			for(int i = 0; i < N; i++){

			}
		} else if (type == "") // step 15-16. 



	}

	// debugging
	std::cout << A << std::endl;

	double out = 0.1;

	out = lambda->getValue() * out;

	//MatrixReal B(2, 2, out);

	RbMath::expMatrixPade(B, B, 4);

	*this->value = A[0][0]; // this will eventually be the lk of the tree + occurrences

}

// required -> swap function. See Variance Function for inspiration
void ComputeLtFunction::swapParameterInternal( const DagNode *oldP, const DagNode *newP ) {

	if ( oldP == listA	)
	{
			listA = static_cast<const TypedDagNode< RbVector<double> >* >( newP ); //tt
	}

}

void ComputeLtFunction::poolTimes(void) {

	const std::vector<double> &a = listA->getValue(); // branching times
	const std::vector<double> &b = listB->getValue(); // sampled ancestors
	const std::vector<double> &c = listC->getValue(); // terminal non-removed
	const std::vector<double> &d = listD->getValue(); // terminal removed
	const std::vector<double> &e = listE->getValue(); // occurrences non-removed
	const std::vector<double> &f = listF->getValue(); // occurrences removed
	const std::vector<double> &g = listG->getValue(); // timeslices

	extant = 0;

	for(int i = 0; i < a.size(); i++){
	    events.push_back( Event(a[i], "branching time", 0) );
	}

	for(int i = 0; i < b.size(); i++){
	    events.push_back( Event(b[i], "sampled ancestor", 0) );
	}

	for(int i = 0; i < c.size(); i++){
	    if(c[i] == 0){
	      events.push_back( Event(c[i], "terminal non-removed", 1) );
				extant += 1;
			} else
	      events.push_back( Event(c[i], "terminal non-removed", 0) );
	}

	for(int i = 0; i < d.size(); i++){
	    if(c[i] == 0){
	      events.push_back( Event(d[i], "terminal removed", 1) );
				extant += 1;
			}
	    else
	      events.push_back( Event(d[i], "terminal removed", 0) );
	}

	for(int i = 0; i < e.size(); i++){
	    events.push_back( Event(e[i], "terminal non-removed", 0) );
	}

	for(int i = 0; i < f.size(); i++){
	    events.push_back( Event(f[i], "occurrence removed", 0) );
	}

	for(int i = 0; i < g.size(); i++){
	    events.push_back( Event(g[i], "time slice", 0) );
	}

	// order times youngest to oldest
	std::sort( events.begin(), events.end(), AgeCompare() );

	// debugging code
	for(int i = 0; i < events.size(); i++){
	 	Event e = events[i];
	 	std::cout << i << " " << e.time << std::endl;
	}

}
#include "MatrixMultiply.hpp"

#include <exception>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <numeric>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <boost/lexical_cast.hpp>
#include <string.h>

#define MAX_THREAD 20

scottgs::MatrixMultiply::MatrixMultiply()  { ; }
scottgs::MatrixMultiply::~MatrixMultiply() { ; }

typedef struct{
	int id;
	int n;
	int rows;
	const scottgs::FloatMatrix *lhs; 
	const scottgs::FloatMatrix *rhs;
	scottgs::FloatMatrix *result;

} domimatrix;

void leaver(int id, int step, const scottgs::FloatMatrix& lhs
									 , const scottgs::FloatMatrix& rhs
									 , scottgs::FloatMatrix& result){
	int i,j,k;
	for (i=id; i < lhs.size1(); i+=step){
		for (j=0; j < rhs.size2(); j++){
			for (k=0; k < rhs.size1(); k++){
				result(i,j) += lhs(i,k) * rhs(k,j);
			}
		}
	}
}


void * worker(void *joesMom) {
	domimatrix *p = (domimatrix *) joesMom;
	leaver(p->id, p->n, *(p->lhs), *(p->rhs), *(p->result));
	return NULL;
}

scottgs::FloatMatrix scottgs::MatrixMultiply::operator()(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
	// Verify acceptable dimensions
	if (lhs.size2() != rhs.size1())
		throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");
	scottgs::FloatMatrix result(lhs.size1(),rhs.size2());


	// YOUR ALGORIHM WITH COMMENTS GOES HERE:

	int             j, k, step, me_no;
	double          sum;
	double          t1, t2;

	pthread_t      *threads;
	pthread_attr_t  pthread_custom_attr;

	domimatrix *subMatrix;
	int n, i;
	n = 12;

	// Make room for domimatrices
	// ----------------------------------------------------------------------------
	threads = (pthread_t *) malloc(n * sizeof(pthread_t));
	pthread_attr_init(&pthread_custom_attr);
	subMatrix=(domimatrix *)malloc(sizeof(domimatrix)*n);

	// Create worker slaves
	// ----------------------------------------------------------------------------
	for (i = 0; i < n; i++){
		subMatrix[i].id   = i;
		subMatrix[i].n    = n;
		subMatrix[i].lhs  = &lhs;
		subMatrix[i].rhs  = &rhs;
		subMatrix[i].result = &result;
		pthread_create(&threads[i], &pthread_custom_attr, worker, (void *)(subMatrix+i));
	}

	for (i = 0; i < n; i++){
		pthread_join(threads[i], NULL);
	}

	free(subMatrix);
	return result;
}

scottgs::FloatMatrix scottgs::MatrixMultiply::multiply(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
	// Verify acceptable dimensions
	if (lhs.size2() != rhs.size1())
		throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");

	return boost::numeric::ublas::prod(lhs,rhs);
}


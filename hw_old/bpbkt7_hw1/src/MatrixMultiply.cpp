#include "MatrixMultiply.hpp"

#include <exception>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <numeric>

scottgs::MatrixMultiply::MatrixMultiply() 
{
	;
}

scottgs::MatrixMultiply::~MatrixMultiply()
{
	;
}


scottgs::FloatMatrix scottgs::MatrixMultiply::operator()(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
	// Verify acceptable dimensions
	if (lhs.size2() != rhs.size1())
		throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");

	scottgs::FloatMatrix result(lhs.size1(),rhs.size2());
	scottgs::FloatMatrix rhs_t(rhs.size2(),rhs.size1());

	unsigned int i=0,j=0,k=0;



	for (i=0;i<lhs.size1();i++){
		for (j=0;j<rhs.size2();j++){
			for (k=0;k<rhs.size1();k++){
				result(i,j) += lhs(i,k) * rhs(k,j);
				//result(i,j) += lhs(i,k) * rhs_t(j,k);
			}
		}
	}

	// ----------
	// leftovers
	// ----------

	// for (i=0;i<rhs.size1();i++){
	// 	for (j=0;j<rhs.size2();j++){
	// 		rhs_t(j,i) = rhs(i,j);
	// 	}
	// }

	// for (i=0;i<lhs.size1();i++){
	// 	for (j=0;j<rhs.size2();j++){
	// 		for (k=0;k<rhs.size1();k++){
	// 			leftHand[i+j+k]  = lhs(i,k);
	// 			rightHand[i+j+k] = rhs(k,j);
	// 		}
	// 	}
	// }

	// unsigned int ari=0;
	// unsigned int ari2=0;
	// while (ari++ < ar_l){
	// 	while (ari2++ < ar_l){
	// 	noHands[ari] = leftHand[ari] * rightHand[ari];
	// 	noHands[ari] += leftHand[ari] * rightHand[ari];
	// 	}
	// }

	// for (i=0;i<lhs.size1();i++){
	// 	for (j=0;j<rhs.size2();j++){
	// 		for (k=0;k<rhs.size1();k++){
	// 			result(i,j) += noHands[i+j+k];
	// 		}
	// 	}
	// }


	return result;
}

scottgs::FloatMatrix scottgs::MatrixMultiply::multiply(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const
{
	// Verify acceptable dimensions
	if (lhs.size2() != rhs.size1())
		throw std::logic_error("matrix incompatible lhs.size2() != rhs.size1()");

	return boost::numeric::ublas::prod(lhs,rhs);
}


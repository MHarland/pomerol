#include "Vertex4Part.h"

inline bool chaseIndices(RowMajorMatrixType::InnerIterator& index1_iter, 
                         ColMajorMatrixType::InnerIterator& index2_iter)
{
    InnerQuantumState index1 = index1_iter.index();
    InnerQuantumState index2 = index2_iter.index();
  
    if(index1 == index2) return true;
    
    if(index1 < index2) 
        for(;InnerQuantumState(index1_iter.index())<index2 && index1_iter; ++index1_iter);
    else 
        for(;InnerQuantumState(index2_iter.index())<index1 && index2_iter; ++index2_iter);
        
    return false;
}

Vertex4Part::Vertex4TermType1::Vertex4TermType1(RealType weight, 
                                                RealType E1, RealType E2, RealType E3, RealType E4,
                                                Permutation3& Permutation)
{
    Residue = -weight*Permutation.sign;
    Poles[Permutation.perm[0]] = E2 - E1;
    Poles[Permutation.perm[1]] = E3 - E2;
    Poles[Permutation.perm[2]] = E4 - E3;
}

inline ComplexType Vertex4Part::Vertex4TermType1::operator()
            (ComplexType Frequency1, ComplexType Frequency2, ComplexType Frequency3) const
{
    return Residue/((Frequency1 - Poles[0])*(Frequency2 - Poles[1])*(-Frequency3 - Poles[2]));
}

Vertex4Part::Vertex4Part(
                FieldOperatorPart& O1, FieldOperatorPart& O2, FieldOperatorPart& O3, CreationOperatorPart& CX4,
                HamiltonianPart& Hpart1, HamiltonianPart& Hpart2, HamiltonianPart& Hpart3, HamiltonianPart& Hpart4,
                DensityMatrixPart& DMpart1, DensityMatrixPart& DMpart2, DensityMatrixPart& DMpart3, DensityMatrixPart& DMpart4,
                Permutation3 Permutation) :
O1(O1), O2(O2), O3(O3), CX4(CX4), 
Hpart1(Hpart1), Hpart2(Hpart2), Hpart3(Hpart3), Hpart4(Hpart4),
DMpart1(DMpart1), DMpart2(DMpart2), DMpart3(DMpart3), DMpart4(DMpart4),
Permutation(Permutation)
{
    // TODO
}

void Vertex4Part::compute(Vertex4Part::ComputationMethod method)
{
    switch(method){
        case ChasingIndices0: computeChasing0(); break;
        case ChasingIndices1: computeChasing1(); break;
        case ChasingIndices2: computeChasing2(); break;
        default: assert(0);
    }
    
    computeReduciblePart();
};

void Vertex4Part::computeChasing0(void)
{
    ColMajorMatrixType& O1matrix = O1.getColMajorValue();
    ColMajorMatrixType& O2matrix = O2.getColMajorValue();    
    ColMajorMatrixType& O3matrix = O3.getColMajorValue();
    ColMajorMatrixType& CX4matrix = CX4.getColMajorValue();
    
    InnerQuantumState index1ket;
    InnerQuantumState index1ketMax = CX4matrix.outerSize();
    
    for(index1ket=0; index1ket<index1ketMax; ++index1ket){
        ColMajorMatrixType::InnerIterator index4bra(CX4matrix,index1ket);       
        while(index4bra){
            InnerQuantumState index4ket = index4bra.index();
            ColMajorMatrixType::InnerIterator index3bra(O3matrix,index4ket);
            while(index3bra){
                InnerQuantumState index3ket = index3bra.index();
                ColMajorMatrixType::InnerIterator index2bra(O2matrix,index3ket);
                while(index2bra){
                    InnerQuantumState index2ket = index2bra.index();
                    ColMajorMatrixType::InnerIterator index1bra(O1matrix,index2ket);
                    while(index1bra){
                        InnerQuantumState index1 = index1bra.index();
                        if(index1 == index1ket){
                            // TODO
                            //DEBUG("been there");
                        }
                        ++index1bra;
                    }
                    ++index2bra;
                }
                ++index3bra;
            }
            ++index4bra;
        }
    }
}

void Vertex4Part::computeChasing1(void)
	// I don't have any pen now, so I'm writing here:
	// <1 | O1 | 2> <2 | O2 | 3> <3 | O3 |4> <4| CX4 |1>
{
    RowMajorMatrixType& O1matrix = O1.getRowMajorValue();
    RowMajorMatrixType& O2matrix = O2.getRowMajorValue();    
    ColMajorMatrixType& O3matrix = O3.getColMajorValue();
    ColMajorMatrixType& CX4matrix = CX4.getColMajorValue();
    
    InnerQuantumState index1;
    InnerQuantumState index1Max = CX4matrix.outerSize();
    
    for(index1=0; index1<index1Max; ++index1){
        ColMajorMatrixType::InnerIterator index4bra(CX4matrix,index1);       
        while(index4bra){
            InnerQuantumState index4ket = index4bra.index();
            RowMajorMatrixType::InnerIterator index2ket(O1matrix,index1);       
            while (index2ket){
                InnerQuantumState index2bra = index2ket.index();
                RowMajorMatrixType::InnerIterator index3ket_iter(O2matrix,index2bra);
            	ColMajorMatrixType::InnerIterator index3bra_iter(O3matrix,index4ket);
            	while(index3bra_iter && index3ket_iter){
                    if(chaseIndices(index3ket_iter,index3bra_iter)){
                        // TODO
                
                        ++index3ket_iter;
                        ++index3bra_iter;
                    }
                };
             	++index2ket;
            };
            ++index4bra;
        };
    };
};

void Vertex4Part::computeChasing2(void)
{
	// I don't have any pen now, so I'm writing here:
	// <1 | O1 | 2> <2 | O2 | 3> <3 | O3 |4> <4| CX4 |1>
    RowMajorMatrixType& O1matrix = O1.getRowMajorValue();
    ColMajorMatrixType& O2matrix = O2.getColMajorValue();    
    RowMajorMatrixType& O3matrix = O3.getRowMajorValue();
    ColMajorMatrixType& CX4matrix = CX4.getColMajorValue();
    
    InnerQuantumState index1;
    InnerQuantumState index1Max = CX4matrix.outerSize();
    
    InnerQuantumState index3;
    InnerQuantumState index3Max = O2matrix.outerSize();

    for(index1=0; index1<index1Max; ++index1){
    for(index3=0; index3<index3Max; ++index3){
        ColMajorMatrixType::InnerIterator index4bra_iter(CX4matrix,index1);       
        RowMajorMatrixType::InnerIterator index4ket_iter(O3matrix,index3);
		std::list<InnerQuantumState> Index4List;
		while (index4bra_iter && index4ket_iter){
            if(chaseIndices(index4ket_iter,index4bra_iter)){
				Index4List.push_back(index4bra_iter.index());
				++index4bra_iter;
				++index4ket_iter;
			}
 		};

		if (Index4List.size()!=0)
		{
            ColMajorMatrixType::InnerIterator index2bra_iter(O2matrix,index3);
            RowMajorMatrixType::InnerIterator index2ket_iter(O1matrix,index1);       
            while (index2bra_iter && index2ket_iter){
                if (chaseIndices(index2ket_iter,index2bra_iter)){
					for (std::list<InnerQuantumState>::iterator index4 = Index4List.begin(); index4!=Index4List.end(); ++index4) 
					{
						// TODO
					}
					++index2bra_iter;
					++index2ket_iter;
                }
			};
		};
    }};
}

void Vertex4Part::computeReduciblePart(void)
{
    // TODO
}

ComplexType Vertex4Part::operator()(ComplexType Frequency1, ComplexType Frequency2, ComplexType Frequency3)
{
    ComplexType Value = 0;
    for(std::list<Vertex4TermType1>::const_iterator pTerm = TermsType1.begin(); pTerm != TermsType1.end(); ++pTerm)
        Value += (*pTerm)(Frequency1,Frequency2,Frequency3);
    
    return Value;
}
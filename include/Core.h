#ifndef CORE_H
#define CORE_H

/*! @file Core.h
    @brief This file contains all the necessary "include" and definition of Getfem++ types we will be using
*/

#include <getfem/getfem_regular_meshes.h>
#include <getfem/getfem_interpolation.h>
#include <getfem/getfem_derivatives.h>
#include <getfem/getfem_config.h>
#include <getfem/getfem_assembling.h> // import assembly methods 
#include <getfem/getfem_import.h>
#include <getfem/getfem_export.h>  // export functions (save the solution in a file)
#include <gmm/gmm.h>
#include <gmm/gmm_inoutput.h>
#include <gmm/gmm_MUMPS_interface.h>
#include <gmm/gmm_superlu_interface.h>

#include <getfem/bgeot_mesh.h>

#include "GetPot"
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>



/*! special class for small (dim < 16) vectors */
using bgeot::base_small_vector;

/*! geometrical nodes (derived from base_small_vector)*/
using bgeot::base_node;

/*! double */
using bgeot::scalar_type;

/*! unsigned long */
using bgeot::size_type;


using std::string;

/* definition of some matrix/vector types. These ones are built
 using the predefined types in Gmm++ */

using sparseVector_Type = gmm::rsvector<scalar_type>;

using sparseMatrix_Type = gmm::row_matrix<sparseVector_Type> ;
using sparseMatrixPtr_Type = std::shared_ptr<sparseMatrix_Type> ;

using scalarVector_Type = std::vector<scalar_type> ;
using scalarVectorPtr_Type = std::shared_ptr<scalarVector_Type> ; // pointer to a vector of double

using sizeVector_Type = std::vector<size_type> ;
using sizeVectorPtr_Type = std::shared_ptr<sizeVector_Type> ;


namespace LifeV{
using Double = scalar_type ;
using UInt = size_type ;
using ID = size_type ;
using Int = int;
}


#endif

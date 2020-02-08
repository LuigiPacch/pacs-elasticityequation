#include "../include/BC.h"

BC::BC ( const GetPot& dataFile,
	 const std::string& problem,
                                 const std::string& section) :
            M_section ( section + problem ),
	    M_nBoundaries( dataFile ( ( M_section + "nBoundaries" ).data (), 4) ),
	    M_BCstring( dataFile ( ( M_section + "bcflag" ).data (), "1") ),
 	    M_BCNeumX( dataFile ( ( M_section + "du_BC_X" ).data (), "1") ),
			M_BCNeumY( dataFile ( ( M_section + "du_BC_Y" ).data (), "1") ),
 	    M_BCDiriX( dataFile ( ( M_section + "u_BC_X" ).data (), "1") ),
			M_BCDiriY( dataFile ( ( M_section + "u_BC_Y" ).data (), "1") )
{

	std::cout << "stringa section: " << M_section << std::endl;
	std::cout << "stringa BC: " << M_BCstring << std::endl;
	std::cout << "condizione Dirichlet x: "<< M_BCDiriX << std::endl;
	std::cout << "condizione Dirichlet y: "<< M_BCDiriY << std::endl;

    M_BC.resize(M_nBoundaries,0);

    M_parser.setString ( M_BCstring );


    for ( size_type i = 0; i < M_nBoundaries; ++i )
    {
        M_BC [ i ] = M_parser.evaluate ( i ); // vettore di indici: suppongo che contenga 0 per Dirichlet e 1 per Neumann nell'ordine dato dal Datafile (sotto, dx, sopra, sx)

        if (M_BC[i]==0)
        {
        	M_DiriRG.push_back(i);
        }
        if (M_BC[i]==1)
        {
        	M_NeumRG.push_back(i);
        }

    }


    M_bdNodesX.resize(M_nBoundaries+1,0);
    std::string xnodes(dataFile ( ( M_section + "boundaryNodesX" ).data (), "[0,1,1,0]"));
    M_parser.setString (xnodes);

    for ( size_type i = 0; i < M_nBoundaries; ++i )
    {

        M_bdNodesX [ i ] = M_parser.evaluate ( i );

    }
    M_bdNodesX [ M_nBoundaries ]= M_bdNodesX [ 0 ];
    M_bdNodesY.resize(M_nBoundaries+1,0);

    std::string ynodes(dataFile ( ( M_section + "boundaryNodesY" ).data (), "[0,1,1,0]"));
    M_parser.setString ( ynodes );

    for ( size_type i = 0; i < M_nBoundaries; ++i )
    {
        M_bdNodesY [ i ] = M_parser.evaluate ( i );

    }
    M_bdNodesY [ M_nBoundaries ]= M_bdNodesY [ 0 ];

}

scalar_type BC::BCNeum(const base_node& x, const size_type what, const size_type& flag)
{
	if(what == 0){ // Prima componente del vettore dei dati
    M_parser.setString ( M_BCNeumX );
	}
	else if (what == 1){ // Seconda componente del vettore dei dati
		M_parser.setString ( M_BCNeumY );
	}
    M_parser.setVariable ( "x", x [ 0 ] );
    M_parser.setVariable ( "y", x [ 1 ] );
    M_parser.setVariable ( "n", flag );
    return M_parser.evaluate ();
}



scalar_type BC::BCDiri(const base_node& x, const size_type what, const size_type& flag)
{
	if(what == 0){
		M_parser.setString ( M_BCDiriX );
	}
	else if (what == 1){
		M_parser.setString ( M_BCDiriY );
	}
    M_parser.setVariable ( "x", x [ 0 ] );
    M_parser.setVariable ( "y", x [ 1 ] );
    M_parser.setVariable ( "n", flag );
    return M_parser.evaluate ();
}


std::vector<size_type> BC::getNeumBD(std::string where)
	{

		return M_NeumRG;
	}

std::vector<size_type> BC::getDiriBD(std::string where)
	{

		return M_DiriRG;
	}


void BC::setBoundaries(getfem::mesh* meshPtr)
{
    getfem::mesh_region border_faces;
    getfem::outer_faces_of_mesh(*meshPtr, border_faces);
    std::vector<int> verticalBDs;
    std::vector<int> horizontalBDs;

    for (int j=0; j<M_nBoundaries;++j)
    {
		if (M_bdNodesX[j]==M_bdNodesX[j+1])
			verticalBDs.push_back(j);
		if (M_bdNodesY[j]==M_bdNodesY[j+1])
			horizontalBDs.push_back(j);
    }

    for (getfem::mr_visitor i(border_faces); !i.finished(); ++i)
    {
    	assert(i.is_face());
        base_node un = meshPtr->normal_of_face_of_convex(i.cv(), i.f());
        un /= gmm::vect_norm2(un);

	if (gmm::abs(un[0])<1.0e-7 && gmm::abs(un[1]+1)<1.0e-7)
      	{
      		meshPtr->region(0).add(i.cv(),i.f());
      	}
      	if (gmm::abs(un[0]-1)<1.0e-7 && gmm::abs(un[1])<1.0e-7)
      	{
      		meshPtr->region(1).add(i.cv(),i.f());
      	}
      	if (gmm::abs(un[0])<1.0e-7 && gmm::abs(un[1]-1)<1.0e-7)
     	{
 	     	 meshPtr->region(2).add(i.cv(),i.f());
      	}
      	if (gmm::abs(un[0]+1)<1.0e-7 && gmm::abs(un[1])<1.0e-7)
      	{
		 meshPtr->region(3).add(i.cv(),i.f());
      	}
    }
}

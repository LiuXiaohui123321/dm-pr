//==========================================================
// AUTHOR : Lixin He , mohan
// DATE : 2008-11-08
//==========================================================
#include <cstdlib>
#ifdef __MPI
#include <mpi.h>
#endif

#include "global.h"
#include "tools.h"
#include "unitcell.h"
using namespace std;

UnitCell::UnitCell()
{
    if (test_unitcell) TITLE("unitcell","Constructor");
    Coordinate = "Direct";
    latName = "none";
    lat0 = 0.0;
    lat0_angstrom = 0.0;

    ntype = 0;
    nat = 0;
    namax = 0;
    nwmax = 0;

    iat2it = nullptr;
    iat2ia = nullptr;
	iwt2iat = nullptr;
	iwt2iw = nullptr;

    itia2iat.create(1,1);
	lc = new int[3];
    itiaiw2iwt.create(1,1,1);

    latvec = Matrix3();
    latvec_supercell = Matrix3();
    G = Matrix3();
    GT = Matrix3();
    GGT = Matrix3();
    invGGT = Matrix3();

    tpiba = 0.0;
    tpiba2 = 0.0;
    omega = 0.0;
}

UnitCell::~UnitCell()
{
#ifndef __EPM
    delete[] atom_label;
    delete[] atom_mass;
    delete[] pseudo_fn;
#endif
    delete[] iat2it;
    delete[] iat2ia;
	delete[] iwt2iat;
	delete[] iwt2iw;
	delete[] lc;
}


#ifdef __MPI
void UnitCell::bcast_unitcell(void)
{
    if (test_unitcell)TITLE("UnitCell","bcast_unitcell");
    Parallel_Common::bcast_string( Coordinate );
    Parallel_Common::bcast_int( nat );

    Parallel_Common::bcast_double( lat0 );
    Parallel_Common::bcast_double( lat0_angstrom );
	Parallel_Common::bcast_double( tpiba );
	Parallel_Common::bcast_double( tpiba2 );

	// distribute lattice vectors.
    Parallel_Common::bcast_double( latvec.e11 );
    Parallel_Common::bcast_double( latvec.e12 );
    Parallel_Common::bcast_double( latvec.e13 );
    Parallel_Common::bcast_double( latvec.e21 );
    Parallel_Common::bcast_double( latvec.e22 );
    Parallel_Common::bcast_double( latvec.e23 );
    Parallel_Common::bcast_double( latvec.e31 );
    Parallel_Common::bcast_double( latvec.e32 );
    Parallel_Common::bcast_double( latvec.e33 );

    Parallel_Common::bcast_int( lc[0] );
    Parallel_Common::bcast_int( lc[1] );
    Parallel_Common::bcast_int( lc[2] );
	
	// distribute lattice vectors.
    Parallel_Common::bcast_double( a1.x );
    Parallel_Common::bcast_double( a1.y );
    Parallel_Common::bcast_double( a1.z );
    Parallel_Common::bcast_double( a2.x );
    Parallel_Common::bcast_double( a2.y );
    Parallel_Common::bcast_double( a2.z );
    Parallel_Common::bcast_double( a3.x );
    Parallel_Common::bcast_double( a3.y );
    Parallel_Common::bcast_double( a3.z );

	// distribute latcenter
    Parallel_Common::bcast_double( latcenter.x );
    Parallel_Common::bcast_double( latcenter.y );
    Parallel_Common::bcast_double( latcenter.z );

	// distribute superlattice vectors.
    Parallel_Common::bcast_double( latvec_supercell.e11 );
    Parallel_Common::bcast_double( latvec_supercell.e12 );
    Parallel_Common::bcast_double( latvec_supercell.e13 );
    Parallel_Common::bcast_double( latvec_supercell.e21 );
    Parallel_Common::bcast_double( latvec_supercell.e22 );
    Parallel_Common::bcast_double( latvec_supercell.e23 );
    Parallel_Common::bcast_double( latvec_supercell.e31 );
    Parallel_Common::bcast_double( latvec_supercell.e32 );
    Parallel_Common::bcast_double( latvec_supercell.e33 );

#ifndef __EPM
    Parallel_Common::bcast_double( mag.start_magnetization, ntype );
#endif
    if(NSPIN != 1)
    for(int it = 0;it<ntype;it++)
    {
         Parallel_Common::bcast_double( soc.m_loc[it].x );
         Parallel_Common::bcast_double( soc.m_loc[it].y );
         Parallel_Common::bcast_double( soc.m_loc[it].z );
    }
    if(NSPIN==4)
    {
         Parallel_Common::bcast_double( soc.ux[0] );
         Parallel_Common::bcast_double( soc.ux[1] );
         Parallel_Common::bcast_double( soc.ux[2] );
    }

    for (int i=0;i<ntype;i++)
    {
        atoms[i].bcast_atom(); // init tau and mbl array
    }
    return;
}

void UnitCell::bcast_unitcell2(void)
{
    for (int i=0;i<ntype;i++)
    {
        atoms[i].bcast_atom2();
    }
    return;
}
#endif

void UnitCell::print_cell(ofstream &ofs)const
{
    if (test_unitcell) TITLE("UnitCell","print_cell");

    OUT(ofs,"print_unitcell()");

    OUT(ofs,"latName",latName);
    OUT(ofs,"ntype",ntype);
    OUT(ofs,"nat",nat);
    OUT(ofs,"lat0",lat0);
    OUT(ofs,"lat0_angstrom",lat0_angstrom);
    OUT(ofs,"tpiba",tpiba);
    OUT(ofs,"omega",omega);

    out.printM3(ofs,"Lattices Vector (R) : ", latvec);
    out.printM3(ofs ,"Supercell lattice vector : ", latvec_supercell);
    out.printM3(ofs, "Reciprocal lattice Vector (G): ", G);
    out.printM3(ofs, "GGT : ", GGT);

    ofs<<endl;
    return;
}

void UnitCell::print_cell_cif(const string &fn)const
{
	if (test_unitcell) TITLE("UnitCell","print_cell_cif");
	
	if(MY_RANK!=0) return;//xiaohui add 2015-03-15

	stringstream ss;
	ss << global_out_dir << fn;
	
	ofstream ofs( ss.str().c_str() );
	ofs << "data_" << latName << endl;
	ofs << endl;
	//xiaohui modify 2015-03-25
	//ofs << "_audit_creation_method" << " generated by MESIA" << endl;
	ofs << "_audit_creation_method" << " generated by ABACUS" << endl;
	ofs << endl;
	ofs << "_cell_length_a " << a1.norm()*lat0*0.529177 << endl;
	ofs << "_cell_length_b " << a2.norm()*lat0*0.529177 << endl;
	ofs << "_cell_length_c " << a3.norm()*lat0*0.529177 << endl;

	//xiaohui modify and add 2014-12-21
	//ofs << "_cell_angle_alpha " << "90.00" << endl; 
	//ofs << "_cell_angle_beta " << "90.00" << endl; 
	//ofs << "_cell_angle_gamma " << "90.00" << endl; 
	//xiaohui modify alpha, beta and gamma 2015-09-29
	double angle_alpha = acos((a2 * a3) / (a2.norm() * a3.norm())) /PI*180.0;
	double angle_beta = acos((a1 * a3) / (a1.norm() * a3.norm())) /PI*180.0;
	double angle_gamma = acos((a1 * a2) / (a1.norm() * a2.norm())) /PI*180.0;
	ofs << "_cell_angle_alpha " << angle_alpha << endl;
	ofs << "_cell_angle_beta " << angle_beta << endl;
	ofs << "_cell_angle_gamma " << angle_gamma << endl;
	ofs << endl;
	ofs << "_symmetry_space_group_name_H-M" << " " << endl;
	ofs << "_symmetry_Int_Tables_number" << " " << endl;
	ofs << endl;
	ofs << "loop_" << endl;
	ofs << "_atom_site_label" << endl;
	ofs << "_atom_site_fract_x" << endl;
	ofs << "_atom_site_fract_y" << endl;
	ofs << "_atom_site_fract_z" << endl;
    for (int it=0; it<ntype; it++)
    {
        for (int ia=0; ia<atoms[it].na; ia++)
        {
            ofs << atoms[it].label
            << " " << atoms[it].taud[ia].x
            << " " << atoms[it].taud[ia].y
            << " " << atoms[it].taud[ia].z << endl;
        }
    }
    ofs.close();
}

void UnitCell::print_cell_xyz(const string &fn)const
{
    if (test_unitcell) TITLE("UnitCell","print_cell_xyz");

	if(MY_RANK!=0) return;//xiaohui add 2015-03-15

    stringstream ss;
    ss << global_out_dir << fn;

    ofstream ofs( ss.str().c_str() );

    ofs << nat << endl;
    ofs << latName << endl;
    for (int it=0; it<ntype; it++)
    {
        for (int ia=0; ia<atoms[it].na; ia++)
        {
            ofs << atoms[it].label
            << " " << atoms[it].tau[ia].x * lat0 * 0.529177
            << " " << atoms[it].tau[ia].y * lat0 * 0.529177
            << " " << atoms[it].tau[ia].z * lat0 * 0.529177 << endl;
        }
    }

    ofs.close();
    return;
}

void UnitCell::set_iat2it(void)
{
	assert(nat>0);
	delete[] iat2it;
	this->iat2it = new int[nat];
	int iat=0;
	for(int it = 0;it < ntype;it++)
	{
		for(int ia=0; ia<atoms[it].na; ia++)
		{
			this->iat2it[iat] = it;
			++iat;
		}	
	}
	return;
}

#ifndef PW_BASIS_K_BIG_H
#define PW_BASIS_K_BIG_H
#include "../module_base/constants.h"
#include "../module_base/global_function.h"

// temporary class, because previous ABACUS consider big grid for fft grids 
// which are used for grid integration in LCAO.
// In fact, it is unnecessary. It will be moved after grid integration is refactored.
namespace ModulePW
{

class PW_Basis_K_Big: public PW_Basis_K
{
public:
    
    // combine [bx,by,bz] FFT grids into a big one
	// typical values are bx=2, by=2, bz=2
	// nbx=nx/bx, nby=ny/by, nbz=nz/bz, 
    PW_Basis_K_Big(){
        bx = 1;
        by = 1;
        bz = 1;
    }
    ~PW_Basis_K_Big(){};
    void setbxyz(const int bx_in, const int by_in, const int bz_in)
    {
        bx = bx_in;
        by = by_in;
        bz = bz_in;
    }
    int bx,by,bz;
    int nbx, nby, nbz;
    virtual void distribute_r()
    {
        this->nbx = this->nx / bx;
        this->nby = this->ny / by;
        this->nbz = this->nz / bz;
        delete[] this->numz; this->numz = new int[this->poolnproc];
        delete[] this->startz; this->startz = new int[this->poolnproc];
        ModuleBase::GlobalFunc::ZEROS(this->numz, this->poolnproc);
        ModuleBase::GlobalFunc::ZEROS(this->startz, this->poolnproc);

        int npbz = this->nbz / this->poolnproc;
        int modbz = this->nbz % this->poolnproc;
        this->startz[0] = 0;
        for(int ip = 0 ; ip < this->poolnproc ; ++ip)
        {
            this->numz[ip] = npbz*this->bz;
            if(ip < modbz)   this->numz[ip]+=this->bz;
            if(ip < this->poolnproc - 1)   this->startz[ip+1] = this->startz[ip] + numz[ip];
            if(ip == this->poolrank) 
            {
                this->nplane = numz[ip];
                this->startz_current = startz[ip];
            }
        }
        this->nrxx = this->numz[this->poolrank] * this->nxy;
        return;
    }

};
}
#endif
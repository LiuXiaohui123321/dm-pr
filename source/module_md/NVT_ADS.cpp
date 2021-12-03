#include "NVT_ADS.h"
#include "MD_func.h"

NVT_ADS::NVT_ADS(MD_parameters& MD_para_in, UnitCell_pseudo &unit_in) : Verlet(MD_para_in, unit_in)
{
    // convert to a.u. unit
    mdp.viscosity *= ModuleBase::AU_to_FS;

    nraise = mdp.viscosity * mdp.dt;
}

NVT_ADS::~NVT_ADS(){}

void NVT_ADS::setup()
{
    ModuleBase::TITLE("NVT_ADS", "setup");
    ModuleBase::timer::tick("NVT_ADS", "setup");

    Verlet::setup();

    ModuleBase::timer::tick("NVT_ADS", "setup");
}

void NVT_ADS::first_half()
{
    ModuleBase::TITLE("NVT_ADS", "first_half");
    ModuleBase::timer::tick("NVT_ADS", "first_half");

    Verlet::first_half();

    ModuleBase::timer::tick("NVT_ADS", "first_half");
}

void NVT_ADS::second_half()
{
    ModuleBase::TITLE("NVT_ADS", "second_half");
    ModuleBase::timer::tick("NVT_ADS", "second_half");

    Verlet::second_half();

    double deviation;
    for(int i=0; i<ucell.nat; ++i)
    {
        if(rand()/double(RAND_MAX) <= 1.0/nraise)
        {
            deviation = sqrt(t_last / allmass[i]);
            for(int k=0; k<3; ++k)
            {
                if(ionmbl[i][k]) 
                {
                    vel[i][k] = deviation * MD_func::gaussrand();
                }
            }
        }
    }

    ModuleBase::timer::tick("NVT_ADS", "second_half");
}

void NVT_ADS::outputMD()
{
    Verlet::outputMD();
}

void NVT_ADS::write_restart()
{
    Verlet::write_restart();
}

void NVT_ADS::restart()
{
    Verlet::restart();
}
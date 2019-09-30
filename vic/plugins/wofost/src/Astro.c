#include <vic_driver_image.h>
#include <plugin.h>

/* ---------------------------------------------------------------------*/
/*  function Astro()                                                    */
/*  Purpose: Calculation of the astronomical parameters used in Wofost  */
/*                                                                      */
/*  Originally written in Fortran by:                                   */
/*         Daniel van Kraalingen, April 1991                            */
/*         revised Allard de Wit, January 2011                          */
/* ---------------------------------------------------------------------*/

void Astro(SimUnit *Grid)
{
    float Declination;
    float SolarConstant;
    float AOB;
    float DSinB;
    float FractionDiffuseRad;
    float tmp_min;
    
    float day_fl = Grid->vic->MeteoDay;
    
    if (fabs(Grid->vic->Latitude) > 90.) {
        log_err("Latitude is > 90 or < -90");
    }

    /* We start at Day= 1, we do not use Day = 0 */
    Declination    = -asin(sin(23.45*RAD_PER_DEG)*cos(2.*CONST_PI*(day_fl+10.)/365.));
    SolarConstant  = 1370.*(1.+0.033*cos(2.*CONST_PI*(day_fl)/365.));
  
    Grid->vic->SinLD = sin(RAD_PER_DEG*Grid->vic->Latitude)*sin(Declination);
    Grid->vic->CosLD = cos(RAD_PER_DEG*Grid->vic->Latitude)*cos(Declination);
    AOB   = Grid->vic->SinLD/Grid->vic->CosLD;
    
   /* Astronomical day length */
    tmp_min = min(24.,12.0*(1.+2.*asin(AOB)/CONST_PI));
    Grid->vic->Daylength = max(0, tmp_min);
    
    /* Photoactive day length */
    tmp_min = min(24.,12.0*(1.+2.*asin((-sin(-4.0*RAD_PER_DEG)+Grid->vic->SinLD)/Grid->vic->CosLD)/CONST_PI));
    Grid->vic->PARDaylength = max(0, tmp_min);
    
    /* Integrals of sine of solar height */
    if (AOB <= 1.0)
    {   
        DSinB  = 3600.*(Grid->vic->Daylength*Grid->vic->SinLD+(24./CONST_PI)*Grid->vic->CosLD*sqrt(1.-AOB*AOB));
        Grid->vic->DSinBE = 3600.*(Grid->vic->Daylength*(Grid->vic->SinLD+0.4*(Grid->vic->SinLD*Grid->vic->SinLD + Grid->vic->CosLD*Grid->vic->CosLD*0.5))+
            12.*Grid->vic->CosLD*(2.+3.*0.4*Grid->vic->SinLD)*sqrt(1.-AOB*AOB)/CONST_PI);
    }  
    else
    {
        DSinB  = 3600.*(Grid->vic->Daylength*Grid->vic->SinLD);
        Grid->vic->DSinBE = 3600.*(Grid->vic->Daylength*(Grid->vic->SinLD+0.4*(Grid->vic->SinLD*Grid->vic->SinLD + Grid->vic->CosLD*Grid->vic->CosLD*0.5)));
    }
    
    /*  Extraterrestrial radiation and atmospheric transmission */
    Grid->vic->AngotRadiation  = SolarConstant*DSinB;
    Grid->vic->AtmosphTransm   = Grid->vic->Radiation/Grid->vic->AngotRadiation;

    if (Grid->vic->AtmosphTransm > 0.75)
       FractionDiffuseRad = 0.23;
  
    if (Grid->vic->AtmosphTransm <= 0.75 && Grid->vic->AtmosphTransm > 0.35)
       FractionDiffuseRad = 1.33-1.46 * Grid->vic->AtmosphTransm;
  
    if (Grid->vic->AtmosphTransm <= 0.35 && Grid->vic->AtmosphTransm > 0.07) 
       FractionDiffuseRad = 1.-2.3*pow((Grid->vic->AtmosphTransm-0.07), 2.);
  
    if (Grid->vic->AtmosphTransm < 0.07)  
       FractionDiffuseRad = 1.0;
    
    Grid->vic->DiffRadPP = 0.5 * FractionDiffuseRad * Grid->vic->AtmosphTransm * SolarConstant;
}

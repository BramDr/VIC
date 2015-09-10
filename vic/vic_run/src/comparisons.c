/******************************************************************************
 * @section DESCRIPTION
 *
 * floating point comparison utilities
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#include <stdbool.h>
#include <math.h>
#include <vic_def.h>

/******************************************************************************
 * @brief    returns false if two doubles are not equal up to desired precision
 *****************************************************************************/
bool
assert_almost_equal(double x,
                    double y,
                    int    decimal)
{
    if (fabs(x - y) < 0.5 * pow(10, -1 * decimal)) {
        return true;
    }
    return false;
}

/******************************************************************************
 * @brief    returns false if two doubles are not equal up to desired tolerance
 *****************************************************************************/
bool
assert_close(double x,
             double y,
             double rtol,
             double abs_tol)
{
    if (fabs(x - y) <= abs_tol + rtol * fabs(y)) {
        return true;
    }
    return false;
}

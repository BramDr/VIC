#include <vic_driver_image.h>
#include <plugin.h>

void
rout_start(void)
{
    extern plugin_option_struct    plugin_options;
    extern plugin_filenames_struct plugin_filenames;
    extern global_param_struct     global_param;
    extern int              mpi_rank;

    int                     status;
    
    // Check domain & get dimensions
    if (mpi_rank == VIC_MPI_ROOT) {
        status = nc_open(plugin_filenames.routing.nc_filename, NC_NOWRITE,
                         &(plugin_filenames.routing.nc_id));
        check_nc_status(status, "Error opening %s",
                        plugin_filenames.routing.nc_filename);
    }

    plugin_options.UH_LENGTH = get_nc_dimension(&(plugin_filenames.routing),
                                           "uh_time");
    compare_ncdomain_with_global_domain(&plugin_filenames.routing);

    if (mpi_rank == VIC_MPI_ROOT) {
        status = nc_close(plugin_filenames.routing.nc_id);
        check_nc_status(status, "Error closing %s",
                        plugin_filenames.routing.nc_filename);
    }

    if(plugin_options.FORCE_ROUTING){
        // Check the forcing
        if (mpi_rank == VIC_MPI_ROOT) {
            sprintf(plugin_filenames.routing_forcing.nc_filename, "%s%4d.nc", 
                    plugin_filenames.rf_path_pfx, global_param.startyear);
            status = nc_open(plugin_filenames.routing_forcing.nc_filename, NC_NOWRITE, 
                    &(plugin_filenames.routing_forcing.nc_id));
            check_nc_status(status, "Error opening %s",  
                    plugin_filenames.routing_forcing.nc_filename);
        }

        plugin_get_forcing_file_info(&plugin_filenames.routing_forcing);

        if (mpi_rank == VIC_MPI_ROOT) {
            status = nc_close(plugin_filenames.routing_forcing.nc_id);
            check_nc_status(status, "Error closing %s", 
                    plugin_filenames.routing_forcing.nc_filename);
        }
    }
}

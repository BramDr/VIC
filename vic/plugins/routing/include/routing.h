#ifndef ROUTING_H
#define ROUTING_H

#define MAX_UPSTREAM 8

enum {
    ROUTING_BASIN,
    ROUTING_RANDOM
};

typedef struct {
    int *basin_map;
    size_t *sorted_basins;
    size_t Nbasin;
    size_t *Ncells;
    size_t **catchment;
} basin_struct;

typedef struct {
    size_t downstream;
    size_t Nupstream;
    size_t *upstream;
    double *inflow_uh;
    double *runoff_uh;
} rout_con_struct;

typedef struct{
    double *discharge;
} rout_force_struct;

typedef struct {
    double stream;
    double discharge;
    double *dt_discharge;
} rout_var_struct;

void get_basins_routing(basin_struct *basins);
void get_basins_file(basin_struct *basins);

bool rout_get_global_param(char *cmdstr);
void rout_validate_global_param(void);
void rout_start(void);
void rout_alloc(void);
void rout_initialize_local_structures(void);
void rout_init(void);

void rout_set_nc_state_file_info(nc_file_struct *);
void rout_add_state_dim(char *, nc_file_struct *);
void rout_add_state_dim_var(char *, nc_file_struct *);
void rout_add_state_dim_var_data(char *, nc_file_struct *);
void rout_set_nc_state_var_info(nc_file_struct *, size_t);
void rout_set_output_met_data_info(void);
void rout_set_state_meta_data_info(void);
void rout_store(nc_file_struct *);

bool rout_history(int, unsigned int *);
void rout_forcing(void);
void rout_basin_run(size_t cur_cell);
void rout_random_run(void);
void rout_put_data(size_t);
void rout_finalize(void);
void rout_add_types(void);

size_t get_downstream_global(size_t id, int direction);
size_t get_downstream_local(size_t id, int direction, size_t n_nx);
void convolute(double, double *, double *, size_t, size_t);

size_t          *routing_order;
rout_var_struct *rout_var;
rout_con_struct *rout_con;
rout_force_struct *rout_force;

#endif

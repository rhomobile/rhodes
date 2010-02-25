#ifndef RHO_PARAMS_H
#define RHO_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#define RHO_PARAM_UNKNWON 0
#define RHO_PARAM_STRING 1
#define RHO_PARAM_ARRAY 2
#define RHO_PARAM_HASH 3

struct rho_param_t;

typedef struct rho_array_t
{
    int size;
    struct rho_param_t **value;
} rho_array;

typedef struct rho_hash_t
{
    int size;
    char **name;
    struct rho_param_t **value;
} rho_hash;

typedef struct rho_param_t
{
    int type;
    union {
        char *string;
        rho_array *array;
        rho_hash *hash;
    } v;
} rho_param;

rho_param *rho_param_str(char *s);
rho_param *rho_param_array(int size);
rho_param *rho_param_hash(int size);

rho_param *rho_param_dup(rho_param *p);
void rho_param_free(rho_param *p);

#ifdef __cplusplus
}
#endif

#endif /* RHO_PARAMS_H */

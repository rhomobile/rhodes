#include "common/rhoparams.h"

#include "common/RhoPort.h"

#include "logging/RhoLog.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

rho_param *rho_param_str(char *s)
{
    rho_param *ret = (rho_param *)malloc(sizeof(rho_param));
    ret->type = RHO_PARAM_STRING;
    ret->v.string = strdup(s);
    return ret;
}

rho_param *rho_param_array(int size)
{
    int i;
    rho_param *ret = (rho_param *)malloc(sizeof(rho_param));
    ret->type = RHO_PARAM_ARRAY;
    ret->v.array = (rho_array *)malloc(sizeof(rho_array));
    ret->v.array->size = size;
    ret->v.array->value = (rho_param **)malloc(size * sizeof(rho_param *));
    for (i = 0; i < size; ++i)
        ret->v.array->value[i] = NULL;
    return ret;
}

rho_param *rho_param_hash(int size)
{
    int i;
    rho_param *ret = (rho_param *)malloc(sizeof(rho_param));
    ret->type = RHO_PARAM_HASH;
    ret->v.hash = (rho_hash *)malloc(sizeof(rho_hash));
    ret->v.hash->size = size;
    ret->v.hash->name = (char **)malloc(size * sizeof(char *));
    ret->v.hash->value = (rho_param **)malloc(size * sizeof(rho_param *));
    for (i = 0; i < size; ++i) {
        ret->v.hash->name[i] = NULL;
        ret->v.hash->value[i] = NULL;
    }
    return ret;
}

rho_param *rho_param_dup(rho_param *p)
{
    int i, lim;

    if (!p) return NULL;

    rho_param *ret = NULL;
    switch (p->type) {
    case RHO_PARAM_STRING:
        ret = rho_param_str(p->v.string);
        break;
    case RHO_PARAM_ARRAY:
        ret = rho_param_array(p->v.array->size);
        for (i = 0, lim = p->v.array->size; i < lim; ++i)
            ret->v.array->value[i] = rho_param_dup(p->v.array->value[i]);
        break;
    case RHO_PARAM_HASH:
        ret = rho_param_hash(p->v.hash->size);
        for (i = 0, lim = p->v.hash->size; i < lim; ++i) {
            ret->v.hash->name[i] = strdup(p->v.hash->name[i]);
            ret->v.hash->value[i] = rho_param_dup(p->v.hash->value[i]);
        }
        break;
    }
    return ret;
}

void rho_param_free(rho_param *p)
{
    int i, lim;

    if (!p)
        return;

    switch (p->type) {
    case RHO_PARAM_STRING:
        free(p->v.string);
        break;
    case RHO_PARAM_ARRAY:
        for (i = 0, lim = p->v.array->size; i < lim; ++i)
            rho_param_free(p->v.array->value[i]);
        free(p->v.array->value);
        free(p->v.array);
        break;
    case RHO_PARAM_HASH:
        for (i = 0, lim = p->v.hash->size; i < lim; ++i) {
            free(p->v.hash->name[i]);
            rho_param_free(p->v.hash->value[i]);
        }
        free(p->v.hash->name);
        free(p->v.hash->value);
        free(p->v.hash);
        break;
    }
    free(p);
}

#ifdef __cplusplus
}
#endif


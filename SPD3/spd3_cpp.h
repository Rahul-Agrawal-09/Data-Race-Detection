#include "spd3.h"

namespace spd3 {

template <typename T>
inline void launch(T &&lambda) {
    hclib_launch(lambda_wrapper<T>, new T(lambda));
}

extern hclib_worker_state *current_ws();
int current_worker();
int num_workers();
int get_num_places(place_type_t type);
void get_places(place_t **pls, place_type_t type);
place_t *get_current_place();
place_t **get_children_of_place(place_t *pl, int *num_children);
place_t *get_root_place();
char *get_place_name(place_t *pl);
}

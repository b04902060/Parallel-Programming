#define uint32_t unsigned int
#define GRIDSIZE 256
#define LOCAL 16
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

__kernel void vecdot(uint32_t keyA, uint32_t keyB, __global uint32_t* C) {
    __local uint32_t buf[GRIDSIZE];

    int globalId = get_global_id(0);
    int groupId = get_group_id(0);
    int localId = get_local_id(0);

    buf[localId] = 0;
    for(int i = 0; i < LOCAL; i++){
        buf[localId] += encrypt(globalId*LOCAL+i, keyA) * encrypt(globalId*LOCAL+i, keyB);
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for(int i = GRIDSIZE/2; i > 0; i/=2){
        if (localId < i){
            buf[localId] += buf[localId+i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(localId == 0){
        C[groupId] = buf[0];
    }
}

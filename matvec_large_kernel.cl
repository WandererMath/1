__kernel void kernel0(__global float *A, __global float *x, __global float *y)
{
    int b0 = get_group_id(0);
    int t0 = get_local_id(0);
    __local float shared_A[32][32];
    __local float shared_x[32];
    float private_y[1];

    {
      private_y[0] = y[32 * b0 + t0];
      for (int c1 = 0; c1 <= 4095; c1 += 32) {
        for (int c2 = 0; c2 <= 31; c2 += 1)
          shared_A[c2][t0] = A[(32 * b0 + c2) * 4096 + (t0 + c1)];
        shared_x[t0] = x[t0 + c1];
        barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
        for (int c3 = 0; c3 <= 31; c3 += 1)
          private_y[0] += (shared_A[t0][c3] * shared_x[c3]);
        barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
      }
      y[32 * b0 + t0] = private_y[0];
    }
}

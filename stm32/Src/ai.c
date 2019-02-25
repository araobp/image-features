/*
 * ai.c
 *
 *  Created on: 2019/02/25
 */

#include <ai.h>
#include <bsp_ai.h>
#include "ai_platform.h"
#include "math.h"
#include "arm_math.h"

#define AI_BUFFER_NULL(ptr_)  \
  AI_BUFFER_OBJ_INIT( \
    AI_BUFFER_FORMAT_NONE|AI_BUFFER_FMT_FLAG_CONST, \
    0, 0, 0, 0, \
    AI_HANDLE_PTR(ptr_))

static ai_u8 activations[AI_MNETWORK_DATA_ACTIVATIONS_SIZE];
ai_handle handle;
ai_network_report report;

ai_buffer ai_input[1];
ai_buffer ai_output[1];
ai_float normalized_data[AI_MNETWORK_IN_1_SIZE];

/**
 * sklearn.preprocessing.minmax_scale(X, feature_range=(0, 1), axis=0, copy=True)
 *
 * X_std = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0))
 */
void normalize(ai_float *in_data, ai_float *normalized_data, int len) {

  ai_float x_max, x_min, x_max_min;
  uint32_t x_max_idx, x_min_idx;

  arm_max_f32 (in_data, len, &x_max, &x_max_idx);
  arm_min_f32 (in_data, len, &x_min, &x_min_idx);
  x_max_min = x_max - x_min;

  for (int i=0; i<len; i++) {
    normalized_data[i] = (in_data[i] - x_min) / x_max_min;
  }
}

// Initialize the neural network
int ai_init(void) {

  ai_error err;
  const char *nn_name;
  const ai_network_params params = {
  AI_BUFFER_NULL(NULL),
  AI_BUFFER_NULL(activations) };

  // Find a network
  nn_name = ai_mnetwork_find(NULL, 0);
  if (nn_name) {
    printf("\nFound network: \"%s\"\n", nn_name);
  } else {
    printf("E: ai_mnetwork_find\n");
    return -1;
  }

  // Create the network
  err = ai_mnetwork_create(nn_name, &handle, NULL);
  if (err.type) {
    printf("E: ai_mnetwork_create\n");
    return -1;
  }

  // Initialize the network
  if (!ai_mnetwork_init(handle, &params)) {
    printf("E: ai_mnetwork_init\n");
    return -1;
  }

  return 0;
}

// Run inference
void ai_infer(ai_float *input_data, ai_float *output_data) {

  normalize(input_data, normalized_data, AI_MNETWORK_IN_1_SIZE);

  // Input parameters for running inference
  ai_input[0] = report.inputs;
  ai_output[0] = report.outputs;
  ai_input[0].n_batches = 1;
  ai_input[0].data = AI_HANDLE_PTR(normalized_data);
  ai_output[0].n_batches = 1;
  ai_output[0].data = AI_HANDLE_PTR(output_data);

  // Run inference
  ai_mnetwork_run(handle, &ai_input[0], &ai_output[0]);

  // Copy inference result
  for (int i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
    output_data[i] = ((ai_float *) (ai_output[0].data))[i];
  }
}

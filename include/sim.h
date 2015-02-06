#ifndef __SIM_H__
#define __SIM_H__

#include <stdint.h>
#include "es.h"

#define INPUTS_DEFAULT {.T=10000,.lambda=1.2*(1000000.0/12000.0),.L=12000,.alpha=1.2*(1000000.0/12000.0)*5.0,.C=1000000,.K=0,.rho=1.2,.rho_end=0.0,.rho_step=0.0};
#define STATE_DEFAULT {0,0,0,0,0.0,0.0,{0,0.0},es_pq_create(5000000)}
#define OUTPUTS_DEFAULT {0.0,0.0,0.0,0.0}

// Inputs into the simulator
typedef struct {
  // Period of simulation
  uint64_t T;

  // Average # of packets generated/arrived per second
  // lambda for generating arrival times. == rho*(C/L)
  double lambda;

  // Average length of packets in bits
  uint64_t L;

  // Average number of observer events per second
  // lambda*5 (or lambda+10?)
  double alpha;

  // Transmission rate of output link in bits per second
  uint64_t C;

  // The size of the buffer (number of packets the buffer can hold)
  uint64_t K;

  // Utilization of the queue == lambda*(L/C)
  double rho;

  // Optional. Simulate for multiple values of rho to rho_end with step size==rho_step
  double rho_end;
  double rho_step;
} sim_inputs_t;

// Holds the state of the simulator
typedef struct {
  // Number of packet arrivals so far
  uint64_t Na;

  // Number of packet departures so far
  uint64_t Nd;

  // Number of observations so far
  uint64_t No;

  // Number of packets lost
  uint64_t Nl;

  // Keep track of last arrival time to calculate sojourn time
  double arrival_time;

  // Keep track of last departure time to calculate departure time for new departure events
  double departure_time;

  // Current event being processed
  es_event_t event;

  // Discrete event scheduler
  es_pq_t *es;
} sim_state_t;

typedef struct {
  // Average number of packets in the system
  double En;

  // Average sojourn time (queueing delay + service time)
  double Et;

  // Proportion of time the server is idle
  double Pidle;

  // Packet loss probability i.e. total # of packets dropped / total # of packets generated
  double Ploss;
} sim_outputs_t;

// Generate a set of poisson distributed observer events
void sim_gen_observers(sim_state_t *state, sim_inputs_t *inputs);

// Generate a set of poisson distributed arrival events
void sim_gen_arrivals(sim_state_t *state, sim_inputs_t *inputs);

// Generate departure event based on state of the system
void sim_gen_departure(sim_state_t *state, sim_inputs_t *inputs);

// Handle observer event
void sim_event_observer(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs);

// Handle arrival event
void sim_event_arrival(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs);

// Handle departure event
void sim_event_departure(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs);

#endif // __SIM_H__

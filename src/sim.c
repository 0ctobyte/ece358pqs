#include "sim.h"
#include "rv.h"

void sim_gen_observers(sim_state_t *state, sim_inputs_t *inputs) {
  // Generate observer events according to poisson distribution with parameter alpha
  for(double time=rv_exp(inputs->alpha); time<=inputs->T; time+=rv_exp(inputs->alpha)) {
    es_event_t event = {.event_type=OBSERVER,.time=time};
    es_pq_enqueue(state->es, event);
  }
}

void sim_gen_arrivals(sim_state_t *state, sim_inputs_t *inputs) {
  for(double time=rv_exp(inputs->lambda); time<=inputs->T; time+=rv_exp(inputs->lambda)) {
    es_event_t arrival = {.event_type=ARRIVAL,.time=time};
    es_pq_enqueue(state->es, arrival);
  }
}

void sim_gen_departure(sim_state_t *state, sim_inputs_t *inputs) {
  double length = rv_exp(1.0/inputs->L);
  double service_time = length/inputs->C;
  state->departure_time = ((state->arrival_time < state->departure_time) ? state->departure_time+service_time : state->arrival_time+service_time);
  es_event_t event = {.event_type=DEPARTURE,.time=state->departure_time};
  es_pq_enqueue(state->es, event);
}

void sim_event_observer(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs) {
  uint64_t num_packets = (state->Na-state->Nl)-state->Nd;
  
  state->No++;

  // Calculate average # of packets in system
  outputs->En = (num_packets + outputs->En*(state->No-1))/(double)state->No;

  // Calculate average sojourn time (queueing delay+service time)
  outputs->Et = ((state->departure_time-state->arrival_time) + outputs->Et*(state->No-1))/(double)state->No;

  // Calculate time that system is idle
  outputs->Pidle = (((num_packets == 0) ? 1.0 : 0.0) + outputs->Pidle*(state->No-1))/(double)state->No;

  // Calculate packet loss ratio
  outputs->Ploss = (double)state->Nl/(state->Nl+state->Na);
}

void sim_event_arrival(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs) {
  uint64_t num_packets = ((++state->Na)-state->Nl)-state->Nd; 
  if(inputs->K == 0 || num_packets < inputs->K) {
    state->arrival_time = state->event.time;
    sim_gen_departure(state, inputs);
  } else {
    state->Nl++;
  }
}

void sim_event_departure(sim_state_t *state, sim_inputs_t *inputs, sim_outputs_t *outputs) {
  state->Nd++;
}


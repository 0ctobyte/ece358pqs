#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "sim.h"

void parse_cmdline_args(int32_t argc, char **argv, sim_inputs_t *args) {
  char *optstring = "t:c:l:k:r:";
  
  if(argc == 1) {
    fprintf(stderr, "Usage: %s [-t nsecs] [-c bps] [-l plength] [-k qsize] [-r rho[,end,step]]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  for(char opt = getopt(argc, argv, optstring); opt != -1; opt = getopt(argc, argv, optstring)) {
    switch(opt) {
      case 't':
        {
          uint64_t t = strtoull(optarg, NULL, 0);
          if(t != 0) args->T = t;
          break;
        }
      case 'c':
        {
          uint64_t c = strtoull(optarg, NULL, 0);
          if(c != 0) args->C = c;
          break;
        }
      case 'l':
        {
          uint64_t l = strtoull(optarg, NULL, 0);
          if(l != 0) args->L = l;
          break;
        }
      case 'k':
        {
          uint64_t k = strtoull(optarg, NULL, 0);
          if(k != 0) args->K = k;
          break;
        }
      case 'r':
        {
          char *endptr = NULL;
          double rho = strtod(optarg, &endptr);
          if(rho != 0.0) args->rho = rho;
          if(endptr != NULL && endptr != optarg && endptr[0] == ',') {
            double end = strtod(++endptr, &endptr);
            if(end != 0.0) args->rho_end = end;
          }
          break;
        }
      default:
        fprintf(stderr, "Usage: %s [-t nsecs] [-c bps] [-l plength] [-k qsize] [-r rho[,end]]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  
  args->lambda = args->rho*((double)args->C/args->L);
  args->alpha = args->lambda*5.0;
  args->rho_end = ((args->rho_end == 0.0) ? args->rho : args->rho_end);
  args->rho_step = ((args->rho <= 2) ? 0.1 : ((args->rho <= 5) ? 0.2 : 0.4)); 
}


int32_t main(int32_t argc, char **argv) {
    sim_inputs_t inputs = INPUTS_DEFAULT;
    char filename[256];

    parse_cmdline_args(argc, argv, &inputs);

    sprintf(filename, "en_vs_rho_k%llu.dat", inputs.K);
    FILE *f1 = fopen(filename, "w");

    sprintf(filename, "pidle_vs_rho_k%llu.dat", inputs.K);
    FILE *f2 = fopen(filename, "w");

    sprintf(filename, "ploss_vs_rho_k%llu.dat", inputs.K);
    FILE *f3 = fopen(filename, "w");

    for(; inputs.rho <= inputs.rho_end; inputs.rho += inputs.rho_step) {
      sim_state_t state = STATE_DEFAULT;
      sim_outputs_t outputs = OUTPUTS_DEFAULT;

      inputs.lambda = inputs.rho*((double)inputs.C/inputs.L);
      inputs.alpha = inputs.lambda*5.0;
      inputs.rho_end = ((inputs.rho_end == 0.0) ? inputs.rho : inputs.rho_end);
      inputs.rho_step = ((inputs.rho <= 2) ? 0.1 : ((inputs.rho <= 5) ? 0.2 : 0.4)); 

      printf("====================================================================================================\n");

      printf("\nINPUTS\n{\n\tT:\t%-25llu (Simulation period)"
          "\n\tlambda:\t%-25f (Average # of packets/sec)"
          "\n\tL:\t%-25llu (Average packet length [bits])"
          "\n\talpha:\t%-25f (Average # of observers/sec)"
          "\n\tC:\t%-25llu (Transmisson rate of output link [bits/sec])"
          "\n\tK:\t%-25llu (Buffer size [packets])"
          "\n\trho:\t%-25f (Utilization of the queue [lambda*(L/C)])"
          "\n\tend:\t%-25f (Iterate from rho to end)"
          "\n\tstep:\t%-25f (Step size of rho each increment)\n}\n\n", 
          inputs.T,inputs.lambda,inputs.L,inputs.alpha,inputs.C,inputs.K,inputs.rho,inputs.rho_end,inputs.rho_step);

      // Generate observer events
      sim_gen_observers(&state, &inputs);
      
      size_t count = es_pq_size(state.es);
      printf("# of observer events generated:\t%-25lu\n", count);

      // Generate arrival events according to poisson distribution with parameter lambda
      sim_gen_arrivals(&state, &inputs);

      printf("# of arrival events generated:\t%-25lu\n", es_pq_size(state.es)-count);

      for(es_event_t event; es_pq_size(state.es)>0;) {
        event = es_pq_dequeue(state.es);
        state.event = event;
        switch(event.event_type) {
          case ARRIVAL:
            sim_event_arrival(&state, &inputs, &outputs);
            break;
          case DEPARTURE:
            sim_event_departure(&state, &inputs, &outputs);
            break;
          case OBSERVER:
            sim_event_observer(&state, &inputs, &outputs);
            break;
          default:
            break;
        }
      }

      printf("\nSTATE\n{\n\tNa:\t%-25llu (# of packet arrivals including dropped packets)"
          "\n\tNd:\t%-25llu (# of packet departures)"
          "\n\tNo:\t%-25llu (# of observer events)"
          "\n\tNl:\t%-25llu (# of dropped packets)\n}\n\n", state.Na, state.Nd, state.No, state.Nl);

      printf("\nOUTPUTS\n{\n\tEn:\t%-25f (Average # of packets in the buffer)"
          "\n\tEt:\t%-25f (Average sojourn time [queueing time+service time])"
          "\n\tPidle:\t%-25f (Proportion of time server is idle)"
          "\n\tPloss:\t%-25f (Ratio of dropped packets to total number of packets generated)\n}\n\n", outputs.En, outputs.Et, outputs.Pidle, outputs.Ploss);

      printf("====================================================================================================\n");
   
      fprintf(f1, "%f,%f\n", inputs.rho, outputs.En);
      fprintf(f2, "%f,%f\n", inputs.rho, outputs.Pidle);
      fprintf(f3, "%f,%f\n", inputs.rho, outputs.Ploss);

      es_pq_delete(state.es);
    }


		return 0;
}


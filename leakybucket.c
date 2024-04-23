
#include <stdio.h>
#include <stdbool.h>
#define BUCKET_SIZE 10
#define OUTPUT_RATE 1

void leaky_bucket(int arrival_times[], int num_packets)
{
    int bucket_level = 0;
    int time = 1;

    for (int i = 0; i < num_packets; i++)
    {
        while (time < arrival_times[i])
        {
            if (bucket_level > 0)
            {
                printf("Time: %d seconds - Packet conforming (Queue size: %d bytes)\n", time, bucket_level);
                bucket_level -= OUTPUT_RATE;
            }
            else
            {
                printf("Time: %d seconds - Packet non-conforming (Queue empty)\n", time);
            }
            time++;
        }

        while (bucket_level > 0)
        {

            printf("Time: %d seconds - Packet conforming (Queue size: %d bytes)\n", time, bucket_level);
            bucket_level -= OUTPUT_RATE;
            time++;
        }
    }
}

int main(){
  //Define the arrival times of packet
  int arrival_times[] = {3,4,7,10,12,17,24,25,26,27};
  int num_packets = sizeof(arrival_times)/sizeof(arrival_times[0]);
  
  //Run the leaky bucket algorithm
  leaky_bucket(arrival_times,num_packets);
  
  return 0;
}

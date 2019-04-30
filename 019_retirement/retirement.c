#include <stdio.h>
#include <stdlib.h>

struct retire_info_tag {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct retire_info_tag retire_info_t;

double bal_cal(int startAge, double initial, retire_info_t state) {
  double curr_bal = initial;
  for (int i = 0; i < state.months; i++) {
    printf(
        "Age %3d month %2d you have $%.2lf\n", (startAge + i) / 12, (startAge + i) % 12, curr_bal);
    curr_bal += curr_bal * state.rate_of_return + state.contribution;
  }
  return curr_bal;
}

void retirement(int startAge,           //in months
                double initial,         //initial savings in dollars
                retire_info_t working,  //info about working
                retire_info_t retired)  //info about retired
{
  initial = bal_cal(startAge, initial, working);
  startAge += working.months;
  initial = bal_cal(startAge, initial, retired);
}

int main() {
  int startAge = 327;
  double initial = 21345;
  retire_info_t working, retired;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045 / 12;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01 / 12;
  retirement(startAge, initial, working, retired);
  return 0;
}

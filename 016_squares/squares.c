int max(int x, int y) {
  if (x > y) {
    return x;
  }
  else {
    return y;
  }
}

int is_between(int a, int lower_bound, int higher_bound) {
  if ((a < higher_bound) & (a >= lower_bound)) {
    return 1;
  }
  else {
    return 0;
  }
}

int either_equal(int b, int choice1, int choice2) {
  if ((b == choice1) || (b == choice2)) {
    return 1;
  }
  else {
    return 0;
  }
}

void squares(int size1, int x_offset, int y_offset, int size2) {
  int w =
      max(size1, x_offset + size2);  //compute the max of size1 and (x_offset + size2).  Call this w
  int h =
      max(size1, y_offset + size2);  //compute the max of size1 and (y_offset + size2).  Call this h

  for (int y = 0; y < h; y++) {    //count from 0 to h. Call the number you count with y
    for (int x = 0; x < w; x++) {  //count from 0 to w. Call the number you count with x
      //check if  EITHER
      int cond1 = is_between(x, x_offset, x_offset + size2);
      int cond2 = either_equal(y, y_offset, y_offset + size2 - 1);
      int cond3 = is_between(y, y_offset, y_offset + size2);
      int cond4 = either_equal(x, x_offset, x_offset + size2 - 1);
      if ((cond1 & cond2) || (cond3 & cond4)) {
        // ((x is between x_offset  and x_offset + size2) AND // y is equal to either y_offset OR y_offset + size2 - 1 )
        //  OR
        // ((y is between y_offset and y_offset + size2) AND // x is equal to either x_offset OR x_offset + size2 -1)
        printf("*");
      }  // if so, print a *
      else {
        int cond5 = (x < size1);
        int cond6 = either_equal(y, 0, size1 - 1);
        int cond7 = (y < size1);
        int cond8 = either_equal(x, 0, size1 - 1);
        if ((cond5 & cond6) || (cond7 & cond8)) {
          // if not,  check if EITHER
          // x is less than size1 AND (y is either 0 or size1-1)
          // OR
          // y is less than size1 AND (x is either 0 or size1-1)
          printf("#");  //if so, print a #
        }
        else {
          printf(" ");  //else print a space
        }
      }
    }
    printf("\n");  //when you finish counting x from 0 to w, //print a newline
  }
}

#include "data_structures/counter.h"


int Counter::count = 0;

int Counter::increase() {return count++;}

int Counter::get() {return count;}

void Counter::resetCount(int val) {count = val;}
#ifndef DATA_STRUCTURES_COUNTER_H_
#define DATA_STRUCTURES_COUNTER_H_


class Counter {
private:
    static int count;
public:
    static int increase();
    static int get();
    static void resetCount(int val=0);
};

#endif //DATA_STRUCTURES_COUNTER_H_
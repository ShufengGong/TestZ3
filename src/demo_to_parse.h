//
// Created by gongshufeng on 2021/1/13.
//

#ifndef Z3_C_EXAMPLE_DEMO_TO_PARSE_H
#define Z3_C_EXAMPLE_DEMO_TO_PARSE_H

class SSSPIngress {

private:
    bool atomic_min(double &a, double b){
        a = a < b ? a : b;
        return a <= b;
    }

public:

    bool aggregate(double &a, double b) {
        return atomic_min(a, b);
    }

    bool update(double &a, double b) {
        return atomic_min(a, b);
    }

    double generate(double v, double m, double w) {
        return m + w;
    }
};

#endif //Z3_C_EXAMPLE_DEMO_TO_PARSE_H

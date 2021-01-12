//
// Created by gongshufeng on 2021/1/11.
//

#include "z3++.h"

using namespace z3;

/**
 *  (assert (not (forall ((x1 Real) (x2 Real))
(or (= (f x1 x2) x1) (= (f x1 x2) x2))))).
 */

float max(float x, float y){
    return x > y ? x : y;
}

expr max_z3(expr x, expr y){
    return x > y ? x : y;
}

void single_dep(){
    context c;
    expr x = c.real_const("x");
    expr y = c.real_const("y");

    solver s(c);
    expr conjecture=(forall(x, y, max_z3(x, y) == x || max_z3(x, y) == y));
    s.add(!conjecture);
    std::cout << s.check() << std::endl;
    model m = s.get_model();
    std::cout << m << std::endl;
}


int main() {

    try {
        single_dep();

        std::cout << "----------------------------\n";

        std::cout << "done\n";
    }
    catch (z3::exception & ex) {
        std::cout << "unexpected error: " << ex << "\n";
    }
    Z3_finalize_memory();
    return 0;
}



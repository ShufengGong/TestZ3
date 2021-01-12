//
// Created by gongshufeng on 2021/1/11.
//

#include "z3++.h"

using namespace z3;

typedef expr (*pfunc) (expr, expr);

float max(float x, float y){
    return x > y ? x : y;
}

expr max_z3(expr x, expr y){
    return x > y ? x : y;
}

pfunc cpp_z3(expr expr1, expr expr2) {
    return &max_z3;
}



void single_dep(){
    context c;
    expr x = c.real_const("x");
    expr y = c.real_const("y");

    solver s(c);
    expr conjecture=(forall(x, y, cpp_z3(x, y) == x || cpp_z3(x, y) == y));
    s.add(!conjecture);
    std::cout << s.check() << std::endl;
    model m = s.get_model();
    std::cout << m << std::endl;
}





int main() {

//    z3_to_cpp(fun_cpp, x, y);

    try {
//        prove_example1();

        std::cout << "----------------------------\n";

        std::cout << "done\n";
    }
    catch (z3::exception & ex) {
        std::cout << "unexpected error: " << ex << "\n";
    }
    Z3_finalize_memory();
    return 0;
}


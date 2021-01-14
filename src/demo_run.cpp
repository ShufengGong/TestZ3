//
// Created by gongshufeng on 2021/1/13.
//

#include "z3++.h"
#include "demo_to_parse.h"
#include "demo_parsed.h"

using namespace z3;

void mem_free_engine(){
    std::cout << "perform memoization free" << std::endl;
}

void mem_path_engine(){
    std::cout << "perform memoization path" << std::endl;
}

void mem_vertex_engine(){
    std::cout << "perform memoization vertex" << std::endl;
}

void mem_edge_engine(){
    std::cout << "perform memoization edge" << std::endl;
}

expr aggregate(expr x, expr y){
    return ite(x < y, x, y);
}

int main(int argc, char* argv[]){


    context c;

    solver s(c);

    expr x1 = c.real_const("x1");
    expr x2 = c.real_const("x2");
    expr x3 = c.real_const("x3");
    z3::sort I = c.real_sort();
    func_decl f1 = z3::function("f1", I, I);

    expr conjecture = forall(x1, x2, x3, aggregate(x1, x3) == aggregate(x1, aggregate(aggregate(x2, f1(x2)), x3)));
    s.add(conjecture);
    if(s.check() == sat){
        mem_free_engine();
        model m = s.get_model();
        std::cout << s.check() << std::endl;
    }

    s.reset();
    expr x = c.real_const("x");
    expr y = c.real_const("y");

    expr conjecture2 = (forall(x, y, aggregate(x, y) == x || aggregate(x, y) == y));
    s.add(!conjecture2);
    if(s.check()==unsat){
        mem_path_engine();
    }

}


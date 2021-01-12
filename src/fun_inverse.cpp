//
// Created by gongshufeng on 2021/1/11.
//

#include"z3++.h"
using namespace z3;

//convert the c++ function to z3 function


//find the inverse function
/**
 逆函数这么定义：G(\Delta, G-(\Delta_1)) = G(\Delta / \Delta_1)
z3代码：
(define-fun g ( (a Real) (b Real) ) Real ( + a b ) )
(declare-fun g1 (Real) Real )
(assert (forall ( (x1 Real) (x2 Real) (x3 Real))
(=  (g x1 (g (g (g1 x2) x2) x3))  (g x1 x3) )))
(check-sat)
(get-model)
结果：
sat
(
  (define-fun g1 ((x!0 Real)) Real
    (* (- 1.0) x!0))
  (define-fun g ((x!0 Real) (x!1 Real)) Real
    (+ x!1 x!0))
)
*/

expr fun_g(expr x, expr y){
    return x + y;
}

void fun_inverse(){
    context c;
    expr x1 = c.real_const("x1");
    expr x2 = c.real_const("x2");
    expr x3 = c.real_const("x3");
    z3::sort I = c.real_sort();
    func_decl f1 = z3::function("f1", I, I);

    solver s(c);
    s.add(forall(x1, x2, x3, fun_g(x1, x3) == fun_g(x1, fun_g(fun_g(x2, f1(x2)), x3))));
    s.check();
    model m = s.get_model();
    std::cout << m.size() << std::endl;
    std::cout << m << std::endl;
}

int main() {

    try {
        fun_inverse();
        std::cout << "----------------------------\n";

        std::cout << "done\n";
    }
    catch (z3::exception & ex) {
        std::cout << "unexpected error: " << ex << "\n";
    }
    Z3_finalize_memory();
    return 0;
}


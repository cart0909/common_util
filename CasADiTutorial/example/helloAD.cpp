#include <casadi/casadi.hpp>
using namespace casadi;

int main()
{
    // Create scalar/matrix symbols
    MX x = MX::sym("x", 5, 1);

    // Compose into expressions
    MX y = norm_2(x);

    // Sensitivity of expression -> new expression
    MX grad_y = gradient(y, x);

    // Create a Function to evaluate expression
    Function f = Function("f", { x }, { grad_y });

    // Evaluate numerically
    std::vector<DM> grad_y_num = f(DM({ 1, 2, 3, 4, 5 }));

    return 0;
}

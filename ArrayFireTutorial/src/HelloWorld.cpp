#include "HelloWorld.h"

void HelloWorld::Foo()
{
    LOG(INFO) << "Create a 5-by-3 matrix of random floats on the GPU" << std::endl;
    af::array A = af::randu(5, 3, f32);
    af_print(A);

    LOG(INFO) << "Element-wise arithmetic" << std::endl;
    af::array B = af::sin(A) + 1.5;
    af_print(B);

    LOG(INFO) << "Negate the first three elements of second column" << std::endl;
    B(af::seq(0, 2), 1) = B(af::seq(0, 2), 1) * -1;
    af_print(B);

    LOG(INFO) << "Fourier transform the result" << std::endl;
    af::array C = af::fft(B);
    af_print(C);

    LOG(INFO) << "Grab last row" << std::endl;
    af::array c = C.row(af::end);
    af_print(c);

    LOG(INFO) << "Scan Test" << std::endl;
    af::dim4 dims(16, 4, 1, 1);
    af::array r = constant(2, dims);
    af_print(r);

    LOG(INFO) << "Scan" << std::endl;
    af::array S = af::scan(r, 0, AF_BINARY_MUL);
    af_print(S);

    LOG(INFO) << "Create 2-by-3 matrix from host data" << std::endl;
    float d[] = { 1, 2, 3, 4, 5, 6 };
    af::array D(2, 3, d, afHost);
    af_print(D);

    LOG(INFO) << "Copy last column onto first" << std::endl;
    D.col(0) = D.col(af::end);
    af_print(D);

    // Sort A
    LOG(INFO) << "Sort A and print sorted array and corresponding indices" << std::endl;
    af::array vals, inds;
    af::sort(vals, inds, A);
    af_print(vals);
    af_print(inds);
}

#include <Eigen/Dense>
#include <chrono>
#include <emmintrin.h>
#include <iostream>
#define MATRIX_ELEMENT(A, cols, i, j) A[i * cols + j]
using namespace Eigen;
using namespace std;

void simd_vec_add(float* a, float* b, float* c, int size);
void simd_mat_mul(float* A, float* B, float* C, int M, int L, int N);
void mat_mul(float* A, float* B, float* C, int M, int L, int N);

class Test {
public:
    void test1();
    void test2();
};

int main()
{
    Test tester;
    //    tester.test1();
    //    cout << endl;
    tester.test2();
    return 0;
}

void simd_vec_add(float* a, float* b, float* c, int size)
{
    int num_group = size * 0.25;

    for (int i = 0; i < num_group; ++i) {
        __m128 veca = _mm_load_ps(a + 4 * i);
        __m128 vecb = _mm_load_ps(b + 4 * i);
        __m128 vecc = _mm_add_ps(veca, vecb);
        _mm_store_ps(c + 4 * i, vecc);
    }

    for (int i = 4 * num_group; i < size; ++i)
        c[i] = a[i] + b[i];
}

void simd_mat_mul(float* A, float* B, float* C, int M, int L, int N)
{
    int rows4 = M * 0.25, cols4 = N * 0.25;

    for (int row_index = 0; row_index < rows4; ++row_index) {
        for (int col_index = 0; col_index < cols4; ++col_index) {
            __m128 vc0 = _mm_set1_ps(0.0f);
            __m128 vc1 = _mm_set1_ps(0.0f);
            __m128 vc2 = _mm_set1_ps(0.0f);
            __m128 vc3 = _mm_set1_ps(0.0f);

            for (int i = 0; i < L; ++i) {
                __m128 vb = _mm_load_ps(&MATRIX_ELEMENT(B, N, i, 4 * col_index));

                __m128 prod = _mm_mul_ps(vb, _mm_set1_ps(MATRIX_ELEMENT(A, L, 4 * row_index, i)));
                vc0 = _mm_add_ps(prod, vc0);

                prod = _mm_mul_ps(vb, _mm_set1_ps(MATRIX_ELEMENT(A, L, 4 * row_index + 1, i)));
                vc1 = _mm_add_ps(prod, vc1);

                prod = _mm_mul_ps(vb, _mm_set1_ps(MATRIX_ELEMENT(A, L, 4 * row_index + 2, i)));
                vc2 = _mm_add_ps(prod, vc2);

                prod = _mm_mul_ps(vb, _mm_set1_ps(MATRIX_ELEMENT(A, L, 4 * row_index + 3, i)));
                vc3 = _mm_add_ps(prod, vc3);
            }

            _mm_store_ps(&MATRIX_ELEMENT(C, N, 4 * row_index, 4 * col_index), vc0);
            _mm_store_ps(&MATRIX_ELEMENT(C, N, 4 * row_index + 1, 4 * col_index), vc1);
            _mm_store_ps(&MATRIX_ELEMENT(C, N, 4 * row_index + 2, 4 * col_index), vc2);
            _mm_store_ps(&MATRIX_ELEMENT(C, N, 4 * row_index + 3, 4 * col_index), vc3);
        }
    }
}

void mat_mul(float* A, float* B, float* C, int M, int L, int N)
{
    for (int row_index = 0; row_index < M; ++row_index) {
        for (int col_index = 0; col_index < N; ++col_index) {
            float element = 0.0f;
            for (int K = 0; K < L; ++K) {
                element += MATRIX_ELEMENT(A, L, row_index, K) * MATRIX_ELEMENT(B, N, K, col_index);
            }
            MATRIX_ELEMENT(C, N, row_index, col_index) = element;
        }
    }
}

void Test::test1()
{
    const int size = 50;
    const int times = 10000000;
    VectorXf a = VectorXf::Random(size), b = VectorXf::Random(size), c;

    cout << "[INFO] -----Eigen add-----" << endl;
    auto t0 = chrono::steady_clock::now();
    for (int i = 0; i < times; ++i)
        c = a + b;
    auto t1 = chrono::steady_clock::now();

    auto delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

    cout << "[INFO] time cost " << delt << "(us)" << endl;
    cout << "[INFO] c = \n";
    for (int i = 0; i < size; ++i)
        cout << c[i] << " ";
    cout << endl;

    cout << "[INFO] -----My add-----" << endl;
    float *d = a.data(), *e = b.data(), f[size];
    t0 = chrono::steady_clock::now();

    for (int i = 0; i < times; ++i)
        for (int j = 0; j < size; ++j)
            f[j] = d[j] + e[j];

    t1 = chrono::steady_clock::now();

    delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    cout << "[INFO] time cost " << delt << "(us)" << endl;
    cout << "[INFO] c = \n";
    for (int i = 0; i < size; ++i)
        cout << f[i] << " ";
    cout << endl;

    cout << "[INFO] -----My simd add-----" << endl;
    t0 = chrono::steady_clock::now();

    for (int i = 0; i < times; ++i)
        simd_vec_add(d, e, f, size);

    t1 = chrono::steady_clock::now();

    delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    cout << "[INFO] time cost " << delt << "(us)" << endl;
    cout << "[INFO] c = \n";
    for (int i = 0; i < size; ++i)
        cout << f[i] << " ";
    cout << endl;
}

void Test::test2()
{
    const int M = 50, L = 50, N = 50;
    const int times = 5000;

    Matrix<float, M, L, RowMajor> A = Matrix<float, M, L, RowMajor>::Random();
    Matrix<float, L, N, RowMajor> B = Matrix<float, L, N, RowMajor>::Random();
    Matrix<float, M, N, RowMajor> C;

    cout << "[INFO] -----Eigen multiple-----" << endl;
    auto t0 = chrono::steady_clock::now();
    for (int i = 0; i < times; ++i)
        C = A * B;
    auto t1 = chrono::steady_clock::now();
    auto delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

    cout << "[INFO] time cost " << delt << "(ms)" << endl;
    cout << "[INFO] C first row: \n";
    for (int i = 0; i < N; ++i)
        cout << C(0, i) << " ";
    cout << endl;

    cout << "[INFO] -----My multiple-----" << endl;
    t0 = chrono::steady_clock::now();
    float *APtr = A.data(), *BPtr = B.data(), CPtr[M * N];
    for (int i = 0; i < times; ++i) {
        mat_mul(APtr, BPtr, CPtr, M, L, N);
    }
    t1 = chrono::steady_clock::now();
    delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

    cout << "[INFO] time cost " << delt << "(ms)" << endl;
    cout << "[INFO] C first row: \n";
    for (int i = 0; i < N; ++i)
        cout << CPtr[i] << " ";
    cout << endl;

    cout << "[INFO] -----My simd multiple-----" << endl;
    t0 = chrono::steady_clock::now();
    for (int i = 0; i < times; ++i) {
        simd_mat_mul(APtr, BPtr, CPtr, M, L, N);
    }
    t1 = chrono::steady_clock::now();
    delt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

    cout << "[INFO] time cost " << delt << "(ms)" << endl;
    cout << "[INFO] C first row: \n";
    for (int i = 0; i < N; ++i)
        cout << CPtr[i] << " ";
    cout << endl;
}

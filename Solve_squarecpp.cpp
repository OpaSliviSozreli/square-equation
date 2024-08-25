#include<stdio.h>
#include<math.h>
#include "TXLib.h"

struct square_equation_coefficients
{
    double a;
    double b;
    double c;
};
struct expected_roots
{
    double expected_x1;
    double expected_x2;
    int number_root_expected;
};

struct test_data
{
    struct square_equation_coefficients coefficients;
    struct expected_roots cur_expected_roots;
};



const double EPSILON = 0.00001;
enum number_of_roots{ ZERO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2, SS_INF_ROOTS = -1};
const int NUMBER_OF_TESTS = 5;


void get_coefficients( double *a);
int compare_with_zero( double a);
int print_roots( int nRoots, double x1, double x2 );
int solve_square( double a, double b, double c, double *x1, double *x2 );
int run_test( test_data data );
int compare_diff( double x1, double x2 );
int test_solver( test_data parameters[] );

int main( void )
{
    double a = 0, b = 0, c = 0;
    get_coefficients( &a );
    get_coefficients( &b );
    get_coefficients( &c );

    double x1 = 0, x2 = 0;
    int nRoots = solve_square( a, b, c, &x1, &x2 );

    print_roots( nRoots, x1, x2 );

    test_data parameters[5] =
    {
        {{ 1,   2,   -3}, {  -3,     1,  2}},
        {{ 1,   2,   -3}, {   1,    -3,  2}},
        {{ 0,   2,   -5}, { 2.5,   2.5,  1}},
        {{ 0,   0,    0}, {   4,    78, -1}},
        {{25,  40,   16}, {-0.8,  -0.8,  1}}
    };
    test_solver( parameters );
/*    test_data test1 = {{ 1,   2,   -3}, {  -3,     1,  2}};
    test_data test2 = {{ 1,   2,   -3}, {   1,    -3,  2}};
    test_data test3 = {{ 0,   2,   -5}, { 2.5,   2.5,  1}};
    test_data test4 = {{ 0,   0,    0}, {   4,    78, -1}};
    test_data test5 = {{25,  40,   16}, {-0.8,  -0.8,  1}};
    run_test(test1);
    run_test(test2);
    run_test(test3);
    run_test(test4);
    run_test(test5);
*/
}

int solve_square ( double a, double b, double c, double *x1, double *x2 )
{
    if ( compare_with_zero(a) )
    {
        if ( compare_with_zero(b) )
        {
            return compare_with_zero(c) ? SS_INF_ROOTS : 0;
        }
        else
        {
            *x1 = *x2 = -c/b;
            return 1;
        }
    }
    else

    {
        double d = b*b - 4*a*c;
        if (compare_with_zero(d))
        {
            *x1 = *x2 = -b/(2*a);
            return 1;
        }
        else if (compare_with_zero( d - EPSILON) == 1)
        {
            double sqrt_d = sqrt(d);
            *x1 = (-b - sqrt_d) / (2 * a);
            *x2 = (-b + sqrt_d) / (2 * a);
            return 2;
        }
        else if (compare_with_zero( d - EPSILON) == 0)
        {
            return 0;
        }
    }
}

void get_coefficients( double *a)
{
    printf( "#Enter a,b,c: \n" );
    while ( scanf("%lg", a)!= 1 )
    {
        printf( "#Enter a again" );
        while (getchar() != '\n')
            continue;
    }
}

int compare_with_zero( double a)
{
    if ( fabs(a) < EPSILON )
        return 1;
    else
        return 0;
}

int print_roots( int nRoots, double x1, double x2 )
{   switch( nRoots )
    {
        case ZERO_ROOTS:
            printf( "No roots\n" );
            break;
        case ONE_ROOT:
            printf("x = %lg\n", x1 );
            break;
        case TWO_ROOTS:
            printf( "x1 = %lg, x2 = %lg\n", x1, x2 );
            break;
        case SS_INF_ROOTS:
            printf( "Any roots\n" );
            break;
        default:
            txSetColor(TX_RED);
            printf( "main(): ERROR: nRoots = %d\n", nRoots );
            return 1;
    }
}

int run_test( test_data data )
{
    double x1 = 0, x2 = 0;
    int nRoots = solve_square( data.coefficients.a, data.coefficients.b, data.coefficients.c, &x1, &x2 );

    int number_root_expected = data.cur_expected_roots.number_root_expected;
    if ( nRoots != number_root_expected )
    {
        txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
        printf( "#Error Test, a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, nRoots = %d\n", data.coefficients.a, data.coefficients.b, data.coefficients.c, x1, x2, nRoots );
    }

    switch(nRoots)
    {
        case ZERO_ROOTS:
            return 0;
            break;
        case ONE_ROOT:
            if ( compare_with_zero( x1 - data.cur_expected_roots.expected_x1 ) )
            {
                txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
                printf( "#OK\n" );
                break;
            }
            else
            {
                txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
                printf( "#Error Test, a = %lg, b = %lg, c = %lg, x = %lg, nRoots = 1\n",
                        data.coefficients.a, data.coefficients.b, data.coefficients.c, x1 );
                printf( "#Expected x = %lg", data.cur_expected_roots.expected_x1 );
                break;
            }
        case TWO_ROOTS:
            if ((((compare_with_zero( x1 - data.cur_expected_roots.expected_x1 )) && (compare_with_zero( x2 - data.cur_expected_roots.expected_x2)))) || (( compare_with_zero( (x2 - data.cur_expected_roots.expected_x1)) && (compare_with_zero( x1 - data.cur_expected_roots.expected_x2 )))))
            {
                txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
                printf("#OK\n");
            }
            else
            {
                txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
                printf("#Error Test, a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, nRoots = 2\n",
                       data.coefficients.a, data.coefficients.b, data.coefficients.c, x1, x2 );
                printf( "#Expected x1 = %lg, x2 = %lg",
                        data.cur_expected_roots.expected_x1, data.cur_expected_roots.expected_x2 );
                break;
            }
        case SS_INF_ROOTS:
            txSetConsoleAttr (FOREGROUND_YELLOW | BACKGROUND_GREEN);
            printf( "#OK\n" );
            break;
    }
}

int test_solver( test_data parameters[] )
{

    for (int i = 0; i <= NUMBER_OF_TESTS; i++)
    {
        run_test(parameters[i]);
    }
    //int count_correct_number_of_roots
}




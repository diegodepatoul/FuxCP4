#ifndef FUX_TESTS_HPP
#define FUX_TEST_HPP

#include "Parts/Part.hpp"

using namespace Gecode;
using namespace std;

class FuxTest{

protected:
    vector<int> species;
    vector<int> cantusFirmus;
    vector<int> v_type;
    int borrowMode;
    vector<int> cp;
    int idx;

public:

    FuxTest(int testNumber);

    FuxTest(int testNumber, int i);

    vector<int> getSpList();
    vector<int> getCf();
    vector<int> getVType();
    int getBMode();
    vector<int> getCp();
    int getIdx();

    void test_2v_1sp_fig22_setter(int i);
    void test_2v_1sp_fig23_setter(int i);

    void test_2v_2sp_fig38_setter(int i);
    void test_2v_2sp_fig39_setter(int i);
    void test_2v_2sp_fig40_setter(int i);
    void test_2v_2sp_fig41_setter(int i);
    void test_2v_2sp_fig42_setter(int i);

    void test_2v_3sp_fig55_setter(int i);

    void test_3v_1sp_fig108_setter(int i);
    void test_3v_1sp_fig109_setter(int i);
    void test_3v_1sp_fig110_setter(int i);
    void test_3v_1sp_fig111_setter(int i);

    void test_3v_2sp_fig125_setter(int i);

    void test_3v_3sp_fig133_setter(int i);

    void test_4v_1sp_fig166_setter(int i);
    void test_4v_1sp_fig167_setter(int i);

    void test_4v_2sp_fig176_setter(int i);
};

#endif
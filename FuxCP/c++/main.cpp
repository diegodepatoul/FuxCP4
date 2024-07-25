#include "headers/Utilities.hpp"
#include "headers/CounterpointUtils.hpp"
#include "headers/Parts/Part.hpp"
#include "headers/Parts/FirstSpeciesCounterpoint.hpp"
#include "headers/Parts/SecondSpeciesCounterpoint.hpp"
#include "headers/Parts/CantusFirmus.hpp"
#include "headers/CounterpointProblems/TwoVoiceCounterpoint.hpp"
#include "headers/CounterpointProblems/CounterpointProblem.hpp"
#include "headers/fuxTest.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    if(argc==1){
        cout << argv[0] << endl;
        cout << "-------------" << endl;
        int upper_bound_domain = 72;
        int lower_bound_domain = 60;
        vector<int> species = {FIRST_SPECIES};
        //la do si re do mi fa mi re do si la
        //57 60 59 62 60 64 65 64 62 60 59 57
        vector<int> cantusFirmus = {57,60,59,62,60,64,65,64,62,60,59,57}; //1sp 2v cf
        
        int size = cantusFirmus.size();
        vector<int> v_type = {1, 2};

        vector<int> melodic_params = {0, 1, 1, 576, 2, 2, 2, 1};
        //borrow, h-5th, h-octave, succ, variety, triad, direct move, penult rule check
        vector<int> general_params = {4, 1, 1, 2, 2, 2, 8, 1};

        //penult sixth, non-ciambata, con m after skip, h triad 3rd species, m2 eq zero, no syncopation, pref species slider
        vector<int> specific_params = {8 , 4 , 0 , 2 , 1 , 8 , 50};


        vector<int> importance = {8,7,5,2,9,3,14,12,6,11,4,10,1,13};

        int borrowMode = 1;

        // create a new problem
        // auto* problem = new TwoVoiceCounterpoint(cantusFirmus, species[0], C, lower_bound_domain, upper_bound_domain);
        auto* problem = create_problem(cantusFirmus, species, C, lower_bound_domain, upper_bound_domain, v_type, melodic_params, general_params, specific_params,
            importance, borrowMode);
        //cout << problem->to_string() << endl;
        // create a new search engine
        
        BAB<CounterpointProblem> e(problem);

        cout << e.best << endl;

        int nb_sol = 0;
        while(CounterpointProblem* pb = e.next()){
            nb_sol++;
            cout << "Solution " << nb_sol << ": " << endl;
            cout << pb->to_string() << endl;
            // cout << int_vector_to_string(cantusFirmus) << endl;

            delete pb;
            //if (nb_sol >= 1)
            //    break;
        }
        cout << "No (more) solutions." << endl;
    } else if(argc==3){
        FuxTest* test = new FuxTest(atoi(argv[1]), atoi(argv[2]));
        int upper_bound_domain = 72;
        int lower_bound_domain = 60;
        vector<int> species = test->getSpList();
        //la do si re do mi fa mi re do si la
        //57 60 59 62 60 64 65 64 62 60 59 57
        vector<int> cantusFirmus = test->getCf();
        
        int size = cantusFirmus.size();
        vector<int> v_type = test->getVType();

        vector<int> melodic_params = {0, 1, 1, 576, 2, 2, 2, 1};
        //borrow, h-5th, h-octave, succ, variety, triad, direct move, penult rule check
        vector<int> general_params = {4, 1, 1, 2, 2, 2, 8, 1};

        //penult sixth, non-ciambata, con m after skip, h triad 3rd species, m2 eq zero, no syncopation, pref species slider
        vector<int> specific_params = {8 , 4 , 0 , 2 , 1 , 8 , 50};

        vector<int> importance = {8,7,5,2,9,3,14,12,6,11,4,10,1,13};

        int borrowMode = test->getBMode();

        // create a new problem
        // auto* problem = new TwoVoiceCounterpoint(cantusFirmus, species[0], C, lower_bound_domain, upper_bound_domain);
        auto* problem = create_problem(cantusFirmus, species, C, lower_bound_domain, upper_bound_domain, v_type, melodic_params, general_params, specific_params,
            importance, borrowMode);
        //cout << problem->to_string() << endl;
        // create a new search engine

        for(int j = 0; j < test->getIdx(); j++){
            rel(problem->getHome(), problem->getCounterpoint()->getBranchingNotes()[j], IRT_EQ, test->getCp()[j]);
        }

        BAB<CounterpointProblem> e(problem);

        int nb_sol = 0;
        while(CounterpointProblem* pb = e.next()){
            nb_sol++;
            cout << "Solution " << nb_sol << ": " << endl;
            cout << pb->to_string() << endl;
            // cout << int_vector_to_string(cantusFirmus) << endl;

            delete pb;
            if (nb_sol >= 1)
                break;
        }
        cout << "No (more) solutions." << endl;
    } else if(argc==2){
        FuxTest* test = new FuxTest(atoi(argv[1]));
        int upper_bound_domain = 72;
        int lower_bound_domain = 60;
        vector<int> species = test->getSpList();
        //la do si re do mi fa mi re do si la
        //57 60 59 62 60 64 65 64 62 60 59 57
        vector<int> cantusFirmus = test->getCf();
        
        int size = cantusFirmus.size();
        vector<int> v_type = test->getVType();

        vector<int> melodic_params = {0, 1, 1, 576, 2, 2, 2, 1};
        //borrow, h-5th, h-octave, succ, variety, triad, direct move, penult rule check
        vector<int> general_params = {4, 1, 1, 2, 2, 2, 8, 1};

        //penult sixth, non-ciambata, con m after skip, h triad 3rd species, m2 eq zero, no syncopation, pref species slider
        vector<int> specific_params = {8 , 4 , 0 , 2 , 1 , 8 , 50};

        vector<int> importance = {8,7,5,2,9,3,14,12,6,11,4,10,1,13};

        int borrowMode = test->getBMode();

        // create a new problem
        // auto* problem = new TwoVoiceCounterpoint(cantusFirmus, species[0], C, lower_bound_domain, upper_bound_domain);
        auto* problem = create_problem(cantusFirmus, species, C, lower_bound_domain, upper_bound_domain, v_type, melodic_params, general_params, specific_params,
            importance, borrowMode);
        //cout << problem->to_string() << endl;
        // create a new search engine

        for(int j = 0; j < size; j++){
            rel(problem->getHome(), problem->getCounterpoint()->getBranchingNotes()[j], IRT_EQ, test->getCp()[j]);
        }

        BAB<CounterpointProblem> e(problem);

        int nb_sol = 0;
        while(CounterpointProblem* pb = e.next()){
            nb_sol++;
            cout << "This test passed successfully!" << endl;
            // cout << int_vector_to_string(cantusFirmus) << endl;

            delete pb;
            if (nb_sol >= 1)
                break;
        }
        if(nb_sol!=1){
            cout << "This test did not pass. An error was found!" << endl;
        }
    } else {
        throw std::invalid_argument("Wrong amount of arguments!");
    }

    return 0;
}


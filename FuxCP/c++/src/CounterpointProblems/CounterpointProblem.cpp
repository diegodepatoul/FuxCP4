#include "../../headers/CounterpointProblems/CounterpointProblem.hpp"
#include "../../headers/CounterpointUtils.hpp"

/**
 * Constructor of the class.
 * @param cf a vector<int> representing the cantus firmus.
 * @param k the key of the score. it takes values from the notes in headers/Utilities.hpp
 * @param lb the lowest note possible for the counterpoint in MIDI
 * @param ub the highest note possible for the counterpoint in MIDI
 */
CounterpointProblem::CounterpointProblem(vector<int> cf, int k, int lb, int ub){
    nMeasures = cf.size();
    key = k;
    lowerBound = lb;
    upperBound = ub;

    cantusFirmus = new CantusFirmus(*this, nMeasures, cf, key);

}

// COPY CONSTRUCTOR
CounterpointProblem::CounterpointProblem(CounterpointProblem& s) : Space(s){
    if (s.cantusFirmus) {
        cantusFirmus = s.cantusFirmus->clone(*this);
    } else {
        cantusFirmus = nullptr;
    }

    nMeasures = s.nMeasures; 
    key = s.key;
    lowerBound = s.lowerBound;
    upperBound = s.upperBound;
}

Space* CounterpointProblem::copy(){   // todo use 'bool share' in copy constructor?
    return new CounterpointProblem(*this);
}


string CounterpointProblem::to_string() const {
    string text = "Counterpoint problem : \n";
    text += cantusFirmus->to_string(); 
    return text;
}
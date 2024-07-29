#ifndef MYPROJECT_FOURVOICECOUNTERPOINT_HPP
#define MYPROJECT_FOURVOICECOUNTERPOINT_HPP

#include "../Utilities.hpp"
#include "CounterpointProblem.hpp"
#include "../Parts/FirstSpeciesCounterpoint.hpp"
#include "../Parts/SecondSpeciesCounterpoint.hpp"
#include "../Parts/ThirdSpeciesCounterpoint.hpp"
#include "../Parts/CantusFirmus.hpp"

/**
 * This class models a counterpoint problem with 4 voices.
 */
class FourVoiceCounterpoint : public CounterpointProblem{
protected:

    Stratum* upper1;
    Stratum* upper2;
    Stratum* upper3;

    vector<int> species;        /// the species of the counterpoints to generate

public:
    /**
     * Constructor of the class.
     * @param cf a vector<int> representing the cantus firmus.
     * @param sp the species of the counterpoint. it takes values from the enum "species" in headers/Utilities.hpp
     * @param k the key of the score. it takes values from the notes in headers/Utilities.hpp
     * @param lb the lowest note possible for the counterpoint in MIDI
     * @param ub the highest note possible for the counterpoint in MIDI
     */
    FourVoiceCounterpoint(vector<int> cf, vector<int> sp, int k, int lb, int ub, vector<int> v_type, vector<int> m_costs, vector<int> g_costs, 
        vector<int> s_costs, vector<int> imp, int bm);

    FourVoiceCounterpoint(FourVoiceCounterpoint& s);
    IntLexMinimizeSpace* copy() override; 

    string to_string() const override;

    /// Getters
    // Part* getCounterpoint(){ return counterpoint; }
    //todo add here other getters if necessary

    ///destructor
    //todo release the allocated memory (each object created must be deleted)
    ~FourVoiceCounterpoint(){
        delete upper1;
        delete upper2;
        delete upper3;
    }  

    void uniteCounterpoints();

    void uniteCosts();
};

#endif
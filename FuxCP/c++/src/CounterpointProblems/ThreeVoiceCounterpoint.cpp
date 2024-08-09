#include "../../headers/CounterpointProblems/ThreeVoiceCounterpoint.hpp"
#include "../../headers/CounterpointUtils.hpp"

/**
 * Constructor of the class.
 * @param cf a vector<int> representing the cantus firmus.
 * @param sp the species of the counterpoint. it takes values from the enum "species" in headers/Utilities.hpp
 * @param k the key of the score. it takes values from the notes in headers/Utilities.hpp
 * @param lb the lowest note possible for the counterpoint in MIDI
 * @param ub the highest note possible for the counterpoint in MIDI
 */
ThreeVoiceCounterpoint::ThreeVoiceCounterpoint(vector<int> cf, vector<Species> sp, vector<int> v_type, vector<int> m_costs, vector<int> g_costs,
    vector<int> s_costs, vector<int> imp, int bm) :
    CounterpointProblem(cf, -1, m_costs, g_costs, s_costs, imp, THREE_VOICES){
    species = sp;
    

    upper1 = new Stratum(*this, nMeasures, 0, 127, lowest->getNotes(), THREE_VOICES);
    upper2 = new Stratum(*this, nMeasures, 0, 127, lowest->getNotes(), THREE_VOICES);

    counterpoint_1 = create_counterpoint(*this, species[0], nMeasures, cf, (6 * v_type[0] - 6) + cf[0], (6 * v_type[0] + 12) + cf[0], lowest, 
        cantusFirmus, v_type[0], m_costs, g_costs, s_costs, bm, THREE_VOICES);
    counterpoint_2 = create_counterpoint(*this, species[1], nMeasures, cf, (6 * v_type[1] - 6) + cf[0], (6 * v_type[1] + 12) + cf[0], lowest, 
        cantusFirmus, v_type[1], m_costs, g_costs, s_costs, bm, THREE_VOICES);

    setLowest(counterpoint_2, nullptr, upper1, upper2, nullptr);

    vector<Part*> parts = {cantusFirmus, counterpoint_1, counterpoint_2};

    triadCostArray = IntVarArray(*this, counterpoint_1->getFirstHInterval().size(), IntSet({0, counterpoint_1->getTriadCost()}));

    int scc_cz = ((cantusFirmus->getSize()/4)-1);
    bool containsThirdSpecies = 0;

    successiveCostArray = IntVarArray(*this, scc_cz, IntSet({0, counterpoint_1->getSuccCost()}));
    
    //G9 last chord must have the same fundamental as the cf (used throughout the composition)
    G9_lastChordSameAsFundamental(*this, lowest, cantusFirmus);

    for(int p = 1; p < parts.size(); p++){
        // G6 : no chromatic melodies (works for 1st, 2nd and 3rd species)
        G6_noChromaticMelodies(*this, parts[p], sp[p-1]);

        // H5 from Thibault : The cp and the cf cannot play the same note
        H5_1_cpAndCfDifferentNotes(*this, parts[p], cantusFirmus);
    }

    //H8 : the triad should be used as much as possible
    H8_3v_preferHarmonicTriad(*this, counterpoint_1, triadCostArray, upper1, upper2);
    //M4 variety cost (notes should be as diverse as possible)
    M4_varietyCost(*this, parts);
    //P4 avoid successive perfect consonances
    P4_successiveCost(*this, parts, scc_cz, successiveCostArray, species);
    //P6 : no move in same direction
    if(counterpoint_1->getSpecies()!=FOURTH_SPECIES&&counterpoint_2->getSpecies()!=FOURTH_SPECIES){
        P6_noMoveInSameDirection(*this, parts);
    }
    
    //P7 : no suxxessive ascending sixths
    P7_noSuccessiveAscendingSixths(*this, parts);

    //2.M2, have to write it here since it has a weird interaction with the third species
    M2_2_3v_melodicIntervalsNotExceedMinorSixth(*this, parts, containsThirdSpecies);
    
    solutionArray = IntVarArray(*this, counterpoint_1->getBranchingNotes().size() + counterpoint_2->getBranchingNotes().size(), 0, 127);

    unitedCosts = IntVarArray(*this, 14, 0, 10000000);
    unitedCostNames = {};
    
    //TEST

    uniteCounterpoints();
    uniteCosts();

    for(int i = 0; i < unitedCostNames.size(); i++){
        cout << unitedCostNames[i] << endl;
    }

    orderCosts();

    branch(*this, solutionArray, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, lowest->getNotes().slice(0, 4/notesPerMeasure.at(FIRST_SPECIES), lowest->getNotes().size()), INT_VAR_DEGREE_MAX(), INT_VAL_SPLIT_MIN());
    if(species[0]==FOURTH_SPECIES){
        branch(*this, counterpoint_1->getSyncopeCostArray(),  INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
    }
    if(species[1]==FOURTH_SPECIES){
        branch(*this, counterpoint_2->getSyncopeCostArray(),  INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
    }


    writeToLogFile(("solution array size : " + std::to_string(solutionArray.size())).c_str());

}

// COPY CONSTRUCTOR
ThreeVoiceCounterpoint::ThreeVoiceCounterpoint(ThreeVoiceCounterpoint& s) : CounterpointProblem(s){
    species = s.species;
    if (s.upper1) {
        upper1 = s.upper1->clone(*this);
    } else {
        upper1 = nullptr;
    }
    if (s.upper2) {
        upper2 = s.upper2->clone(*this);
    } else {
        upper2 = nullptr;
    }
}

IntLexMinimizeSpace* ThreeVoiceCounterpoint::copy(){  
    return new ThreeVoiceCounterpoint(*this);
}


string ThreeVoiceCounterpoint::to_string() const {
    string text = CounterpointProblem::to_string();
    text += "Counterpoint 1 : \n";
    text += counterpoint_1->to_string();
    text += "\n";
    text += "Counterpoint 2 : \n";
    text += counterpoint_2->to_string();
    text += "\n";
    text += "Upper 1 : \n";
    text += upper1->to_string();
    text += "\n";
    text += "Upper 2 : \n";
    text += upper2->to_string();
    text += "\n";
    text += " Solution array : \n";
    text += intVarArray_to_string(solutionArray);
    text += "\n";
    return text;
}

void ThreeVoiceCounterpoint::uniteCounterpoints(){
    int idx = 0;
    for(int i = 0; i < counterpoint_1->getBranchingNotes().size(); i++){
        rel(*this, solutionArray[idx], IRT_EQ, counterpoint_1->getBranchingNotes()[i]);
        idx++;
    }
    for(int i = 0; i < counterpoint_2->getBranchingNotes().size(); i++){
        rel(*this, solutionArray[idx], IRT_EQ, counterpoint_2->getBranchingNotes()[i]);
        idx++;
    }
}

void ThreeVoiceCounterpoint::uniteCosts(){
    int cp1_idx = 0;
    int cp2_idx = 0;
    for(int i = 0; i < 14; i++){
        string name = importanceNames[i];
        if(name=="succ"){
            unitedCostNames.push_back(name);
            rel(*this, unitedCosts[i], IRT_EQ, expr(*this, sum(IntVarArgs(successiveCostArray))));
        } else if(name=="triad"){
            unitedCostNames.push_back(name);
            rel(*this, unitedCosts[i], IRT_EQ, expr(*this, sum(IntVarArgs(triadCostArray))));
        } else {

            //decides if the cost is present for any counterpoint

            bool cp1_contains = 0;
            bool cp2_contains = 0;
            int sz = 0;
            for(int t = 0; t < counterpoint_1->getCostNames().size(); t++){
                if(name==counterpoint_1->getCostNames()[t]){
                    cp1_contains=1;
                    sz++;
                }
            }
            for(int t = 0; t < counterpoint_2->getCostNames().size(); t++){
                if(name==counterpoint_2->getCostNames()[t]){
                    cp2_contains=1;
                    sz++;
                }
            }
            if(!cp1_contains && !cp2_contains){
                unitedCostNames.push_back("NOT ADDED");
            } else {
                unitedCostNames.push_back(name);
                //adds the cost to the IntVarArgs
                IntVarArgs x(sz);
                int idx=0;
                if(cp1_contains){
                    x[idx] = counterpoint_1->getCosts()[cp1_idx];
                    idx++;
                    cp1_idx++;
                }
                if(cp2_contains){
                    x[idx] = counterpoint_2->getCosts()[cp2_idx];
                    idx++;
                    cp2_idx++;
                }
                rel(*this, unitedCosts[i], IRT_EQ, expr(*this, sum(x)));
            }
        }
    }
}
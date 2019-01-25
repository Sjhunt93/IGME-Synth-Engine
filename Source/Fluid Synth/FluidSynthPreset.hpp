//
//  FluidSynthPreset.hpp
//  Beta Music Software
//
//  Created by Samuel Hunt on 17/10/2018.
//

#ifndef FluidSynthPreset_hpp
#define FluidSynthPreset_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class FluidSynthPreset {
public:
    struct FSPreset {
        int bank;
        String name;
        int number;
        
        int uniqueId;
    };
    
    FluidSynthPreset ();
    ~FluidSynthPreset ();
    
    
    FSPreset getPreset (const int _id);
    const int getTotalPresets ();
    void clearPresets ();
    void addPreset (FSPreset fs);
    
private:
    std::vector<FSPreset> preset;
    
};



#endif /* FluidSynthPreset_hpp */

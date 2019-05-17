//
//  FluidSynthPreset.cpp
//  Beta Music Software
//
//  Created by Samuel Hunt on 17/10/2018.
//

#include "FluidSynthPreset.hpp"


FluidSynthPreset::FluidSynthPreset ()
{
    
}
FluidSynthPreset::~FluidSynthPreset ()
{
    
}

FluidSynthPreset::FSPreset FluidSynthPreset::getPreset (const int _id)
{
    if (_id < getTotalPresets() && _id >= 0) {
        return preset[_id];
    }
    return preset[0];
}


const int FluidSynthPreset::getTotalPresets ()
{
    return preset.size();
}
void FluidSynthPreset::clearPresets ()
{
    preset.clear();
}
void FluidSynthPreset::addPreset (FSPreset fs)
{
    preset.push_back(fs);
}



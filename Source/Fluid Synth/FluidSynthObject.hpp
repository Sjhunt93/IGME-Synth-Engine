//
//  FluidSynthObject.hpp
//  JUCE Fluid Synth  - App
//
//  Created by Samuel Hunt on 16/10/2018.
//

#ifndef FluidSynthObject_hpp
#define FluidSynthObject_hpp


#include "../JuceLibraryCode/JuceHeader.h"
//Ughh this is so stupid.. not sure why i cant just include that file as is...

//This will have to be changed for each enviroment
#include "/Users/sj4-hunt/Documents/Code/music-content-analysis-sequencing-library/externals/fluidsynth_mac/fluidsynth/fluidsynth/include/fluidsynth.h"
#include "FluidSynthPreset.hpp"


//#include "fluidsynth_mac/fluidsynth/fluidsynth/include/fluidsynth.h"
//#include "../../../externals/fluidsynth_mac/fluidsynth/fluidsynth/include/fluidsynth.h"

class FluidSynthObject: public AudioSource
{
    
public:
    
    FluidSynthObject ();
    ~FluidSynthObject ();
    
    void loadSoundfont (const String& soundfont);
    
    void processNote (int noteNumber, int velocity, int channel);
    void loadSound (int channel, int prgmChange, int bank);
    void setVolume (int channel, float volume);
    void MIDIIN (MidiMessage message);
    
    
    void prepareToPlay     (int samplesPerBlockExpected, double sampleRate) ;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)     ;
    void releaseResources  ()                                                 ;
    
    void allNotesOff ();
    
    FluidSynthPreset * getPresets ();
private:
    
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    
    int currentSoundfont;
    FluidSynthPreset fsynthPresets;
};


#endif /* FluidSynthObject_hpp */

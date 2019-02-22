//
//  AudioMain.h
//  MusicContentAnalysisLibrary
//
//  Created by Samuel Hunt on 10/08/2017.
//
//

#ifndef __MusicContentAnalysisLibrary__AudioMain__
#define __MusicContentAnalysisLibrary__AudioMain__

#include "../JuceLibraryCode/JuceHeader.h"
//#include "juce_audio_utils.h"
#include "FluidSynthObject.hpp"

/*
 The way i imagine this class working is as follows..
 
 The existing app - the note editor
 sends out midi when sequencing.
 This now gets passed to a swtich in the main MIDI interface
 either it gets sent out or passed directly into this class.
 
 A real cheap and dirty way of doing is to simply pick up each callback
 
 */

class AudioMain : public AudioAppComponent,
public ChangeListener,
public MidiInputCallback
{
public:
    
    AudioMain();
    ~AudioMain();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void resized();
    void changeListenerCallback (ChangeBroadcaster* source);

    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);

//    AudioEngineFileIO & getFileIO ();
    void setSoundForTrack (const int trackId, const int soundId);
    void setVolumeForTrack (const int trackId, const float volume);
    FluidSynthObject & getFluidSynthObject ();
    
private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState (TransportState newState);


//==========================================================================

//    AudioDeviceManager
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;

    
    FluidSynthObject * fluidSynth;
    
    std::vector<MidiInput *> inputs;
    
    AudioDeviceSelectorComponent audioSettingsComp;
};



//Component* createMainContentComponent()     { return new AudioMain(); }

#endif /* defined(__MusicContentAnalysisLibrary__AudioMain__) */

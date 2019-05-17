//
//  AudioMain.cpp
//  MusicContentAnalysisLibrary
//
//  Created by Samuel Hunt on 10/08/2017.
//
//

#include "AudioMain.h"



#define REPEAT(B,A) for (int ghjhg = 0; ghjhg < A; ghjhg++) {B;}

AudioMain::AudioMain() :   state (Stopped), audioSettingsComp(deviceManager, 0, 0, 2,2, false, false, true, true )
{
    
    
	start = false;
    fluidSynth = new FluidSynthObject();
    setAudioChannels (2, 2);
    formatManager.registerBasicFormats();
    
    //For now this will be picking up the outgoing calls from the IOMidiInterface class.
    
    if (0) {
    StringArray devices = MidiInput::getDevices();
    for (int i = 0; i < devices.size(); i++) {
        String name = devices[i];
        printf("%s \n", name.toRawUTF8());
        MidiInput * input = MidiInput::openDevice(i, this);
        jassert(input);
        input->start();
        inputs.push_back(input);
    }
    }
    

    
    File tutorialSettings = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile ).getChildFile("Contents").getChildFile("Resources");
    
    File soundFont = ("C:\\Users\\Sam\\Documents\\code projects\\IGME-Synth-Engine\\Source\\GeneralUser GS MuseScore v1.442.sf2");
		//tutorialSettings.getChildFile("GeneralUser GS MuseScore v1.442.sf2");
    jassert(soundFont.exists());
    //fluidSynth->loadSoundfont(soundFont.getFullPathName());
    
    
    addAndMakeVisible(audioSettingsComp);
    
	
//    DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComp, Colours::azure, true);
    
}
AudioMain::~AudioMain()
{
	delete fluidSynth;

    shutdownAudio();


    
    for (int i = 0; i < inputs.size(); i++) {
        inputs[i]->stop();
        delete inputs[i];
    }
    
}
void AudioMain::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
     fluidSynth->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioMain::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	
	static int c = 0;
		bufferToFill.clearActiveBufferRegion();
		fluidSynth->getNextAudioBlock(bufferToFill);
		c++;
	
}

void AudioMain::releaseResources()
{
    //    transportSource.releaseResources();
    fluidSynth->releaseResources();
}

void AudioMain::resized()
{
    audioSettingsComp.setBounds(0, 0, 400, 300);
}

void AudioMain::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else
            changeState (Stopped);
    }
}




void AudioMain::changeState (TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        
        switch (state)
        {
            case Stopped:
                transportSource.setPosition (0.0);
                break;
                
            case Starting:
                transportSource.start();
                break;
            case Playing:
                break;
                
            case Stopping:
                transportSource.stop();
                break;
        }
    }
}
void AudioMain::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
	start = true;
   fluidSynth->MIDIIN(message);
}



void AudioMain::setSoundForTrack (const int trackId, const int soundId)
{
    if (trackId < 16) {
        
        FluidSynthPreset::FSPreset preset =  fluidSynth->getPresets()->getPreset(soundId);
        fluidSynth->loadSound(trackId, preset.number, preset.bank);
    }
    else {
        jassertfalse;
    }
}

void AudioMain::setVolumeForTrack (const int trackId, const float volume)
{
    fluidSynth->setVolume(trackId, volume);
}

FluidSynthObject & AudioMain::getFluidSynthObject ()
{
    return *fluidSynth;
}


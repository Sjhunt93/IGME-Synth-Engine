//
//  FluidSynthObject.cpp
//  JUCE Fluid Synth  - App
//
//  Created by Samuel Hunt on 16/10/2018.
//

#include "FluidSynthObject.hpp"






FluidSynthObject::FluidSynthObject()
:
currentSoundfont(-1)
{
    settings = new_fluid_settings ();
    synth      = new_fluid_synth    (settings);
    
    fluid_synth_set_gain      (synth, 5.0);
    fluid_synth_set_polyphony (synth, 256);
    
    
}

FluidSynthObject::~FluidSynthObject()
{
    //   fluid_synth_stop (synth, 1);
    fluid_synth_stop (synth, 0);
    delete_fluid_synth    (synth);
    delete_fluid_settings (settings);
}

void FluidSynthObject::allNotesOff()
{
    fluid_synth_all_notes_off(synth, 1);
}

void FluidSynthObject::loadSoundfont(const String& soundfont)
{
    std::cout << "Loading Soundfont: " << soundfont << "\n";
    
    fluid_synth_all_notes_off(synth, 1);
    
    if (currentSoundfont != -1) {
        fluid_synth_sfunload(synth, currentSoundfont, 0);
    }
    
    
    
    
    currentSoundfont = fluid_synth_sfload(synth, soundfont.toRawUTF8(), 1);
    
    
    
    
    //    fluid_preset_t * t = fluid_synth_find_preset(synth, 0, 0);
    
    std::cout << fluid_synth_sfcount(synth) << "\n";
    
    
    int soundfontCount = fluid_synth_sfcount(synth);
    
    if (soundfontCount == 0) {
        // no soundfont selected
        
    }
    unsigned int sfont_id = 1;
    fluid_sfont_t* sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
    if(sfont == nullptr) {
        // no soundfont found by that ID
        // the above guard (soundfontCount) protects us for the
        // main case we're expecting. this guard is just defensive programming.
        jassertfalse;
    }
    
    int offset = fluid_synth_get_bank_offset(synth, sfont_id);
    
    sfont->iteration_start(sfont);
    
    fluid_preset_t preset;
    
    
    
    
    while(sfont->iteration_next(sfont, &preset)) {
        
        std::cout << "preset: num: " << preset.get_num(&preset) << " | bank: " << preset.get_banknum(&preset) << " | " << preset.get_name(&preset) << "\n";
        
        FluidSynthPreset::FSPreset fsPreset;
        fsPreset.bank = preset.get_banknum(&preset);
        fsPreset.number = preset.get_num(&preset);
        fsPreset.name = preset.get_name(&preset);
        fsPreset.uniqueId = fsynthPresets.getTotalPresets();
        fsynthPresets.addPreset(fsPreset);
    }
    

    
#if 0
    fluid_preset_t* preset = new fluid_preset_t();
    
    // Reset the iteration
    sf->iteration_start(sf);
    
    // Go through all the presets within the soundfont
    int more = 1;
    while (more) {
        more = sf->iteration_next(sf, preset); // Will return 0 if no more soundfonts left
        if (more) {
            // Get preset name
            char* presetname = preset->get_name(preset);
            int banknum = preset->get_banknum(preset);
            int num = preset->get_num(preset);
            
            // Do something with the presetname, bank number and program number
            // Such as add it to some list so that you can refer to it later
        }
    }
#endif
    
    
}

void FluidSynthObject::setVolume (int channel, float volume)
{
//    fluid_synth_set_chan
//    fluid_synth_set_ch
    fluid_synth_cc(synth, channel, 7, (int) (volume*10));
}

void FluidSynthObject::processNote(int note, int velocity, int channel)
{
    std::cerr << "FluidSynthObject note: " << note << " " << velocity << std::endl;
    
    
    if (note > 0 && note < 127) {
        if (velocity == 0) {
            fluid_synth_noteoff (synth, channel, note);
        }
        else {
            fluid_synth_noteon (synth, channel, note, velocity);
        }
    }
    
}

void FluidSynthObject::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    fluid_synth_set_sample_rate (synth, sampleRate);
}

void FluidSynthObject::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    fluid_synth_write_float (synth,
                             bufferToFill.buffer->getNumSamples(),
                             bufferToFill.buffer->getWritePointer(0), 0, 1,
                             bufferToFill.buffer->getWritePointer(1), 0, 1);
}

void FluidSynthObject::releaseResources()
{
//    delete_fluid_synth    (synth);
 //   delete_fluid_settings (settings);
    fluid_synth_system_reset(synth);
}

void FluidSynthObject::loadSound (int channel, int prgmChange, int bank)
{
    fluid_synth_program_select(synth, channel, 1, bank, prgmChange);
    
    //luid_synth_program_select(fluid_synth_t* synth, int chan, unsigned int sfont_id, unsigned int bank_num, unsigned int preset_num)
}


void FluidSynthObject::MIDIIN (MidiMessage message)
{
    if (message.isProgramChange()) {
//        fluid_synth_program_change(synth, message.getChannel(), message.getProgramChangeNumber());
        
           fluid_synth_program_select(synth, message.getChannel(), 1, 0, static_cast<unsigned int>(message.getProgramChangeNumber()));
    }
    else if (message.isNoteOnOrOff()) {
        processNote(message.getNoteNumber(), message.isNoteOn() ? message.getVelocity() : 0, message.getChannel()-1);
    }

    
    //int fluid_synth_program_change(fluid_synth_t* synth, int chan, int program);
}


FluidSynthPreset * FluidSynthObject::getPresets ()
{
    return &fsynthPresets;
}

/*
  ==============================================================================

    Fluidsynth.cpp
    Created: 31 Jan 2016 4:43:50pm
    Author:  Chris Penny

  ==============================================================================
*/

#include "JFluidsynth.h"

Fluidsynth::Fluidsynth()
:
currentSoundfont(-1)
{
    settings = new_fluid_settings ();
    synth 	 = new_fluid_synth    (settings);

    fluid_synth_set_gain      (synth, 5.0);
    fluid_synth_set_polyphony (synth, 256);
}

Fluidsynth::~Fluidsynth()
{
    delete_fluid_synth    (synth);
    delete_fluid_settings (settings);
}

void Fluidsynth::allNotesOff()
{
    fluid_synth_all_notes_off(synth, 1);
}

void Fluidsynth::loadSoundfont(const String& soundfont)
{
    std::cerr << "Loading Soundfont: " << soundfont << std::endl;

    fluid_synth_all_notes_off(synth, 1);

    if (currentSoundfont != -1) {
        fluid_synth_sfunload(synth, currentSoundfont, 0);
    }

	currentSoundfont = fluid_synth_sfload(synth, soundfont.toRawUTF8(), 1);


	int soundfontCount = fluid_synth_sfcount(synth);

	if (soundfontCount == 0) {
		// no soundfont selected

	}
	unsigned int sfont_id = 1;
	fluid_sfont_t* sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
	if (sfont == nullptr) {
		// no soundfont found by that ID
		// the above guard (soundfontCount) protects us for the
		// main case we're expecting. this guard is just defensive programming.
		jassertfalse;
	}

	int offset = fluid_synth_get_bank_offset(synth, sfont_id);

	sfont->iteration_start(sfont);

	fluid_preset_t preset;




	while (sfont->iteration_next(sfont, &preset)) {

		//System.Diagnostics.Debug.WriteLine("preset: ");
		std::cout << "preset: num: " << preset.get_num(&preset) << " | bank: " << preset.get_banknum(&preset) << " | " << preset.get_name(&preset) << "\n";

		FluidSynthPreset::FSPreset fsPreset;
		fsPreset.bank = preset.get_banknum(&preset);
		fsPreset.number = preset.get_num(&preset);
		fsPreset.name = preset.get_name(&preset);
		fsPreset.uniqueId = fsynthPresets.getTotalPresets();
		fsynthPresets.addPreset(fsPreset);
	}
}



void Fluidsynth::processNote(int note, int velocity, int channel)
{
	std::cerr << "FluidSynthObject note: " << note << " " << velocity << std::endl;


	if (note > 0 && note < 127) {
		if (velocity == 0) {
			fluid_synth_noteoff(synth, channel, note);
		}
		else {
			fluid_synth_noteon(synth, channel, note, velocity);
		}
	}
}
void Fluidsynth::loadSound(int channel, int prgmChange, int bank)
{
	fluid_synth_program_select(synth, channel, 1, bank, prgmChange);

}
void Fluidsynth::setVolume(int channel, float volume)
{
	fluid_synth_cc(synth, channel, 7, (int)(volume * 10));

}
void Fluidsynth::MIDIIN(MidiMessage message)
{
	if (message.isProgramChange()) {
		//        fluid_synth_program_change(synth, message.getChannel(), message.getProgramChangeNumber());

		fluid_synth_program_select(synth, message.getChannel(), 1, 0, static_cast<unsigned int>(message.getProgramChangeNumber()));
	}
	else if (message.isNoteOnOrOff()) {
		processNote(message.getNoteNumber(), message.isNoteOn() ? message.getVelocity() : 0, message.getChannel() - 1);
	}
}

void Fluidsynth::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    fluid_synth_set_sample_rate (synth, sampleRate);
}

void Fluidsynth::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    fluid_synth_write_float (synth, 
    						 bufferToFill.buffer->getNumSamples(),
    						 bufferToFill.buffer->getWritePointer(0), 0, 1, 
    						 bufferToFill.buffer->getWritePointer(1), 0, 1);
}

void Fluidsynth::releaseResources()
{
	//delete_fluid_synth    (synth);
    //delete_fluid_settings (settings);
}

FluidSynthPreset * Fluidsynth::getPresets()
{
	return &fsynthPresets;
}

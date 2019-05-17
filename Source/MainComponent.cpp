/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "coms.h"
//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

	fSynth.loadSoundfont("C:\\Users\\Sam\\Documents\\code projects\\IGME-Synth-Engine\\Source\\GeneralUser GS MuseScore v1.442.sf2");

	addAndMakeVisible(btn);
	btn.onClick = [this]()
	{
		fSynth.processNote(60, 127, 0);
	};
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);

	{
		//set up OSC

		const int listenPort = 9003;
		const int sendPort = 9004;

		if (!connect(listenPort)) { //listen
			//alert
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 1010 already in use", "Please restart aserve.");
			std::cout << "Error opening 1010 for listening \n";
		}

		if (!oscSender.connect("127.0.0.1", sendPort)) {
			//error
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 1011 already in use", "Please restart aserve.");
			std::cout << "Error opening 1011 for reciving \n";
		}

		// tell the component to listen for OSC messages matching this address:
		addListener(this);

		oscSender.send(OSCMessage(Coms::synthStart));
	}
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}
void MainComponent::oscMessageReceived(const OSCMessage& message)
{
	if (message.getAddressPattern().toString().startsWith(Coms::loadPreset)) {
		if (message[0].isInt32() && message[1].isInt32()) {
			const int track = message[0].getInt32();
			const int soundId = message[1].getInt32();
			FluidSynthPreset::FSPreset preset = fSynth.getPresets()->getPreset(soundId);
			fSynth.loadSound(track, preset.number, preset.bank);
			std::cout << "Load preset " << track << " : " << soundId << "\n";
		}
	}
	else if (message.getAddressPattern().toString().startsWith(Coms::getPresets)) {
		/*
		 Que up message!
		 */
		const int total = fSynth.getPresets()->getTotalPresets();
		for (int i = 0; i < total; i++) {
			FluidSynthPreset::FSPreset fSynthPreset = fSynth.getPresets()->getPreset(i);
			OSCMessage message(Coms::preset, fSynthPreset.name, fSynthPreset.number, fSynthPreset.bank, fSynthPreset.uniqueId);
			oscSender.send(message);
		}
		oscSender.send(OSCMessage(Coms::presetSent));
	}
	else if (message.getAddressPattern().toString().startsWith(Coms::midi)) {
		if (message.size() == 3) {
			int s1 = message[0].getInt32();
			int d1 = message[1].getInt32();
			int d2 = message[2].getInt32();

			MidiMessage m(s1, d1, d2);
			fSynth.MIDIIN(m);
			printf("%i %i %i \n", s1, d1, d2);
		}
	}
	else if (message.getAddressPattern().toString().startsWith(Coms::trackVolume)) {
		if (message.size() == 2) {
			if (message[0].isInt32() && message[1].isFloat32()) {
				const int track = message[0].getInt32();
				const float vol = message[1].getFloat32();
				fSynth.setVolume(track, vol);
			}
		}

	}
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
	fSynth.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
	fSynth.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
	fSynth.releaseResources();


    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	btn.setBounds(50, 50, 400, 100);

}

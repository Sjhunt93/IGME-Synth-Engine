/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioMain.h"
#include "coms.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
private OSCReceiver,
private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
       // audioMain.setSoundForTrack(0, 5);
        
        const int listenPort = 9003;
        const int sendPort = 9004;
        
        if (! connect (listenPort)) { //listen
            //alert
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 1010 already in use", "Please restart aserve.");
            std::cout << "Error opening 1010 for listening \n";
        }
        
        if (! oscSender.connect ("127.0.0.1", sendPort)) {
            //error
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 1011 already in use", "Please restart aserve.");
            std::cout << "Error opening 1011 for reciving \n";
        }
        
        // tell the component to listen for OSC messages matching this address:
        addListener (this);
        
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void oscMessageReceived (const OSCMessage& message) override
    {
         if (message.getAddressPattern().toString().startsWith(Coms::loadPreset)) {
            if (message[0].isInt32() && message[1].isInt32()) {
                const int track = message[0].getInt32();
                const int soundId = message[1].getInt32();
                audioMain.setSoundForTrack(track, soundId);
            }
        }
        else if (message.getAddressPattern().toString().startsWith(Coms::getPresets)) {
            /*
             Que up message!
             */
            const int total = audioMain.getFluidSynthObject().getPresets()->getTotalPresets();
            for (int i = 0; i < total; i++) {
                FluidSynthPreset::FSPreset fSynthPreset = audioMain.getFluidSynthObject().getPresets()->getPreset(i);
                OSCMessage message(Coms::preset, fSynthPreset.name, fSynthPreset.number, fSynthPreset.bank, fSynthPreset.uniqueId);
                oscSender.send(message);
            }
        }
        else if (message.getAddressPattern().toString().startsWith(Coms::midi)) {
            if (message.size() == 3) {
                int s1 = message[0].getInt32();
                int d1 = message[1].getInt32();
                int d2 = message[2].getInt32();
                
                MidiMessage m(s1, d1, d2);
                audioMain.handleIncomingMidiMessage(nullptr, m);
                printf("%i %i %i \n", s1, d1, d2);
            }
        }
    }
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        audioMain.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
        audioMain.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
        audioMain.releaseResources();
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================
    AudioMain   audioMain;
    OSCSender   oscSender;
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }



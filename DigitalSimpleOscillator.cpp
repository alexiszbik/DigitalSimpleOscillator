#include "daisy_seed.h"
#include "daisysp.h"

#include "DaisyYMNK/Display/DisplayManager.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;
Oscillator osc;
AdEnv      env;

MidiUartHandler midi;

Switch button1;

int note = 60;

#define WAVE_LENGTH 64
float DSY_QSPI_BSS qspi_buffer[WAVE_LENGTH];

DisplayManager *display = DisplayManager::GetInstance();

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{

    //Fill the block with samples
    for(size_t i = 0; i < size; i += 2)
    {
        //Get the next envelope value
        //get the next oscillator sample
        float osc_out = osc.Process();

        //Set the left and right outputs
        out[i]     = osc_out;
        out[i + 1] = osc_out;
    }
}


// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    if (m.type == NoteOn) {
        NoteOnEvent noteOn = m.AsNoteOn();
        note = noteOn.note;
        osc.SetFreq(mtof(note));
    }
    else if (m.type == NoteOff) {
        NoteOffEvent noteOff = m.AsNoteOff();
    } 
}

void InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();

    display->Init(&hardware);
    hardware.SetLed(true);

	hardware.SetAudioBlockSize(128); // number of samples handled per callback
	hardware.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    System::Delay(1000); // as if something wrong happenend ....

    InitMidi();

    //How many samples we'll output per second
    float samplerate = hardware.AudioSampleRate();

    //Set up oscillator
    osc.Init(samplerate);
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(1.f);
    osc.SetFreq(440);

    hardware.StartAudio(AudioCallback);

    float wavform_ram[WAVE_LENGTH];

    /** Fill 64kB wave */
    for(uint32_t i = 0; i < WAVE_LENGTH; i++)
    {
        float frac = (float)i / (float)WAVE_LENGTH;

        /** Simple saw wave gen */
        float accum = 0;
        for(int j = 1; j < 64; j++)
        {
            accum += sin(TWOPI_F * frac * j) / j;
        }
        wavform_ram[i] = accum;
    }

    size_t size = sizeof(wavform_ram[0]) * WAVE_LENGTH;
    /** Grab physical address from pointer */
    size_t address = (size_t)qspi_buffer;
    /** Erase qspi and then write that wave */
    display->WriteNow("Hello", "World", "Of", "Darkness");
    hardware.qspi.Erase(address, address + size);

    display->WriteNow("Erase", "Done");

    hardware.qspi.Write(address, size, (uint8_t*)wavform_ram);

    display->WriteNow("Save", "Done");

    //Start the adc
    //hardware.adc.Start();

    //Start calling the audio callback
   

    // Loop forever
    
    for(;;)
    {
        midi.Listen();
        // Handle MIDI Events
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
    };
}

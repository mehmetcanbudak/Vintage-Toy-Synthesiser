/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "../../BBB/VintageToySynthProject/globals.h"


//==============================================================================
//==============================================================================
//==============================================================================
//Constructor

MainContentComponent::MainContentComponent()
                    :   timeSliceThread ("contents list thread")
{
    //==============================================================================
    //backend...
    
    timeSliceThread.startThread();
    
    patchDir = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + File::separatorString + "Vintage Toy Synthesiser" + File::separatorString + "patches";
    
    //if the patch directory doesn't yet exist, create it
    if (patchDir.isDirectory() == false)
    {
        patchDir.createDirectory();
    }
    
    //init audio device manager
    audioDeviceManager.initialise (0, 0, NULL, true);
    
    //attach this class as the MIDI input callback to the audio device manager
    audioDeviceManager.addMidiInputCallback(String::empty, this);
    
    for (int i = 0; i < 127; i++)
        patchData[i] = -1;
    
    patchFileFilter = new WildcardFileFilter ("*", String::empty, "All files");
    
    patchFileDirContentsList = new DirectoryContentsList (patchFileFilter, timeSliceThread);
    patchFileDirContentsList->setDirectory (patchDir, false, true);
    
    //==============================================================================
    //GUI
    addAndMakeVisible (audioDeviceSelectorComponent = new AudioDeviceSelectorComponent (audioDeviceManager, 0, 0, 0, 0, true, true, false, false));
    
    addAndMakeVisible(requestPatchDataButton = new TextButton());
    requestPatchDataButton->addListener(this);
    requestPatchDataButton->setButtonText("Get Patch Data From Synth");
    
    
    addAndMakeVisible (patchNameEditor = new Label("patch name editor", "insert patch name"));
    patchNameEditor->setEditable(true);
    patchNameEditor->setColour(Label::backgroundColourId, Colours::white);
    
    addAndMakeVisible(saveButton = new TextButton());
    saveButton->addListener(this);
    saveButton->setButtonText("Save");
    
    addAndMakeVisible(patchDirFileListComponent = new FileListComponent(*patchFileDirContentsList));
    patchDirFileListComponent->setColour(FileListComponent::backgroundColourId, Colours::white);
    patchDirFileListComponent->addListener(this);
    
    addAndMakeVisible(eventLabel = new Label());
    eventLabel->setText("Application started", dontSendNotification);
    eventLabel->setJustificationType(Justification::right);
    
    addAndMakeVisible(resetSoundEngineButton = new TextButton());
    resetSoundEngineButton->addListener(this);
    resetSoundEngineButton->setButtonText("Reset Synth to Panel Settings");
    
    addAndMakeVisible(disablePanelButton = new TextButton());
    disablePanelButton->addListener(this);
    disablePanelButton->setButtonText("Disable Synth Panel");
    disablePanelButton->setClickingTogglesState(true);
    
    addAndMakeVisible(titleLabel = new Label());
    titleLabel->setText("VTS Editor", dontSendNotification);
    titleLabel->setJustificationType(Justification::topLeft);
    Font title_font ("calibri", 96, Font::bold);
    titleLabel->setFont(title_font);
    
    addAndMakeVisible(saveGroup = new GroupComponent("save group", "Save Patch"));
    addAndMakeVisible(loadGroup = new GroupComponent("load group", "Load Patch"));
    addAndMakeVisible(midiIoGroup = new GroupComponent("midi IO group", "MIDI I/O Settings"));

    setSize (800, 400);
}

//==============================================================================
//==============================================================================
//==============================================================================
//Destructor

MainContentComponent::~MainContentComponent()
{
    
}

//==============================================================================
//==============================================================================
//==============================================================================
//JUCE Component function called to position all components

void MainContentComponent::resized()
{
    titleLabel->setBounds(0, 0, getWidth(), 80);
    eventLabel->setBounds(getWidth()/2, 50, getWidth()/2, 20);
    
    saveGroup->setBounds(0, 100, getWidth(), 55);
    requestPatchDataButton->setBounds(10, 120, 200, 20);
    patchNameEditor->setBounds(220, 120, 500, 20);
    saveButton->setBounds(getWidth()-60, 120, 50, 20);
 
    loadGroup->setBounds(0, 170, (getWidth()/2) - 10, getHeight() - 170);
    patchDirFileListComponent->setBounds(10, 190, (getWidth()/2) - 30, 150);
    resetSoundEngineButton->setBounds(10, getHeight() - 55, (getWidth()/2) - 30, 20);
    disablePanelButton->setBounds(10, getHeight() - 30, (getWidth()/2) - 30, 20);
    
    midiIoGroup->setBounds(getWidth()/2, 170, (getWidth()/2) - 10, getHeight() - 170);
    audioDeviceSelectorComponent->setBounds(getWidth()/2, getHeight()/2, getWidth()/2, getHeight()/2);
}

//==============================================================================
//==============================================================================
//==============================================================================
//JUCE Component function called to paint/draw GUI elements

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
}

//==============================================================================
//==============================================================================
//==============================================================================
//JUCE button callback function called when a button is clicked

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == saveButton)
    {
        savePatchFile();
    }
    
    else if (button == requestPatchDataButton)
    {
        printf("Requesting patch data from synth\r\n");
        eventLabel->setText("Requesting patch data from synth...", dontSendNotification);
        
        //send patch data request command CC to synth via MIDI
        MidiMessage message = MidiMessage::controllerEvent(1, PARAM_CMD, CMD_REQUEST_ALL_PATCH_DATA);
        sendMidiMessage (message);
        
    } //else if (button == requestPatchDataButton)
    
    else if (button == resetSoundEngineButton)
    {
        printf("Reseting synth to panel settings\r\n");
        eventLabel->setText("Reseting synth to panel settings...", dontSendNotification);
        
        //send command CC to synth via MIDI
        MidiMessage message = MidiMessage::controllerEvent(1, PARAM_CMD, CMD_REQUEST_PANEL_PARAM_DATA);
        sendMidiMessage (message);
        
    } //else if (button == resetSoundEngineButton)
    
    else if (button == disablePanelButton)
    {
        if (button->getToggleState() == true)
        {
            printf("Disabling synth panel\r\n");
            eventLabel->setText("Disabling synth panel...", dontSendNotification);
            
            //send command CC to synth via MIDI
            MidiMessage message = MidiMessage::controllerEvent(1, PARAM_CMD, CMD_DISABLE_PANEL);
            sendMidiMessage (message);
            
        } //if (button->getToggleState() == true)
        
        else
        {
            printf("Enabling synth panel\r\n");
            eventLabel->setText("Enabling synth panel...", dontSendNotification);
            
            //send command CC to synth via MIDI
            MidiMessage message = MidiMessage::controllerEvent(1, PARAM_CMD, CMD_ENABLE_PANEL);
            sendMidiMessage (message);
            
        } //else (button->getToggleState() == false)
        
    } //else if (button == disablePanelButton)
}

//==============================================================================
//==============================================================================
//==============================================================================
//JUCE FileBrowserComponent callback functions called when there is interaction with a file browser component

void MainContentComponent::selectionChanged()
{
    
}

void MainContentComponent::fileClicked (const File &file, const MouseEvent &e)
{
    
}

void MainContentComponent::fileDoubleClicked (const File &file)
{
    //trigger a load of the clicked file
    loadPatchFile (file);
}

void MainContentComponent::browserRootChanged (const File &newRoot)
{
    
}

//==============================================================================
//==============================================================================
//==============================================================================
//JUCE MidiInput callback function called whenever a MIDI message is received


void MainContentComponent::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
//    //debugging raw message
//    const int message_size = message.getRawDataSize();
//    const uint8_t* raw_message_buffer = message.getRawData();
//
//    printf("Incoming MIDI Message: ");
//    
//    for (int i = 0; i < message_size; i++)
//    {
//         printf("%d ", raw_message_buffer[i]);
//    }
//    
//    printf("\r\n");
    
    if (message.isController())
    {
        printf("CC %d of value %d received\r\n", message.getControllerNumber(), message.getControllerValue());
        
        //if a patch CC (not CC 127 which is used for commands instead)
        if (message.getControllerNumber() < 127)
        {
            //store the CC value
            patchData[message.getControllerNumber()] = message.getControllerValue();
            
            MessageManagerLock mmlock;
            String output_string = "Received CC num ";
            output_string += message.getControllerNumber();
            output_string += " with value ";
            output_string += message.getControllerValue();
            eventLabel->setText(output_string, dontSendNotification);
            
        } //if (message.getControllerNumber() < 127)
        
        //if a command CC (127)
        else
        {
            if (message.getControllerValue() == CMD_SENT_ALL_PATCH_DATA)
            {
                MessageManagerLock mmlock;
                eventLabel->setText("Got all patch data from synth!", dontSendNotification);
                
            } //if (message.getControllerValue() == CMD_SENT_ALL_PATCH_DATA)
            
        } //else
        
    } //if (message.isController())
}

//==============================================================================
//==============================================================================
//==============================================================================
//Function that sends a MIDI message to the MIDI output set by the audio device manager object

void MainContentComponent::sendMidiMessage (MidiMessage midiMessage)
{
    //If midi output exists (it won't if the user hasn't chosen an output device...)
    if (audioDeviceManager.getDefaultMidiOutput())
    {
        audioDeviceManager.getDefaultMidiOutput()->startBackgroundThread();
        audioDeviceManager.getDefaultMidiOutput()->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
    }
    else
    {
        std::cout << "A MIDI output device doesn't not exist!" << std::endl;
        eventLabel->setText("Please select a MIDI-out!", dontSendNotification);
    }

}

//==============================================================================
//==============================================================================
//==============================================================================
//Function that saves a set of patch data to file

void MainContentComponent::savePatchFile()
{
    std::cout << "Saving patch file with name: " << patchNameEditor->getText() << std::endl;
    eventLabel->setText("Saving patch to file...", dontSendNotification);
    
    //create patch file object
    File patch_file = patchDir.getFullPathName() + File::separatorString + patchNameEditor->getText()/* + ".vts"*/;
    
    //create string for holding the contents of the file
    String file_contents_string;
    
    //for each patch parameter
    for (int i = 0; i < 127; i++)
    {
        //put into the following format on it's on line: [param num]:[param value]
        file_contents_string += i;
        file_contents_string += ":";
        file_contents_string += patchData[i];
        file_contents_string += "\n";
    }
    
    //add string to file
    patch_file.replaceWithText (file_contents_string);
    
    //create the file
    patch_file.create();
    
    patchFileDirContentsList->refresh();
    
    eventLabel->setText("Finished saving patch", dontSendNotification);
}

//==============================================================================
//==============================================================================
//==============================================================================
//Function that loads and reads data from a patch file, sending data to the synth via MIDI

void MainContentComponent::loadPatchFile (const File file)
{
    std::cout << "Loading patch file: " << file.getFileName() << std::endl;
    
    eventLabel->setText("Loading patch and sending to synth...", dontSendNotification);
    
    //load the contents of the file into a string
    String file_contents_string = file.loadFileAsString();
    
    //for each patch parameter
    for (int i = 0; i < 127; i++)
    {
        //find the parameter number in the file contents...
        
        String param_num_string(i);
        param_num_string += ":";
        //find the start index of the param num string
        int param_num_start_index = file_contents_string.indexOf (0, param_num_string);
        
        //if found the parameter number
        if (param_num_start_index != -1)
        {
            //std::cout << "Found " << param_num_string << " at start index " << param_num_start_index << std::endl;
            
            //get the end index of the param num string value
            int param_num_end_index = file_contents_string.indexOf (param_num_start_index, "\n");
            
            if (param_num_end_index != -1)
            {
                //std::cout << "Found " << param_num_string << " at end index " << param_num_end_index << std::endl;
                
                //get a substring of the contents based on the start and end indexes
                String param_val_string = file_contents_string.substring (param_num_start_index, param_num_end_index);
                //get the parameter value out of the substring
                param_val_string = param_val_string.fromFirstOccurrenceOf(":", false, true);
                
                std::cout << "Param number " << i << " has a value of " << param_val_string << std::endl;
                
                //put the parameter value into the patch data array
                
                patchData[i] = param_val_string.getIntValue();
                
                //send the parameter value to the synth, if a valid value
                if (patchData[i] >= 0 && patchData[i] <= 127)
                {
                    MidiMessage midi_message = MidiMessage::controllerEvent(1, i, patchData[i]);
                    sendMidiMessage (midi_message);
                    
                } //if (patchData[i] >= 0 && patchData[i] <= 127)
                
            } //if (param_num_end_index != -1)

        } //if (param_num_index != -1)

    } //for (int i = 0; i < 127; i++)
    
    eventLabel->setText("Finished loading patch", dontSendNotification);
}




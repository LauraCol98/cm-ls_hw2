# CMLS - Homework 2
This chorus plugin was developed using JUCE framework in C++ for the course of Computer Music - Languages and Systems at the Politecnico of Milan.
The interface was created using https://github.com/ffAudio/foleys_gui_magic and is stored inside the XML file.

# How to build?
On Windows:

    Download Juce (http://www.juce.com/)
    Install the module foleys gui magic with the following command line lines:
        - cd "YOUR_PATH"
        - mkdir "module folder"
        - cd "module folder name"
        - git clone https://github.com/ffAudio/foleys_gui_magic
        - open the .jucer file from this repository and add the new module. allow binary data.
   
    Save and open in your IDE. I used Visual Studio.
    Compile and generate VST3/AU file, that you'll find in your build folder.
    Load the plugin in your DAW or use the plugin host provided by JUCE.
    Have fun!

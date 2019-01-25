//
//  coms.h
//  IGME Synth
//
//  Created by Samuel Hunt on 23/01/2019.
//

#ifndef coms_h
#define coms_h

#include "../JuceLibraryCode/JuceHeader.h"

namespace Coms {
    
    static const String root = "/IGME/";
    static const String note = root + "note";
    static const String loadPreset = root + "loadPreset";
    static const String getPresets= root + "getPresets";
    static const String preset = root + "preset";
    static const String midi = root + "midi";
    
    static const String synthStart = root + "synthstart";
    static const String IGMEStart = root + "igmestart";
    
    
}

#endif /* coms_h */

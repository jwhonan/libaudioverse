document_enum(name = "Lav_ERRORS",
doc = r"""
All functions in this library return one of the following enum values, indicating their error condition.
""",
members = {
    "Lav_ERROR_NONE": "No error occured.",
    "Lav_ERROR_UNKNOWN": "Something went wrong.  This error indicates that we couldn't figure out what.",
    "Lav_ERROR_NOT_INITIALIZED": """Failure to call {{"Lav_initialized"|codelit}}.""",
    "Lav_ERROR_TYPE_MISMATCH": "Indicates an attempt to manipulate a property through a function that does not work with that property's type.",
    "Lav_ERROR_INVALID_PROPERTY": "An attempt to access a property which does not exist on the specified node.",
    "Lav_ERROR_NULL_POINTER": "You passed a null pointer into Libaudioverse in a context where null pointers are not allowed.",
    "Lav_ERROR_MEMORY": "Libaudioverse triedd to allocate a pointer, but could not.  This probably means out of memory.",
    "Lav_ERROR_INVALID_POINTER": "Attempt to free a pointer that Libaudioverse doesn't know about.",
    "Lav_ERROR_INVALID_HANDLE": "A value passed in as a handle is not currently a handle which is valid.",
    "Lav_ERROR_RANGE": "A function parameter is not within a valid range.  This could be setting property values outside their range, accessing inputs and outputs that do not exist, or any of a variety of other range error conditions.",
    "Lav_ERROR_CANNOT_INIT_AUDIO":  "The audio subsystem could not be initialized.",
    "Lav_ERROR_NO_SUCH_DEVICE": "Attempt to use an I/O device that doesn't exist.  In addition to being caused by your code, this can happen if the user unplugs the device.",
    "Lav_ERROR_FILE": "Represents a miscelaneous file error.",
    "Lav_ERROR_FILE_NOT_FOUND": "Libaudioverse could not find a specified file.",
    "Lav_ERROR_HRTF_INVALID": "An attempt to use an invalid HRTF database.",
    "Lav_ERROR_CANNOT_CROSS_SERVERS": "An attempt was made to relate two objects from different servers. This could be assigning to buffer properties, connecting nodes, or any other such condition.",
    "Lav_ERROR_CAUSES_CYCLE": "The requested operation would cause a cycle in the graph of nodes that need processing.",
    "Lav_ERROR_PROPERTY_IS_READ_ONLY": "Attempt to set a read-only property.",
    "Lav_ERROR_OVERLAPPING_AUTOMATORS": "An attempt to schedule an automator within the duration of another.",
    "Lav_ERROR_CANNOT_CONNECT_TO_PROPERTY": "Attempt to connect a node to a property which cannot be automated.",
    "Lav_ERROR_BUFFER_IN_USE": "Indicates an attempt to modify a buffer while something is reading its data.",
    "Lav_ERROR_INTERNAL": "If you see this error, it's a bug.",
})
document_enum(name = "Lav_PROPERTY_TYPES",
doc = """
Indicates the type of a property.
""",
members = {
    "Lav_PROPERTYTYPE_INT": "Property holds a 32-bit integer.",
    "Lav_PROPERTYTYPE_FLOAT": "Property holds a 32-bit floating point value.",
    "Lav_PROPERTYTYPE_DOUBLE": "Property holds a 64-bit double.",
    "Lav_PROPERTYTYPE_FLOAT3": "Property holds a float3, a vector of 3 floats.",
    "Lav_PROPERTYTYPE_FLOAT6": "Property holds a float6, a vector of 6 floats.",
    "Lav_PROPERTYTYPE_FLOAT_ARRAY": "Property is an array of floats.",
    "Lav_PROPERTYTYPE_INT_ARRAY": "Property is an array of ints.",
    "Lav_PROPERTYTYPE_BUFFER": "Property holds a handle to a buffer.",
})
document_enum(name = "Lav_NODE_STATES",
doc = """
Used to indicate the state of a node.
This is the value of the node's state property and determins how the node is processed.
""",
members = {
    "Lav_NODESTATE_PAUSED": "This node is paused.",
    "Lav_NODESTATE_PLAYING": "This node advances if other nodes need audio from it.",
    "Lav_NODESTATE_ALWAYS_PLAYING": "This node advances always.",
})
document_enum(name = "Lav_LOGGING_LEVELS",
doc = """
Possible levels for logging.
""",
members ={
    "Lav_LOGGING_LEVEL_OFF": "No log messages will be generated.",
    "Lav_LOGGING_LEVEL_CRITICAL": "Logs critical messages such as failures to initialize and error conditions.",
    "Lav_LOGGING_LEVEL_INFO": "Logs informative messages.",
    "Lav_LOGGING_LEVEL_DEBUG": "Logs everything possible.",
})
document_enum(name = "Lav_PANNING_STRATEGIES",
doc = """
Indicates a strategy to use for panning.
This is mostly for the {{"Lav_OBJTYPE_MULTIPANNER_NODE"|node}} and the 3D components of this library.
""",
members = {
    "Lav_PANNING_STRATEGY_HRTF": "Indicates HRTF panning.",
    "Lav_PANNING_STRATEGY_STEREO": "Indicates stereo panning.",
    "Lav_PANNING_STRATEGY_SURROUND40": "Indicates 4.0 surround sound (quadraphonic) panning.",
    "Lav_PANNING_STRATEGY_SURROUND51": "Indicates 5.1 surround sound panning.",
    "Lav_PANNING_STRATEGY_SURROUND71": "Indicates 7.1 surround sound panning.",
})
document_enum(name = "Lav_BIQUAD_TYPES",
doc = """
Indicates a biquad filter type, used with the {{"Lav_OBJTYPE_BIQUAD_NODE"|node}} and in a few other places.
""",
members ={
    "Lav_BIQUAD_TYPE_LOWPASS": "Indicates a lowpass filter.",
    "Lav_BIQUAD_TYPE_HIGHPASS": "Indicates a highpass filter.",
    "Lav_BIQUAD_TYPE_BANDPASS": "Indicates a bandpass filter.",
    "Lav_BIQUAD_TYPE_NOTCH": "Indicates a notch filter.",
    "Lav_BIQUAD_TYPE_ALLPASS": "Indicates an allpass filter.",
    "Lav_BIQUAD_TYPE_PEAKING": "Indicates a peaking filter.",
    "Lav_BIQUAD_TYPE_LOWSHELF": "Indicates a lowshelf filter.",
    "Lav_BIQUAD_TYPE_HIGHSHELF": "Indicates a highshelf filter.",
    "Lav_BIQUAD_TYPE_IDENTITY": "This filter does nothing.",
})
document_enum(name = "Lav_DISTANCE_MODELS",
doc = """
used in the 3D components of this library.
Indicates how sound should become quieter as objects move away from the listener.

Libaudioverse computes the distance from the center of the source to the listener, then subtracts the size.
In the following, {{"distance"|codelit}} refers to this distance.
Libaudioverse also computes a second value called {{"distancePercent"|codelit}}; specifically, {{"distancePercent = distance/maxDistance"|codelit}}.
""",
members = {
    "Lav_DISTANCE_MODEL_LINEAR": """Sound falls off as {{"1-distancePercent"|codelit}}.""",
    "Lav_DISTANCE_MODEL_INVERSE": """Sounds fall off as {{"1/(1+315*distancePercent)"|codelit}}.  Just before {{"maxDistance"|codelit}}, the gain of the sound will be approximately -25 DB.  This distance model emphasizes distance changes when sounds are close, but treats distance changes of further away sources more subtly.  For full benefit of the effect of close sounds, this distance model must be used with fairly large values for {{"maxDistance"|codelit}}, usually around 300.""",
    "Lav_DISTANCE_MODEL_INVERSE_SQUARE": """Sounds fall off as {{"1.0/(1+315*distancePercent*distancePercent)"|codelit}}.  This is a standard inverse square law, modified such that the sound volume just before {{"maxDistance"|codelit}} is about -25 DB.  Of the available distance models, this is the closest to an accurate simulation of large, wide-open places such as fields and stadiums.""",
})
document_enum(name = "Lav_FDN_FILTER_TYPES",
doc = "Possible filter types for a feedback delay network's feedback path.",
members = {
    "Lav_FDN_FILTER_TYPE_DISABLED": "Don't insert filters on the feedback path.",
    "Lav_FDN_FILTER_TYPE_LOWPASS": "Insert lowpass filters on the FDN's feedback path.",
    "Lav_FDN_FILTER_TYPE_HIGHPASS": "Insert highpass filters on the FDN's feedback path.",
})
document_enum(name = "Lav_CHANNEL_INTERPRETATIONS",
doc = "Specifies how to treat inputs to this node for upmixing and downmixing.",
members = {
    "Lav_CHANNEL_INTERPRETATION_DISCRETE": "If channel counts mismatch, don't apply mixing matrices. Either drop or fill with zeros as appropriate.",
    "Lav_CHANNEL_INTERPRETATION_SPEAKERS": "Apply mixing matrices if needed.",
})
document_enum(name = "Lav_NOISE_TYPES",
doc= "Specifies types of noise.",
members = {
    "Lav_NOISE_TYPE_WHITE": "Gaussian white noise.",
    "Lav_NOISE_TYPE_PINK": "Pink noise.  Pink noise falls off at 3 DB per octave.",
    "Lav_NOISE_TYPE_BROWN": "Brown noise.  Brown noise decreases at 6 DB per octave.",
})